#include "px4_code/MavWrapper.h"


MavWrapper::MavWrapper():nh("~"){    
    // parameter parsing 
    nh.param<string>("mav_frame_id",mav_frame_id,"/iris__base_link");
    nh.param<string>("world_frame_id",world_frame_id,"/world");
    nh.param<double>("hovering_height",hovering_height,1.0);
    double time_out; // sec 
    nh.param<double>("init_timeout",time_out,2.0);
    mav_init_timeout = ros::Duration(time_out);        
    tf_listener = new (tf::TransformListener);
    
    // ros comm 
    sub_control_pose = nh.subscribe("/mav_wrapper/setpoint_planning",1,&MavWrapper::cb_setpoint,this);
    sub_state =nh.subscribe("/mavros/state", 10, &MavWrapper::cb_state,this);
    pub_setpoint = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
    pub_cur_pose = nh.advertise<geometry_msgs::PoseStamped>("/mavros/vision_pose/pose",10);
    server_init_home = nh.advertiseService("/mav_wrapper/init_home",&MavWrapper::init_home_callback,this);        
    server_key_input = nh.advertiseService("/mav_wrapper/keyboard_in",&MavWrapper::keyboard_callback,this);
    server_switch_mode = nh.advertiseService("/mav_wrapper/swtich_mode",&MavWrapper::swtich_mode_callback,this);    

}

MavWrapper::~MavWrapper(){
    delete tf_listener;
}   

bool MavWrapper::update_tf(){
    
    try{
        tf::StampedTransform transform; 
        tf_listener->lookupTransform(world_frame_id,mav_frame_id,ros::Time(0), transform);
        geometry_msgs::PoseStamped pose_stamped;
        pose_stamped.header.stamp = ros::Time::now();
        pose_stamped.header.frame_id = world_frame_id;

        pose_stamped.pose.position.x = transform.getOrigin().getX();
        pose_stamped.pose.position.y = transform.getOrigin().getY();
        pose_stamped.pose.position.z = transform.getOrigin().getZ();

        pose_stamped.pose.orientation.x = transform.getRotation().getX();
        pose_stamped.pose.orientation.y = transform.getRotation().getY();
        pose_stamped.pose.orientation.z = transform.getRotation().getZ();
        pose_stamped.pose.orientation.w = transform.getRotation().getW();

        
        ROS_INFO_ONCE("[Mav Wrapper] tf of mav received. "); is_tf_recieved = true; pose_cur = pose_stamped; 
        return true;
        
    }
    catch (tf::TransformException ex){
        ROS_ERROR_ONCE("[Mav Wrapper] tf of mav does not exist.",ex.what());  
        return false;
    }
}

/**
 * @brief receive the setpoint from other planning node 
 * 
 * @param pose 
 */
void MavWrapper::cb_setpoint(geometry_msgs::PoseStampedConstPtr pose){
    is_planning_received = true;
    pose_des_planner = *pose;
}
/**
 * @brief receive px4 state information from mavros  
 * 
 * @param state 
 */
void MavWrapper::cb_state(mavros_msgs::StateConstPtr state){
    mav_state = *state;
    is_mavros_connected = state->connected;
    is_offboard = state->mode == "OFFBOARD";   
}


bool MavWrapper::mav_init(){
    if (is_tf_recieved){
        pose_des_keyboard = pose_cur;        
        is_init_mav = true;
        ROS_INFO("[Mav Wrapper] Initializing the homing point with the current pose");
        return true;
    }
    else 
        {ROS_WARN("[Mav Wrapper] tf information has not been recieved yet."); return false;};
}
void MavWrapper::publish_setpoint(){    

    if (mode == 0) // keyboard mode (default)  
        pub_setpoint.publish(pose_des_keyboard);
    else // planner mode (changed mode)
        pub_setpoint.publish(pose_des_planner);

}
void MavWrapper::publish_externally_estimated_pose(){      
    pub_cur_pose.publish(pose_cur);
}
/**
 * @brief modify current desired pose for keyboard input with current UAV position
 * 
 * @param req 
 * @param resp 
 * @return true 
 * @return false 
 */
bool MavWrapper::init_home_callback(px4_code::InitHomeRequest & req,px4_code::InitHomeResponse& resp){
    resp.is_success = mav_init();
    return true;
};
/**
 * @brief : move MAV w.r.t current desired frame 
 * 
 * @param req 
 * @param resp 
 * @return true 
 * @return false 
 */
bool MavWrapper::keyboard_callback(px4_code::KeyboardInputRequest & req,px4_code::KeyboardInputResponse & resp){
    std::string keyinput = req.key;
    bool is_success = false;     
    double increment_xyz = 0.05;
    double increment_yaw = 3.141592 / 8; // maybe to be tuned  
    Eigen::Vector4d move_pose; move_pose.setZero();
    
    // prepare delta pose  
    switch (tolower(keyinput.c_str()[0])){
        case 'w': 
            move_pose(0) = increment_xyz;
            break;            
        case 's': 
            move_pose(0) = -increment_xyz;
            break;
        case 'a':
            move_pose(1) = increment_xyz;
            break;
        case 'd':
            move_pose(1) = -increment_xyz;
            break;
        case 'z':
            move_pose(2) = -increment_xyz;
            break;
        case 'c':
            move_pose(2) = increment_xyz;
            break;        
        case 'q':
            move_pose(3) = increment_yaw;
            break;
        case 'e':
            move_pose(3) = -increment_yaw;
            break;                            
   } 

    // apply delta pose 
    // std::cout<<move_pose<<std::endl;
    return move_mav(move_pose(0),move_pose(1),move_pose(2),move_pose(3));    
}
            
bool MavWrapper::swtich_mode_callback(px4_code::SwitchModeRequest & req, px4_code::SwitchModeResponse & resp){
    // keyboard mode requested 
    if (req.mode == 0){
        if(mav_init()){
            mode = 0;
            return true;}
        else{
            ROS_WARN("[Mav Wrapper] mav init was denied. is it listening tf?");
            return false;

        }


    // planning mode requested 
    }else{
        // if there is a planning 
        if (is_planning_received){
            mode =1 ;
            return true;}
        else{            
            return false;
        
        }
    }
}
void MavWrapper::run(){
    
    // Phase 1 
    // initialize the mav homing postion from mav_wrapper side 
    ros::Rate rate(40);
    ros::Time last_request = ros::Time::now();
    
    while(ros::ok()){
        
        // regular routine
        update_tf();
        publish_externally_estimated_pose(); // the pose published will be used for mavros
        
        // still not initialized
        if(not is_init_mav)  
            mav_init();
        // If it is initialized do the followings 
        else{
            if (is_mavros_connected ) 
            publish_setpoint();
            publish_externally_estimated_pose();
        }

        ros::spinOnce();
        rate.sleep();
    }
    
}


// move the desired control pose of mav w.r.t body frame 
bool MavWrapper::move_mav(double dx,double dy,double dz,double dyaw){
    // if keyboard mode
    if (mode == 0 and is_init_mav){
        
        // translation w.r.t body frame
        Eigen::Vector3d dpose(dx,dy,dz);

        // reference frame = current ? current_des ?  which would be better ?  
        tf::Quaternion q_cur_des;
        q_cur_des.setX(pose_des_keyboard.pose.orientation.x);
        q_cur_des.setY(pose_des_keyboard.pose.orientation.y);
        q_cur_des.setZ(pose_des_keyboard.pose.orientation.z);
        q_cur_des.setW(pose_des_keyboard.pose.orientation.w);

        tf::Vector3 t_cur_des(pose_des_keyboard.pose.position.x,
                        pose_des_keyboard.pose.position.y,
                        pose_des_keyboard.pose.position.z);


        tf::Transform transform_cur;
        transform_cur.setRotation(q_cur_des);
        // transform_cur.setOrigin(t_cur_des);

        // transform matrix 
        Eigen::Isometry3d Twb;
        tf::transformTFToEigen(transform_cur,Twb);

        Eigen::Vector3d dpose_w = Twb*dpose;
        
        // Step 1: modify xyz 
        pose_des_keyboard.pose.position.x += dpose_w(0);
        pose_des_keyboard.pose.position.y += dpose_w(1);
        pose_des_keyboard.pose.position.z += dpose_w(2);
        
        // Step 2: modify yaw direcion
        double roll,pitch,yaw;
        tf::Matrix3x3(q_cur_des).getEulerYPR(yaw,pitch,roll);
        yaw += dyaw; // here, we adjust yaw
        tf::Quaternion q_des = tf::Quaternion();
        q_des.setRPY(roll,pitch,yaw);

        pose_des_keyboard.pose.orientation.x = q_des.getX();
        pose_des_keyboard.pose.orientation.y = q_des.getY();
        pose_des_keyboard.pose.orientation.z = q_des.getZ();
        pose_des_keyboard.pose.orientation.w = q_des.getW();

        return true;
    }
    else
    {
        if (not mode == 0)
            ROS_WARN("mav receives planning setpoint. To enable keyboard, switch mode");
        else 
            ROS_WARN("No initialized homing point");
        return false;
    }
    
}






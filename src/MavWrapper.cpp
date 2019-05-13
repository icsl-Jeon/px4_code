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
    sub_control_pose = nh.subscribe("control_pose",1,&MavWrapper::cb_setpoint,this);
    sub_state =nh.subscribe("/mavros/state", 10, &MavWrapper::cb_state,this);
    pub_setpoint = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
    pub_cur_pose = nh.advertise<geometry_msgs::PoseStamped>("/mavros/vision_pose/pose",10);
    server_init_home = nh.advertiseService("/mav_wrapper/init_home",&MavWrapper::init_home_callback,this);        
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
        ROS_INFO("Initializing the homing point with the current pose");
        return true;
    }
    else 
        {ROS_WARN("tf information has not been recieved yet."); return false;};
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

bool MavWrapper::init_home_callback(px4_code::InitHomeRequest & req,px4_code::InitHomeResponse& resp){
    resp.is_success = mav_init();
    return true;
};

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







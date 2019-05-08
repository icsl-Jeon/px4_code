#include "px4_code/MavWrapper.h"


MavWrapper::MavWrapper(){

}

MavWrapper::~MavWrapper(){

}
/**
 * @brief ros communication init. The components are not inclucded in the constructor for qt integration
 * 
 * @param nh 
 */
void MavWrapper::ros_init(ros::NodeHandle nh){


    nh.param<string>("mav_frame_id",mav_frame_id,"/iris__base_link");
    nh.param<string>("world_frame_id",world_frame_id,"/world");

    // ros comm 
    tf_listener = new (tf::TransformListener);
    sub_control_pose = nh.subscribe("control_pose",1,&MavWrapper::cb_setpoint,this);
    sub_state =nh.subscribe("/mavros/state", 10, &MavWrapper::cb_state,this);
    pub_setpoint = nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 10);
    pub_cur_pose = nh.advertise<geometry_msgs::PoseStamped>("/mavros/mocap/pose",10);    

    // these are to be included in GCS part 
    arming_client=  nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");            
    set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");

    pose_init.header.frame_id = pose_des.header.frame_id = pose_cur.header.frame_id = world_frame_id;
    
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
  
        ROS_ERROR_ONCE("[Mav Wrapper] tf of mav does not exist. ",ex.what());  
        return false;
    }
}


bool MavWrapper::set_offboard(){
    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";
    bool is_success = set_mode_client.call(offb_set_mode) and offb_set_mode.response.mode_sent;
    ROS_INFO("Offboard requested.");    
    if (is_success){
        ROS_INFO("Switched to offboard mode!");
        is_offboard = true;
    }

    return is_success;
}

bool MavWrapper::arming(){

    if (is_offboard){

        mavros_msgs::CommandBool arm_cmd;
        arm_cmd.request.value = true;

        bool is_success = arming_client.call(arm_cmd) and arm_cmd.response.success;
        ROS_INFO("Arming requested.");            
        if (is_success){
            ROS_INFO("Vehicle armed!");
            is_armed = true;
        }
        return is_success;
    }else{

        ROS_WARN("Arming requested but still not offboard.");
    }
    
}
/**
 * @brief receive the setpoint from other planning node 
 * 
 * @param pose 
 */
void MavWrapper::cb_setpoint(geometry_msgs::PoseStampedConstPtr pose){

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

/**
 * @brief Initializing the takeoff point of MAV with the current pose
 * 
 */
void MavWrapper::mav_init(){

    if (is_tf_recieved){
        ROS_INFO("Initializing the takeoff point with the current pose");
        pose_init = pose_cur;
        is_init_mav = true;
    }
    else 
        ROS_WARN("tf information has not been recieved yet.");
}


void MavWrapper::update_hovering(double hovering_height){
    pose_init.pose.position.z = hovering_height;
    pose_des = pose_init;
}

/**
 * @brief publish the current setpoint. 
 * 
 */
void MavWrapper::publish_setpoint(){    
    pub_setpoint.publish(pose_des);
}
/**
 * @brief mocap data is replaced with gazebo2rviz package 
 * 
 */
void MavWrapper::publish_mocap_pose(){    

    pub_cur_pose.publish(pose_cur);
}








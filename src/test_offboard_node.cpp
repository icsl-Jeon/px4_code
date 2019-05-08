#include "px4_code/MavWrapper.h"

int main(int argc,char ** argv){
    
    ros::init(argc, argv, "mav_wrapper_test_node");
    ROS_INFO("[Mav Wrapper] node started");

    ros::NodeHandle nh("~");
    ros::Rate loop_rate(200);

    MavWrapper mav_wrapper;
    mav_wrapper.ros_init(nh);
    
    // publish dummy setpoint for offboard mode  
    
    mav_wrapper.publish_setpoint();
    
    while(ros::ok()){
        
        // update tf information 
        mav_wrapper.update_tf();
        mav_wrapper.publish_mocap_pose(); // the pose published will be used for mavros 
        
        // initialize MAV position and this is published initial setpoint 
        if (not mav_wrapper.is_init_mav){
                mav_wrapper.mav_init();
        } 

        if (mav_wrapper.is_init_mav){
            // hover command 
            ROS_INFO_ONCE("setpoint publish.");
            mav_wrapper.publish_setpoint();
        }

        // request arming if in offboard mode 
        if (not mav_wrapper.is_armed){
                mav_wrapper.arming();                
        }

        // request offboard switching 
        if (not mav_wrapper.is_offboard)
            mav_wrapper.set_offboard();

        // if every procedure is completed,  hovering requested.
        if (mav_wrapper.is_init_mav and mav_wrapper.is_armed and mav_wrapper.is_offboard ){
            ROS_INFO_ONCE("hovering requested!");
            mav_wrapper.update_hovering(2.0);
        }
        loop_rate.sleep();
    }

}





#include "px4_code/MavWrapper.h"

int main(int argc,char ** argv){    
    ros::init(argc, argv, "mav_wrapper_test_node");
    ROS_INFO("[Mav Wrapper] node started");
    MavWrapper mav_wrapper;
    mav_wrapper.run();         
}





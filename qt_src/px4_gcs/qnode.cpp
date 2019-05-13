#include "qnode.h"

QNode::QNode(int argc, char** argv, const std::string &name ) :
    init_argc(argc),
    init_argv(argv),
    node_name(name)
    {
        
}

// destructor 
QNode::~QNode() {
    shutdown();
}

/**
 * @brief register the publisher or subscriber 
 * 
 */
void QNode::ros_comms_init(){
    
    ros::NodeHandle nh("~");
    
    // communication start 
    arming_client=  nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");            
    set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
    init_home_client = nh.serviceClient<px4_code::InitHome>("/mav_wrapper/init_home");
}

// SLOT 

void QNode::on_init(bool * result){

    ros::init(init_argc,init_argv,node_name);
    if ( ! ros::master::check() ) {
        *result = false;
    }
    ros::start(); // our node handles go out of scope, so we want to control shutdown explicitly.
    ros_comms_init();
    start();
    *result = true;
}

void QNode::shutdown() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    // wait();
}

// void QNode::run(){

//     ros::Rate loop_rate(50);
//     while(ros::ok()){


//     }
    
//     Q_EMIT rosShutdown();
// }


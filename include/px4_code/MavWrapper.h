#include <ros/ros.h>
#include <string>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <px4_code/InitHome.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>


/**
 * @brief 
 * This node performs the following things:
 * 1. Listen the tf from pose estimator and publish /mavros/vision_pose/pose if needed 
 * (estimator node can directly publish)
 * 
 * 2. Publish setpoint from either keyboard control or path planner 
 * 
 * 3. Initialize homing position from gcs. 
 *    However, this node will initialize the positioin itself   
  */

using namespace std;
class MavWrapper{

    private:
        ros::NodeHandle nh;

        string mav_frame_id; 
        string world_frame_id;
        
        geometry_msgs::PoseStamped pose_cur; // current pose of MAV 
        geometry_msgs::PoseStamped pose_des_keyboard; 
        geometry_msgs::PoseStamped pose_des_planner; 
        geometry_msgs::PoseStamped pose_init;

        mavros_msgs::State mav_state; // px4 state from mavros 
        tf::TransformListener* tf_listener; 

        ros::Publisher pub_setpoint; // published merged 
        ros::Publisher pub_cur_pose; // publish for mocap/pose  
        ros::Subscriber sub_control_pose; // pose which is to be controlled (planner mode)
        ros::Subscriber sub_state; // mav state callback from mavros 
        ros::ServiceServer server_init_home; // initialize homing point 

        unsigned int mode = 0; // 0: keyboard 1: planner setpoint 
        
        double hovering_height;
        ros::Duration mav_init_timeout; // we will request mav_init with this timeouts

    public:        
        MavWrapper();        
        ~MavWrapper();
        
        // initialize homing         
        bool mav_init(); 
        bool init_home_callback(px4_code::InitHomeRequest & req,px4_code::InitHomeResponse& resp);
        
        // routine in the loop  
        void run(); 
        void publish_setpoint();
        void publish_externally_estimated_pose(); 
        
        // callback 
        bool update_tf();
        void cb_setpoint(geometry_msgs::PoseStampedConstPtr pose);
        void cb_state(mavros_msgs::StateConstPtr state);
                        
        // flags
        bool is_mavros_connected = false;
        bool is_armed = false;
        bool is_offboard = false;
        bool is_tf_recieved = false;
        bool is_init_mav = false;  
};


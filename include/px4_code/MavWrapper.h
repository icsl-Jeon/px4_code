#include <ros/ros.h>
#include <string>
#include <ctype.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <nav_msgs/Odometry.h>
#include <px4_code/InitHome.h>
#include <px4_code/KeyboardInput.h>
#include <px4_code/SwitchMode.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include <tf_conversions/tf_eigen.h>
#include <Eigen/Core>
#include <rosgraph_msgs/Clock.h>

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
        ros::Time cur_clock; // current clock 
        string mav_frame_id; 
        string world_frame_id;
        string ext_pose_topic; // topic of extenal unit 
        string ext_pose_cov_topic; // topic of extenal unit 
        string ext_odom_topic; // topic of external input

        geometry_msgs::PoseStamped pose_cur; // current pose of MAV from external unit 
        geometry_msgs::PoseWithCovarianceStamped pose_cov_cur; // current pose of MAV from external unit 
        geometry_msgs::PoseStamped pose_des_keyboard; 
        geometry_msgs::PoseStamped pose_des_planner; 
        geometry_msgs::PoseStamped pose_init;
        geometry_msgs::PoseStamped pose_mavros; // mavros local position


        mavros_msgs::State mav_state; // px4 state from mavros 

        
        ros::Publisher clock_server; //clock server 
        ros::Publisher pub_setpoint; // published merged 
        ros::Publisher pub_cur_pose; // publish for vision/pose  
        ros::Publisher pub_cur_pose_cov; // publish for vision/pose also  

        ros::Subscriber sub_control_pose; // pose which is to be controlled (planner mode)
        ros::Subscriber sub_mavros_pose; // pose 
        ros::Subscriber sub_state; // mav state callback from mavros 
        ros::Subscriber sub_ext_pose;
        ros::Subscriber sub_ext_pose_cov;
        ros::Subscriber sub_ext_odom;
        
        ros::ServiceServer server_init_home; // initialize homing point 
        ros::ServiceServer server_key_input; // receive keyinput and modify the setpoint
        ros::ServiceServer server_switch_mode; // 

        unsigned int mode = 0; // 0: keyboard 1: planner setpoint 
        
        double hovering_height;
        ros::Duration mav_init_timeout; // we will request mav_init with this timeouts

    public:        
        MavWrapper();        
        ~MavWrapper();
        
        // initialize start point         
        bool mav_init(); 
        bool init_home_callback(px4_code::InitHomeRequest & req,px4_code::InitHomeResponse& resp);
        bool keyboard_callback(px4_code::KeyboardInputRequest & req,px4_code::KeyboardInputResponse & resp);
            
        // routine in the loop  
        void run(); 
        void publish_setpoint();
        void publish_externally_estimated_pose(); 
        bool swtich_mode_callback(px4_code::SwitchModeRequest & req, px4_code::SwitchModeResponse & resp);

        // callback 
        bool update_tf_mocap();
        bool update_tf_vio();
        void talk_init_tf_vo();
        void cb_ext_pose(geometry_msgs::PoseStampedConstPtr pose);
        void cb_ext_pose_cov(geometry_msgs::PoseWithCovarianceStampedConstPtr pose_cov);
        void cb_ext_odom(nav_msgs::OdometryConstPtr odom);


        void cb_setpoint(geometry_msgs::PoseStampedConstPtr pose);
        void cb_state(mavros_msgs::StateConstPtr state);        
        void cb_mavros_local_pose(geometry_msgs::PoseStampedConstPtr local_pose);        
        // move mav from current pose    
        bool move_mav(double dx,double dy,double dz,double dyaw);                        
        // flags
        bool is_mavros_connected = false;
        bool is_armed = false;
        bool is_offboard = false;
        bool is_tf_recieved = false;
        bool is_init_mav = false;  // initialized homing point 
        bool is_planning_received  = false;      
        bool is_mavros_pose_recieved = false;
        
        bool is_cov = false; 
        bool is_pose = true;
        bool is_odom = false;

};


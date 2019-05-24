#include <ros/ros.h>
#include <string>
#include <ctype.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
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
        string vo_ref_frame_id; 
        string vo_frame_id;

        geometry_msgs::PoseStamped pose_cur; // current pose of MAV 
        geometry_msgs::PoseStamped pose_des_keyboard; 
        geometry_msgs::PoseStamped pose_des_planner; 
        geometry_msgs::PoseStamped pose_init;
        geometry_msgs::PoseStamped pose_mavros; // mavros local position

        mavros_msgs::State mav_state; // px4 state from mavros 
        tf::TransformListener* tf_listener; 
        tf::TransformBroadcaster* tf_talker; // for connecting referance frame of vo moudle and global positioning sensor
        tf::StampedTransform transform_from_world_to_vo_ref; // required only when global position is important even VIO 
        
        ros::Publisher pub_setpoint; // published merged 
        ros::Publisher pub_cur_pose; // publish for mocap/pose  
        ros::Subscriber sub_control_pose; // pose which is to be controlled (planner mode)
        ros::Subscriber sub_mavros_pose; // pose 
        ros::Subscriber sub_state; // mav state callback from mavros 
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
        bool listen_vo_init_pose = false; // get start pose of camera map frame from vicon  
        // the start pose of camera referance frame is assumed to have same rotation matrix with vicon  
        bool is_vo_init_pose = false;
};


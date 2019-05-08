#include <ros/ros.h>
#include <string>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

using namespace std;

class MavWrapper{

    private:
        string mav_frame_id;
        string world_frame_id;
        
        geometry_msgs::PoseStamped pose_cur;
        geometry_msgs::PoseStamped pose_des;  
        geometry_msgs::PoseStamped pose_init;

        mavros_msgs::State mav_state; 
        tf::TransformListener* tf_listener; 

        ros::Publisher pub_setpoint;
        ros::Publisher pub_cur_pose; // publish for mocap/pose  
        ros::Subscriber sub_control_pose; // pose which is to be controlled 
        ros::Subscriber sub_state; // mav state callback from mavros 
        ros::ServiceClient arming_client;
        ros::ServiceClient set_mode_client;
        

    public:        
        MavWrapper();        
        ~MavWrapper();
        
        // procedure 
        void ros_init(ros::NodeHandle nh);        
        void mav_init(); 
        bool set_offboard();
        bool arming();
        
        // routine in the loop  
        void publish_setpoint();
        void publish_mocap_pose(); 
        
        // callback 
        bool update_tf();
        void cb_setpoint(geometry_msgs::PoseStampedConstPtr pose);
        void cb_state(mavros_msgs::StateConstPtr state);
                
        void update_hovering(double hovering_height);

        // flags
        bool is_mavros_connected = false;
        bool is_armed = false;
        bool is_offboard = false;
        bool is_tf_recieved = false;
        bool is_init_mav = false;  
};


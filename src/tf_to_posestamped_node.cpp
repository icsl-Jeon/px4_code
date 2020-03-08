#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>

int main(int argc, char** argv){
    ros::init(argc, argv, "my_tf_listner");
    
    std::string map_frame,mav_frame;
    std::string output_pose_topic;

    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");
    ros::Rate rate(30.0);

    private_nh.param<std::string>("map_frame",map_frame,"/world");
    private_nh.param<std::string>("base_frame",mav_frame,"/iris__base_link");

    private_nh.param<std::string>("output_pose_topic",output_pose_topic,"/vicon/mav_felipe/mav_felipe");

    ros::Publisher pose_info = nh.advertise<geometry_msgs::PoseStamped>(output_pose_topic,30);
    
    tf::TransformListener listener;
    std::string tf_prefix = tf::getPrefixParam(private_nh);
    while (nh.ok()){
        tf::StampedTransform transform;
        bool tf_ok = true;
        try{
            listener.lookupTransform(map_frame, mav_frame,ros::Time(0),transform);
        }
        catch(tf::TransformException &ex){
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
            tf_ok = false;

        }
        if (tf_ok){
            geometry_msgs::PoseStamped pos_vicon;
        
            pos_vicon.header.stamp = ros::Time::now();
            pos_vicon.header.frame_id = tf_prefix+"/"+map_frame;
            pos_vicon.pose.position.x = transform.getOrigin().getX();
            pos_vicon.pose.position.y = transform.getOrigin().getY();
            pos_vicon.pose.position.z = transform.getOrigin().getZ();
        
            pos_vicon.pose.orientation.x = transform.getRotation().getX();
            pos_vicon.pose.orientation.y = transform.getRotation().getY();
            pos_vicon.pose.orientation.z = transform.getRotation().getZ();
            pos_vicon.pose.orientation.w = transform.getRotation().getW();

            pose_info.publish(pos_vicon);

        }
        rate.sleep();
    }
    return 0;
}

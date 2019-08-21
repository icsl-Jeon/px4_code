#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/PointCloud2.h>
#include <nav_msgs/Path.h>


/**
 * @brief This code does :
 * 1. log the /zed/zed_node/pose and broadcast the tf from world to zed_camera_link based on pose  
 * 2. log pcl generated from zed and assign ros::Time::now() to be synchronized with other topic
 * 3. log mav ros local position (same with zed_pose except roll and pitch) and stack it to a path  
 */

geometry_msgs::PoseStamped input_pose;
sensor_msgs::PointCloud2 pcl;
nav_msgs::Path mav_path;
bool is_pcl_received = false;
bool is_pose_received = false;

/**
 * @brief This callback receives pcl from outside and set the time as the current ros time 
 * 
 * @param pose_stamped_ptr 
 */
void pose_callback(geometry_msgs::PoseStampedConstPtr pose_stamped_ptr){
    input_pose = *pose_stamped_ptr;
    is_pose_received = true;
    mav_path.poses.push_back(input_pose);
}

void pcl_callback(sensor_msgs::PointCloud2ConstPtr pcl_ptr){
    pcl = *pcl_ptr;
    pcl.header.stamp = ros::Time::now();
    is_pcl_received = true;
    ROS_INFO_ONCE("pcl received");
}

int main(int argc, char** argv){
    ros::init(argc, argv, "tf_broad_caster");
    
    std::string map_frame,mav_frame;
    std::string input_pose_topic;
    std::string pcl_topic;

    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");
    ros::Rate rate(60);

    private_nh.param<std::string>("input_pose_topic",input_pose_topic,"/vicon/mav_felipe/mav_felipe");
    private_nh.param<std::string>("map_frame",map_frame,"/world");
    private_nh.param<std::string>("base_frame",mav_frame,"/iris__base_link");
    private_nh.param<std::string>("pointcloud_topic",pcl_topic,"/zed/zed_node/point_cloud/cloud_registered");
    
    mav_path.header.frame_id = map_frame;

    ros::Subscriber pose_info = nh.subscribe(input_pose_topic,30,pose_callback);
    ros::Subscriber sub_pcl = nh.subscribe(pcl_topic,2,pcl_callback);
    ros::Publisher pub_pcl = nh.advertise<sensor_msgs::PointCloud2>(pcl_topic+"_sync",1);
    ros::Publisher pub_path = nh.advertise<nav_msgs::Path>("/logger/mav_path",1);

    tf::TransformBroadcaster broadcaster;

    while (nh.ok()){
            tf::StampedTransform transform;
            tf::Vector3 position; tf::Quaternion quat;

            position.setX(input_pose.pose.position.x);
            position.setY(input_pose.pose.position.y);
            position.setZ(input_pose.pose.position.z);
            
            quat.setX(input_pose.pose.orientation.x);
            quat.setY(input_pose.pose.orientation.y);
            quat.setZ(input_pose.pose.orientation.z);
            quat.setW(input_pose.pose.orientation.w);
            
            transform.setOrigin(position);    
            transform.setRotation(quat);
            if (is_pose_received)
                broadcaster.sendTransform(tf::StampedTransform(transform,input_pose.header.stamp,map_frame,mav_frame));
            if (is_pcl_received)
                pub_pcl.publish(pcl); 

            pub_path.publish(mav_path);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

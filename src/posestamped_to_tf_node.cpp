#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/PointCloud2.h>

geometry_msgs::PoseStamped input_pose;

void pose_callback(geometry_msgs::PoseStampedConstPtr pose_stamped_ptr){
    input_pose = *pose_stamped_ptr;
}

int main(int argc, char** argv){
    ros::init(argc, argv, "tf_broad_caster");
    
    std::string map_frame,mav_frame;
    std::string input_pose_topic;

    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");
    ros::Rate rate(40.0);

    private_nh.param<std::string>("input_pose_topic",input_pose_topic,"/vicon/mav_felipe/mav_felipe");
    private_nh.param<std::string>("map_frame",map_frame,"/world");
    private_nh.param<std::string>("base_frame",mav_frame,"/iris__base_link");

    ros::Subscriber pose_info = nh.subscribe(input_pose_topic,30,pose_callback);
    
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
            
            broadcaster.sendTransform(tf::StampedTransform(transform,input_pose.header.stamp,map_frame,mav_frame));
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

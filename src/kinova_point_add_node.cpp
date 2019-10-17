#include <cstdlib>
#include "ros/ros.h"
#include "kinova_msgs/AddPoseToCartesianTrajectory.h"
#include "tf/transform_datatypes.h"

namespace iqr {
class KinovaPointAdd {
public:

  KinovaPointAdd(ros::NodeHandle& nod);
  ros::Subscriber sub;
  ros::ServiceClient client;
  double yaw_init, pitch_init, roll_init;
  kinova_msgs::AddPoseToCartesianTrajectory kinova_pose;

private:

  void kinovaCallback(const geometry_msgs::Pose::ConstPtr& pose);

};
}
iqr::KinovaPointAdd::KinovaPointAdd(ros::NodeHandle& nod) {
  sub = nod.subscribe<geometry_msgs::Pose>("right_arm_pose", 1000, &iqr::KinovaPointAdd::kinovaCallback, this);
  client = nod.serviceClient<kinova_msgs::AddPoseToCartesianTrajectory>("j2n6s300_driver/in/add_pose_to_Cartesian_trajectory");

}

void iqr::KinovaPointAdd::kinovaCallback(const geometry_msgs::Pose::ConstPtr& pose) {
  
  tf::Matrix3x3 mat(tf::Quaternion(pose->orientation.x, pose->orientation.y, pose->orientation.z, pose->orientation.w));    
  double yaw, pitch, roll;    
  mat.getEulerYPR(yaw, pitch, roll);
  yaw_init = yaw;
  pitch_init = pitch;
  roll_init = roll;
  // kinova_pose.request.X = pose->position.x;
  // kinova_pose.request.ThetaX = yaw_init;
  // kinova_pose.request.ThetaY = pitch_init;
  // kinova_pose.request.ThetaZ = roll_init;
  // kinova_pose.request.Y = pose->position.y;
  // kinova_pose.request.Z = pose->position.z;
  // kinova_pose.request.X = 0.209288;
  // kinova_pose.request.ThetaX = 1.63993;
  // kinova_pose.request.ThetaY = 0.991298;
  // kinova_pose.request.ThetaZ = 0.122501;
  // kinova_pose.request.Y = -0.342748;
  // kinova_pose.request.Z = 0.505158;
  double angle= 0;
  double radius = 0.1;            
  double angle_resolution = 10;                /*    */
  double d_angle = angle_resolution*3.14/180; /* 两个采样点间的弧度值 */
    double y_center = -0.342748;
    double z_center = 0.505158;
    kinova_pose.request.X = 0.209288;
    kinova_pose.request.ThetaX = 1.63993;
    kinova_pose.request.ThetaY = 0.991298;
    kinova_pose.request.ThetaZ = 0.122501;
    //  采样圆上的点
    for (int i= 0; i< (360/angle_resolution); i++)
    {
        angle+= d_angle; 
        kinova_pose.request.Y = y_center + radius*sin(angle);
        kinova_pose.request.Z = z_center + radius*cos(angle); 
        client.call(kinova_pose);
    }
  ROS_INFO("out");
}

int main(int argc, char **argv)
{
    // ROS节点初始化
    ros::init(argc, argv, "add_point_client");
    // 创建节点句柄
    ros::NodeHandle n;
    iqr::KinovaPointAdd kinova_point_add(n);

    // 创建一个client

    // double angle= 0;
    // double radius = 0.1;            
    // double angle_resolution = 10;                /*    */
    // double d_angle = angle_resolution*3.14/180; /* 两个采样点间的弧度值 */
    //   double y_center = -0.342748;
    //   double z_center = 0.505158;
    //   pose.request.X = 0.209288;
    //   pose.request.ThetaX = 1.63993;
    //   pose.request.ThetaY = 0.991298;
    //   pose.request.ThetaZ = 0.122501;
    //   //  采样圆上的点
    //   // kinova::kinovaArm kinova_arm;
    //   // kinova_arm.kinova_comm_.setCartesianPosition(pose, 0, false);
    //   for (int i= 0; i< (360/angle_resolution); i++)
    //   {
    //       angle+= d_angle; 
    //       pose.request.Y = y_center + radius*sin(angle);
    //       pose.request.Z = z_center + radius*cos(angle); 
    //       // comm.setCartesianPosition(pose, 0, false);
    //       // ROS_INFO("out");
    //   }

    ros::spin();
    return 0;
}
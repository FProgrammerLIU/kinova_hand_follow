#include "kinova_driver/kinova_comm.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "kinova_cartesian_position");
  ros::NodeHandle nh;

  boost::recursive_mutex api_mutex;
  bool is_first_init = false;
  std::string kinova_robotType = "j2n6s300";
  std::string kinova_robotName = "j2n6s300";
  kinova::KinovaComm comm(nh, api_mutex, is_first_init,kinova_robotType);
  // kinova::KinovaArm kinova_arm(comm, nh, kinova_robotType, kinova_robotName);  // create the action client

  double angle= 0;
  double radius = 0.1;            
  double angle_resolution = 10;                /* 两个采样点间的角度值，减小可提高轨迹规划成功率 */
  double d_angle = angle_resolution*3.14/180; /* 两个采样点间的弧度值 */
  kinova::KinovaPose pose;
    double y_center = -0.342748;
    double z_center = 0.505158;
    pose.X = 0.209288;
    pose.ThetaX = 1.63993;
    pose.ThetaY = 0.991298;
    pose.ThetaZ = 0.122501;
    //  采样圆上的点
    // kinova::kinovaArm kinova_arm;
    // kinova_arm.kinova_comm_.setCartesianPosition(pose, 0, false);
    for (int i= 0; i< (360/angle_resolution); i++)
    {
        angle+= d_angle; 
        pose.Y = y_center + radius*sin(angle);
        pose.Z = z_center + radius*cos(angle); 
        // pose.Y = y_center;
        // pose.Z = z_center;
        comm.setCartesianPosition(pose, 0, false);
        // pose.Y = -0.301191;
        // comm.setCartesianPosition(pose, 0, true);
        ROS_INFO("out");
    }

}
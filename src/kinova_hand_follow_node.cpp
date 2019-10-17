#include "kinova_driver/kinova_comm.h"
#include "tf/transform_datatypes.h"
namespace iqr {
class KinovaHandFollow {
public:

  KinovaHandFollow(ros::NodeHandle& nod);
  ros::Subscriber sub;
  double yaw_init, pitch_init, roll_init;
  // kinova::KinovaComm &arm_comm_;
private:

  void kinovaCallback(const geometry_msgs::Pose::ConstPtr& pose);

};
}
iqr::KinovaHandFollow::KinovaHandFollow(ros::NodeHandle& nod) {
  sub = nod.subscribe<geometry_msgs::Pose>("right_arm_pose", 1000, &iqr::KinovaHandFollow::kinovaCallback, this);
}

void iqr::KinovaHandFollow::kinovaCallback(const geometry_msgs::Pose::ConstPtr& pose) {

  tf::Matrix3x3 mat(tf::Quaternion(pose->orientation.x, pose->orientation.y, pose->orientation.z, pose->orientation.w));    
  double yaw, pitch, roll;    
  mat.getEulerYPR(yaw, pitch, roll);
  yaw_init = yaw;
  pitch_init = pitch;
  roll_init = roll;
  // kinova_comm_.setCartesianPosition(pose, 0, false);
  // comm.setCartesianPosition(pose, 0, false);
  ROS_INFO("out");
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "kinova_cartesian_position");
  ros::NodeHandle nh;
  iqr::KinovaHandFollow kinova_hand_follw(nh);
  boost::recursive_mutex api_mutex;
  bool is_first_init = false;
  std::string kinova_robotType = "j2n6s300";
  std::string kinova_robotName = "j2n6s300";
  kinova::KinovaComm comm(nh, api_mutex, is_first_init,kinova_robotType);
  // kinova::KinovaArm kinova_arm(comm, nh, kinova_robotType, kinova_robotName);  // create the action client

  double angle= 0;
  double radius = 0.1;            
  double angle_resolution = 10;                /*    */
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
        // comm.setCartesianPosition(pose, 0, false);
        // ROS_INFO("out");
    }
  ros::spin();
  return 0;
}
// KinovaArm::homeArmServiceCallback
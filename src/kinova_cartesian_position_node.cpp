#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include "kinova_msgs/ArmPoseAction.h"
#include "math.h"
#include "geometry_msgs/PoseStamped.h"
typedef actionlib::SimpleActionClient<kinova_msgs::ArmPoseAction> Client;
int main(int argc, char** argv)
{

  ros::init(argc, argv, "kinova_hand_follow_node");
  // create the action client
  // true causes the client to spin its own thread
  Client ac("/j2n6s300_driver/pose_action/tool_pose", true);
  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  double y_center = -0.24817;
  double z_center = 0.53327;              
  double angle= 0;
  double radius = 0.1;            
  double angle_resolution = 10;                /* 两个采样点间的角度值，减小可提高轨迹规划成功率 */
  double d_angle = angle_resolution*3.14/180; /* 两个采样点间的弧度值 */


  kinova_msgs::ArmPoseGoal goal;
  goal.pose.header.frame_id = "j2n6s300_link_base";
  goal.pose.pose.position.x = 0.32455;
  // goal.pose.pose.position.y = y_center;
  // goal.pose.pose.position.z = z_center;
  goal.pose.pose.orientation.x = 0.6474;
  goal.pose.pose.orientation.y = 0.31579;
  goal.pose.pose.orientation.z = 0.42669;
  goal.pose.pose.orientation.w = 0.54688;
  //  采样圆上的点
  for (int i= 0; i< (360/angle_resolution); i++)
  {
      angle+= d_angle; 
      y_center = y_center + radius*sin(angle);
      z_center = z_center + radius*cos(angle); 
      goal.pose.pose.position.y = y_center;
      goal.pose.pose.position.z = z_center;
      ac.sendGoal(goal);
      ac.waitForResult();
    ROS_INFO("out");

  }
  // goal.order = 20;
  // ac.sendGoal(goal);
  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));
  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");
  return 0;
}
//g++ test2.cpp -o test2.out -ldl

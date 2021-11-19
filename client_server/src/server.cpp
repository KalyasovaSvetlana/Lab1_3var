#include "ros/ros.h"
#include "my_service/SquidGame.h"
#include <std_msgs/String.h>
#include <iostream>
#include <string>

ros::ServiceServer service;
ros::Subscriber sub;
ros::Publisher pub;

std::string bridge (18, 'x');
std::string output = "";
int num = 0;
bool dead = false;

bool squid_game(my_service::SquidGame::Request &req,
  my_service::SquidGame::Response &res)
{
  if ((req.step != "r") && (req.step != "l"))
    return false;

  if (req.step[0] == bridge[num])
  {
    res.msg = "move next";
  }
  else
  {
    res.msg = "dead";
    dead = true; 
  }

  num ++;
  if (res.msg != "dead")
    pub.publish(req);

  return true;
}

void recieve(const std_msgs::String message)
{  
  if (num == 18) 
  {
    ROS_INFO ("Congratulations, you won!");
    ros::shutdown();
  }
  output += message.data; 
  ROS_INFO ("%s",output.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "squid_game_server");
  ros::NodeHandle n;

  srand(time(NULL));
  for (int i=0; i<18; i++) 
  {
    int a = rand() % 2;
    if (a == 0) 
      bridge[i] = 'l';
    else
      bridge[i] = 'r';   
  }  
  
  ROS_INFO ("The right way: %s",bridge.c_str());   
  
  	
  pub = n.advertise<std_msgs::String>("my_topic", 100);
  
  service = n.advertiseService ("squid_game", squid_game);

  sub = n.subscribe ("my_topic", 100, recieve);
  
  ros::Rate loop_rate(100);

  while (ros::ok()) 
  {
    if (dead == true)
      ros::shutdown(); 
    
    ros::spinOnce();
    loop_rate.sleep();
  }


  return 0;
}

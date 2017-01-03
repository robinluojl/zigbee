#include <ros/ros.h>
#include <zigbee/ZigbeeNode.h>
#include <zigbee/zigbeeAdapter.h>
#include <stdio.h>

using namespace std;

ZigbeeAdapter *adapter;
ZigbeeNode *zigbeeNode;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "zibgee_node");			//初始化ROS
  ros::NodeHandle nh;
	ros::NodeHandle nh_private("~");


	adapter = new ZigbeeAdapter;
	zigbeeNode = new ZigbeeNode(nh, nh_private);

/*	ros::Rate loop_rate(10);
	while(ros::ok())
	{
		ros::spinOnce();
      	loop_rate.sleep();
	}*/

	ros::AsyncSpinner spinner(4); // Use 4 threads
	spinner.start();
	ros::waitForShutdown();

	delete zigbeeNode;
	zigbeeNode = NULL;
  delete adapter;
  adapter = NULL;


  return 0;
}

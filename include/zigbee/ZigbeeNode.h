#ifndef _ZIGBEENODE_H
#define _ZIGBEENODE_H

#include <sstream>
#include <string>
#include <iostream>

#include <ros/ros.h>
#include "zigbee/zigbeeAdapter.h"
#include "zigbee/MsgCode.h"

#include "zigbee/GPS.h"
#include "std_msgs/Empty.h"
#include "zigbee/ShapeConfig.h"
#include "zigbee/Posi.h"
#include "zigbee/Ack.h"
#include "zigbee/Attitude.h"

using namespace std;
using namespace zigbee;

class ZigbeeNode
{
private:
	unsigned char OwnID;						//own ID
	int target_ID_num;							//需要几架飞机信息？
	unsigned char target_ID[10];		//最多同时获取10架飞机信息
/*************************************************************************/
//要发布给上层节点的msg
//value
	std_msgs::Empty InitShakeAck_value;
	GPS LocalFrame_value;
	ShapeConfig ShapeConfig_value;
	Posi TakeOff_value;
	Ack NoArguCmd_value;

	Posi OtherPosi_value;
	Posi OtherVel_value;
	Attitude OtherAttitude_value;
//Publisher
	ros::Publisher InitShakeAck_pub;
	ros::Publisher LocalFrame_pub;
	ros::Publisher ShapeConfig_pub;
	ros::Publisher TakeOff_pub;
	ros::Publisher NoArguCmd_pub;

	ros::Publisher OtherPosi_pub;
	ros::Publisher OtherVel_pub;
	ros::Publisher OtherAttitude_pub;
/**************************************************************************/
//从上层节点接收来的msg；
//Subscriber
	ros::Subscriber InitShake_sub;
	ros::Subscriber Ack_sub;
	//对外广播
	ros::Subscriber OwnPosi_sub;			//本机的局部位置（统一）
	ros::Subscriber OwnVel_sub;				//本机速度
	ros::Subscriber	OwnAttitude_sub;	//本机的姿态
//Subscriber callback function
public:
	void InitShake_sub_callback(GPS tmp);
	void Ack_sub_callback(Ack tmp);

	void OwnPosi_sub_callback(Posi tmp);
	void OwnVel_sub_callback(Posi tmp);
	void OwnAttitude_sub_callback(Attitude tmp);
// private function
private:
	void init_publisher(ros::NodeHandle& nh);
	void init_subscriber(ros::NodeHandle& nh);
	void init_param(ros::NodeHandle& nh_private);

	int CheckData(SDKFilter *a);
	int CheckID(SDKFilter *a);
public:
	ZigbeeNode(ros::NodeHandle& nh, ros::NodeHandle& nh_private);
	~ZigbeeNode();

	void HandleData(SDKFilter *a);

};

extern ZigbeeNode *zigbeeNode;

#endif

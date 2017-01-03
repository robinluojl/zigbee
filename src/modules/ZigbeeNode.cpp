#include "zigbee/ZigbeeNode.h"


const string target_ID_string[10]={"zigbee_target_ID0",
															"zigbee_target_ID1",
															"zigbee_target_ID2",
															"zigbee_target_ID3",
															"zigbee_target_ID4",
															"zigbee_target_ID5",
															"zigbee_target_ID6",
															"zigbee_target_ID7",
															"zigbee_target_ID8",
															"zigbee_target_ID9"

};

//收到上层节点的msg后立即编码送入fifo发送

void ZigbeeNode::InitShake_sub_callback(GPS tmp)
{
	int length;
	unsigned char a[100];

	length = encode_InitShake(tmp,a);
	for(int i = 0;i<length;i++)
	{
		adapter->send_queue->EnQueue(a[i]);
	}
}

void ZigbeeNode::Ack_sub_callback(Ack tmp)
{
	int length;
	unsigned char a[100];

	length = encode_CmdAck(tmp.msgID,tmp.targetID,a);
	for(int i = 0;i<length;i++)
	{
		adapter->send_queue->EnQueue(a[i]);
	}
}

void ZigbeeNode::OwnPosi_sub_callback(Posi tmp)
{
	int length;
	unsigned char a[100];

	length = encode_Posi(tmp,a);
	for(int i = 0;i<length;i++)
	{
		adapter->send_queue->EnQueue(a[i]);
	}
}

void ZigbeeNode::OwnVel_sub_callback(Posi tmp)
{
	int length;
	unsigned char a[100];

	length = encode_Vel(tmp,a);
	for(int i = 0;i<length;i++)
	{
		adapter->send_queue->EnQueue(a[i]);
	}
}

void ZigbeeNode::OwnAttitude_sub_callback(Attitude tmp)
{
	int length;
	unsigned char a[100];

	length = encode_Attitude(tmp,a);
	for(int i = 0;i<length;i++)
	{
		adapter->send_queue->EnQueue(a[i]);
	}
}

/************************************************************/
void ZigbeeNode::init_subscriber(ros::NodeHandle& nh)
{
	InitShake_sub = nh.subscribe<GPS>("UAV/InitShake",10,&ZigbeeNode::InitShake_sub_callback,this);
	Ack_sub = nh.subscribe<Ack>("UAV/Ack",10,&ZigbeeNode::Ack_sub_callback,this);

	OwnPosi_sub = nh.subscribe<Posi>("UAV/OwnPosi",10,&ZigbeeNode::OwnPosi_sub_callback,this);
	OwnVel_sub = nh.subscribe<Posi>("UAV/OwnVel",10,&ZigbeeNode::OwnVel_sub_callback,this);
	OwnAttitude_sub = nh.subscribe<Attitude>("UAV/Attitude",10,&ZigbeeNode::OwnAttitude_sub_callback,this);
}

void ZigbeeNode::init_publisher(ros::NodeHandle& nh)
{
	InitShakeAck_pub = nh.advertise<std_msgs::Empty>("GS/InitShakeAck",10);
	LocalFrame_pub = nh.advertise<GPS>("GS/LocalFrame",10);
	ShapeConfig_pub = nh.advertise<ShapeConfig>("GS/ShapeConfig",10);
	TakeOff_pub = nh.advertise<Posi>("GS/TakeOff",10);
	NoArguCmd_pub = nh.advertise<Ack>("GS/NoArguCmd",10);

	OtherPosi_pub = nh.advertise<Posi>("UAV/OtherPosi",10);
	OtherVel_pub = nh.advertise<Posi>("UAV/OtherVel",10);
	OtherAttitude_pub = nh.advertise<Attitude>("UAV/OtherAttitude",10);
}

void ZigbeeNode::init_param(ros::NodeHandle& nh_private)
{
	string serial_name;
  int baud_rate;
	int temp_ID;

	nh_private.param("zigbee_serial_name", serial_name, string("/dev/ttyUSB0"));
	nh_private.param("zigbee_baud_rate", baud_rate, 115200);
	nh_private.param("zigbee_own_ID",temp_ID, 0);
	OwnID = (unsigned char)temp_ID;
	nh_private.param("zigbee_target_ID_num",target_ID_num, 1);
	for(int i = 0;i<target_ID_num;i++)
	{
		nh_private.param(target_ID_string[i],temp_ID, i);
		target_ID[i] = (unsigned char)temp_ID;
	}

	adapter->init(serial_name,baud_rate);
}

//简单的和校验
int ZigbeeNode::CheckData(SDKFilter *a)
{
	if(CountSum(a->recvBuf,a->recvIndex-3) == a->recvBuf[a->recvIndex-3])
	{
		//cout<<"check sum ok"<<endl;
		return 0;
	}
	else
	{
		//cout<<"check sum error"<<endl;
		return 1;
	}

}

//根据消息的来源进行过滤
int ZigbeeNode::CheckID(SDKFilter *a)
{
	int flag_temp = 1;
	uint8_t temp;
	uint16_t sourceID = (((uint16_t)a->recvBuf[a->recvIndex-1])<<8)|a->recvBuf[a->recvIndex-2];

	search_uavIDbyZigbeeID(sourceID,temp);

	if(temp == 0x00)	//消息来自地面站,有效
		flag_temp = 0;
	else																//消息若来自其他主机，有效
	{
		for(int i = 0;i < target_ID_num;i++)
		{
			if(temp == target_ID[i])
			{
				flag_temp = 0;
				break;
			}
		}
	}
	return flag_temp;
}

/**************************************************************/
ZigbeeNode::ZigbeeNode(ros::NodeHandle& nh, ros::NodeHandle& nh_private)
{
	init_publisher(nh);
	init_subscriber(nh);
	init_param(nh_private);
}

ZigbeeNode::~ZigbeeNode()
{

}

//处理接收到的字符串流
void ZigbeeNode::HandleData(SDKFilter *a)
{
	MsgHeader *p_head = (MsgHeader*)(a->recvBuf);

	if((0 == CheckData(a)) && (0 == CheckID(a)))		//校验正确,且为待接收ID包
	{
		switch(p_head->msgID)
		{
			case msgID_InitShakeAck:
				InitShakeAck_pub.publish(InitShakeAck_value);			//随便发个啥告诉上层node收到InitShakeAck了
				break;
			case msgID_LocalFrame:
			//cout<<"decode LSA2"<<endl;
				decode_LocalFrame(LocalFrame_value,a->recvBuf);
				LocalFrame_pub.publish(LocalFrame_value);
				break;
			case msgID_ShapeConfig:
				decode_ShapeConfig(ShapeConfig_value,a->recvBuf);
				ShapeConfig_pub.publish(ShapeConfig_value);
				break;
			case msgID_TakeOff:
				decode_TakeOff(TakeOff_value,a->recvBuf);
				TakeOff_pub.publish(TakeOff_value);
				break;
			case msgID_Meet:																	//这些命令必然来自地面站
			case msgID_Fly:
			case msgID_Stop:
			case msgID_Return:
			case msgID_Land:
				NoArguCmd_value.msgID = p_head->msgID;
				NoArguCmd_value.targetID = 0x00;
				NoArguCmd_pub.publish(NoArguCmd_value);
				break;

			//收到其他飞机的位姿信息
			case msgID_Posi:
				decode_Posi(OtherPosi_value,a->recvBuf);
				OtherPosi_pub.publish(OtherPosi_value);
				break;
			case msgID_Vel:
				decode_Vel(OtherVel_value,a->recvBuf);
				OtherVel_pub.publish(OtherVel_value);
				break;
			case msgID_Attitude:
				decode_Attitude(OtherAttitude_value,a->recvBuf);
				OtherAttitude_pub.publish(OtherAttitude_value);
				break;
			default:
				break;
		}
	}

}

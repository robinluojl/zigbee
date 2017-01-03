#include "zigbee/MsgCode.h"
#include "zigbee/MyDataProcess.h"


MAP_ID map_id[10] = {	{0x00,0x0000},
                      {0x01,0x0100},
                      {0x02,0x3E14},
                      {0x03,0x7B28},
                      {0x04,0x0004},
                      {0x05,0x0005},
                      {0x06,0x0006},
                      {0x07,0x0007},
                      {0x08,0x0008},
                      {0x09,0x0009}};         //短地址为0x143E，则此处应该填写0x3E14


//InitShake message is sent from uav to gs
int encode_InitShake(GPS Tmp, unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

	a[length] = 0x00;  length++;
	a[length] = 0x00; length++;								//Ground Station ID

	a[length] = msgID_InitShake; length++;		//message ID
	//////数据拆分解码////////////
	EncodeDoubleData(&Tmp.latitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.longitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.altitude, a+length);length+=8;

	/**补上data length**/
	a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

	return length;
}

//LocalFrame message is sent from gs to uav
//send local frame to specific uav.if uavID is 0xff,that means broadcasting local frame to all uav.
//In general, we use 0xff when configurate
int encode_LocalFrame(GPS Tmp,unsigned char uavID,unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

  if(uavID!=0xff)
  {
    EncodeU16Data(&(map_id[uavID].ZigbeeID),a+length);length+=2;		//uav对应的zigbee短地址
  }
	else
  {
    a[length] = 0xff;  length++;
		a[length] = 0xff; length++;
  }

	a[length] = msgID_LocalFrame; length++;		//message ID
	//////数据拆分解码////////////
	EncodeDoubleData(&Tmp.latitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.longitude, a+length);length+=8;
	EncodeDoubleData(&Tmp.altitude, a+length);length+=8;
  /**补上data length**/
	a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

	return length;
}

//ShapeConfig message is sent from gs/UAV to uav
//send ShapeConfig to specific uav.if uavID is 0xff,that means broadcasting ShapeConfig to all uav.
//In general, we use 0xff when configurate
int encode_ShapeConfig(ShapeConfig tmp,unsigned char uavID,unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

  if(uavID!=0xff)
  {
    EncodeU16Data(&map_id[uavID].ZigbeeID, a+length);length+=2;		//uav对应的zigbee短地址
  }
  else
  {
    a[length] = 0xff;  length++;
    a[length] = 0xff; length++;
  }

	a[length] = msgID_ShapeConfig; length++;		//message ID
	a[length] = tmp.i;  length++;   a[length] = tmp.j;   length++;
	EncodeFloatData(&tmp.x, a+length);length+=4;
	EncodeFloatData(&tmp.y, a+length);length+=4;
	EncodeFloatData(&tmp.z, a+length);length+=4;
	EncodeFloatData(&tmp.fi, a+length);length+=4;
	a[length] = tmp.totol_uav_num;  length++;

  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

	return length;
}

//Posi数据只使用高度数据设置起飞高度
int encode_TakeOff(Posi tmp,unsigned char uavID,unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

  if(uavID!=0xff)
  {
    EncodeU16Data(&map_id[uavID].ZigbeeID, a+length);length+=2;		//uav对应的zigbee短地址
  }
  else
  {
    a[length] = 0xff;  length++;
    a[length] = 0xff; length++;
  }

	a[length] = msgID_TakeOff; length++;		//message ID

	//EncodeFloatData(&tmp.x, a+length);length+=4;           //无用
	//EncodeFloatData(&tmp.y, a+length);length+=4;           //无用
	EncodeFloatData(&tmp.z, a+length);length+=4;             //起飞命令只有高度有效
	//EncodeFloatData(&tmp.fi, a+length);length+=4;          //无用

  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

	return length;
}

//无参数命令编码
int encode_NoArguCmd(unsigned char msgID,unsigned char uavID,unsigned char *a)
{
  int length = 0;

  a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

  if(uavID!=0xff)
  {
    EncodeU16Data(&map_id[uavID].ZigbeeID, a+length);length+=2;		//uav对应的zigbee短地址
  }
  else
  {
    a[length] = 0xff;  length++;
    a[length] = 0xff; length++;
  }

  a[length] = msgID; length++;		//message ID
  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

	return length;
}

int encode_CmdAck(unsigned char msgID,unsigned char targetID,unsigned char *a)
{
	int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

	if(targetID!=0xff)
	{
		EncodeU16Data(&map_id[targetID].ZigbeeID, a+length);length+=2;
	}
	else
	{
		a[length] = 0xff;  length++;
		a[length] = 0xff; length++;
	}

	a[length] = msgID; length++;		//message ID
	//////数据////////////
  a[length] = 'a';length++;
  a[length] = 'c';length++;
  a[length] = 'k';length++;
  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

	return length;
}

void decode_InitShake(GPS &Tmp,unsigned char *a)
{
  int length = 5;
	u16 temp;

  DecodeDoubleData(&Tmp.latitude,a+length);length+=8;
  DecodeDoubleData(&Tmp.longitude,a+length);length+=8;
  DecodeDoubleData(&Tmp.altitude,a+length);length+=8;

  length+=1;
  DecodeU16Data(&temp, a+length);
  search_uavIDbyZigbeeID(temp,Tmp.ID);

}


void decode_LocalFrame(GPS &Tmp,unsigned char *a)
{
  int length = 5;

  DecodeDoubleData(&Tmp.latitude, a+length);length+=8;
  DecodeDoubleData(&Tmp.longitude, a+length);length+=8;
  DecodeDoubleData(&Tmp.altitude, a+length);length+=8;

}

void decode_ShapeConfig(ShapeConfig &tmp,unsigned char *a)
{
	int length = 5;
	tmp.i = a[length];length++;
	tmp.j = a[length];length++;

	DecodeFloatData(&tmp.x,a+length);length+=4;
	DecodeFloatData(&tmp.y,a+length);length+=4;
	DecodeFloatData(&tmp.z,a+length);length+=4;
	DecodeFloatData(&tmp.fi,a+length);length+=4;

	tmp.totol_uav_num = a[length];length++;
}

void decode_TakeOff(Posi &tmp,unsigned char *a)
{
	int length = 5;

	//DecodeFloatData(&tmp.x,a+length);length+=4;
	//DecodeFloatData(&tmp.y,a+length);length+=4;
	DecodeFloatData(&tmp.z,a+length);length+=4;
	//DecodeFloatData(&tmp.fi,a+length);length+=4;
}


int encode_Posi(Posi tmp,unsigned char *a)
{
  int length = 0;

	a[length] = 0xFD;  length++;							//包头
	a[length] = 0;   	length++;								//data length

  //广播
  a[length] = 0xff;  length++;
	a[length] = 0xff; length++;

	a[length] = msgID_Posi; length++;		//message ID

  EncodeFloatData(&tmp.x,a+length);length+=4;
	EncodeFloatData(&tmp.y,a+length);length+=4;
	EncodeFloatData(&tmp.z,a+length);length+=4;

  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

	return length;
}

int encode_Vel(Posi tmp,unsigned char *a)
{
  int length = 0;

  a[length] = 0xFD;  length++;							//包头
  a[length] = 0;   	length++;								//data length

  //广播
  a[length] = 0xff;  length++;
  a[length] = 0xff; length++;

  a[length] = msgID_Vel; length++;		//message ID

  EncodeFloatData(&tmp.x,a+length);length+=4;
  EncodeFloatData(&tmp.y,a+length);length+=4;
  EncodeFloatData(&tmp.z,a+length);length+=4;

  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

  return length;

}

int encode_Attitude(Attitude tmp,unsigned char *a)
{
  int length = 0;

  a[length] = 0xFD;  length++;							//包头
  a[length] = 0;   	length++;								//data length

  //广播
  a[length] = 0xff;  length++;
  a[length] = 0xff; length++;

  a[length] = msgID_Attitude; length++;		//message ID

  EncodeFloatData(&tmp.q0,a+length);length+=4;
  EncodeFloatData(&tmp.q1,a+length);length+=4;
  EncodeFloatData(&tmp.q2,a+length);length+=4;
  EncodeFloatData(&tmp.q3,a+length);length+=4;
  /**补上data length**/
  a[1] = (unsigned char)length-3;
  ////计算校验和//////
  a[length]=CountSum(a,length);length++;

  return length;
}

void decode_Posi(Posi &tmp,unsigned char *a)
{
  int length = 5;

  uint16_t zigbeeID;

  DecodeFloatData(&tmp.x,a+length);length+=4;
  DecodeFloatData(&tmp.y,a+length);length+=4;
  DecodeFloatData(&tmp.z,a+length);length+=4;

  length+=1;
  DecodeU16Data(&zigbeeID, a+length);
  search_uavIDbyZigbeeID(zigbeeID,tmp.ID);
}

void decode_Vel(Posi &tmp,unsigned char *a)
{
  int length = 5;

  uint16_t zigbeeID;

  DecodeFloatData(&tmp.x,a+length);length+=4;
  DecodeFloatData(&tmp.y,a+length);length+=4;
  DecodeFloatData(&tmp.z,a+length);length+=4;

  length+=1;
  DecodeU16Data(&zigbeeID, a+length);
  search_uavIDbyZigbeeID(zigbeeID,tmp.ID);
}

void decode_Attitude(Attitude &tmp,unsigned char *a)
{
  int length = 5;

  uint16_t zigbeeID;

  DecodeFloatData(&tmp.q0,a+length);length+=4;
  DecodeFloatData(&tmp.q1,a+length);length+=4;
  DecodeFloatData(&tmp.q2,a+length);length+=4;
  DecodeFloatData(&tmp.q3,a+length);length+=4;

  length+=1;
  DecodeU16Data(&zigbeeID, a+length);
  search_uavIDbyZigbeeID(zigbeeID,tmp.ID);
}

unsigned char CountSum(unsigned char *a,int length)
{
	unsigned char sum = 0x00;
	for(int i=0;i<length;i++)
	{
		sum+=a[i];
	}
	return sum;
}

//根据zigbee短地址得到与之对应的无人机ID   uavID为oxff代表是广播类型
bool search_uavIDbyZigbeeID(uint16_t zigbeeID,uint8_t &uavID)
{
  bool temp = false;
  uavID = 0xfd;
  if(zigbeeID==0xffff)
  {
    uavID = 0xff;
    temp =  true;
  }
  else
  {
    for(int i = 0;i<10;i++)
    {
      if(zigbeeID == map_id[i].ZigbeeID)
      {
        uavID = map_id[i].UAV_ID;
        temp = true;
        break;
      }
    }
  }
  return temp;
}

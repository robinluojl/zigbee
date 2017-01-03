#ifndef _MSGCODE_H
#define _MSGCODE_H

#include "zigbee/GPS.h"
#include "zigbee/ShapeConfig.h"
#include "zigbee/Posi.h"
#include "zigbee/Attitude.h"

using namespace zigbee;

#define msgID_InitShake         'A'
#define msgID_InitShakeAck      'A'
#define msgID_LocalFrame        'B'
#define msgID_LocalFrameAck     'B'
#define msgID_ShapeConfig       'C'
#define msgID_ShapeConfigAck    'C'
#define msgID_TakeOff           'D'
#define msgID_TakeOffAck        'D'

#define msgID_Meet              'E'
#define msgID_MeetAck           'E'
#define msgID_Fly               'F'
#define msgID_FlyAck            'F'
#define msgID_Return            'G'
#define msgID_ReturnAck         'G'
#define msgID_Land              'H'
#define msgID_LandAck           'H'
#define msgID_Stop              'I'
#define msgID_StopAck           'I'

#define msgID_Posi              'U'
#define msgID_Vel               'V'
#define msgID_Attitude          'W'


typedef struct MsgHeader
{
  unsigned char head;      //0xFD
  unsigned char length;
  unsigned char targetID_1;
  unsigned char targetID_2;

  unsigned char msgID;
  unsigned char reserve1;
  unsigned char reserve2;
  unsigned char reserve3;
}MsgHeader;

typedef struct MAP_ID
{
  unsigned char UAV_ID;
  uint16_t ZigbeeID;
}MAP_ID;

extern MAP_ID map_id[10];

/********************************************************/
int encode_InitShake(GPS Tmp, unsigned char *a);
int encode_LocalFrame(GPS Tmp,unsigned char uavID,unsigned char *a);
int encode_ShapeConfig(ShapeConfig tmp,unsigned char uavID,unsigned char *a);
int encode_TakeOff(Posi tmp,unsigned char uavID,unsigned char *a);
int encode_NoArguCmd(unsigned char msgID,unsigned char uavID,unsigned char *a);

int encode_CmdAck(unsigned char msgID,unsigned char targetID,unsigned char *a);

void decode_InitShake(GPS &Tmp,unsigned char *a);
void decode_LocalFrame(GPS &Tmp,unsigned char *a);
void decode_ShapeConfig(ShapeConfig &tmp,unsigned char *a);
void decode_TakeOff(Posi &tmp,unsigned char *a);
/******************************************************/

int encode_Posi(Posi tmp,unsigned char *a);
int encode_Vel(Posi tmp,unsigned char *a);
int encode_Attitude(Attitude tmp,unsigned char *a);
void decode_Posi(Posi &tmp,unsigned char *a);
void decode_Vel(Posi &tmp,unsigned char *a);
void decode_Attitude(Attitude &tmp,unsigned char *a);
/*****辅助函数****************/
unsigned char CountSum(unsigned char *a,int length);
bool search_uavIDbyZigbeeID(uint16_t zigbeeID,uint8_t &uavID);
//bool search_ZigbeeIDbyuavID(uint16_t zigbeeID,uint8_t &uavID);


#endif

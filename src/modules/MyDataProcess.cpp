#include "zigbee/MyDataProcess.h"





void EncodeDoubleData(double* f, unsigned char* buff)
{
	union data{
		double a;
		unsigned char b[8];
	}temp;
	temp.a = *f;
	memcpy(buff,temp.b,8);
}

void DecodeDoubleData(double* f, unsigned char* buff)
{
	union data{
		double a;
		unsigned char b[8];
	}temp;
	memcpy(temp.b,buff,8);
	*f = temp.a;
}

//Name:	EncodeFloatData()
//Describe:float�������޾��ȶ�ʧ�ؽس�4���ֽڣ�����buff��
//Input:float* f, unsigned char* buff
//Output:None
//Return:None
void EncodeFloatData(float* f, unsigned char* buff)
{
	u16 f_c[2];
	*(float*)f_c = *f;
	buff[0] = (f_c[0]&0x00ff);
	buff[1] = ((f_c[0]>>8)&0x00ff);
	buff[2] = (f_c[1]&0x00ff);
	buff[3] = ((f_c[1]>>8)&0x00ff);
}

//Name:	DecodeFloatData()
//Describe:����buff��ʼ��4���ֽ�λƴ��ת���ɸ�����
//Input:float* f, unsigned char* buff
//Output:None
//Return:None
void DecodeFloatData(float* f, unsigned char* buff)
{
	u16 f_c[2];
	f_c[0]=((buff[1]&0x00ff)<<8)|buff[0];
	f_c[1]=((buff[3]&0x00ff)<<8)|buff[2];
	*f=*(float *)f_c;
}

void EncodeS32Data(int32_t* f, unsigned char* buff)
{
	uint16_t f_c[2];
	*(int32_t*)f_c = *f;

	buff[0] = (f_c[0]&0x00ff);
	buff[1] = ((f_c[0]>>8)&0x00ff);
	buff[2] = (f_c[1]&0x00ff);
	buff[3] = ((f_c[1]>>8)&0x00ff);
}

void DecodeS32Data(int32_t* f, unsigned char* buff)
{
	uint16_t f_c[2];
	f_c[0]=((buff[1]&0x00ff)<<8)|buff[0];
	f_c[1]=((buff[3]&0x00ff)<<8)|buff[2];
	*f=*(int32_t *)f_c;
}


//Name:	EecodeS16Data()
//Describe:��16λ�з�������ת����2���ֽڷ���buff��
//Input:s16* f, unsigned char* buff
//Output:None
//Return:None
void EncodeS16Data(s16* f, unsigned char* buff)
{
	buff[0] = (uint8_t)(*f);
	buff[1] = (uint8_t)((*f)>>8);
}

//Name:	DecodeS16Data()
//Describe:��buff�е�2���ֽ�ƴ��Ϊs16
//Input:s16* f, unsigned char* buff
//Output:None
//Return:None
void DecodeS16Data(s16* f, unsigned char* buff)
{
	*f = (s16)(((u16)(buff[1])<<8)|buff[0]);
}


//Name:	EecodeU16Data()
//Describe:��16λ�޷�������ת����2���ֽڷ���buff��
//Input:u16* f, unsigned char* buff
//Output:None
//Return:None
void EncodeU16Data(u16* f, unsigned char* buff)
{
	buff[0] = (uint8_t)(*f);
	buff[1] = (uint8_t)((*f)>>8);
}

//Name:	DecodeU16Data()
//Describe:��buff�е�2���ֽ�ƴ��Ϊu16
//Input:u16* f, unsigned char* buff
//Output:None
//Return:None
void DecodeU16Data(u16* f, unsigned char* buff)
{
	*f = ((u16)(buff[1])<<8)|buff[0];
}

//Name:	EecodeU12Data()
//Describe:��12λ�޷�������ת����2���ֽڷ���buff��,ͬʱ����0x30
//Input:u16* f, unsigned char* buff
//Output:None
//Return:None
void EncodeU12Data(u16* f, unsigned char* buff)
{
	buff[0] = (uint8_t)((*f)&0x003F)+0x30;
	buff[1] = (uint8_t)(((*f)&0x0FC0)>>6)+0x30;
}

//Name:	DecodeU16Data()
//Describe:��buff�е�2���ֽڼ���0x30ƴ��Ϊu12
//Input:u16* f, unsigned char* buff
//Output:None
//Return:None
void DecodeU12Data(u16* f, unsigned char* buff)
{
	*f = ((u16)(buff[1]-0x30)<<6)|(buff[0]-0x30);
}

//Name:	EecodeU16to5Dec()
//Describe:��16λ�޷�������ת����5λʮ����ascll���ƣ���u16=12345��ת����Ϊa[5]={'1'��'2'��'3'��'4'��'5'��}
//Input:u16* f, unsigned char* buff
//Output:None
//Return:None
void EncodeU16to5Dec(u16* f, unsigned char* buff)
{
	s8 i;
	u16 temp = *f;
	for(i=4;i>=0;i--)
	{
		buff[i]=temp%10+0x30;
		temp = temp/10;
	}
}

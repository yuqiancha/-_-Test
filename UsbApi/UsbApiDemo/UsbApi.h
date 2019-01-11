#ifndef USBAPI_H_
#define USBAPI_H_

#include <iostream>
#include <Windows.h>
#include <SetupAPI.h>

#include <queue>
#include <cstring>
using std::queue;
using namespace std;


#include <CyAPI.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")


extern CCyUSBDevice *USBDevice;
extern CCyBulkEndPoint *BulkInEpt;			//�豸-->PC
extern CCyBulkEndPoint *BulkOutEpt;			//PC-->�豸
extern CCyControlEndPoint *ControlEndPoint;
extern char Register80H;					//���ƼĴ���

void DC_SendCMD(int addr, int value);//���豸�Ĵ���д��
bool DC_DeviceOpen();//��ʼ�������豸
void DC_ReadEnable();//ʹ�ܶ����ܣ����豸���޷���ȡ���ݣ�������ô˺����ſ��Զ�ȡ���ݣ�����һ��һֱ��Ч
void DC_ReadData(unsigned char* buf, LONG &length);//���豸�������ݣ����ݱ�����buf���飬������������Ϊlength
void DC_WriteData(unsigned char* buf, LONG &length);//��buf����д���豸��д�볤��Ϊlength

#endif // !USBAPI_H_
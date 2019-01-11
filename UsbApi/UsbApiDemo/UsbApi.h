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
extern CCyBulkEndPoint *BulkInEpt;			//设备-->PC
extern CCyBulkEndPoint *BulkOutEpt;			//PC-->设备
extern CCyControlEndPoint *ControlEndPoint;
extern char Register80H;					//控制寄存器

void DC_SendCMD(int addr, int value);//向设备寄存器写数
bool DC_DeviceOpen();//初始化并打开设备
void DC_ReadEnable();//使能读功能，打开设备后无法读取数据，必须调用此函数才可以读取数据，调用一次一直有效
void DC_ReadData(unsigned char* buf, LONG &length);//从设备读出数据，数据保存在buf数组，读出数据数量为length
void DC_WriteData(unsigned char* buf, LONG &length);//将buf数组写入设备，写入长度为length

#endif // !USBAPI_H_
#pragma once
#include "Export.h"

class USB_API  myUsbDLL : public Export
{
public:
	myUsbDLL();
	~myUsbDLL();

public:
	virtual void SendCMD(int addr, int value);//向设备寄存器写数
	virtual bool DeviceOpen();//初始化并打开设备
	virtual void ReadEnable();//使能读功能，打开设备后无法读取数据，必须调用此函数才可以读取数据，调用一次一直有效
	virtual void ReadData(unsigned char* buf, long &length);//从设备读出数据，数据保存在buf数组，读出数据数量为length
	virtual void WriteData(unsigned char* buf, long &length);//将buf数组写入设备，写入长度为length
	virtual void Release();

	virtual void Fun1();
	virtual void Fun2();
	virtual void Fun3();
	virtual void Fun4();
	virtual void Fun5();
	virtual void Fun6();
	virtual void Fun7();

private:
	CCyUSBDevice *USBDevice;
	CCyBulkEndPoint *BulkInEpt;				//设备-->PC
	CCyBulkEndPoint *BulkOutEpt;			//PC-->设备
	CCyControlEndPoint *ControlEndPoint;
	char Register80H = 0x00;

};


USB_API Export* _stdcall CreateMyUsbObj()
{
	return new myUsbDLL;
}

USB_API void _stdcall DestroyMyUsbObj(Export* pExport)
{
	pExport->Release();
}

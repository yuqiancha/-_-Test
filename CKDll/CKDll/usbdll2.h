#pragma once
#include "Export.h"

class USB_API  myUsbDLL : public Export
{
public:
	myUsbDLL();
	~myUsbDLL();

public:
	virtual void SendCMD(int addr, int value);//���豸�Ĵ���д��
	virtual bool DeviceOpen();//��ʼ�������豸
	virtual void ReadEnable();//ʹ�ܶ����ܣ����豸���޷���ȡ���ݣ�������ô˺����ſ��Զ�ȡ���ݣ�����һ��һֱ��Ч
	virtual void ReadData(unsigned char* buf, long &length);//���豸�������ݣ����ݱ�����buf���飬������������Ϊlength
	virtual void WriteData(unsigned char* buf, long &length);//��buf����д���豸��д�볤��Ϊlength
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
	CCyBulkEndPoint *BulkInEpt;				//�豸-->PC
	CCyBulkEndPoint *BulkOutEpt;			//PC-->�豸
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

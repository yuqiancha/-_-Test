#include "stdafx.h"
#include "usbdll2.h"

myUsbDLL::myUsbDLL()
{
	printf("测控模块设备驱动加载成功！\r\n");
}


myUsbDLL::~myUsbDLL()
{
	printf("测控模块设备驱动Release OK \r\n");
}


void myUsbDLL::Release()
{
	delete this;
}

void myUsbDLL::Fun1()
{
}

void myUsbDLL::Fun2()
{
}

void myUsbDLL::Fun3()
{
}

void myUsbDLL::Fun4()
{
}

void myUsbDLL::Fun5()
{
}

void myUsbDLL::Fun6()
{
}

void myUsbDLL::Fun7()
{
}

void myUsbDLL::SendCMD(int addr, int value)
{
	try
	{
		ControlEndPoint = USBDevice->ControlEndPt;
		ControlEndPoint->Target = TGT_ENDPT;
		ControlEndPoint->ReqType = REQ_VENDOR;
		ControlEndPoint->Direction = DIR_FROM_DEVICE;
		ControlEndPoint->Index = 0;


		ControlEndPoint->ReqCode = addr;
		ControlEndPoint->Value = value;

		unsigned char buf[8];
		ZeroMemory(buf, 8);
		LONG buflen = 8;
		ControlEndPoint->XferData(buf, buflen);
		printf("SendCMD %x %x!", addr, value);
		Sleep(10);

	}
	catch (const std::exception&)
	{
		printf("SendCMD Error!");
	}

}

bool myUsbDLL::DeviceOpen()
{
	USBDevice = new CCyUSBDevice(NULL, CYUSBDRV_GUID, 1);

	if (USBDevice->IsOpen()) {
		printf("设备打开成功");

		int epnum = USBDevice->EndPointCount();
		for (int j = 1; j < epnum; j++) {
			bool bIn = ((USBDevice->EndPoints[j]->Address & 0x80) == 0x80);
			bool bBulk = (USBDevice->EndPoints[j]->Attributes == 2);
			if (bBulk&&bIn) {
				BulkInEpt = (CCyBulkEndPoint *)USBDevice->EndPoints[j];
				printf("BulkInEpt address is %x \n", BulkInEpt->Address);
			}
			if (bBulk && !bIn) {
				BulkOutEpt = (CCyBulkEndPoint *)USBDevice->EndPoints[j];
				printf("BulkOutEpt address is %x \n", BulkOutEpt->Address);

			}
		}
		return true;
	}
	else
	{
		printf("设备打开失败");
		return false;
	}

	return 1;
}

void myUsbDLL::ReadEnable()
{
	USBDevice->Reset();
	SendCMD(0x80, 0x01);
	SendCMD(0x80, 0x00);

	Register80H = Register80H | 0x04;
	SendCMD(0x80, Register80H);
}

void myUsbDLL::ReadData(unsigned char* buf, long &length)
{
	if (BulkInEpt)
		BulkInEpt->XferData(buf, length);	//读出到PC
	if (length == 4096)
	{//全速读取
	}
	else if (length == 0)
	{//缓存区已读空
		Sleep(100);
	}
	else
	{//半包状态
		Sleep(10);
	}
}


void myUsbDLL::WriteData(unsigned char* buf, long &length)
{
	Register80H = Register80H | 0x02;
	SendCMD(0x80, Register80H);
	Register80H = Register80H & 0xFD;
	SendCMD(0x80, Register80H);

	unsigned char * sendbuf = (unsigned char *)malloc((length + 20) * sizeof(unsigned char));
	sendbuf[0] = 0x1d;
	sendbuf[1] = 0x00;
	sendbuf[2] = (length & 0xff00) >> 8;
	sendbuf[3] = length & 0xff;

	memcpy(sendbuf + 4, buf, length);


	sendbuf[length + 4] = 0xC0;
	sendbuf[length + 5] = 0xDE;
	sendbuf[length + 6] = 0xC0;
	sendbuf[length + 7] = 0xDE;
	sendbuf[length + 8] = 0xC0;
	sendbuf[length + 9] = 0xDE;
	sendbuf[length + 10] = 0xC0;
	sendbuf[length + 11] = 0xDE;
	sendbuf[length + 12] = 0xC0;
	sendbuf[length + 13] = 0xDE;
	sendbuf[length + 14] = 0xC0;
	sendbuf[length + 15] = 0xDE;
	sendbuf[length + 16] = 0xC0;
	sendbuf[length + 17] = 0xDE;
	sendbuf[length + 18] = 0xC0;
	sendbuf[length + 19] = 0xDE;
	LONG sendlen = length + 20;
	if (BulkOutEpt) {
		BulkOutEpt->XferData(sendbuf, sendlen);	//写到板卡
		printf("写入成功; %d\n", sendlen);
	}
	else
	{
		printf("写入失败，设备为空");
	}
}
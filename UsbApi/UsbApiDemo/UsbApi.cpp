#include "UsbApi.h"

CCyUSBDevice *USBDevice;
CCyBulkEndPoint *BulkInEpt;				//�豸-->PC
CCyBulkEndPoint *BulkOutEpt;			//PC-->�豸
CCyControlEndPoint *ControlEndPoint;
char Register80H = 0x00;

void DC_SendCMD(int addr, int value)
{
	try
	{
		ControlEndPoint = USBDevice->ControlEndPt;
		//	ControlEndPoint->Target = TGT_DEVICE;
		ControlEndPoint->Target = TGT_ENDPT;
		ControlEndPoint->ReqType = REQ_VENDOR;
		//	ControlEndPoint->Direction = DIR_TO_DEVICE;
		ControlEndPoint->Direction = DIR_FROM_DEVICE;
		ControlEndPoint->Index = 0;


		ControlEndPoint->ReqCode = addr;
		ControlEndPoint->Value = value;

		unsigned char buf[8];
		ZeroMemory(buf, 8);
		LONG buflen = 8;
		ControlEndPoint->XferData(buf, buflen);
		printf("DC_SendCMD %x %x!",addr,value);
		Sleep(10);

	}
	catch (const std::exception&)
	{
		printf("DC_SendCMD Error!");
	}
	
}

bool DC_DeviceOpen()
{
	USBDevice = new CCyUSBDevice(NULL, CYUSBDRV_GUID, 1);

	if (USBDevice->IsOpen()) {
		printf("�豸�򿪳ɹ�");

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
		printf("�豸��ʧ��");
		return false;
	}

	return 1;
}

void DC_ReadEnable()
{
	USBDevice->Reset();
	DC_SendCMD(0x80, 0x01);
	DC_SendCMD(0x80, 0x00);

	Register80H = Register80H | 0x04;
	DC_SendCMD(0x80, Register80H);
}

void DC_ReadData(unsigned char* buf, LONG &length)
{
	BulkInEpt->XferData(buf, length);	//������PC
}

void DC_WriteData(unsigned char* buf, LONG &length)
{
	Register80H = Register80H | 0x02;
	DC_SendCMD(0x80, Register80H);
	Register80H = Register80H & 0xFD;
	DC_SendCMD(0x80, Register80H);

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
		BulkOutEpt->XferData(sendbuf, sendlen);	//д���忨
		printf("д��ɹ�; %d\n", sendlen);
	}
	else
	{
		printf("д��ʧ�ܣ��豸Ϊ��");
	}
}
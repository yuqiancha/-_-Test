#include "stdafx.h"
#include "usbdll2.h"


IAntennaInterface*  CreateAntennInstance() {
	return new myUsbDLL;
}

void  DestoryAntennInstance(IAntennaInterface*& instance) {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

myUsbDLL::myUsbDLL()
{
	printf("���ģ���豸�������سɹ���\r\n");
}


myUsbDLL::~myUsbDLL()
{
	printf("���ģ���豸����Release OK \r\n");
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

void myUsbDLL::ADFunc_Enable()
{
	SendCMD(0x83, 0x01);
}


bool __stdcall myUsbDLL::PowerOn_Target()
{
	SendCMD(0x84, 0x01);
	return true;
}


bool __stdcall myUsbDLL::PowerOff_Target()
{
	SendCMD(0x84, 0x00);
	return true;
}

void myUsbDLL::SendCMD(int addr, int value)
{
	cmdlock.lock();
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
		printf("SendCMD %x %x! \r\n", addr, value);
		Sleep(10);

	}
	catch (const std::exception&)
	{
		printf("SendCMD Error!");
	}
	cmdlock.unlock();

}

volatile double dataRe_AD02[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

DWORD WINAPI RetADFun(LPVOID ptr)
{
	myUsbDLL* pTr = (myUsbDLL*)(ptr);

	double b2 = 0;
	double b12 = 0;
	double b9 = 0;
	double b0 = 0;
	double b13 = 0;
	double b6 = 0;
	double b10 = 0;
	double b1 = 0;
	double b14 = 0;
	double b15 = 0;
	double b4 = 0;
	double b5 = 0;
	while (pTr->RecvTag)
	{
		if (pTr->mRecalFun) {

			pTr->mylock.lock();

#pragma region Ԥ��+7V
			if (pTr->dataRe_AD01[9] > 3)
			{
				if (pTr->dataRe_AD01[9] < 4)
				{
					b9 = 2 * pTr->dataRe_AD01[9];
				}
			}

			if (pTr->dataRe_AD01[0] > 2.506)
			{
				if (pTr->dataRe_AD01[0] < 2.6)
				{
					b0 = (pTr->dataRe_AD01[0] - 2.506) / 0.555;
				}
			}

			pTr->mRecalFun("Ԥ��+7V", b9, b0);
			//	pTr->mRecalFun("Ԥ��+7V", (2 * pTr->dataRe_AD01[9]), (pTr->dataRe_AD01[0] - 2.506) / 0.555);
#pragma endregion

#pragma region Ԥ��-5V

			if (pTr->dataRe_AD01[10] > 1.8)
			{
				if (pTr->dataRe_AD01[10] < 2.2)
				{
					b10 = ((4.33*pTr->dataRe_AD01[10]) - 13.33);
				}
			}

			if (pTr->dataRe_AD01[1] > 2.552)
			{
				if (pTr->dataRe_AD01[1] < 2.65) {
					b1 = (pTr->dataRe_AD01[1] - 2.552) / 0.555;
				}
			}
			pTr->mRecalFun("Ԥ��-5V", b10, b1);

#pragma endregion

#pragma region ����+4V
			if (pTr->dataRe_AD01[12] > 1.5)
			{
				if (pTr->dataRe_AD01[12] < 2.25)
				{
					b12 = 2 * pTr->dataRe_AD01[12];
				}
			}

			if (pTr->dataRe_AD01[2] > 2.451)
			{
				if (pTr->dataRe_AD01[2] < 2.73)
				{
					b2 = (pTr->dataRe_AD01[2] - 2.451) / 0.555;
				}
			}
			pTr->mRecalFun("����+4V", b12, b2);
#pragma endregion

#pragma region ����+12V

			if (pTr->dataRe_AD01[14] > 2.75)
			{
				if (pTr->dataRe_AD01[14] < 3.25)
				{
					b14 = 4 * pTr->dataRe_AD01[14];
				}
			}

			if (pTr->dataRe_AD01[4] > 3.8)
			{
				if (pTr->dataRe_AD01[4] < 4.2) {
					b4 = pTr->dataRe_AD01[4] / 2;
				}
			}
			pTr->mRecalFun("����+12V", b14, b4);
			//	pTr->mRecalFun("����+12V", (4 * pTr->dataRe_AD01[14]), pTr->dataRe_AD01[4] / 2);

#pragma endregion

#pragma region ����+5V

			if (pTr->dataRe_AD01[15] > 2.25)
			{
				if (pTr->dataRe_AD01[15] < 2.75)
				{
					b15 = 2 * pTr->dataRe_AD01[15];
				}
			}

			if (pTr->dataRe_AD01[5] > 0)
			{
				if (pTr->dataRe_AD01[5] < 0.4) {
					b5 = pTr->dataRe_AD01[5] / 2;
				}
			}
			pTr->mRecalFun("����+5V", b15, b5);
			//	pTr->mRecalFun("����+5V", (2 * pTr->dataRe_AD01[15]), pTr->dataRe_AD01[5] / 2);

#pragma endregion

#pragma region ����-5V
			if (pTr->dataRe_AD01[13] > 1.8)
			{
				if (pTr->dataRe_AD01[13] < 2.2)
				{
					b13 = ((4.33*pTr->dataRe_AD01[13]) - 13.33);
				}
			}

			if (pTr->dataRe_AD01[6] > 2.526)
			{
				if (pTr->dataRe_AD01[6] < 2.65)
				{
					b6 = (pTr->dataRe_AD01[6] - 2.526) / 0.555;
				}
			}
			pTr->mRecalFun("����-5V", b13, b6);

#pragma endregion

			//bool ErrorDetected;
			//if (ErrorDetected) {
			//	int ErrorCode1;
			//	int ErrorCode2;
			//	pTr->mRecalFun("У����ERROR", ErrorCode1, ErrorCode2);
			//}
					//pTr->mRecalFun("Ԥ��+7V", pTr->dataRe_AD01[9], pTr->dataRe_AD01[0]);

					//pTr->mRecalFun("Ԥ��-5V", pTr->dataRe_AD01[10], pTr->dataRe_AD01[1]);

					//pTr->mRecalFun("����+4V", pTr->dataRe_AD01[12], pTr->dataRe_AD01[2]);

					//pTr->mRecalFun("����+12V", pTr->dataRe_AD01[14], pTr->dataRe_AD01[4]);

					//pTr->mRecalFun("����+5V", pTr->dataRe_AD01[15], pTr->dataRe_AD01[5]);

					//pTr->mRecalFun("����-5V", pTr->dataRe_AD01[13], pTr->dataRe_AD01[6]);

					//pTr->mRecalFun("END", 0, 0);

			pTr->mylock.unlock();

		}
		Sleep(pTr->freqtime);
	}

	return 1;


}



DWORD WINAPI GetDataFun(LPVOID ptr)
{
	myUsbDLL* pTr = (myUsbDLL*)(ptr);
	int count = 0;

	int size = 4096 * 128;
	unsigned char* dealbuf = (unsigned char *)malloc(size * sizeof(char));
	int pos = 0;
	memset(dealbuf, 0, size);

	int adsize = 200;
	unsigned char* adbuf01 = (unsigned char *)malloc(200 * sizeof(char));
	int adpos1 = 0;
	memset(adbuf01, 0, adsize);

	unsigned char* adbuf02 = (unsigned char *)malloc(200 * sizeof(char));
	int adpos2 = 0;
	memset(adbuf02, 0, adsize);

	double value = 0;

	pTr->Register80H = pTr->Register80H | 0x04;
	pTr->SendCMD(0x80, pTr->Register80H);

	while (pTr->RecvTag)
	{

		try {
			count++;
			unsigned char * buf = (unsigned char *)malloc(4096 * sizeof(char));
			LONG length = 4096;

			if (pTr->BulkInEpt)
			{
				pTr->BulkInEpt->XferData(buf, length);	//������PC

				if (length > 0)
				{//ȫ�ٶ�ȡ
					memcpy(dealbuf + pos, buf, length);
					pos += length;
					if (pos >= 4096)
					{
						//		printf("Seq:%d %d ReturnLen = %d %x %x %x %x \n", count, pos / 4096, length, dealbuf[0], dealbuf[1], dealbuf[2], dealbuf[3]);

						if (dealbuf[0] == 0xff && dealbuf[1] == 0x08)
						{
							unsigned char *bufsav = (unsigned char *)malloc(4092 * sizeof(char));
							memcpy(bufsav, dealbuf + 4, 4092);

							for (int i = 0; i < 6; i++)
							{
								if (bufsav[i * 682 + 0] == 0x1D && bufsav[i * 682 + 1] == 0x06)
								{
									int num = bufsav[i * 682 + 2] * 256 + bufsav[i * 682 + 3];//��Чλ
									memcpy(adbuf01, bufsav + 4 + i * 682, num);
									adpos1 += num;

									if (adpos1 >= 20)
									{
										pTr->mylock.lock();
										for (int k = 2; k < 10; k++)
										{
											int temp = (adbuf01[2 * k] & 0x7f) * 256 + adbuf01[2 * k + 1];

											if ((adbuf01[2 * k] & 0x80) == 0x80)
											{
												temp = 0x8000 - temp;
											}
											value = temp;
											value = 10 * (value / 32767);

											if ((buf[2 * k] & 0x80) == 0x80)
											{
												pTr->dataRe_AD01[k - 2] = -value;
											}
											else
											{
												pTr->dataRe_AD01[k - 2] = value;
												//	dataRe_AD02[k - 2] = value;
											}
										}
										pTr->mylock.unlock();
										memmove(adbuf01, adbuf01 + 20, adpos1 - 20);
										adpos1 -= 20;

									}
								}
								else if (bufsav[i * 682 + 0] == 0x1D && bufsav[i * 682 + 1] == 0x07)
								{
									int num = bufsav[i * 682 + 2] * 256 + bufsav[i * 682 + 3];//��Чλ
									memcpy(adbuf02, bufsav + 4 + i * 682, num);
									adpos2 += num;

									if (adpos2 >= 20)
									{
										pTr->mylock.lock();
										for (int k = 2; k < 10; k++)
										{
											int temp = (adbuf02[2 * k] & 0x7f) * 256 + adbuf02[2 * k + 1];

											if ((adbuf02[2 * k] & 0x80) == 0x80)
											{
												temp = 0x8000 - temp;
											}
											value = temp;
											value = 10 * (value / 32767);
											if ((buf[2 * k] & 0x80) == 0x80)
											{
												pTr->dataRe_AD01[k - 2 + 8] = -value;
											}
											else
											{
												pTr->dataRe_AD01[k - 2 + 8] = value;
											}
										}
										pTr->mylock.unlock();

										memmove(adbuf02, adbuf02 + 20, adpos2 - 20);
										adpos2 -= 20;
									}

								}
								else
								{

								}

							}

							free(bufsav);
							bufsav = nullptr;
						}

						memmove(dealbuf, dealbuf + 4096, pos - 4096);
						pos -= 4096;
					}
				}
				else if (length == 0)
				{
					Sleep(100);

				}
				else
				{//���״̬
					Sleep(50);
				}
			}

			free(buf);
			buf = NULL;

		}
		catch (...)
		{
			printf("Transfer Error");
		}
	}

	return 1;
}


bool myUsbDLL::DeviceOpen()
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

		SendCMD(0x84, 0x00);

		SendCMD(0x85, 0x00);

		return true;
	}
	else
	{
		printf("�豸��ʧ��");
		return false;
	}

	return 1;
}

void myUsbDLL::ReadEnable()
{
	Sleep(100);
	SendCMD(0x80, 0x01);
	SendCMD(0x80, 0x00);
	Sleep(100);

	USBDevice->Reset();

	RecvTag = true;

	DWORD dwThreadId;
	HANDLE hThread1;

	hThread1 = CreateThread(NULL, NULL, GetDataFun, this, 0, &dwThreadId);
	CloseHandle(hThread1);

	HANDLE hThread2;

	hThread2 = CreateThread(NULL, NULL, RetADFun, this, 0, &dwThreadId);
	CloseHandle(hThread2);
}

void myUsbDLL::ReadData(unsigned char* buf, long &length)
{
	if (BulkInEpt)
		BulkInEpt->XferData(buf, length);	//������PC
	if (length == 4096)
	{//ȫ�ٶ�ȡ

	}
	else if (length == 0)
	{//�������Ѷ���
		Sleep(100);
	}
	else
	{//���״̬
		Sleep(10);
	}
}

void myUsbDLL::WriteData_CS(unsigned char* buf, long &length)
{
	Register81H = Register81H | 0x01;
	SendCMD(0x81, Register81H);
	Register81H = Register81H & 0x7e;
	SendCMD(0x81, Register81H);


	WriteData_WithChan(0, buf, length);

	//Register80H = Register80H | 0x02;
	//SendCMD(0x80, Register80H);
	//Register80H = Register80H & 0xFD;
	//SendCMD(0x80, Register80H);

	//int SendSize = length + 20;
	//int add = length % 4;
	//if (add)
	//{
	//	SendSize += (4 - add);
	//}
	//unsigned char * sendbuf = (unsigned char *)malloc(SendSize * sizeof(unsigned char));
	//memset(sendbuf, 0, SendSize);

	//sendbuf[0] = 0x1d;
	//sendbuf[1] = 0x00;
	//sendbuf[2] = (length & 0xff00) >> 8;
	//sendbuf[3] = length & 0xff;
	//memcpy(sendbuf + 4, buf, length);

	//sendbuf[length + 4] = 0xC0;
	//sendbuf[length + 5] = 0xDE;
	//sendbuf[length + 6] = 0xC0;
	//sendbuf[length + 7] = 0xDE;
	//sendbuf[length + 8] = 0xC0;
	//sendbuf[length + 9] = 0xDE;
	//sendbuf[length + 10] = 0xC0;
	//sendbuf[length + 11] = 0xDE;
	//sendbuf[length + 12] = 0xC0;
	//sendbuf[length + 13] = 0xDE;
	//sendbuf[length + 14] = 0xC0;
	//sendbuf[length + 15] = 0xDE;
	//sendbuf[length + 16] = 0xC0;
	//sendbuf[length + 17] = 0xDE;
	//sendbuf[length + 18] = 0xC0;
	//sendbuf[length + 19] = 0xDE;

	//long sendlen = SendSize;

	//if (BulkOutEpt) {
	//	BulkOutEpt->XferData(sendbuf, sendlen);	//д���忨
	//	printf("д��ɹ�; %d\n", sendlen);
	//}
	//else
	//{
	//	printf("д��ʧ�ܣ��豸Ϊ��");
	//}
}

void myUsbDLL::WriteData_YXQ1(unsigned char* buf, long &length)
{
	Register81H = Register81H | 0x02;
	SendCMD(0x81, Register81H);
	Register81H = Register81H & 0x7d;
	SendCMD(0x81, Register81H);
	WriteData_WithChan(1, buf, length);

}

void myUsbDLL::WriteData_YXQ2(unsigned char* buf, long &length)
{
	Register81H = Register81H | 0x04;
	SendCMD(0x81, Register81H);
	Register81H = Register81H & 0x7b;
	SendCMD(0x81, Register81H);
	WriteData_WithChan(2, buf, length);
}

void myUsbDLL::WriteData_DY1(unsigned char* buf, long &length)
{
	Register81H = Register81H | 0x08;
	SendCMD(0x81, Register81H);
	Register81H = Register81H & 0x77;
	SendCMD(0x81, Register81H);
	WriteData_WithChan(3, buf, length);
}

void myUsbDLL::WriteData_DY2(unsigned char* buf, long &length)
{
	Register81H = Register81H | 0x10;
	SendCMD(0x81, Register81H);
	Register81H = Register81H & 0x6f;
	SendCMD(0x81, Register81H);
	WriteData_WithChan(4, buf, length);
}

void myUsbDLL::WriteData(unsigned char* buf, long &length)
{
	Register80H = Register80H | 0x02;
	SendCMD(0x80, Register80H);
	Register80H = Register80H & 0xFD;
	SendCMD(0x80, Register80H);

	int SendSize = length + 20;
	int add = length % 4;
	if (add)
	{
		SendSize += (4 - add);
	}
	unsigned char * sendbuf = (unsigned char *)malloc(SendSize * sizeof(unsigned char));
	memset(sendbuf, 0, SendSize);

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
	LONG sendlen = SendSize;
	if (BulkOutEpt) {
		BulkOutEpt->XferData(sendbuf, sendlen);	//д���忨
		printf("д��ɹ�; %d\n", sendlen);
	}
	else
	{
		printf("д��ʧ�ܣ��豸Ϊ��");
	}
}

void myUsbDLL::WriteData_WithChan(int chan, unsigned char* buf, long &length)
{
	Register80H = Register80H | 0x02;
	SendCMD(0x80, Register80H);
	Register80H = Register80H & 0xFD;
	SendCMD(0x80, Register80H);

	int SendSize = length + 20;
	int more = length % 4;
	int add = 0;
	if (more)
	{
		add = 4 - more;
		SendSize += add;
	}
	unsigned char * sendbuf = (unsigned char *)malloc(SendSize * sizeof(unsigned char));
	memset(sendbuf, 0, SendSize);

	sendbuf[0] = 0x1d;
	sendbuf[1] = chan;
	sendbuf[2] = (length & 0xff00) >> 8;
	sendbuf[3] = length & 0xff;

	memcpy(sendbuf + 4, buf, length);

	sendbuf[length + add + 4] = 0xC0;
	sendbuf[length + add + 5] = 0xDE;
	sendbuf[length + add + 6] = 0xC0;
	sendbuf[length + add + 7] = 0xDE;
	sendbuf[length + add + 8] = 0xC0;
	sendbuf[length + add + 9] = 0xDE;
	sendbuf[length + add + 10] = 0xC0;
	sendbuf[length + add + 11] = 0xDE;
	sendbuf[length + add + 12] = 0xC0;
	sendbuf[length + add + 13] = 0xDE;
	sendbuf[length + add + 14] = 0xC0;
	sendbuf[length + add + 15] = 0xDE;
	sendbuf[length + add + 16] = 0xC0;
	sendbuf[length + add + 17] = 0xDE;
	sendbuf[length + add + 18] = 0xC0;
	sendbuf[length + add + 19] = 0xDE;

	LONG sendlen = SendSize;

	if (BulkOutEpt) {
		BulkOutEpt->XferData(sendbuf, sendlen);	//д���忨
		printf("д��ɹ�; %d\n", sendlen);
		for (int j = 0; j < sendlen; j++)printf("%2x,", sendbuf[j]);
	}
	else
	{
		printf("д��ʧ�ܣ��豸Ϊ��");
	}
}

bool __stdcall myUsbDLL::Open(const char* Addr) {
	DeviceOpen();
	ReadEnable();

	return true;
}

void  __stdcall myUsbDLL::Close() {
	PowerOff_Target();
	RecvTag = false;
}

bool  __stdcall myUsbDLL::IsOpen() {
	return USBDevice->IsOpen();
}


bool  __stdcall myUsbDLL::SetAntennaMode(myUsbDLL::AntennaStatus mode)
{
	SendCMD(0x85, 0x01);//ֹͣ
	SendCMD(0x85, 0x00);//
	Sleep(500);

	SendCMD(0x83, 0x01);
	SendCMD(0x84, 0x01);

	Fun_DY1();
	Sleep(500);

	Fun_DY2();
	Sleep(500);

	//д���������
	switch (mode)
	{
	case IAntennaInterface::SendMode:
		tmode = 0x01;
		break;
	case IAntennaInterface::ReceiveMode:
		tmode = 0x02;
		break;
	case IAntennaInterface::SendReveiveMode:
		tmode = 0x80;
		break;
	case IAntennaInterface::NullMode:
		tmode = 0x80;
		break;
	default:
		break;
	}
	return true;
}


bool  __stdcall myUsbDLL::SetAntennaSendAndReceiveSwitchTime(double mTime) {
	tc = mTime * 50000;
	return true;
}

//
// ���õ�ѹ��������
//
bool __stdcall  myUsbDLL::SetVotateLimit(int tag, double LoVotage, double HiVotage, double LoCurrent, double HiCurrent)
{
	return true;
}

//
// ��ʼѭ��
//
bool __stdcall  myUsbDLL::Start()
{
	LONG bytesToWrite = 31;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	memset(buf, 0x00, bytesToWrite);

	buf[0] = tmode;//����ģʽ

	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	buf[4] = 0xff;//�շ��л�ģʽ�еĳɹ����Դ���

	buf[5] = 0x10;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0xff;//�շ��л�ģʽ�еĿɵ���ʱ

	//buf[5] = (tc & 0xff000000) >> 24;
	//buf[6] = (tc & 0xff0000) >> 16;
	//buf[7] = (tc & 0xff00) >> 8;
	//buf[8] = tc & 0xff;//�շ��л�ģʽ�еĿɵ���ʱ

	buf[9] = 0xff;
	buf[10] = 0xff;
	buf[11] = 0xff;
	buf[12] = 0xff;
	buf[13] = 0xff;
	buf[14] = 0xff;//��������ʱ��

	buf[15] = 0xff;
	buf[16] = 0xff;
	buf[17] = 0x00;
	buf[18] = 0x00;//+7V��ѹ
	buf[19] = 0xff;
	buf[20] = 0xff;
	buf[21] = 0x00;
	buf[22] = 0x00;//+7V����
	buf[23] = 0xff;
	buf[24] = 0xff;
	buf[25] = 0x00;
	buf[26] = 0x00;//+4V��ѹ
	buf[27] = 0xff;
	buf[28] = 0xff;
	buf[29] = 0x00;
	buf[30] = 0x00;//+4V����

	WriteData_CS(buf, bytesToWrite);

	return true;
}


//
// ֹͣѭ��
//
bool __stdcall  myUsbDLL::Stop()
{
	SendCMD(0x85, 0x01);
	return true;
}

bool __stdcall  myUsbDLL::Fun_YXQ1_Not()
{
	string temp = "3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f3f1f00";
	char *p;
	p = (char*)(temp.c_str());

	LONG bytesToWrite = 217;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	for (int i = 0; i < 217; i++)
	{
		int nValue;
		sscanf_s(p + 2 * i, "%2x", &nValue);
		buf[i] = nValue;
	}

	WriteData_YXQ1(buf, bytesToWrite);
	return true;
}

bool __stdcall  myUsbDLL::Fun_YXQ1_Yes()
{
	string temp = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	char *p;
	p = (char*)(temp.c_str());

	LONG bytesToWrite = 217;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	for (int i = 0; i < 217; i++)
	{
		int nValue;
		sscanf_s(p + 2 * i, "%2x", &nValue);
		buf[i] = nValue;
	}

	WriteData_YXQ1(buf, bytesToWrite);
	return true;
}

bool __stdcall  myUsbDLL::Fun_YXQ2_Not()
{
	string temp = "3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f00";
	char *p;
	p = (char*)(temp.c_str());

	LONG bytesToWrite = 217;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	for (int i = 0; i < 217; i++)
	{
		int nValue;
		sscanf_s(p + 2 * i, "%2x", &nValue);
		buf[i] = nValue;
	}

	WriteData_YXQ2(buf, bytesToWrite);
	return true;
}

bool __stdcall  myUsbDLL::Fun_YXQ2_Yes()
{
	string temp = "00200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000200020002000";
	char *p;
	p = (char*)(temp.c_str());

	LONG bytesToWrite = 217;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	for (int i = 0; i < 217; i++)
	{
		int nValue;
		sscanf_s(p + 2 * i, "%2x", &nValue);
		buf[i] = nValue;
	}

	WriteData_YXQ2(buf, bytesToWrite);
	return true;
}

bool __stdcall  myUsbDLL::Fun_DY1()
{
	LONG bytesToWrite = 5;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	//00c0ffffc0
	buf[0] = 0x00;
	buf[1] = 0xc0;
	buf[2] = 0xff;
	buf[3] = 0xff;
	buf[4] = 0xc0;

	WriteData_DY1(buf, bytesToWrite);

	return true;
}

bool __stdcall  myUsbDLL::Fun_DY2()
{
	LONG bytesToWrite = 5;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	//00c0ffffc0
	buf[0] = 0xff;
	buf[1] = 0x3f;
	buf[2] = 0x00;
	buf[3] = 0xf0;
	buf[4] = 0x30;
	WriteData_DY2(buf, bytesToWrite);
	return true;
}


bool  __stdcall myUsbDLL::SetAmpAndPhpTable(const char* fileName) {

	//unsigned char input_VZ[108];//��ֵ--˥��
	//unsigned char input_XW[108];//��λ--�Ƕ�
	////�ӿڹ����������������˥����ʵ��ֵ������ת��Ϊ���֣�Ȼ�󷢳�ȥ
	//UCHAR b1 = 0x0;
	//UCHAR b2 = 0x0;
	//for (int i = 0; i < 108; i++)
	//{		
	//	
	//}
	return true;
}



bool  __stdcall myUsbDLL::SetSingleAmpAndPhp(unsigned ch, double amp, double php) {
	return true;
}


bool  __stdcall myUsbDLL::SetPhaseStatus(unsigned ch, double php) {
	return true;
}



void  __stdcall myUsbDLL::BindRecalFun(myUsbDLL::VoltageAndCurrentFun fun) {
	mRecalFun = fun;
}


void  __stdcall myUsbDLL::SetCallFunTimeInter(double time) {
	freqtime = time;
}


bool __stdcall myUsbDLL::Write(const unsigned char* data, long len) {
	WriteData(const_cast<unsigned char*>(data), len);
	return true;
}


unsigned __stdcall myUsbDLL::Read(unsigned char*& data, long& len) {
	ReadData(data, len);
	return len;
}
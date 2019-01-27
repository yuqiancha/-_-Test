#include "stdafx.h"
#include "usbdll2.h"

struct MyStruct
{
	int Chan;
	float Amp;
	float Php;
	bool Used;
};

MyStruct* myStruct;
const int Size = 108;

IAntennaInterface*  CreateAntennInstance() {

	return new myUsbDLL;
}

void  DestoryAntennInstance(IAntennaInterface*& instance) {
	if (instance) {

		//		Logger::shutdown();

		delete instance;
		instance = nullptr;
	}

}

myUsbDLL::myUsbDLL()
{
	//printf("测控模块设备驱动加载成功！\r\n");
}


myUsbDLL::~myUsbDLL()
{
	//	Logger::getRoot().shutdown();
		//printf("测控模块设备驱动Release OK \r\n");
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

bool myUsbDLL::DeviceOpen()
{
	try {
		myStruct = (struct MyStruct*)malloc(sizeof(struct MyStruct)*Size);
		for (int m = 0; m < Size; m++)
		{
			myStruct[m].Chan = m;
			myStruct[m].Amp = 0;
			myStruct[m].Php = 0;
			myStruct[m].Used = false;
		}

		//PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("D:\\CKDLL_LOG\\log.properties"));
		//LOG4CPLUS_DEBUG(logger, "log start here in DeviceOpen");

		USBDevice = new CCyUSBDevice(NULL, CYUSBDRV_GUID, 1);

		//printf("before is open!");
		if (USBDevice->IsOpen()) {
			//printf("设备打开成功");

			int epnum = USBDevice->EndPointCount();
			for (int j = 1; j < epnum; j++) {
				bool bIn = ((USBDevice->EndPoints[j]->Address & 0x80) == 0x80);
				bool bBulk = (USBDevice->EndPoints[j]->Attributes == 2);
				if (bBulk&&bIn) {
					BulkInEpt = (CCyBulkEndPoint *)USBDevice->EndPoints[j];
					//printf("BulkInEpt address is %x \n", BulkInEpt->Address);
				}
				if (bBulk && !bIn) {
					BulkOutEpt = (CCyBulkEndPoint *)USBDevice->EndPoints[j];
					//printf("BulkOutEpt address is %x \n", BulkOutEpt->Address);
				}
			}

			//SendCMD(0x84, 0x01);
			////LOG4CPLUS_DEBUG(logger, "上电..");

			return true;
		}
		else
		{
			//printf("设备打开失败");
			return false;
		}
		return true;
	}
	catch (...)
	{
		return false;
	}

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

DWORD WINAPI RetADFun(LPVOID ptr)
{
	myUsbDLL* pTr = (myUsbDLL*)(ptr);
	//LOG4CPLUS_DEBUG(pTr->logger, "RetADFun..");
	while (pTr->RecvTag)
	{
		//if (pTr->shutpower)
		//{
		//	if (pTr->mRecalFun)
		//	{
		//		pTr->mRecalFun("CKLOG -- 电压电流超限，关闭设备，请重新开始测试", 0, 0);
		//		pTr->Stop();
		//		pTr->Close();
		//	}
		//	break;
		//}

		if (pTr->mRecalFun) {
			pTr->mylock.lock();
			double a = 2 * pTr->dataRe_AD01[9];
			double b = (pTr->dataRe_AD01[0] - 2.494) / 0.555;

			pTr->mRecalFun("预放+7V", a, b);
			if (a < pTr->LoV_7)
			{
				pTr->mRecalFun("预放+7V 电压小于设定值 设定值，实际值   Error", pTr->LoV_7, a);
			}
			if (a > pTr->HiV_7)
			{
				pTr->mRecalFun("预放+7V 电压大于设定值 设定值，实际值   Error", pTr->HiV_7, a);
				pTr->mRecalFun("ABORT", 0, 0);
			}

			if (b < pTr->LoC_7)
			{
				pTr->mRecalFun("预放+7V 电流小于设定值 设定值，实际值  Error", pTr->LoC_7, b);
			}
			if (b > pTr->HiC_7)
			{
				pTr->mRecalFun("预放+7V 电流大于设定值 设定值，实际值  Error", pTr->HiC_7, b);
				pTr->mRecalFun("ABORT", 0, 0);
			}

			pTr->mRecalFun("预放-5V", (4.33*pTr->dataRe_AD01[10]) - 13.33, (pTr->dataRe_AD01[1] - 2.555) / 0.555);

			double c = 2 * pTr->dataRe_AD01[12];
			double d = (pTr->dataRe_AD01[2] - 2.529) / 0.555;
			d = 1.4*d;
			pTr->mRecalFun("波控+4V", c, d);

			if (c < pTr->LoV_4)
			{
				pTr->mRecalFun("波控+4V 电压小于设定值 设定值，实际值 Error", pTr->LoV_4, c);
			}
			if (c > pTr->HiV_4)
			{
				pTr->mRecalFun("波控+4V 电压大于设定值 设定值，实际值 Error", pTr->HiV_4, c);
				pTr->mRecalFun("ABORT", 0, 0);
			}
			if (d < pTr->LoC_4)
			{
				pTr->mRecalFun("波控+4V 电流小于设定值 设定值，实际值 Error", pTr->LoC_4, d);
			}
			if (d > pTr->HiC_4)
			{
				pTr->mRecalFun("波控+4V 电流大于设定值 设定值，实际值 Error", pTr->HiC_4, d);
				pTr->mRecalFun("ABORT", 0, 0);
			}

			pTr->mRecalFun("波控+12V", (3 * pTr->dataRe_AD01[4]), (pTr->dataRe_AD01[14] / 2) - 0.012);
			pTr->mRecalFun("波控+5V", (2 * pTr->dataRe_AD01[15]), pTr->dataRe_AD01[5] / 2);
			pTr->mRecalFun("波控-5V", (4.33*pTr->dataRe_AD01[13]) - 13.33, (pTr->dataRe_AD01[6] - 2.534) / 0.555);

			/*	//LOG4CPLUS_DEBUG(pTr->logger, "预放+7V " << "电压：" << 2 * pTr->dataRe_AD01[9] << "  电流：" << (pTr->dataRe_AD01[0] - 2.506) / 0.555);
				//LOG4CPLUS_DEBUG(pTr->logger, "预放-5V " << "电压：" << (4.33*pTr->dataRe_AD01[10]) - 13.33 << "  电流：" << (pTr->dataRe_AD01[1] - 2.552) / 0.555);
				//LOG4CPLUS_DEBUG(pTr->logger, "波控+4V " << "电压：" << 2 * pTr->dataRe_AD01[12] << "  电流：" << (pTr->dataRe_AD01[2] - 2.451) / 0.555);
				//LOG4CPLUS_DEBUG(pTr->logger, "波控+12V" << "电压：" << (3 * pTr->dataRe_AD01[4]) << "  电流：" << pTr->dataRe_AD01[14] / 2);
				//LOG4CPLUS_DEBUG(pTr->logger, "波控+5V " << "电压：" << (2 * pTr->dataRe_AD01[15]) << "  电流：" << pTr->dataRe_AD01[5] / 2);
				//LOG4CPLUS_DEBUG(pTr->logger, "波控-5V " << "电压：" << (4.33*pTr->dataRe_AD01[13]) - 13.33 << "  电流：" << (pTr->dataRe_AD01[6] - 2.526) / 0.555);
			*/
			pTr->mRecalFun("END", 0, 0);

			pTr->mylock.unlock();

		}
		Sleep(pTr->freqtime);
	}

	//LOG4CPLUS_DEBUG(pTr->logger, "RetADFun Exit..");
	return 1;


}

DWORD WINAPI GetDataFun(LPVOID ptr)
{
	myUsbDLL* pTr = (myUsbDLL*)(ptr);

	//LOG4CPLUS_DEBUG(pTr->logger, "GetDataFun..");

	int count = 0;

	int size = 4096 * 128;
	unsigned char* dealbuf = (unsigned char *)malloc(size * sizeof(char));
	int pos = 0;
	memset(dealbuf, 0, size);

	double value = 0;

	pTr->Register80H = pTr->Register80H | 0x04;
	pTr->SendCMD(0x80, pTr->Register80H);

	pTr->SendCMD(0x83, 0x01);//AD打开
	//LOG4CPLUS_DEBUG(pTr->logger, "AD打开..");

	char path[128];
	GetCurrentDirectoryA(128, path);
	string temp = path;
	string SYSTEMPATH = temp + "\\源码数据\\";

	string command;
	command = "mkdir " + SYSTEMPATH;
	system(command.c_str());

	//SYSTEMTIME st;
	//GetLocalTime(&st);
	//FILE *fileFF08_1D06;
	//FILE *fileFF08_1D07;
	//FILE *fileFF08;

	//string tt = "";
	//char array[10];

	//_itoa_s(st.wYear, array, 10);
	//tt = tt + array + "年";
	//_itoa_s(st.wMonth, array, 10);
	//tt = tt + array + "月";
	//_itoa_s(st.wDay, array, 10);
	//tt = tt + array + "日";


	//_itoa_s(st.wHour, array, 10);
	//tt = tt + array + "时";
	//_itoa_s(st.wMinute, array, 10);
	//tt = tt + array + "分";
	//_itoa_s(st.wSecond, array, 10);
	//tt = tt + array + "秒";

	//string tempstrFF08_1D06 = SYSTEMPATH + tt + "1d06.dat";
	//string tempstrFF08_1D07 = SYSTEMPATH + tt + "1d07.dat";
	//string tempstrFF08 = SYSTEMPATH + tt + "FF08.dat";

	//fopen_s(&fileFF08_1D06, tempstrFF08_1D06.c_str(), "wb+");
	//fopen_s(&fileFF08_1D07, tempstrFF08_1D07.c_str(), "wb+");
	//fopen_s(&fileFF08, tempstrFF08.c_str(), "wb+");

	queue<unsigned char> que06;
	queue<unsigned char> que07;
	while (pTr->RecvTag)
	{
		try {
			count++;
			unsigned char * buf = (unsigned char *)malloc(4096 * sizeof(char));
			LONG length = 4096;

			if (pTr->BulkInEpt)
			{
				pTr->BulkInEpt->XferData(buf, length);	//读出到PC

				if (length > 0)
				{//全速读取
					memcpy(dealbuf + pos, buf, length);
					pos += length;
					if (pos >= 4096)
					{
						//		//printf("Seq:%d %d ReturnLen = %d %x %x %x %x \n", count, pos / 4096, length, dealbuf[0], dealbuf[1], dealbuf[2], dealbuf[3]);

						if (dealbuf[0] == 0xff && dealbuf[1] == 0x08)
						{
							unsigned char *bufsav = (unsigned char *)malloc(4092 * sizeof(char));
							memcpy(bufsav, dealbuf + 4, 4092);

							/*try {
								fwrite(dealbuf, sizeof(char), 4096, fileFF08);
								fflush(fileFF08);
							}
							catch (exception &e)
							{
								//printf(e.what());
							}*/

							for (int i = 0; i < 6; i++)
							{

								if (bufsav[i * 682 + 0] == 0x1D && bufsav[i * 682 + 1] == 0x00)
								{
									int num = bufsav[i * 682 + 2] * 256 + bufsav[i * 682 + 3];//有效位
									if (num == 2) {
										//	pTr->mRecalFun("校验码ERROR", (int)bufsav[i * 682 + 4], (int)bufsav[i * 682 + 5]);
									}
								}
								if (bufsav[i * 682 + 0] == 0x1D && bufsav[i * 682 + 1] == 0x01)
								{
									int num = bufsav[i * 682 + 2] * 256 + bufsav[i * 682 + 3];//有效位
									if (num == 1) {
										pTr->TagAB = bufsav[i * 682 + 4];
										//LOG4CPLUS_DEBUG(pTr->logger, "TagAB is :"<<pTr->TagAB);
									}
								}
								else if (bufsav[i * 682 + 0] == 0x1D && bufsav[i * 682 + 1] == 0x06)
								{
									int num = bufsav[i * 682 + 2] * 256 + bufsav[i * 682 + 3];//有效位

								/*	try {
										fwrite(bufsav + i * 682 + 4, sizeof(char), num, fileFF08_1D06);
										fflush(fileFF08_1D06);
									}
									catch (exception &e)
									{
										//printf(e.what());
									}*/

									pTr->mylock.lock();
									for (int j = 0; j < num; j++)
									{
										que06.push(bufsav[4 + i * 682 + j]);
									}
									if (que06.size() >= 20)
									{
										unsigned char *buf06 = (unsigned char *)malloc(20 * sizeof(char));

										for (int t = 0; t < 20; t++)
										{
											buf06[t] = que06.front();
											que06.pop();
										}

										pTr->tspan1[0] = buf06[0];
										pTr->tspan1[1] = buf06[1];
										pTr->tspan1[2] = buf06[2];
										pTr->tspan1[3] = buf06[3];
										for (int k = 2; k < 10; k++)
										{
											int temp = (buf06[2 * k] & 0x7f) * 256 + buf06[2 * k + 1];

											if ((buf06[2 * k] & 0x80) == 0x80)
											{
												temp = 0x8000 - temp;
											}
											value = temp;
											value = 10 * (value / 32767);

											if ((buf06[2 * k] & 0x80) == 0x80)
											{
												pTr->dataRe_AD01[k - 2] = -value;
											}
											else
											{
												pTr->dataRe_AD01[k - 2] = value;
											}

											/*					if (k == 4) {
																	if ((pTr->dataRe_AD01[k - 2] < 2) || (pTr->dataRe_AD01[k - 2] > 3))
																	{
																		//printf("Here is Error %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n", buf06[0], buf06[1], buf06[2], buf06[3], buf06[4], buf06[5], buf06[6], buf06[7], buf06[8], buf06[9], buf06[10],
																			buf06[11], buf06[12], buf06[13], buf06[14], buf06[15], buf06[16], buf06[17], buf06[18], buf06[19]);
																	}
																}*/
										}
										free(buf06);
										buf06 = NULL;
									}

									pTr->mylock.unlock();
								}
								else if (bufsav[i * 682 + 0] == 0x1D && bufsav[i * 682 + 1] == 0x07)
								{
									int num = bufsav[i * 682 + 2] * 256 + bufsav[i * 682 + 3];//有效位

								/*	try {
										fwrite(bufsav + i * 682 + 4, sizeof(char), num, fileFF08_1D07);
										fflush(fileFF08_1D07);
									}
									catch (exception &e)
									{
										//printf(e.what());
									}*/

									pTr->mylock.lock();

									for (int j = 0; j < num; j++)
									{
										que07.push(bufsav[4 + i * 682 + j]);
									}
									if (que07.size() >= 20)
									{
										unsigned char *buf07 = (unsigned char *)malloc(20 * sizeof(char));

										for (int t = 0; t < 20; t++)
										{
											buf07[t] = que07.front();
											que07.pop();
										}
										pTr->tspan2[0] = buf07[0];
										pTr->tspan2[1] = buf07[1];
										pTr->tspan2[2] = buf07[2];
										pTr->tspan2[3] = buf07[3];
										for (int k = 2; k < 10; k++)
										{
											int temp = (buf07[2 * k] & 0x7f) * 256 + buf07[2 * k + 1];

											if ((buf07[2 * k] & 0x80) == 0x80)
											{
												temp = 0x8000 - temp;
											}
											value = temp;
											value = 10 * (value / 32767);

											if ((buf07[2 * k] & 0x80) == 0x80)
											{
												pTr->dataRe_AD01[k - 2 + 8] = -value;
											}
											else
											{
												pTr->dataRe_AD01[k - 2 + 8] = value;
											}
										}
										free(buf07);
										buf07 = NULL;
									}

									pTr->mylock.unlock();
								}
								else if (bufsav[i * 682 + 0] == 0x1D && bufsav[i * 682 + 1] == 0x0F)
								{
									//空闲帧
								}
								else
								{

								}

							}

							free(bufsav);
							bufsav = NULL;
						}
						else
						{
							pTr->mRecalFun("设备收到数据ERROR", 0, 0);
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
				{//半包状态
					Sleep(50);
				}
			}

			free(buf);
			buf = NULL;

		}
		catch (...)
		{
			//printf("Transfer Error");
		}
	}

	//LOG4CPLUS_DEBUG(pTr->logger, "GetDataFun Exit..");

	return 1;
}

void myUsbDLL::ReadEnable()
{
	//LOG4CPLUS_DEBUG(logger, "ReadEnable..");
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

void myUsbDLL::WriteData_CS(unsigned char* buf, long &length)
{
	//LOG4CPLUS_DEBUG(logger, "WriteData_CS..");
	Register81H = Register81H | 0x01;
	SendCMD(0x81, Register81H);
	Register81H = Register81H & 0x7e;
	SendCMD(0x81, Register81H);
	WriteData_WithChan(0, buf, length);

}

void myUsbDLL::WriteData_YXQ1(unsigned char* buf, long &length)
{
	//LOG4CPLUS_DEBUG(logger, "WriteData_YXQ1..");
	Register81H = Register81H | 0x02;
	SendCMD(0x81, Register81H);
	Register81H = Register81H & 0x7d;
	SendCMD(0x81, Register81H);
	WriteData_WithChan(1, buf, length);

}

void myUsbDLL::WriteData_YXQ2(unsigned char* buf, long &length)
{
	//LOG4CPLUS_DEBUG(logger, "WriteData_YXQ2..");
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
		BulkOutEpt->XferData(sendbuf, sendlen);	//写到板卡
		//printf("写入成功; %d\n", sendlen);
	}
	else
	{
		//printf("写入失败，设备为空");
	}
}

void myUsbDLL::WriteData_WithChan(int chan, unsigned char* buf, long &length)
{
	//LOG4CPLUS_DEBUG(logger, "WriteData_WithChan..");
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
		BulkOutEpt->XferData(sendbuf, sendlen);	//写到板卡
		//printf("写入成功; %d\n", sendlen);


		char outbuf[4096];
		int pos = 0;
		for (int j = 0; j < sendlen; j++)
		{
			//	//printf("%2x,", sendbuf[j]);
			pos += sprintf_s(outbuf + pos, 4096, "%02x,", sendbuf[j]);
		}
		outbuf[pos - 1] = '\n';
		//LOG4CPLUS_DEBUG(logger, outbuf);

	}
	else
	{
		//printf("写入失败，设备为空");
	}
}

bool __stdcall myUsbDLL::Open(const char* Addr) {
	//LOG4CPLUS_DEBUG(logger, "Open");
	bool tag = DeviceOpen();
	if (tag == false)
	{
		return false;
	}
	else
	{
		ReadEnable();
		return true;

	}

}



bool  __stdcall myUsbDLL::IsOpen() {
	//LOG4CPLUS_DEBUG(logger, "IsOpen");
	try {
		bool tag = USBDevice->IsOpen();
		return tag;
	}
	catch (...)
	{
		return false;
	}
}

bool  __stdcall myUsbDLL::SetAntennaMode(myUsbDLL::AntennaStatus mode)
{
	//LOG4CPLUS_DEBUG(logger, "SetAntennaMode.. mode is " << mode);

	//写入测试数据
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
	//LOG4CPLUS_DEBUG(logger, "SetAntennaSendAndReceiveSwitchTime.. mTime is " << mTime);
	tc = mTime * 50000;
	return true;
}

bool __stdcall  myUsbDLL::SetVotateLimit(int tag, double LoVotage, double HiVotage, double LoCurrent, double HiCurrent)
{
	//LOG4CPLUS_DEBUG(logger, "SetVotateLimit.. " << tag << "低压" << LoVotage << "高压" << HiVotage << "低电流" << LoCurrent << "高电流" << HiCurrent);


	if (tag == 0)//+4V
	{
		LoV_4 = LoVotage;
		HiV_4 = HiVotage;
		LoC_4 = LoCurrent;
		HiC_4 = HiCurrent;
	}

	if (tag == 1)//+7V
	{
		LoV_7 = LoVotage;
		HiV_7 = HiVotage;
		LoC_7 = LoCurrent;
		HiC_7 = HiCurrent;
	}

	return true;
}

bool __stdcall myUsbDLL::Continue()
{
	//mRecalFun("CKLOG --Continue()", 0, 0);
	Fun_DY1();
	Sleep(100);

	Fun_DY2();
	Sleep(100);

	Fun_YXQ_SET();

	LONG bytesToWrite = 31;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	memset(buf, 0x00, bytesToWrite);

	buf[0] = tmode;//测试模式

	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	buf[4] = 0xff;//收发切换模式中的成功测试次数
	buf[5] = (tc & 0xff000000) >> 24;
	buf[6] = (tc & 0xff0000) >> 16;
	buf[7] = (tc & 0xff00) >> 8;
	buf[8] = tc & 0xff;//收发切换模式中的可调延时

	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x26;
	buf[13] = 0x25;
	buf[14] = 0xa0;//高温老练时间

	int h4v = (HiV_4 / 2)*3276.7;
	int l4v = (LoV_4 / 2)*3276.7;
	int h4c = (HiC_4 * 0.555 + 2.529)*3276.7;
	int l4c = (LoC_4 * 0.555 + 2.529)*3276.7;

	int h7v = (HiV_7 / 2)*3276.7;
	int l7v = (LoV_7 / 2)*3276.7;
	int h7c = (HiC_7 * 0.555 + 2.494)*3276.7;
	int l7c = (LoC_7 * 0.555 + 2.494)*3276.7;

	buf[15] = (h7v & 0xff00) >> 8;
	buf[16] = h7v & 0xff;
	buf[17] = (l7v & 0xff00) >> 8;
	buf[18] = l7v & 0xff;//+7V电压
	buf[19] = (h4v & 0xff00) >> 8;
	buf[20] = h4v & 0xff;
	buf[21] = (l4v & 0xff00) >> 8;
	buf[22] = l4v & 0xff;//+4V电压
	buf[23] = (h7c & 0xff00) >> 8;
	buf[24] = h7c & 0xff;
	buf[25] = (l7c & 0xff00) >> 8;
	buf[26] = l7c & 0xff;//+7V电流
	buf[27] = (h4c & 0xff00) >> 8;
	buf[28] = h4c & 0xff;
	buf[29] = (l4c & 0xff00) >> 8;
	buf[30] = l4c & 0xff;//+4V电流

	WriteData_CS(buf, bytesToWrite);

	return true;
}

bool __stdcall  myUsbDLL::Start()
{
	//LOG4CPLUS_DEBUG(logger, "Start..");

	shutpower = false;

	SendCMD(0x84, 0x01);
	//LOG4CPLUS_DEBUG(logger, "上电..");

	Fun_DY1();
	Sleep(100);

	Fun_DY2();
	Sleep(100);

	Fun_YXQ_SET();

	LONG bytesToWrite = 31;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	memset(buf, 0x00, bytesToWrite);

	buf[0] = tmode;//测试模式

	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	buf[4] = 0xff;//收发切换模式中的成功测试次数

	//buf[5] = 0x10;
	//buf[6] = 0x00;
	//buf[7] = 0x00;
	//buf[8] = 0xff;//收发切换模式中的可调延时

	buf[5] = (tc & 0xff000000) >> 24;
	buf[6] = (tc & 0xff0000) >> 16;
	buf[7] = (tc & 0xff00) >> 8;
	buf[8] = tc & 0xff;//收发切换模式中的可调延时

	buf[9] = 0x00;
	buf[10] = 0x00;
	buf[11] = 0x00;
	buf[12] = 0x26;
	buf[13] = 0x25;
	buf[14] = 0xa0;//高温老练时间

	int h4v = (HiV_4 / 2)*3276.7;
	int l4v = (LoV_4 / 2)*3276.7;
	int h4c = (HiC_4 * 0.555 + 2.529)*3276.7;
	int l4c = (LoC_4 * 0.555 + 2.529)*3276.7;

	int h7v = (HiV_7 / 2)*3276.7;
	int l7v = (LoV_7 / 2)*3276.7;
	int h7c = (HiC_7 * 0.555 + 2.494)*3276.7;
	int l7c = (LoC_7 * 0.555 + 2.494)*3276.7;

	buf[15] = (h7v & 0xff00) >> 8;
	buf[16] = h7v & 0xff;
	buf[17] = (l7v & 0xff00) >> 8;
	buf[18] = l7v & 0xff;//+7V电压

	//buf[19] = (h7c & 0xff00) >> 8;
	//buf[20] = h7c & 0xff;
	//buf[21] = (l7c & 0xff00) >> 8;
	//buf[22] = l7c & 0xff;//+7V电流
	buf[19] = (h4v & 0xff00) >> 8;
	buf[20] = h4v & 0xff;
	buf[21] = (l4v & 0xff00) >> 8;
	buf[22] = l4v & 0xff;//+4V电压

	buf[23] = (h7c & 0xff00) >> 8;
	buf[24] = h7c & 0xff;
	buf[25] = (l7c & 0xff00) >> 8;
	buf[26] = l7c & 0xff;//+7V电流



	buf[27] = (h4c & 0xff00) >> 8;
	buf[28] = h4c & 0xff;
	buf[29] = (l4c & 0xff00) >> 8;
	buf[30] = l4c & 0xff;//+4V电流



	//buf[15] = 0xff;
	//buf[16] = 0xff;
	//buf[17] = 0x00;
	//buf[18] = 0x00;//+7V电压
	//buf[19] = 0xff;
	//buf[20] = 0xff;
	//buf[21] = 0x00;
	//buf[22] = 0x00;//+7V电流

	//buf[23] = 0xff;
	//buf[24] = 0xff;
	//buf[25] = 0x00;
	//buf[26] = 0x00;//+4V电压

	//buf[27] = 0xff;
	//buf[28] = 0xff;
	//buf[29] = 0x00;
	//buf[30] = 0x00;//+4V电流

	WriteData_CS(buf, bytesToWrite);

	return true;
}

bool __stdcall  myUsbDLL::Stop()
{
	PowerOff_Target();
	//LOG4CPLUS_DEBUG(logger, "下电..");

	SendCMD(0x85, 0x01);
	Sleep(100);
	SendCMD(0x85, 0x00);
	//LOG4CPLUS_DEBUG(logger, "手动停止..");
	return true;
}

void  __stdcall myUsbDLL::Close() {
	//LOG4CPLUS_DEBUG(logger, "Close");
	PowerOff_Target();
	RecvTag = false;
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

	for (int j = 0; j < 108; j++)
	{
		myStruct[j].Amp = 0;
		myStruct[j].Php = 0;
		myStruct[j].Used = true;
	}
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

	for (int j = 0; j < 108; j++)
	{
		myStruct[j].Amp = 15.5;
		myStruct[j].Php = 354.375;
		myStruct[j].Used = true;
	}
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

	for (int j = 0; j < 108; j++)
	{
		myStruct[j].Amp = 0;
		myStruct[j].Php = 0;
		myStruct[j].Used = true;
	}

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

	for (int j = 0; j < 108; j++)
	{
		myStruct[j].Amp = 15.5;
		myStruct[j].Php = 354.375;
		myStruct[j].Used = true;
	}
	return true;
}

bool __stdcall  myUsbDLL::Fun_DY1()
{
	//LOG4CPLUS_DEBUG(logger, "Fun_DY1..");
	LONG bytesToWrite = 5;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	//00c0ffffc0,1代表关闭，0代表打开，默认全部关闭
	buf[0] = 0xff;
	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	buf[4] = 0x00;

	int i = 0;
	for (i = 0; i < 64; i++)
	{
		if (myStruct[i].Used)
		{
			buf[0] = buf[0] & (0xff - (0x1 << (i / 8)));
		}
	}
	for (i = 64; i < 108; i++)
	{
		if (myStruct[i].Used)
		{
			buf[1] = buf[1] & (0xff - (0x1 << ((i - 64) / 8)));
		}
	}
	unsigned char crc = 0x00;
	for (int j = 0; j < 4; j++)
	{
		crc ^= buf[j];
	}
	buf[4] = crc;

	WriteData_DY1(buf, bytesToWrite);

	return true;
}

bool __stdcall  myUsbDLL::Fun_DY2()
{
	//LOG4CPLUS_DEBUG(logger, "Fun_DY2..");
	LONG bytesToWrite = 5;
	unsigned char * buf2 = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	//ff3f00f030,密切注意，这里的b15 b14是用到接收码字里面了！！！！
	buf2[0] = 0xff;
	buf2[1] = 0xff;
	buf2[2] = 0xff;
	buf2[3] = 0xff;
	buf2[4] = 0x00;

	int i = 0;
	for (i = 0; i < 8; i++)
		if (myStruct[i].Used)
			buf2[1] = buf2[1] & 0xbf;

	for (i = 8; i < 16; i++)
		if (myStruct[i].Used)
			buf2[1] = buf2[1] & 0x7f;

	for (i = 16; i < 80; i++)
	{
		if (myStruct[i].Used)
		{
			buf2[2] = buf2[2] & (0xff - (0x1 << ((i - 16) / 8)));
		}
	}
	for (i = 80; i < 108; i++)
	{
		if (myStruct[i].Used)
		{
			buf2[3] = buf2[3] & (0xff - (0x1 << ((i - 80) / 8)));
		}
	}
	unsigned char crc2 = 0x00;
	for (int j = 0; j < 4; j++)
	{
		crc2 ^= buf2[j];
	}
	buf2[4] = crc2;

	WriteData_DY2(buf2, bytesToWrite);
	return true;
}

bool __stdcall myUsbDLL::Fun_YXQ_SET()
{
	//LOG4CPLUS_DEBUG(logger, "Fun_YXQ_SET..");

	int temp1 = 0;
	int temp2 = 0;
	int _amp = 0;
	int _pha = 0;

	LONG bytesToWrite = 217;
	unsigned char * buf_send = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	memset(buf_send, 0x0, bytesToWrite);
	unsigned char * buf_recv = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	memset(buf_recv, 0x0, bytesToWrite);

	for (int t = 0; t < 108; t++)
	{
		if (myStruct[t].Used)
		{
			temp1 = myStruct[t].Amp / 0.5;
			_amp = temp1 & 0xff;

			temp2 = myStruct[t].Php / 5.625;
			_pha = temp2 & 0xff;

			int b1 = 0x3f;
			int b2 = 0x3f;

			b1 = b1 & 0x3f;//最高2bit未使用设置为0
			b2 = b2 & 0x3f;//最高2bit未使用设置为0
			if (_amp & 0x01)//0.5db bit 0有效
			{
				b1 = b1 & 0xef;
			}
			if (_amp & 0x02)//1db bit 0有效
			{
				b1 = b1 & 0xf7;
			}
			if (_amp & 0x04)//2db bit 有效
			{
				b1 = b1 & 0xfb;
			}
			if (_amp & 0x08)//4db bit 有效
			{
				b1 = b1 & 0xfd;
			}
			if (_amp & 0x10)//8db bit 有效
			{
				b1 = b1 & 0xfe;
			}

			if (_pha & 0x01)// 5.625 有效
			{
				b2 = b2 & 0xfb;
			}
			if (_pha & 0x02)// 11.25 有效
			{
				b2 = b2 & 0xfd;
			}
			if (_pha & 0x04)// 22.5 有效
			{
				b2 = b2 & 0xfe;
			}
			if (_pha & 0x08)// 45 有效
			{
				b2 = b2 & 0xf7;
			}
			if (_pha & 0x10)// 90 有效
			{
				b1 = b1 & 0xdf;
			}
			if (_pha & 0x20)// 180 有效
			{
				b2 = b2 & 0xef;
			}
			//发射
			b2 = b2 & 0x1f;
			buf_send[2 * t] = b1;
			buf_send[2 * t + 1] = b2;

			//接收
			b2 = b2 | 0x20;
			buf_recv[2 * t] = b1;
			buf_recv[2 * t + 1] = b2;
		}
	}
	UCHAR hexcrc = buf_send[0];
	for (int m = 1; m < 216; m++)
	{
		hexcrc ^= buf_send[m];
	}
	buf_send[216] = hexcrc & 0x3f;
	WriteData_YXQ1(buf_send, bytesToWrite);
	Sleep(200);

	UCHAR hexcrc2 = buf_recv[0];
	for (int m = 1; m < 216; m++)
	{
		hexcrc2 ^= buf_recv[m];
	}
	buf_recv[216] = hexcrc2 & 0x3f;
	WriteData_YXQ2(buf_recv, bytesToWrite);
	Sleep(200);

	return true;
}


bool  __stdcall myUsbDLL::SetAmpAndPhpTable(const char* fileName) {

	for (int m = 0; m < Size; m++)
	{
		myStruct[m].Used = false;
	}

	tempfilename = fileName;
	//LOG4CPLUS_DEBUG(logger, "SetAmpAndPhpTable:" << tempfilename.c_str());

	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, fileName, "r");
	if (fp == NULL) {
		//LOG4CPLUS_DEBUG(logger, "Open File failed.");
		return false;
	}
	else
	{
		//LOG4CPLUS_DEBUG(logger, "Open File Successed.");

		int chan;
		float Amp;
		float Php;
		while (!feof(fp)) {
			fscanf_s(fp, "%d,%f,%f", &chan, &Amp, &Php);
			//printf("%d,%.1f,%.1f\n", chan, Amp, Php);
			myStruct[chan - 1].Amp = Amp;
			myStruct[chan - 1].Php = Php;
			myStruct[chan - 1].Used = true;
		}
		fclose(fp);

		return true;
	}

}



bool  __stdcall myUsbDLL::SetSingleAmpAndPhp(unsigned ch, double amp, double php)
{
	SetAmpAndPhpTable(tempfilename.c_str());

	TagAB = 0x00;
	//LOG4CPLUS_DEBUG(logger, "SetSingleAmpAndPhp ch="<<ch<<" amp="<<amp<<" php="<<php);

	if (ch < 108 && ch>0) {
		myStruct[ch - 1].Amp = amp;
		myStruct[ch - 1].Php = php;
		myStruct[ch - 1].Used = true;;

		Continue();

		int timeout = 0;
		while (TagAB != 0xAB)
		{
			//LOG4CPLUS_DEBUG(logger, "SetSingleAmpAndPhp TagAB!= 0xAB ");
			Sleep(100);
			timeout += 1;
			if (timeout >= 100)
			{
				if (mRecalFun) {
					mRecalFun("CKLOG --SetSingleAmpAndPhp() failed", 0, 0);
				}
				return false;
			}
		}
		//LOG4CPLUS_DEBUG(logger, "SetSingleAmpAndPhp Successed!");
		//if (mRecalFun) {
		//	mRecalFun("CKLOG --SetSingleAmpAndPhp() successed", 0, 0);
		//}
		return true;
	}
	else
	{
		return false;
	}

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
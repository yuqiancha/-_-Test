// UsbApiDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <iostream>

#include "Export.h"
#include "IAntennaInterface.h"

#include <fstream>
using namespace std;

#include <String>
#include <iostream>
#include <assert.h>
#include <vector>

IAntennaInterface* pExport;

void fun1(const char* msg, double votage, double current)
{
	const char* temp = "波控+4V";
	
	int ptr = strcmp(msg, temp);
	if (ptr == 0)
	{
	//	cout << msg << " 电压" << votage << "   电流" << current << endl;
		printf("%s 电压:%6.4f 电流:%6.4f ok\n", msg, votage, current);
	}
}

int main()
{
	//ifstream infile;
	//infile.open("example.txt");

	//string str;

	//while (getline(infile,str))
	//{
	//	cout << str << endl;
	//}


	////FILE *fp;
	////fp = fopen("example.txt", "r");
	////if (fp == NULL) {
	////	printf("Open File failed.\n");
	////	exit(0);
	////}
	////int chan;
	////float Votage;
	////float Current;
	////
	////int ChanBuf[108];
	////float VotageBuf[108];
	////float CurrentBuf[108];

	////struct MyStruct
	////{
	////	int Chan;
	////	float Votage;
	////	float Current;
	////	bool Used;
	////};

	////MyStruct* myStruct;
	////const int Size = 108;
	////myStruct = (struct MyStruct*)malloc(sizeof(struct MyStruct)*Size);
	////for (int m = 0; m < Size; m++)
	////{
	////	myStruct[m].Chan = m;
	////	myStruct[m].Votage = 0;
	////	myStruct[m].Current = 0;
	////	myStruct[m].Used = false;
	////}

	////for (int i = 0; i < 108; i++)
	////{
	////	ChanBuf[i] = 0;
	////	VotageBuf[i] = 0;
	////	CurrentBuf[i] = 0;
	////}

	////while (!feof(fp)) {
	////	fscanf(fp, "%d,%f,%f", &chan, &Votage, &Current);
	////	printf("%d,%.1f,%.1f\n", chan, Votage, Current);
	////	myStruct[chan - 1].Votage = Votage;
	////	myStruct[chan - 1].Current = Current;
	////	myStruct[chan - 1].Used = true;
	////}
	////fclose(fp);
	
	DWORD dwThreadId;
	HANDLE hThread;

	pExport = CreateAntennInstance();

	pExport->Open("");

	pExport->BindRecalFun(fun1);

//  	pExport->SetAntennaMode(IAntennaInterface::SendReveiveMode);
//	pExport->SetAntennaMode(IAntennaInterface::SendMode);
	pExport->SetAntennaMode(IAntennaInterface::ReceiveMode);

	pExport->SetCallFunTimeInter(500);

 	pExport->SetAntennaSendAndReceiveSwitchTime(5000);

	pExport->Fun_YXQ1_Not();

	pExport->Fun_YXQ2_Not();

	pExport->SetVotateLimit(0, 3, 5, 1, 2);

	pExport->SetVotateLimit(1, 6, 8, 1, 2);

	pExport->Start();

	////调用打开设备
	////DeviceOpen();	

	////创建线程读取数据
	//hThread = CreateThread(NULL, NULL, GetDataFun, NULL, 0, &dwThreadId);
	//CloseHandle(hThread);

	//pExport->Fun_YXQ1_Not();

	////写入测试数据
 // 	LONG bytesToWrite = 1024;
	//unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	//memset(buf, 0x5a, bytesToWrite);
	//pExport->Write(buf, bytesToWrite);

	system("pause");
}



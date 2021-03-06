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
	//const char* temp = "波控+4V";
	//
	//int ptr = strcmp(msg, temp);
	//if (ptr == 0)
	//{
	////	cout << msg << " 电压" << votage << "   电流" << current << endl;
	//	printf("%s 电压:%6.4f 电流:%6.4f ok\n", msg, votage, current);
	//}

	printf("%-8s 电压:%5.3f 电流:%5.3f ok\n", msg, votage, current);
}

int main()
{	
	DWORD dwThreadId;
	HANDLE hThread;

	pExport = CreateAntennInstance();

	int tag = pExport->Open("");

	if (tag == false)
	{
		printf("device not open");
		system("pause");
	}

	int tag2 = pExport->IsOpen();
	if (tag2 == false)
	{
		printf("is not open");
		system("pause");
	}
	pExport->BindRecalFun(fun1);

//  	pExport->SetAntennaMode(IAntennaInterface::SendReveiveMode);
	pExport->SetAntennaMode(IAntennaInterface::SendMode);
	
	pExport->SetCallFunTimeInter(1000);
 	pExport->SetAntennaSendAndReceiveSwitchTime(5000);

//	pExport->Fun_YXQ1_Not();
//	pExport->Fun_YXQ2_Not();

	pExport->SetVotateLimit(0, 0, 10, 0, 10);

	pExport->SetVotateLimit(1, 0, 10, 0, 10);

//	pExport->SetAmpAndPhpTable("C:\\Users\\Administrator\\GitHub\\测控模块_功能Test\\UsbApi\\UsbApiDemo\\全通道衰减.txt");
//	pExport->SetAmpAndPhpTable("C:\\Users\\Administrator\\GitHub\\测控模块_功能Test\\UsbApi\\UsbApiDemo\\全通道不衰.txt");
	pExport->SetAmpAndPhpTable("D:\\CSharpproject\\测控模块_功能Test\\UsbApi\\UsbApiDemo\\通道值.txt");
	
	pExport->Start();

	////写入测试数据
 // 	LONG bytesToWrite = 1024;
	//unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	//memset(buf, 0x5a, bytesToWrite);
	//pExport->Write(buf, bytesToWrite);

	system("pause");
	pExport->Close();
}



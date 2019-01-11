// UsbApiDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <iostream>

#include "Export.h"

DWORD WINAPI GetDataFun(LPVOID lpParam);

Export* pExport;

int main()
{
	DWORD dwThreadId;
	HANDLE hThread;
	
	pExport = CreateMyUsbObj();
	pExport->DeviceOpen();

	

	//调用打开设备
	
	//DeviceOpen();	

	//创建线程读取数据
	hThread = CreateThread(NULL, NULL, GetDataFun, NULL, 0, &dwThreadId);
	CloseHandle(hThread);

	system("pause");

	//写入测试数据
  	LONG bytesToWrite = 1024;
	unsigned char * buf = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	memset(buf, 0x5a, bytesToWrite);
	pExport->WriteData(buf, bytesToWrite);
}


DWORD WINAPI GetDataFun(LPVOID lpParam)
{
	pExport->ReadEnable(); 

	for (int i = 0; i < 9999999; i++) {

		unsigned char * buf = (unsigned char *)malloc(4096 * sizeof(char));
		LONG bytesToRead = 4096;		

		pExport->ReadData(buf, bytesToRead);

		if (bytesToRead) {
			printf("Seq:%d  ReturnLen = %d %x %x %x %x \n", i, bytesToRead, buf[0], buf[1], buf[2], buf[3]);
			
		}
		else
		{
			printf("Seq:%d  ReturnLen = %d \n", i, bytesToRead);
			Sleep(100);
		}
		
		free(buf);
		buf = NULL;
	}

	return 1;
}

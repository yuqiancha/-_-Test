// TestConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>

#include <cstdlib>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;


struct MyStruct
{
	int Chan;
	float Amp;
	float Php;
	bool Used;
};

MyStruct* myStruct;
const int Size = 108;


int main()
{

	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.properties"));
	Logger logger = Logger::getRoot();

	LOG4CPLUS_DEBUG(logger, "this is debug log: " << "rand integer: " << rand());
	LOG4CPLUS_ERROR(logger, "ooooooh, there is an error....");
	//

	//Logger loggerTest = log4cplus::Logger::getInstance("test");
	//LOG4CPLUS_ERROR(loggerTest, "anther logger,error ");


	FILE *fp;
	errno_t err;

	err = fopen_s(&fp, "D:\\CSharpproject\\测控模块_功能Test\\UsbApi\\UsbApiDemo\\通道值.txt", "r");
	if (fp == NULL) {
		printf("Open File failed.\n");
		exit(0);
	}

	myStruct = (struct MyStruct*)malloc(sizeof(struct MyStruct)*Size);
	for (int m = 0; m < Size; m++)
	{
		myStruct[m].Chan = m;
		myStruct[m].Amp = 0;
		myStruct[m].Php = 0;
		myStruct[m].Used = false;
	}

	int chan;
	float Amp;
	float Php;

	while (!feof(fp)) {
		fscanf_s(fp, "%d,%f,%f", &chan, &Amp, &Php);
		printf("%d,%.1f,%.1f\n", chan, Amp, Php);
		myStruct[chan - 1].Amp = Amp;
		myStruct[chan - 1].Php = Php;
		myStruct[chan - 1].Used = true;
	}
	fclose(fp);

	LONG bytesToWrite = 5;
	unsigned char * buf1 = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));
	//00c0ffffc0,1代表关闭，0代表打开，默认全部关闭
	buf1[0] = 0xff;	buf1[1] = 0xff;	buf1[2] = 0xff;	buf1[3] = 0xff;	buf1[4] = 0x00;

	int i = 0;
	for (i = 0; i < 64; i++)
		if (myStruct[i].Used)
			buf1[0] = buf1[0] & (0xff - (0x1 << (i / 8)));

	for (i = 64; i < 108; i++)
		if (myStruct[i].Used)
			buf1[1] = buf1[1] & (0xff - (0x1 << ((i - 64) / 8)));

	unsigned char crc1 = 0x00;
	for (int j = 0; j < 4; j++)
	{
		crc1 ^= buf1[j];
	}
	buf1[4] = crc1;

	unsigned char * buf2 = (unsigned char *)malloc(bytesToWrite * sizeof(unsigned char));	//ff3f00f030,密切注意，这里的b15 b14是用到接收码字里面了！！！！
	buf2[0] = 0xff;	buf2[1] = 0xff;	buf2[2] = 0xff;	buf2[3] = 0xff;	buf2[4] = 0x00;

	i = 0;
	for (i = 0; i < 8; i++)
		if (myStruct[i].Used)
			buf2[1] = buf2[1] & 0xbf;

	for (i = 8; i < 16; i++)
		if (myStruct[i].Used)
			buf2[1] = buf2[1] & 0x7f;

	for (i = 16; i < 80; i++)
		if (myStruct[i].Used)
			buf2[2] = buf2[2] & (0xff - (0x1 << ((i - 16) / 8)));

	for (i = 80; i < 108; i++)
		if (myStruct[i].Used)
			buf2[3] = buf2[3] & (0xff - (0x1 << ((i - 80) / 8)));


	unsigned char crc2 = 0x00;
	for (int j = 0; j < 4; j++)
	{
		crc2 ^= buf2[j];
	}
	buf2[4] = crc2;

	int temp1 = 0;
	int temp2 = 0;
	unsigned char _amp = 0;
	unsigned char _pha = 0;
	
	unsigned char * buf_send = (unsigned char *)malloc(217 * sizeof(unsigned char));
	memset(buf_send, 0x0, 217);

	unsigned char * buf_recv = (unsigned char *)malloc(217 * sizeof(unsigned char));
	memset(buf_recv, 0x0, 217);

	for (int t = 0; t < 108; t++)
	{
		if (myStruct[t].Used)
		{
			temp1 = myStruct[t].Amp / 0.5;
			int _amp = temp1 & 0xff;

			temp2 = myStruct[t].Php / 5.625;
			int _pha = temp2 & 0xff;

			printf("chan:%3d -- %x %x \n", myStruct[t].Chan, _amp, _pha);

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
			buf_send[2 * t+1] = b2;

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

	UCHAR hexcrc2 = buf_recv[0];
	for (int m = 1; m < 216; m++)
	{
		hexcrc2 ^= buf_recv[m];
	}
	buf_recv[216] = hexcrc2 & 0x3f;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#pragma once

#ifdef MMYSQL_EXPORTS
#define USB_API __declspec(dllimport)
#else
#define USB_API __declspec(dllexport)
#endif


class Export
{
public:
	virtual void SendCMD(int addr, int value) = 0;						//向设备寄存器写数
	virtual bool DeviceOpen() = 0;										//初始化并打开设备
	virtual void ReadEnable() = 0;										//使能读功能，打开设备后无法读取数据，必须调用此函数才可以读取数据，调用一次一直有效
	virtual void ReadData(unsigned char* buf, long &length) = 0;		//从设备读出数据，数据保存在buf数组，读出数据数量为length
	virtual void WriteData(unsigned char* buf, long &length) = 0;		//将buf数组写入设备，写入长度为length
	virtual void Release() = 0;

	virtual void ADFunc_Enable() = 0;//AD采集使能
	virtual void PowerOn_Target() = 0;//控制电源输出
	virtual void PowerOff_Target() = 0;//控制电源不输出

	virtual void WriteData_CS(unsigned char* buf, long &length) = 0;		//参数注入

	virtual void WriteData_YXQ1(unsigned char* buf, long &length) = 0;		//移相器数据1注入
	virtual void WriteData_YXQ2(unsigned char* buf, long &length) = 0;		//移相器数据2注入

	virtual void WriteData_DY1(unsigned char* buf, long &length) = 0;		//电源数据1注入
	virtual void WriteData_DY2(unsigned char* buf, long &length) = 0;		//电源数据2注入

	virtual void Fun1()=0;
	virtual void Fun2()=0;
	virtual void Fun3()=0;
	virtual void Fun4()=0;
	virtual void Fun5()=0;
	virtual void Fun6()=0;
	virtual void Fun7()=0;
};

extern "C" USB_API Export* _stdcall CreateMyUsbObj();
extern "C" USB_API void _stdcall DestroyMyUsbObj(Export* pExport);
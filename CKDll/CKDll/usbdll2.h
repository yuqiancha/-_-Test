#pragma once
#include "Export.h"
#include "IAntennaInterface.h"
class USB_API  myUsbDLL : public Export,public IAntennaInterface
{
public:
	myUsbDLL();
	~myUsbDLL();

public:
	virtual void SendCMD(int addr, int value);					//向设备寄存器写数
	virtual bool DeviceOpen();									//初始化并打开设备
	virtual void ReadEnable();									//使能读功能，打开设备后无法读取数据，必须调用此函数才可以读取数据，调用一次一直有效
	virtual void ReadData(unsigned char* buf, long &length);	//从设备读出数据，数据保存在buf数组，读出数据数量为length
	virtual void WriteData(unsigned char* buf, long &length);	//将buf数组写入设备，写入长度为length
	virtual void WriteData_WithChan(int chan,unsigned char* buf, long &length);	//将buf数组写入设备，写入长度为length

	virtual void Release();

	virtual void WriteData_CS(unsigned char* buf, long &length);		//参数注入

	virtual void WriteData_YXQ1(unsigned char* buf, long &length);		//移相器数据1注入
	virtual void WriteData_YXQ2(unsigned char* buf, long &length);		//移相器数据2注入

	virtual void WriteData_DY1(unsigned char* buf, long &length);		//电源数据1注入
	virtual void WriteData_DY2(unsigned char* buf, long &length);		//电源数据2注入

	virtual void ADFunc_Enable();//AD采集使能

	virtual void Fun1();
	virtual void Fun2();
	virtual void Fun3();
	virtual void Fun4();
	virtual void Fun5();
	virtual void Fun6();
	virtual void Fun7();

	//
	// 打开设备
	//
	virtual bool __stdcall Open(const char* Addr);//done
	virtual void  __stdcall Close();//done
	virtual bool  __stdcall IsOpen();//done

	virtual bool  __stdcall SetAntennaMode(AntennaStatus mode);

	virtual bool  __stdcall SetAntennaSendAndReceiveSwitchTime(double mTime);

	virtual bool  __stdcall SetAmpAndPhpTable(const char* fileName);

	virtual bool  __stdcall SetSingleAmpAndPhp(unsigned ch, double amp, double php);

	virtual bool  __stdcall SetPhaseStatus(unsigned ch, double php);

	virtual void  __stdcall BindRecalFun(VoltageAndCurrentFun fun);

	void  __stdcall SetCallFunTimeInter(double time);

	virtual bool __stdcall Write(const unsigned char* data, long len);

	virtual unsigned __stdcall Read(unsigned char*& data, long& len);

	virtual bool __stdcall SetVotateLimit(int tag, double LoVotage, double HiVotage, double LoCurrent, double HiCurrent);
	virtual bool __stdcall Start();
	virtual bool __stdcall Stop();
	virtual bool __stdcall Continue();
	virtual bool __stdcall PowerOn_Target();
	virtual bool __stdcall PowerOff_Target();

	virtual bool __stdcall Fun_YXQ1_Not();
	virtual bool __stdcall Fun_YXQ1_Yes();
	virtual bool __stdcall Fun_YXQ2_Not();
	virtual bool __stdcall Fun_YXQ2_Yes();


	virtual bool __stdcall Fun_DY1();
	virtual bool __stdcall Fun_DY2();

	virtual bool __stdcall Fun_YXQ_SET();


public:
	CCyUSBDevice *USBDevice;
	CCyBulkEndPoint *BulkInEpt;				//设备-->PC
	CCyBulkEndPoint *BulkOutEpt;			//PC-->设备
	CCyControlEndPoint *ControlEndPoint;

	char Register80H = 0x00;
	char Register81H = 0x00;
	char Register82H = 0x00;
	char Register83H = 0x00;
	char Register84H = 0x00;

	double HiV_4 = 6;
	double LoV_4 = 2;
	double HiC_4 = 5;
	double LoC_4 = 0;
	double HiV_7 = 8;
	double LoV_7 = 6;
	double HiC_7 = 2;
	double LoC_7 = 0;

	bool shutpower = false;

	mutex mylock;

	mutex cmdlock;	

	VoltageAndCurrentFun  mRecalFun{ nullptr };
	bool RecvTag = true;
	double dataRe_AD01[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int tc = 0xff;
	int freqtime = 1000;
	int tmode = 0x01;

	unsigned char tspan1[4] = { 0,0,0,0 };
	unsigned char tspan2[4] = { 0,0,0,0 };

	string tempfilename;

	//Logger logger = Logger::getRoot();
	//Logger logger = Logger::getInstance(L"test");

	int TagAB = 0x0;
};


USB_API Export* _stdcall CreateMyUsbObj()
{
	return new myUsbDLL;
}

USB_API void _stdcall DestroyMyUsbObj(Export* pExport)
{
	pExport->Release();
}

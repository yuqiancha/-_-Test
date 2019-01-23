#pragma once
#include "Export.h"
#include "IAntennaInterface.h"
class USB_API  myUsbDLL : public Export,public IAntennaInterface
{
public:
	myUsbDLL();
	~myUsbDLL();

public:
	virtual void SendCMD(int addr, int value);					//���豸�Ĵ���д��
	virtual bool DeviceOpen();									//��ʼ�������豸
	virtual void ReadEnable();									//ʹ�ܶ����ܣ����豸���޷���ȡ���ݣ�������ô˺����ſ��Զ�ȡ���ݣ�����һ��һֱ��Ч
	virtual void ReadData(unsigned char* buf, long &length);	//���豸�������ݣ����ݱ�����buf���飬������������Ϊlength
	virtual void WriteData(unsigned char* buf, long &length);	//��buf����д���豸��д�볤��Ϊlength
	virtual void WriteData_WithChan(int chan,unsigned char* buf, long &length);	//��buf����д���豸��д�볤��Ϊlength

	virtual void Release();


	virtual void WriteData_CS(unsigned char* buf, long &length);		//����ע��

	virtual void WriteData_YXQ1(unsigned char* buf, long &length);		//����������1ע��
	virtual void WriteData_YXQ2(unsigned char* buf, long &length);		//����������2ע��

	virtual void WriteData_DY1(unsigned char* buf, long &length);		//��Դ����1ע��
	virtual void WriteData_DY2(unsigned char* buf, long &length);		//��Դ����2ע��


	virtual void ADFunc_Enable();//AD�ɼ�ʹ��


	virtual void Fun1();
	virtual void Fun2();
	virtual void Fun3();
	virtual void Fun4();
	virtual void Fun5();
	virtual void Fun6();
	virtual void Fun7();


	//
	// ���豸
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
	CCyBulkEndPoint *BulkInEpt;				//�豸-->PC
	CCyBulkEndPoint *BulkOutEpt;			//PC-->�豸
	CCyControlEndPoint *ControlEndPoint;

	char Register80H = 0x00;
	char Register81H = 0x00;
	char Register82H = 0x00;
	char Register83H = 0x00;
	char Register84H = 0x00;

	int HiV_4 = 0;
	int LoV_4 = 0;
	int HiC_4 = 0;
	int LoC_4 = 0;
	int HiV_7 = 0;
	int LoV_7 = 0;
	int HiC_7 = 0;
	int LoC_7 = 0;

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

	Logger logger = Logger::getRoot();
};


USB_API Export* _stdcall CreateMyUsbObj()
{
	return new myUsbDLL;
}

USB_API void _stdcall DestroyMyUsbObj(Export* pExport)
{
	pExport->Release();
}

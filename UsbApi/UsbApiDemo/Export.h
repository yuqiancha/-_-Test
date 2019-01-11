#pragma once

#ifdef MMYSQL_EXPORTS
#define USB_API __declspec(dllimport)
#else
#define USB_API __declspec(dllexport)
#endif


class Export
{
public:
	virtual void SendCMD(int addr, int value) = 0;//���豸�Ĵ���д��
	virtual bool DeviceOpen() = 0;//��ʼ�������豸
	virtual void ReadEnable() = 0;//ʹ�ܶ����ܣ����豸���޷���ȡ���ݣ�������ô˺����ſ��Զ�ȡ���ݣ�����һ��һֱ��Ч
	virtual void ReadData(unsigned char* buf, long &length) = 0;//���豸�������ݣ����ݱ�����buf���飬������������Ϊlength
	virtual void WriteData(unsigned char* buf, long &length) = 0;//��buf����д���豸��д�볤��Ϊlength
	virtual void Release() = 0;


	virtual void Fun1() = 0;
	virtual void Fun2() = 0;
	virtual void Fun3() = 0;
	virtual void Fun4() = 0;
	virtual void Fun5() = 0;
	virtual void Fun6() = 0;
	virtual void Fun7() = 0;
};

extern "C" USB_API Export* _stdcall CreateMyUsbObj();
extern "C" USB_API void _stdcall DestroyMyUsbObj(Export* pExport);
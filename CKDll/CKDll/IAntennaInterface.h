#pragma  once



//+--------------------------------------------
//
// ң��ң����ƽӿ�
//
//+--------------------------------------------
class IAntennaInterface{

public:
	enum AntennaStatus{
		SendMode = 0, // ����ģʽ
		ReceiveMode, // ����ģʽ
		SendReveiveMode, // �շ��л�
		SingleMode,   // ��ͨ��ģʽ
		NullMode, // ��ģʽ,����
	};


	//
	// ��ѹ�����ص�����
	// const char*  ��ѹ���� ���� +4V -4V .....
	// [Error] --- ����
	// 
	// double ����ѹ
	// double , ����
	//
	typedef void(*VoltageAndCurrentFun)(const char*, double, double);

	IAntennaInterface(){}
	virtual ~IAntennaInterface(){}


	//
	// ���豸
	//
	virtual bool __stdcall Open(const char* Addr) = 0;

	//
	// �ر��豸
	//
	virtual void  __stdcall Close() = 0;

	//
	// ����豸�Ƿ��
	//
	virtual bool  __stdcall IsOpen() = 0;

	//
	// ���ù���ģʽ
	//
	virtual bool  __stdcall SetAntennaMode(AntennaStatus mode) = 0;

	//
	// �����շ��л�ʱ���� ��λ ms
	//
	virtual bool  __stdcall SetAntennaSendAndReceiveSwitchTime(double mTime) = 0;


	//
	// ���õ�ѹ��������
	//tag = 0 : +4V tag = 1: +7V
	//
	virtual bool __stdcall SetVotateLimit(int tag, double LoVotage, double HiVotage, double LoCurrent, double HiCurrent) = 0;


	//
	// ��ʼѭ��
	//
	virtual bool __stdcall Start() = 0;


	//
	// ֹͣѭ��
	//
	virtual bool __stdcall Stop() = 0;

	//
	// �ϵ磬��Դ���
	// ��Start�����л��Զ����ã�Ҳ�ɵ�������
	//
	virtual bool __stdcall PowerOn_Target() = 0;


	//
	// �µ磬��Դ�����
	// ��Stop�����л��Զ����ã�Ҳ�ɵ�������
	//
	virtual bool __stdcall PowerOff_Target() = 0;


	//
	// ���÷�����
	// �ı��ļ�
	// �ļ���ʽ
	// ͨ�� ��ֵ--˥�� ��λ--�������Ƕ�
	//108��ͨ����ͨ���ţ���ֵ����λ����ͨ���Ŵ����ϵ磬û�д����ϵ�
	virtual bool  __stdcall SetAmpAndPhpTable(const char* fileName) = 0;


	//
	// ���õ�ͨ������
	//
	virtual bool  __stdcall SetSingleAmpAndPhp(unsigned ch, double amp, double php) = 0;

	//
	// ���õ�ͨ����λ
	// 
	virtual bool  __stdcall SetPhaseStatus(unsigned ch, double php) = 0;



	//
	// ���õ�ѹ�����ص���
	//
	virtual void  __stdcall BindRecalFun(VoltageAndCurrentFun fun) = 0;

	//
	// �ɼ���ѹ����ʱ����
	//
	virtual void  __stdcall SetCallFunTimeInter(double time) = 0;

	//
	// ����ԭʼ����
	//
	virtual bool __stdcall Write(const unsigned char* data, long len) = 0;


	// Fun_YXQ1_Not--�����������뱾��˥
	virtual bool __stdcall Fun_YXQ1_Not() = 0;

	// Fun_YXQ1_Yes--�����������뱾ȫ˥
	virtual bool __stdcall Fun_YXQ1_Yes() = 0;

	// Fun_YXQ_Not--�����������뱾��˥
	virtual bool __stdcall Fun_YXQ2_Not() = 0;

	// Fun_YXQ2_Yes--�����������뱾ȫ˥
	virtual bool __stdcall Fun_YXQ2_Yes() = 0;

	//
	// Fun_DY1--��Դ��������
	//
	virtual bool __stdcall Fun_DY1() = 0;

	//
	// Fun_DY2--��Դ��������
	//
	virtual bool __stdcall Fun_DY2() = 0;
};



#ifdef ANTENNAINTERFACE_EXPORTS
# define __ANTENNINTERFACE__ __declspec(dllexport)
#else
# define __ANTENNINTERFACE__ __declspec(dllimport )
#endif


extern "C"{

	//
	// ���û���ṩ��ɨ��ܵ�ʵ����ô����ʹ��֮ǰ���� __NO_SCANNER__
	//
#ifndef __NO_ANTENNINTERFACE__
	__ANTENNINTERFACE__ IAntennaInterface*  CreateAntennInstance();
	__ANTENNINTERFACE__ void  DestoryAntennInstance(IAntennaInterface*& instance);
#else
	__ANTENNINTERFACE__ IAntennaInterface*  CreateAntennInstance(){
		return nullptr;
	}
	__ANTENNINTERFACE__ void  DestoryAntennInstance(IAntennaInterface*& instance){
		instance = nullptr;
	}
#endif

}
#pragma  once



//+--------------------------------------------
//
// 遥控遥测控制接口
//
//+--------------------------------------------
class IAntennaInterface{

public:
	enum AntennaStatus{
		SendMode = 0, // 发射模式
		ReceiveMode, // 接收模式
		SendReveiveMode, // 收发切换
		SingleMode,   // 单通道模式
		NullMode, // 无模式,待机
	};


	//
	// 电压电流回调函数
	// const char*  电压描述 比如 +4V -4V .....
	// [Error] --- 出错
	// 
	// double ，电压
	// double , 电流
	//
	typedef void(*VoltageAndCurrentFun)(const char*, double, double);

	IAntennaInterface(){}
	virtual ~IAntennaInterface(){}


	//
	// 打开设备
	//
	virtual bool __stdcall Open(const char* Addr) = 0;

	//
	// 关闭设备
	//
	virtual void  __stdcall Close() = 0;

	//
	// 检测设备是否打开
	//
	virtual bool  __stdcall IsOpen() = 0;

	//
	// 设置工作模式
	//
	virtual bool  __stdcall SetAntennaMode(AntennaStatus mode) = 0;

	//
	// 设置收发切换时间间隔 单位 ms
	//
	virtual bool  __stdcall SetAntennaSendAndReceiveSwitchTime(double mTime) = 0;


	//
	// 设置电压电流门限
	//tag = 0 : +4V tag = 1: +7V
	//
	virtual bool __stdcall SetVotateLimit(int tag, double LoVotage, double HiVotage, double LoCurrent, double HiCurrent) = 0;


	//
	// 开始循环
	//
	virtual bool __stdcall Start() = 0;


	//
	// 停止循环
	//
	virtual bool __stdcall Stop() = 0;

	//
	// 上电，电源输出
	// 在Start函数中会自动调用，也可单独调用
	//
	virtual bool __stdcall PowerOn_Target() = 0;


	//
	// 下电，电源不输出
	// 在Stop函数中会自动调用，也可单独调用
	//
	virtual bool __stdcall PowerOff_Target() = 0;


	//
	// 设置幅相码
	// 文本文件
	// 文件格式
	// 通道 幅值--衰减 相位--移相器角度
	//108个通道，通道号，幅值，相位，有通道号代表上电，没有代表不上电
	virtual bool  __stdcall SetAmpAndPhpTable(const char* fileName) = 0;


	//
	// 设置单通道幅相
	//
	virtual bool  __stdcall SetSingleAmpAndPhp(unsigned ch, double amp, double php) = 0;

	//
	// 设置单通道相位
	// 
	virtual bool  __stdcall SetPhaseStatus(unsigned ch, double php) = 0;



	//
	// 设置电压电流回调函
	//
	virtual void  __stdcall BindRecalFun(VoltageAndCurrentFun fun) = 0;

	//
	// 采集电压电流时间间隔
	//
	virtual void  __stdcall SetCallFunTimeInter(double time) = 0;

	//
	// 发送原始数据
	//
	virtual bool __stdcall Write(const unsigned char* data, long len) = 0;


	// Fun_YXQ1_Not--移相器发射码本不衰
	virtual bool __stdcall Fun_YXQ1_Not() = 0;

	// Fun_YXQ1_Yes--移相器发射码本全衰
	virtual bool __stdcall Fun_YXQ1_Yes() = 0;

	// Fun_YXQ_Not--移相器接收码本不衰
	virtual bool __stdcall Fun_YXQ2_Not() = 0;

	// Fun_YXQ2_Yes--移相器接收码本全衰
	virtual bool __stdcall Fun_YXQ2_Yes() = 0;

	//
	// Fun_DY1--电源发射数据
	//
	virtual bool __stdcall Fun_DY1() = 0;

	//
	// Fun_DY2--电源接收数据
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
	// 如果没有提供有扫描架的实现那么请在使用之前定义 __NO_SCANNER__
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
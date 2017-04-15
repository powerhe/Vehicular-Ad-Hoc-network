#include <reg51.h>                                  
typedef unsigned char uchar;                          
typedef unsigned int uint;                           
#define ModeLed P0                                  //定义数码管显示端口
uchar code  LedShowData[]={0x03,0x9F,0x25,0x0D,0x99,  //定义数码管显示数据
                            0x49,0x41,0x1F,0x01,0x19};//0,1,2,3,4,5,6,7,8,9
static unsigned int RecvData;                         //定义接收红外数据变量
static unsigned char AddData;                         //定义自增变量
static unsigned int LedFlash;                         //定义闪动频率计数变量
unsigned char HeardData;                              //定义接收到数据的高位变 量
unsigned char carMode;								  //定义小车运行模式，0是自主模式，9是人工模式
bit RunFlag=0;                                        //定义运行标志位
sbit LeftIR=P3^5;                                     //定义前方左侧红外探头
sbit RightIR=P3^6;                                    //定义前主右侧红外探头	 蜂鸣器
sbit FontIR=P3^7;                                      //定义正前方红外探头	     继电器
sbit M1A=P1^0;                                        //定义电机1正向端口
sbit M1B=P1^1;                                        //定义电机1反向端口
sbit M2A=P1^2;                                        //定义电机2正向端口
sbit M2B=P1^3;                                        //定义电机2反向端口
sbit M3A=P1^4;
sbit M3B=P1^5;
sbit M4A=P1^6;
sbit M4B=P1^7;
void Delay()                                          
{ uint DelayTime=35000;                               
  while(DelayTime--);                                 
  return;                                            
}

void Delay2()
{
  uint DelayTime2 = 15000;
  while(DelayTime2--);
  return;
}

void ControlCar(uchar CarType)                        //定义小车控制子程序
{
      M1A=0;
	    M1B=0;                                         
      M2A=0;
	    M2B=0;  
	    M3A=0;
      M3B=0;
      M4A=0;
      M4B=0;                                       
 switch(CarType)                                      
 {  
	case 0: //自主模式前进                                 
    {   M1A=1;
	    M1B=0;                                         
        M2A=1;
	    M2B=0;  
	    M3A=1;
        M3B=0;
        M4A=1;
        M4B=0;                     
      break;                                        
    }
	case 1: //前进                                 
    {   M1A=1;
	    M1B=0;                                         
        M2A=1;
	    M2B=0;  
	    M3A=1;
        M3B=0;
        M4A=1;
        M4B=0;                      
      break;                                        
    }
    case 2: //后退                                    
    {   M1A=0;
	    M1B=1;                                         
        M2A=0;
	    M2B=1;  
	    M3A=0;
        M3B=1;
        M4A=0;
        M4B=1;                                                                                                                  
      break;                                        
    }
    case 3: //右转                                    
    {    
	  M1A=0;
	  M1B=0;                                         
      M2A=1;
	  M2B=0;  
	  M3A=1;
      M3B=0;
      M4A=0;
      M4B=0;                                                                                             
      break;                                         
    }
    case 4: //左转                                   
    {  

	  M1A=1;
	  M1B=0;                                         
      M2A=0;
	  M2B=0;  
	  M3A=0;
      M3B=0;
      M4A=1;
      M4B=0;                                                 
      break;                                         
    }
	case 8:  //停止
	{
	  break;
	}
    default:                                          
    {
     break;                                          
    }
 } 
}

void ComBreak() interrupt 4                         //定义串口通信子程序
{unsigned char RecvData;                            //定义串口数据接收变量
 if(RI==1)                                          //判断是否接收数据
 { RecvData=SBUF;                                   //将接收到的数据放入暂时变量
   RecvData=RecvData-'0';
   if(RecvData==9)                                  //判断接收到的数据是否小于10
   {
	carMode = 9;
    
    ControlCar(7);                                  //将串口数据置于小于状态
   }
   else if (RecvData == 0)
	{
   	 carMode = 0;
	 ControlCar(0);
	}
	else if(RecvData ==5)
	{
	  carMode = 0;
		ControlCar(8);
	}
	else if(RecvData ==6)
	{
	   if(carMode == 0)
		ControlCar(0);
		 if(carMode !=0)
		 carMode==9;
		 
	}
   else if (RecvData!=5 && RecvData!=6 && RecvData!=7 &&RecvData!=0 &&RecvData<9 && carMode == 9)	   //人工模式
   {
    ControlCar(RecvData);  
   }
   RI=0;                                            //清除接收中断标志位
 }
 if(TI==1)                                          //判断是否是发送中断
 {
   TI=0;                                            //清除发送中断标志位
 }
}

void main(void)                                     //主程序入口
{
	RecvData=0;                                      //将接收变量数值初始化
	TMOD=0x01;                                       //选择定时器0为两个16位定时器
	TH0=0xFF;                                        //对定时器进行计数值进行初始化
	TL0=0x19;                                        
	TR0=1;                                           
	EX1=1;                                         
	IT1=1;                                         
	ET0=0;
	SCON=80;                                         //设置串口模式为8位数据
	TMOD=33;                                         //设置定时/计数器模式
	TH1=0xFD;                                        //给定时器1高八位初始化初值
	TL1=0xFD;                                        //给定时器1低八位初始化初值
	TR1=1;                                           //开启定时器1
	ES=1;                                            //开启串口通信功能
	REN=1;                                           //开启接收中断标志
	EA=1;                                            //总中断开启
	carMode = 0;                                    
	ControlCar(8);                                   //将小车置于前进状态
	ModeLed=LedShowData[0];                          //数码管显示数字0
    PCON=0;                     //SMOD=0
	while(1)                                         //程序主循环
	{
	    
			if(carMode ==0 )
			{
			  
				if(FontIR==0 )     //判断正前方,前左侧,前右侧红外探头状态
				{
					ControlCar(2);                       
					Delay();                             
					Delay();                             
					ControlCar(3);                       
					Delay();                             
					Delay();                           
					ControlCar(0);                        
				}
				if(LeftIR==0 && RightIR ==1)      //左前方有障碍物，右前方没有
				{
					ControlCar(2);                        
					Delay();                             
					Delay();                              	
					ControlCar(3);
					Delay();
					Delay();					
					ControlCar(0); 
				}				
				if(LeftIR==1 && RightIR ==0)      //右前方有障碍物，左前方没有
				{
					ControlCar(2);                        
					Delay();                             
					Delay();                              					
					ControlCar(4);
					Delay();
					Delay();				
					ControlCar(0); 
				}
			}
		}	
}

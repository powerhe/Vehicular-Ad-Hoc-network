#include <reg51.h>                                  
typedef unsigned char uchar;                          
typedef unsigned int uint;                           
#define ModeLed P0                                  //定义数码管显示端口
uchar  idata trdata[]={'5'};
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
sbit SIGNAL=P0^1;
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
    { M1A=1;
	  M1B=0;                                         
      M2A=1;
	  M2B=0;  
	  M3A=1;
      M3B=0;
      M4A=1;
      M4B=0;                                 
      ModeLed=LedShowData[0];   //液晶显示                   
      break;                                        
    }
	case 1: //前进                                 
    { M1A=1;
	  M1B=0;                                         
      M2A=1;
	  M2B=0;  
	  M3A=1;
      M3B=0;
      M4A=1;
      M4B=0;                                    
      ModeLed=LedShowData[1];                      
      break;                                        
    }
    case 2: //后退                                    
    { M1A=0;
	  M1B=1;                                         
      M2A=0;
	  M2B=1;  
	  M3A=0;
      M3B=1;
      M4A=0;
      M4B=1;       
	  //LedBit=0xfe;                                                              
      ModeLed=LedShowData[2];                                                           
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
      ModeLed=LedShowData[3];                                                           
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
      ModeLed=LedShowData[4];                                                     
      break;                                         
    }
 	
	case 8:  //停止
	{
	  ModeLed=LedShowData[8];
	  break;
	}
    default:                                          
    {
     break;                                          
    }
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
     SCON= 0x40;                                      //串口方式1
     PCON=0;                                         //SMOD=0
     TMOD= 0x20;                                     //定时器1定时方式2
	while(1)                                         //程序主循环
	{
	    int i,j;
		if(SIGNAL==0)
		 {			
              SBUF=trdata[0];			
              while(TI==0);
              TI=0;
              for (j=0;j<25000;j++);   
      		
         }
	    if(SIGNAL==1)
		 {     
              SBUF='A';
              while(TI==0);
              TI=0;
      
         } 
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

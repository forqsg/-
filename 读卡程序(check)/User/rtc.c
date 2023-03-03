#include "sys.h"
#include "rtc.h" 
#include "delay.h"
#include "stm32f10x.h"
#include "usart.h" 
#include <string.h>
#include "JLX12864.h"
u8 t=0;	 
char time_str[8];
u8 light_flag,light_time_count;
u8 read_flag1,read_flag1_count,read_flag2,err_flag,err_count,clear_id_count,clear_id_flag;
u8 now_time[2]; //��ʱ���־
tm timer;//ʱ�ӽṹ�� 	   
//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������
void show_time(void)
{
	
	  RTC_Get();//����ʱ�� 	
    now_time[0]=timer.hour;
	  now_time[1]=timer.min;
//	if((timer.hour>=20&&timer.hour<=23)||(timer.hour<=5))
//		yejia_flag=1;
//	else yejia_flag=0;
	  sprintf(time_str,"%02d-%02d %02d:%02d:%02d",timer.w_month,timer.w_date,timer.hour,timer.min ,timer.sec);		/*ʹ��c��׼��ѱ���ת�����ַ���*/
//	  display_num_string(3,41,0x80,0,2,timer.hour);//�������������»���
//		display_string_8x16(3,57,":");	
//    display_num_string(3,65,0x80,0,2,timer.min);
//		display_string_8x16(3,81,":");		
//    display_num_string(3,89,0x80,0,2,timer.sec);		
}
void RTC_NVIC_Config(void)
{	
     NVIC_InitTypeDef NVIC_InitStructure;
	   NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//��ռ���ȼ�0λ,�����ȼ�4λ
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	   NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}
u8 RTC_Init(void)
{
	//����ǲ��ǵ�һ������ʱ��
	u8 temp=0;
	if(BKP->DR1!=0X5050)//��һ������
	{	 
	  	RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	    
		RCC->APB1ENR|=1<<27;     //ʹ�ܱ���ʱ��	    
		PWR->CR|=1<<8;           //ȡ��������д����
		RCC->BDCR|=1<<16;        //����������λ	   
		RCC->BDCR&=~(1<<16);     //����������λ����	  	 
	    RCC->BDCR|=1<<0;         //�����ⲿ�������� 
	    while((!(RCC->BDCR&0X02))&&temp<250)//�ȴ��ⲿʱ�Ӿ���	 
		{
			temp++;
			delay_ms(10);
		};
		if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	    

		RCC->BDCR|=1<<8; //LSI��ΪRTCʱ�� 	    
		RCC->BDCR|=1<<15;//RTCʱ��ʹ��	  
	  	while(!(RTC->CRL&(1<<5))) ;//�ȴ�RTC�Ĵ����������	 
    	while(!(RTC->CRL&(1<<3)));//�ȴ�RTC�Ĵ���ͬ��  
    	RTC->CRH|=0X01;  		  //�������ж�
    	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������	 
		RTC->CRL|=1<<4;              //��������	   
		RTC->PRLH=0X0000;
		RTC->PRLL=32767;             //ʱ����������(�д��۲�,���Ƿ�������?)����ֵ��32767										 
//		Auto_Time_Set();
			RTC_Set(2016,3,13,10,0,0); //����ʱ��	 
		RTC->CRL&=~(1<<4);           //���ø���
		while(!(RTC->CRL&(1<<5)));   //�ȴ�RTC�Ĵ����������		 									  
		BKP->DR1=0X5050;
		//BKP_Write(1,0X5050);;//�ڼĴ���1����Ѿ������� 
		//printf("FIRST TIME\n");
	}else//ϵͳ������ʱ
	{
    	while(!(RTC->CRL&(1<<3)));//�ȴ�RTC�Ĵ���ͬ��  
    //	RTC->CRH|=0X01;  		  //�������ж�
		RTC->CRH|=0X03;  		  //������������ж�
    	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������
		//printf("OK\n");
	}		    				  
//	MY_NVIC_Init(0,0,RTC_IRQChannel,2);//RTC,G2,P2,S2.���ȼ����
     RTC_NVIC_Config();
     
	RTC_Get();//����ʱ�� 
	return 0; //ok
}
//RTC�жϷ�����		 
//const u8* Week[2][7]=
//{
//{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"},
//{"��","һ","��","��","��","��","��"}
//};					    
//RTCʱ���ж�
//ÿ�봥��һ��   
void RTC_IRQHandler(void)
{


if(light_flag==1)//�а������µ�ʱ�򱳹⿪��10s
{
	LEDA=1;
	light_time_count++;
	if(light_time_count==6)
	{
		light_time_count=0;
		//LEDA=0;
		light_flag=0;
	}
}
if(read_flag1==1)//���յ��û�ˢ����Ϣ��ʱ��
{
	read_flag1_count++;
	if(read_flag1_count==3)
	{
		read_flag1_count=0;
		display_GB2312_string(3,57,0,"      ");
		display_GB2312_string(5,73,0,"      ");
		read_flag1=0;
	}
	
}

if(err_flag==1)//���յ��û�ˢ����Ϣ��ʱ��
{
	err_count++;
	if(err_count==3)
	{
		err_count=0;
		display_GB2312_string(3,1,0,"�û���:        ");

		err_flag=0;
	}
	
}

if(clear_id_flag==1)//���յ��û�ˢ����Ϣ��ʱ��
{
	clear_id_count++;
	if(clear_id_count==4)
	{
		clear_id_count=0;
    display_GB2312_string(1,1,0,"�û�ID:00"); 
		clear_id_flag=0;
	}
	
}




	if(RTC->CRL&0x0001)//�����ж�
	{							
		RTC_Get();//����ʱ�� 	 
		show_time();
 	}
	if(RTC->CRL&0x0002)//�����ж�
	{
		//printf("Alarm!\n");	
		RTC->CRL&=~(0x0002);//�������ж�
		}
    RTC->CRL&=0X0FFA;         //�������������жϱ�־
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������		   							 	   	 
}
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
													    
	//����ʱ��
    RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
    RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    //ȡ��������д����
	//���������Ǳ����!
	RTC->CRL|=1<<4;   //�������� 
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//���ø���
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������  	
	return 0;	    
}



u8 RTC_Set_Alr(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
													    
	//����ʱ��
    RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
    RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    //ȡ��������д����
	//���������Ǳ����!
	RTC->CRL|=1<<4;   //�������� 
	RTC->ALRL=seccount&0xffff;
	RTC->ALRH=seccount>>16;
	RTC->CRL&=~(1<<4);//���ø���
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������  	
	return 0;	    
}


//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	   
	timecount=RTC->CNTH;//�õ��������е�ֵ(������)
	timecount<<=16;
	timecount+=RTC->CNTL;			 

	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else break;  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		timer.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		timer.w_month=temp1+1;//�õ��·�
		timer.w_date=temp+1;  //�õ����� 
	}
	temp=timecount%86400;     //�õ�������   	   
	timer.hour=temp/3600;     //Сʱ
	timer.min=(temp%3600)/60; //����	
	timer.sec=(temp%3600)%60; //����
	return 0;
}	 














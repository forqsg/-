#ifndef __RTC_H
#define __RTC_H	   
#include "stm32f10x.h"

extern  u8 light_flag,light_time_count;
extern u8 read_flag1,read_flag1_count,read_flag2,err_flag,err_count,clear_id_count,clear_id_flag;//�û���ر�־����
extern u8 now_time[2];
//ʱ��ṹ��
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	//������������
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}tm;					 
 
extern u8 const mon_table[12];//�·��������ݱ�
void Disp_Time(u8 x,u8 y,u8 size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//��ָ��λ����ʾ����
u8 RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��
u8 RTC_Set_Alr(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ������	  
void show_time(void);
//void Auto_Time_Set(void);//����ʱ��Ϊ����ʱ��
void Delay_Time(void);
#endif




























 

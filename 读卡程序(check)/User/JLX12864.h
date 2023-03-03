#ifndef _JLX12864_H
#define _JLX12864_H
#include "sys.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define lcd_cs1 		PAout(8)  						//
#define lcd_reset 	PBout(15)  						//
#define lcd_rs 			PBout(14)  						//
#define lcd_sid 		PBout(13) 
#define lcd_sclk 		PBout(12)  
#define Rom_CS     	PBout(6)  
#define Rom_SCK     PBout(7)  
#define Rom_IN     	PBout(9)  
#define LEDA        PAout(0)
//IO��������
#define ROM_OUT_IN()  {GPIOB->CRH&=0xFFFF0FFF;GPIOC->CRH|=0x00000008;}				//�Ͱ�λ���ŵ�PB8�Ŷ���Ϊ����
#define ROM_OUT_OUT() {GPIOB->CRH&=0xFFFF0FFF;GPIOC->CRH|=0x00000003;}  	   	//�Ͱ�λ���ŵ�PB8�Ŷ���Ϊ���
//IO��������											   
#define	Rom_OUT 		PBout(8) 			//���ݶ˿�	PB8
#define	Rom_OUT_cin PBin(8)  			//���ݶ˿�	PB8

void JLX12864G_132_GPIOInit(void);
void transfer_command_lcd(int data1);
void initial_lcd(void);
void clear_screen(void); 
void display_128x64(uchar *dp); //��ʾ128*64�ĵ���ͼ��
void display_graphic_16x16(uint page,uint column,uchar reverse,uchar *dp);
void display_graphic_8x16(uint page,uchar column,uchar reverse,uchar *dp); 
void display_graphic_5x7(uint page,uchar column,uchar reverse,uchar *dp); 
void display_GB2312_string(uchar y,uchar x,uchar reverse,uchar *text); 
void display_string_5x7(uchar y,uchar x,uchar *text); 

//void transfer_command_lcd(int data1);
//void transfer_data_lcd(int data1);
//void initial_lcd(void);
//void lcd_address(uint page,uint column);
//void clear_screen(void);
//void display_128x64(uchar *dp);
//void display_graphic_16x16(uchar page,uchar column,uchar *dp);
//void display_graphic_8x16(uchar page,uchar column,uchar *dp);
//void display_graphic_5x8(uchar page,uchar column,uchar *dp);
//void send_command_to_ROM( uchar datu );
//static uchar get_data_from_ROM(void);
//void get_and_write_16x16(ulong fontaddr,uchar page,uchar column);
//void get_and_write_8x16(ulong fontaddr,uchar page,uchar column);
//void get_and_write_5x8(ulong fontaddr,uchar page,uchar column);
//void display_GB2312_string(uchar page,uchar column,uchar *text);
//void display_string_5x8(uchar page,uchar column,uchar *text);

#endif

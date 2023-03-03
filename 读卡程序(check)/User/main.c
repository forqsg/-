#include "main.h"
#include "usart.h"
#include "spi_driver.h"
#include "RC522.h"
#include "delay.h"
#include "string.h"
#include "adc.h"
#include "math.h"
#define BLOCK 6
char readBlock();
char str_send[6];
void BEEP_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//ʹ�� GPIOB �˿�ʱ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //BEEP-->GPIOA11 �˿�����
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�Ϊ 50MHz
GPIO_Init(GPIOB, &GPIO_InitStructure); //���ݲ�����ʼ�� GPIOA11
GPIO_SetBits(GPIOB,GPIO_Pin_5); //��� 1���رշ��������
}

int main(void)
{
	u8 uid[10];
	uint8_t status;
	delay_init();
	SystemInit();//ϵͳʱ�ӳ�ʼ��
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	BEEP_Init();
	RC522_IO_Init();
	PcdReset();
	PcdAntennaOff();
	delay_ms(2);
	PcdAntennaOn();
	while(1)
	{
  	status = readBlock(6,uid);
		if(MI_OK == status)
		{					
			PAout(11)=0;//��������
			delay_ms(50);//����50����
			PAout(11)=1;//�رշ�����		
			
		
			sprintf(str_send,"%02s",uid);//����ȡ��UIDת��Ϊ2λ���ַ���ʽ������ֵ��str_send
		  printf("book%s\r\n",str_send);//��������֡ǰ��book�����ڽ��ն����ݽ����ж�
	
		}
   }
 }
char readBlock(int block,u8 *uid)
{
	
	uint8_t i;
	uint8_t Card_Type1[2];
	uint8_t Card_ID[4];
	uint8_t Card_KEY[6] = {0xff,0xff,0xff,0xff,0xff,0xff};    //{0x11,0x11,0x11,0x11,0x11,0x11};   //����
	uint8_t Card_Data[16];
	uint8_t status;
	delay_ms(10);
	if(MI_OK==PcdRequest(0x26, Card_Type1))
	{
		uint16_t cardType = (Card_Type1[0]<<8)|Card_Type1[1];//��2��8λ������Card_Type1[0]��Card_Type1[1]��ϳ�һ��16λ������

		delay_ms(10);
		status = PcdAnticoll(Card_ID);//����ײ
		if(status != MI_OK){
		//	printf("Anticoll Error\r\n");
		}else{
			
			sprintf(uid,"%02X",Card_ID[1]);
		}
		status = PcdSelect(Card_ID);  //ѡ��
		if(status != MI_OK){
		//	printf("Select Card Error\r\n");
		}
		status = PcdAuthState(PICC_AUTHENT1A,5,Card_KEY,Card_ID);
		if(status != MI_OK){
		//	printf("Auth State Error\r\n");
			return MI_ERR;
		}
		memset(Card_ID,1,4);
		delay_ms(15);
		PcdHalt();
		return MI_OK;
	}
}



/*********************************END OF FILE**********************************/

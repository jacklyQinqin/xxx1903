#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"
#include "DownloadFile.h"


unsigned char spiReadDummy[32]={0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5,0xe5};

unsigned char spiReadFactoryCmd[]= {0x40,0x42,0x53,0x55,0x0E,0x00,0x00,0x00,0xBF,0x48,0x00,0x00,0x12,0xEF,0xff,0xff};

#define RECEIVE_BUFF_LEN 32
unsigned char receive_buff[RECEIVE_BUFF_LEN];

unsigned char chip_status = 0;

extern unsigned long right_count;
extern unsigned long error_count;


static void ClearReceiveBuff(void)
{
    memset(receive_buff,0x00,RECEIVE_BUFF_LEN);
}

static void ClearDummyBuff(void)
{
    memset(spiReadDummy,0x00,RECEIVE_BUFF_LEN);
}
/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

/* USER CODE BEGIN 1 */
//使用DMA发送一组数据
static void MX_SPI1_TxDMA(unsigned char *pTx,unsigned long txLength)
{
    
    int i = 0;
    //拉低SS
    HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin,GPIO_PIN_RESET);
    //for(i=0;i<50;i++);
    HAL_SPI_Transmit_DMA(&hspi1,pTx,txLength);
    //等待发送完毕
   while(HAL_SPI_STATE_BUSY_TX == HAL_SPI_GetState(&hspi1));
    
   HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin,GPIO_PIN_SET); 
}



static unsigned long MX_SPI1_Rx(unsigned char *pRx,unsigned short len)
{
	
    int i = 0;
    //拉低SS
    HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin,GPIO_PIN_RESET);
    for(i=0;i<50;i++);
    if(HAL_OK != HAL_SPI_Receive_DMA(&hspi1,pRx,len))
    {
        while(1);
    }
    HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin,GPIO_PIN_SET); 
    return 0;
}






void SM2_Update_Send(unsigned char *buff, unsigned int len)
{
    int time = 0;
    int  i = 0;
    time  = (len+15)/16;
    if(time == 0)
        return;
    while(time--)
    {
        MX_SPI1_TxDMA(&buff[i],16);
        i+=16;
        HAL_Delay(1);
    }
}


void SM2_Update_Receive(unsigned char *buff, unsigned int len)
{
    int time = 0;
    int  i = 0;
    time  = (len+15)/16;
    if(time == 0)
        return;
    while(time--)
    {
        MX_SPI1_Rx(&buff[i],16);
        i+=16;
    }
}

//unsigned char SPI2ReceiveRespond(unsigned char * prespond)
//{
//    volatile long i ,time,len,count,j;
//    unsigned char  redownloadTime = 0;
//    unsigned char  xorv = 0;
//    unsigned char *addr = 0;
//    MX_SPI1_Rx(prespond,16);
//    
//    //指令错误
//    if(prespond[0]==0x63)
//    {
//        return 3;
//    }    
//    //        指令头不对。
//    if(prespond[0]!=0x50)
//    {
//        return 1; //指令返回头不对。
//    }
//    
//    len = prespond[4];
//    count = len/16;
//    
//    xorv = get_xor(prespond,len);
//    if(xorv!=0)
//    {
//        return 0;
//    }
//    
//   for(i=0;i<count;i++)
//    {
//        SPI2ReceiveData(prespond+16*i+16,16);
//    }     
//}

unsigned char SyncBootloader(void)
{
   volatile long i ,time,len,count,j;
    unsigned char  redownloadTime = 0;
    unsigned char  xorv = 0;
    unsigned char *addr = 0;
    unsigned char tempstatus;

    memset(receive_buff,sizeof(receive_buff),0x00);
    SM2_Update_Receive(receive_buff,16);
    for(i=0;i<0x10000;i++);
    if(((receive_buff[0] == 0x63) && (receive_buff[1] == 0x62) 
        &&(receive_buff[2] == 0x63)&& (receive_buff[3] == 0x65)) || ((receive_buff[0] == 0x50) && (receive_buff[1] == 0x42) 
        &&(receive_buff[2] == 0x53)&& (receive_buff[3] == 0x55)))
    {

        ClearReceiveBuff();
        return 0;
    }   
    return 1;
}


extern unsigned int  TEST_POINT; 
#define READ_ERROR 0X10
#define STATUS_ERROR 0XFF
//测试通过
//unsigned char ReadChipStatus(void)
//{
//    volatile long i ,time,len,count,j;
//    unsigned char  redownloadTime = 0;
//    unsigned char  xorv = 0;
//    unsigned char *addr = 0;
//    unsigned char tempstatus;
//    unsigned char oneByteString[20];

//    SM2_Update_Send(spiReadFactoryCmd,16);
//    
//     HAL_Delay(2);
//        ClearReceiveBuff();

//    SM2_Update_Receive(receive_buff,16);
//    HAL_Delay(2);
//    
//     ClearReceiveBuff();
//     SM2_Update_Receive(receive_buff,16);
//     HAL_Delay(2);
//    if((receive_buff[0] == 0x50) && (receive_buff[1] == 0x42) 
//        &&(receive_buff[2] == 0x53)&& (receive_buff[3] == 0x55) && (receive_buff[3] != 0xbf))
//    {
//        HAL_Delay(1);
//        SM2_Update_Receive((receive_buff+16),16);
//        tempstatus = receive_buff[25];
//        ClearReceiveBuff();
//        return tempstatus;
//        
//    }   
//    
//    return READ_ERROR;
//   
//}

////改1、测试

//#define STATUS_ERROR 0XFF
//unsigned char ReadChipStatus(void)
//{
//    volatile long i ,time,len,count,j;
//    unsigned char  redownloadTime = 0;
//    unsigned char  xorv = 0;
//    unsigned char *addr = 0;
//    unsigned char tempstatus;
//    unsigned char oneByteString[20];

//    SM2_Update_Send(spiReadFactoryCmd,16);
//    
//    //120M时钟。120 000 000  120=1us
//    for(i=0;i<12000;i++); 
//    SM2_Update_Receive((receive_buff),16);
//    for(time=0;time<6;time++)
//    {
//        //如果在出错的状态
//        if((receive_buff[0] == 0x40) && (receive_buff[1] == 0x42) 
//        &&(receive_buff[2] == 0x53)&& (receive_buff[3] == 0x55))
//        {
//            SM2_Update_Receive((receive_buff),16);        
//            for(i=0;i<12000;i++); 
//            SM2_Update_Receive((receive_buff+16),16);
//            tempstatus = receive_buff[25];
//            ClearReceiveBuff();
//            return tempstatus;
//        } 
//        
//        if((receive_buff[0] == 0x50) && (receive_buff[1] == 0x42) 
//        &&(receive_buff[2] == 0x53)&& (receive_buff[3] == 0x55))
//        {
//            for(i=0;i<12000;i++); 
//            SM2_Update_Receive((receive_buff+16),16);
//            tempstatus = receive_buff[25];
//            ClearReceiveBuff();
//            return tempstatus;
//        }  
//        //如果上一次的指令错误。则发送完指令之后接收到的第一包数据，就是63 62 63 65
//        //如何处理？
//        if((receive_buff[0] == 0x63) && (receive_buff[1] == 0x62) 
//            &&(receive_buff[2] == 0x63)&& (receive_buff[3] == 0x65))
//        {
//            for(i=0;i<12000;i++); 
//            //SM2_Update_Receive((receive_buff),16);
//            ClearReceiveBuff();
//            return STATUS_ERROR;
//        } 
//        for(i=0;i<12000;i++); 
//        SM2_Update_Receive((receive_buff),16);    

//        
//    }
//    

//    ClearReceiveBuff();
//    return STATUS_ERROR;
//}

//改2、测试
#define STATUS_ERROR 0XFF
unsigned char ReadChipStatus(void)
{
    volatile long i ,time,len,count,j;
    unsigned char  redownloadTime = 0;
    unsigned char  xorv = 0;
    unsigned char *addr = 0;
    unsigned char tempstatus;
    unsigned char oneByteString[20];
    SM2_Update_Send(spiReadFactoryCmd,16);
    
    //120M时钟。120 000 000  120=1us  此处延时10us。
    for(i=0;i<12000;i++); 
    SM2_Update_Receive((receive_buff),16);
    for(i=0;i<12000;i++); 
    SM2_Update_Receive((receive_buff),16);
    
    if((receive_buff[0] == 0x40) && (receive_buff[1] == 0x42) 
    &&(receive_buff[2] == 0x53)&& (receive_buff[3] == 0x55))
    {
        for(i=0;i<12000;i++); 
        SM2_Update_Receive((receive_buff),16);
        for(i=0;i<12000;i++); 
        SM2_Update_Receive((receive_buff+16),16);
        tempstatus = receive_buff[25];
        ClearReceiveBuff();
        return tempstatus;
    }  

    if((receive_buff[0] == 0x50) && (receive_buff[1] == 0x42) 
    &&(receive_buff[2] == 0x53)&& (receive_buff[3] == 0x55))
    {
        for(i=0;i<12000;i++); 
        SM2_Update_Receive((receive_buff+16),16);
        tempstatus = receive_buff[25];
        ClearReceiveBuff();
        return tempstatus;
    }  
    //如果上一次的指令错误。则发送完指令之后接收到的第一包数据，就是63 62 63 65
    if((receive_buff[0] == 0x63) && (receive_buff[1] == 0x62) 
        &&(receive_buff[2] == 0x63)&& (receive_buff[3] == 0x65))
    {
        for(i=0;i<12000;i++); 
        ClearReceiveBuff();
        return STATUS_ERROR;
    } 

    ClearReceiveBuff();
    return STATUS_ERROR;
}

void SPI2SendCommand(unsigned char *pcommand)
{

	volatile long i,len,count,j;
    unsigned char temp[16];
    
	len=0;
	memcpy(temp,pcommand,16);
    
	len = temp[5];
	len = (len*256)+temp[4];
	count = (len+15)/16;     
	for(i=0;i<count;i++)
	{
		MX_SPI1_TxDMA(pcommand+16*i,16);
        for(j=0;j<1200;j++);    //10us
	}    
}

#define MAX_LINE  76
volatile long time; 
unsigned char DownloadStatusProcess(void)
{
	volatile long i ,len,count,j;
    unsigned char  tempstatus = 0,returnstatus=0;
	unsigned char  xorv = 0;
	unsigned char  tempreceive[16];
    if(1)
    {

        for(time=0;time< MAX_LINE;time++)
        {
            SPI2SendCommand((unsigned char *)arrayPointer[time]);
            //一般第二条为全片擦指令需要多等待
            if((time == 1) || (time == 3))
            {
                HAL_Delay(60);//第二条为片擦指令。需要延时 30ms。
            }
            else
            {
                HAL_Delay(3);
            }
            ClearDummyBuff();
            SM2_Update_Receive(spiReadDummy,16);
            for(i=0;i< 1200;i++);//10us
            
            SM2_Update_Receive(receive_buff,16);
            if(receive_buff[8]== 0x90)
            {
                right_count++;
            }
            else
            {
                error_count++;
                while(1);
            }
            ClearReceiveBuff();
        }
    }
}


void SM2_Update(void)
{
    while(1)
    {
       chip_status  = ReadChipStatus(); 
       if(( chip_status == 0x03)  || ( chip_status == 0x04) || ( chip_status == 0x05))
       {
            right_count++;
            DownloadStatusProcess();
            while(1);
       }
       else
       {    
           error_count++;
       }
    }
}
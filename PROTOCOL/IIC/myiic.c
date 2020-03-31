#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "esp_heap_caps.h"

#include "myiic.h"

#define i2c_master_wait     os_delay_us
#define i2c_delay_times     5

void mIIC_Init(void)
{
    gpio_config_t io_conf;				 	    //创建一个GPIO结构体
    io_conf.intr_type = GPIO_INTR_DISABLE;	    //失能中断服务
    io_conf.mode = GPIO_MODE_OUTPUT;			//设置成输出模式
    io_conf.pin_bit_mask = IIC_OUTPUT_PIN_SEL;  //指定要配置的GPIO
    io_conf.pull_down_en = 0;				    //不使能下拉
    io_conf.pull_up_en = 0;					    //不使能上拉
    
    gpio_config(&io_conf);  					//配置GPIO
    
    IIC_SCL_OUT(1);
    IIC_SDA_OUT(1);
}

void mIIC_SDA_Mode_Change(sda_mode_t SDA_Mode)
{
    gpio_config_t io_conf;				 	    //创建一个GPIO结构体
    io_conf.intr_type = GPIO_INTR_DISABLE;	    //失能中断服务
    io_conf.mode = SDA_Mode;			        
    io_conf.pin_bit_mask = (1ULL<<IIC_SDA_PIN); //指定要配置的GPIO
    io_conf.pull_down_en = 0;				    //不使能下拉
    io_conf.pull_up_en = 0;					    //不使能上拉
    gpio_config(&io_conf);  					//配置GPIO
}

void mIIC_Start(void)
{
    mIIC_SDA_Mode_Change(SDA_MODE_WRITE);	//SDA切换到输出模式

    IIC_SCL_OUT(1);
    IIC_SDA_OUT(1);
	
	i2c_master_wait(i2c_delay_times);
	IIC_SDA_OUT(0);
	i2c_master_wait(i2c_delay_times);
	IIC_SCL_OUT(0);		                    //保持占用总线，时钟线为低电平的时候，可以修改SDA的值
	i2c_master_wait(i2c_delay_times);

}

void mIIC_Stop(void)
{
    mIIC_SDA_Mode_Change(SDA_MODE_WRITE);	//SDA切换到输出模式
    
	IIC_SCL_OUT(1);
    IIC_SDA_OUT(0);
    
	i2c_master_wait(i2c_delay_times);
	
	IIC_SDA_OUT(1);
	
	i2c_master_wait(i2c_delay_times);
}

void mIIC_Send_Ack(void)
{
    mIIC_SDA_Mode_Change(SDA_MODE_WRITE);	//SDA切换到输出模式

	IIC_SCL_OUT(0);
    IIC_SDA_OUT(0);
	i2c_master_wait(i2c_delay_times);
	
	IIC_SCL_OUT(1);
	i2c_master_wait(i2c_delay_times);
	
	IIC_SCL_OUT(0);
	i2c_master_wait(i2c_delay_times);
}

void mIIC_Send_NAck(void)                   //即SCL在SDA为高电平下完成高低电平跳变
{
    mIIC_SDA_Mode_Change(SDA_MODE_WRITE);	//SDA切换到输出模式
    
	IIC_SCL_OUT(0);
    IIC_SDA_OUT(0);
	i2c_master_wait(i2c_delay_times);
	
	IIC_SDA_OUT(1);
	i2c_master_wait(i2c_delay_times);
	
	IIC_SCL_OUT(1);
	i2c_master_wait(i2c_delay_times);
	IIC_SCL_OUT(0);
	i2c_master_wait(i2c_delay_times);
}

uint8_t mIIC_Wait_Ack(void)
{
	uint8_t ask = 0;
	
	mIIC_SDA_Mode_Change(SDA_MODE_READ);	//SDA切换到输入模式
	
	IIC_SCL_OUT(1);                         //拉高时钟线
	
	i2c_master_wait(i2c_delay_times);
	
	if(IIC_SDA_IN())			            //IIC_SDA_IN() = 1无应答
		ask = 1;
	else ask = 0;
	
	IIC_SCL_OUT(0);
	
	i2c_master_wait(i2c_delay_times);

	mIIC_SDA_Mode_Change(SDA_MODE_WRITE);   //切换回输出模式
	
	return ask;
}	

void mIIC_Send_Byte(uint8_t byte)
{
	int8_t i=0;
	
	IIC_SCL_OUT(0);
    IIC_SDA_OUT(0);
	
	i2c_master_wait(i2c_delay_times);
	
	for(i=7; i>=0; i--)
	{
		if(byte & (1<<i))
			IIC_SDA_OUT(1);
		else
			IIC_SDA_OUT(0);
		
		i2c_master_wait(i2c_delay_times);
		
		IIC_SCL_OUT(1);
		i2c_master_wait(i2c_delay_times);	
	
		IIC_SCL_OUT(0);
		i2c_master_wait(i2c_delay_times);		
	}
}

uint8_t mIIC_Read_Byte(uint8_t ask)
{
	int8_t i = 0;
	uint8_t byte = 0;
	
	mIIC_SDA_Mode_Change(SDA_MODE_READ);	//SDA切换到输入模式
	i2c_master_wait(i2c_delay_times);		
	
	for(i = 7; i >=0 ; i--)
	{
		IIC_SCL_OUT(1);                     //时钟线为高时，采集SDA数据电平
		i2c_master_wait(i2c_delay_times);		
		
		if(IIC_SDA_IN())
			byte |= 1<<i;

		IIC_SCL_OUT(0);
		i2c_master_wait(i2c_delay_times);		
	
	}
	
	mIIC_SDA_Mode_Change(SDA_MODE_WRITE);	//SDA切换到输出模式
	
	if(ask)
		mIIC_Send_Ack();
	else
		mIIC_Send_NAck();
	
	
	return byte;
}




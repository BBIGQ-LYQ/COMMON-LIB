#ifndef _MYIIC_H_
#define _MYIIC_H_

#include <stdint.h>
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IIC_SCL_PIN     2
#define IIC_SDA_PIN     14

#define IIC_OUTPUT_PIN_SEL  ((1ULL<<IIC_SCL_PIN) | (1ULL<<IIC_SDA_PIN)) 

#define IIC_SCL_OUT(state)      (gpio_set_level(IIC_SCL_PIN, state))
#define IIC_SDA_OUT(state)      (gpio_set_level(IIC_SDA_PIN, state))

#define IIC_SDA_IN()            (gpio_get_level(IIC_SDA_PIN))

typedef enum {
    SDA_MODE_WRITE = GPIO_MODE_OUTPUT,  /*!< SDA 写模式 */
    SDA_MODE_READ = GPIO_MODE_INPUT,    /*!< SDA 读模式 */
} sda_mode_t;

void mIIC_Init(void);                               /*!< I2C初始化 */

void mIIC_SDA_Mode_Change(sda_mode_t SDA_Mode);     /*!< I2C SDA模式切换 */

void mIIC_Start(void);                              /*!< I2C起始信号 */

void mIIC_Stop(void);                               /*!< I2C停止信号 */

void mIIC_Send_Ack(void);                           /*!< I2C发送应答 */

void mIIC_Send_NAck(void);                          /*!< I2C发送非应答 */

uint8_t mIIC_Wait_Ack(void);                         /*!< I2C等待应答 */

void mIIC_Send_Byte(uint8_t byte);                  /*!< I2C发送一个字节数据 */

uint8_t mIIC_Read_Byte(uint8_t ask);                /*!< I2C读取一个字节数据 */


#ifdef __cplusplus
}
#endif

#endif



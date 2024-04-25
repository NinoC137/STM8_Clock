#ifndef __OLED_H
#define __OLED_H

#define        uint8_t        unsigned    char
#define        uint16_t         unsigned    int
#define        uint32_t        unsigned    long

#define OLED_CMD 0 
#define OLED_DATA 1
#define OLED_MODE 0
#define SIZE 8
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

#define SCL PB_ODR_ODR4 
#define SDA PB_ODR_ODR5 
#define SDAM PB_IDR_IDR5

#define SET_SCL_OUT() {PB_DDR_DDR4=1; PB_CR1_C14 = 1; PB_CR2_C24 = 0;} 
#define SET_SDA_OUT() {PB_DDR_DDR5=1; PB_CR1_C15 = 1; PB_CR2_C25 = 0;} 
#define SET_SDA_IN() {PB_DDR_DDR5=0; PB_CR1_C15= 0; PB_CR2_C25 = 0;} 

void Delay_5us(void) ;
void delayms(unsigned int ms);

void I2C_Init(void);
void I2C_Start(void) ;
void I2C_Stop(void) ;
void I2C_SendACK(uint8_t ack) ;
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
uint8_t I2C_RecvByte() ;

void OLED_Init(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void fill_picture(unsigned char fill_Data);
void OLED_Clear(void);
void OLED_On(void);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size);
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char const *chr);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned long int num,unsigned char len,unsigned char size);
void OLED12864_ShowChianese(unsigned char x,unsigned char y,unsigned char *s,unsigned char len);

#endif
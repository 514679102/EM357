// *******************************************************************
//  lcd.c
//
//  common functions for lcd driver  128*64
//
//  Copyright by kaiser. All rights reserved.          
// *******************************************************************

#include <reg52.H> 
#include <intrins.h> 
 
sbit lcd_sclk=P3^2;  /*�ӿڶ���:lcd_sclk ���� LCD �� sclk*/ 
sbit lcd_sid=P3^1;   /*�ӿڶ���:lcd_sid ���� LCD �� sid*/ 
sbit lcd_rs=P3^0;    /*�ӿڶ���:lcd_rs ���� LCD �� rs*/ 
sbit lcd_reset=P1^0; /*�ӿڶ���:lcd_reset ���� LCD �� reset*/ 
sbit lcd_cs1=P1^1;   /*�ӿڶ���:lcd_cs1 ���� LCD �� cs1*/ 
 
sbit Rom_IN=P1^2;    /*�ֿ� IC �ӿڶ���:Rom_IN �����ֿ� IC �� SI*/ 
sbit Rom_OUT=P1^3;   /*�ֿ� IC �ӿڶ���:Rom_OUT �����ֿ� IC �� SO*/ 

sbit Rom_SCK=P1^4;   /*�ֿ� IC �ӿڶ���:Rom_SCK �����ֿ� IC �� SCK*/ 
sbit Rom_CS=P1^5;    /*�ֿ� IC �ӿڶ��� Rom_CS �����ֿ� IC �� CS#*/ 
 
 
typedef  unsigned char  uchar
typedef  unsigned int  uint
typedef  unsigned long ulong
 
const uint8_t jiong1[]={/*--  ����:  ��  --*/ 
/*--  ���� 12;  �������¶�Ӧ�ĵ���Ϊ���� x ��=16x16   --*/ 
0x00,0xFE,0x82,0x42,0xA2,0x9E,0x8A,0x82,0x86,0x8A,0xB2,0x62,0x02,0xFE,0x00,0x00, 
0x00,0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x7F,0x00,0x00}; 
 
const uint8_t lei1[]={/*--  ����:  ��  --*/ 
/*--  ���� 12;  �������¶�Ӧ�ĵ���Ϊ���� x ��=16x16   --*/ 
0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00, 
0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00}; 

const uint8_t bmp1[]={ 
/*--  ������һ��ͼ��D:\�ҵ��ĵ�\My Pictures\12864-555.bmp  --*/ 
/*--  ��� x �߶�=128x64  --*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,0xC0,0xC0,0xE0,0x60,0xE0,0xE0,0xE0,0xE0,0x60, 
 
0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xC0,0xC0,0x80,0x80, 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
 
}; 
 
const uint8_t bmp2[]={ 
/*--  ������һ��ͼ��E:\work\ͼƬ�ղؼ�\JLX12864G-1016-1.bmp  --*/ 
/*--  ��� x �߶�=128x64  --*/ 
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01, 
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xC1,0xC1,0x41,0x41,0x41,0x41,0x41,0xC1,0xC1, 
0x01,0x01,0x01,0x01,0x41,0xC1,0xC1,0x41,0x41,0xC1,0xC1,0x61,0x61,0xC1,0x81,0x01, 
0xE1,0xE1,0x41,0x01,0x01,0x01,0x21,0xE1,0xC1,0x81,0x41,0x41,0xC1,0xC1,0x41,0x41, 
0x41,0xC1,0xC1,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xE1,0xE1,0x01, 
 
};
 
 
/*дָ� LCD ģ��*/ 
void transfer_command_lcd(int data1)    
{ 
 char i; 
 lcd_rs=0; 
 for(i=0;i<8;i++) 
 { 
   lcd_sclk=0; 
   if(data1&0x80) lcd_sid=1; 
   else lcd_sid=0; 
   lcd_sclk=1; 
   data1=data1<<=1; 
 } 
} 
 
/*д���ݵ� LCD ģ��*/ 
void transfer_data_lcd(int data1) 
{ 
 char i; 
 lcd_rs=1; 
 for(i=0;i<8;i++) 
 { 
   lcd_sclk=0; 
   if(data1&0x80) lcd_sid=1; 
   else lcd_sid=0; 
   lcd_sclk=1; 
   data1=data1<<=1; 
 } 
} 
 
/*��ʱ*/ 
void delay(int n_ms)                
{ 
 int j,k; 
 for(j=0;j<n_ms;j++) 
   for(k=0;k<110;k++); 
} 
 
 
/*�ȴ�һ���������ҵ��������� P2.0 �� GND ֮���һ������*/ 
void waitkey() 
{ 
 /*repeat: 
    if (P2&0x01) goto repeat; 
 else delay(6); 
 if (P2&0x01) goto repeat; 
 else 
 delay(40);; */
} 
 
 
/*LCD ģ���ʼ��*/ 
void initial_lcd() 
{ 
   lcd_cs1=0; 
   Rom_CS = 1; 
   lcd_reset=0;        /*�͵�ƽ��λ*/ 
   delay(20); 
   lcd_reset=1;        /*��λ���*/ 
   delay(20);         
   transfer_command_lcd(0xe2);  /*��λ*/ 
   delay(5); 
   transfer_command_lcd(0x2c);  /*��ѹ���� 1*/ 
   delay(5);  
   transfer_command_lcd(0x2e);  /*��ѹ���� 2*/ 
   delay(5); 
   transfer_command_lcd(0x2f);  /*��ѹ���� 3*/ 
   delay(5); 
   transfer_command_lcd(0x23);  /*�ֵ��Աȶȣ������÷�Χ 0x20��0x27*/ 
   transfer_command_lcd(0x81);  /*΢���Աȶ�*/ 
   transfer_command_lcd(0x1f);  /*0x28,΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f*/ 
   transfer_command_lcd(0xa2);  /*1/9 ƫѹ�ȣ�bias��*/ 
   transfer_command_lcd(0xc8);  /*��ɨ��˳�򣺴��ϵ���*/ 
   transfer_command_lcd(0xa0);  /*��ɨ��˳�򣺴�����*/ 
   transfer_command_lcd(0x40);  /*��ʼ�У���һ�п�ʼ*/ 
   transfer_command_lcd(0xaf);  /*����ʾ*/ 
   lcd_cs1=1; 
} 
 
void lcd_address(uint page,uint column) 
{ 
 
 column=column-0x01; 
 transfer_command_lcd(0xb0+page-1);   /*����ҳ��ַ*/ 
 transfer_command_lcd(0x10+(column>>4&0x0f));  /*�����е�ַ�ĸ� 4 λ*/ 
 transfer_command_lcd(column&0x0f);  /*�����е�ַ�ĵ� 4 λ*/   
} 
 
/*ȫ������*/ 
void clear_screen() 
{ 
 unsigned char i,j; 
 lcd_cs1=0; 
 Rom_CS = 1;   
 for(i=0;i<9;i++) 
 { 
   transfer_command_lcd(0xb0+i); 
   transfer_command_lcd(0x10); 
   transfer_command_lcd(0x00); 
   for(j=0;j<132;j++) 
   { 
       transfer_data_lcd(0x00); 
   } 
 } 
 lcd_cs1=1; 
} 
 
/*��ʾ 128x64 ����ͼ��*/ 
void display_128x64(uchar *dp) 
{ 
 uint i,j; 
 lcd_cs1=0; 
 for(j=0;j<8;j++) 
 { 
   lcd_address(j+1,1); 
   for (i=0;i<128;i++) 
   {   
     transfer_data_lcd(*dp);         /*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/ 
     dp++; 
   } 
 } 
 lcd_cs1=1; 
} 
 
 
/*��ʾ 16x16 ����ͼ�񡢺��֡���Ƨ�ֻ� 16x16 ���������ͼ��*/ 
void display_graphic_16x16(uint page,uint column,uchar *dp) 
{ 
 uint i,j; 
 lcd_cs1=0; 
 Rom_CS = 1;    
 for(j=0;j<2;j++) 
 { 
   lcd_address(page+j,column); 
   for (i=0;i<16;i++) 
   {   
     transfer_data_lcd(*dp);         /*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/ 
     dp++; 
   } 
 } 
 lcd_cs1=1; 
} 
 
 
/*��ʾ 8x16 ����ͼ��ASCII, �� 8x16 ����������ַ�������ͼ��*/ 
void display_graphic_8x16(uint page,uchar column,uchar *dp) 
{ 
 uint i,j; 
 lcd_cs1=0;   
 for(j=0;j<2;j++) 
 { 
   lcd_address(page+j,column); 
   for (i=0;i<8;i++) 
   {   
     transfer_data_lcd(*dp);         /*д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1*/ 
     dp++; 
   } 
 } 
 lcd_cs1=1; 
}
/*��ʾ 5*7 ����ͼ��ASCII, �� 5x7 ����������ַ�������ͼ��*/ 
void display_graphic_5x7(uint page,uchar column,uchar *dp) 
{ 
 uint col_cnt; 
 uchar page_address; 
 uchar column_address_L,column_address_H; 
 page_address = 0xb0+page-1; 
  
 lcd_cs1=0;   
  
 column_address_L =(column&0x0f)-1; 
 column_address_H =((column>>4)&0x0f)+0x10; 
  
 transfer_command_lcd(page_address);    /*Set Page Address*/ 
 transfer_command_lcd(column_address_H);  /*Set MSB of column Address*/ 
 transfer_command_lcd(column_address_L);  /*Set LSB of column Address*/ 
  
 for (col_cnt=0;col_cnt<6;col_cnt++) 
 {   
   transfer_data_lcd(*dp); 
   dp++; 
 } 
 lcd_cs1=1; 
} 
 
/****��ָ�����Ѷ�ֿ� IC***/ 
void send_command_to_ROM( uchar datu ) 
{ 
 uchar i; 
 for(i=0;i<8;i++ ) 
 { 
   if(datu&0x80) 
     Rom_IN = 1; 
   else 
     Rom_IN = 0; 
   datu = datu<<1; 
   Rom_SCK=0; 
   Rom_SCK=1; 
 } 
} 
 
/****�Ӿ���Ѷ�ֿ� IC ��ȡ���ֻ��ַ����ݣ�1 ���ֽڣ�***/ 
static uchar get_data_from_ROM( ) 
{ 
 uchar i; 
 uchar ret_data=0; 
 Rom_SCK=1; 
 for(i=0;i<8;i++) 
 { 
   Rom_OUT=1; 
   Rom_SCK=0; 
   ret_data=ret_data<<1; 
   if( Rom_OUT ) 
     ret_data=ret_data+1; 
   else 
     ret_data=ret_data+0; 
   Rom_SCK=1; 
 } 
 return(ret_data); 
} 
 
 
 
/*����ص�ַ��addrHigh����ַ���ֽ�,addrMid����ַ���ֽ�,addrLow����ַ���ֽڣ����������� DataLen ���ֽڵ����ݵ� pBuff �ĵ�
ַ*/ 
/*������ȡ*/ 
void get_n_bytes_data_from_ROM(uchar addrHigh,uchar addrMid,uchar addrLow,uchar *pBuff,uchar DataLen ) 
{ 
 uchar i; 
 Rom_CS = 0; 
 lcd_cs1=1;   
 Rom_SCK=0; 
 send_command_to_ROM(0x03); 
 send_command_to_ROM(addrHigh); 
 send_command_to_ROM(addrMid); 
 send_command_to_ROM(addrLow); 
 for(i = 0; i < DataLen; i++ ) 
   *(pBuff+i) =get_data_from_ROM(); 
 Rom_CS = 1; 
} 
 
 
/******************************************************************/ 
 
ulong  fontaddr=0; 
void display_GB2312_string(uchar y,uchar x,uchar *text) 
{ 
 uchar i= 0; 
 uchar addrHigh,addrMid,addrLow ; 
 uchar fontbuf[32];      
 while((text[i]>0x00))
 { 
   if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1)) 
   {             
     /*������壨GB2312�������ھ���Ѷ�ֿ� IC �еĵ�ַ�����¹�ʽ�����㣺*/ 
     /*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/ 
     /*���ڵ��� 8 λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/ 
     fontaddr = (text[i]- 0xb0)*94;  
     fontaddr += (text[i+1]-0xa1)+846; 
     fontaddr = (ulong)(fontaddr*32); 
      
     addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ� 8 λ,�� 24 λ*/ 
     addrMid = (fontaddr&0xff00)>>8;      /*��ַ���� 8 λ,�� 24 λ*/ 
     addrLow = fontaddr&0xff;       /*��ַ�ĵ� 8 λ,�� 24 λ*/ 
     get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ 32 ���ֽڵ����ݣ��浽"fontbuf[32]"*/ 
     display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ� LCD �ϣ�yΪҳ��ַ��x Ϊ�е�ַ��fontbuf[]Ϊ����*/ 
     i+=2; 
     x+=16; 
   } 
   else if((text[i]>=0x20) &&(text[i]<=0x7e))   
   {             
     unsigned char fontbuf[16];       
     fontaddr = (text[i]- 0x20); 
     fontaddr = (unsigned long)(fontaddr*16); 
     fontaddr = (unsigned long)(fontaddr+0x3cf80);       
     addrHigh = (fontaddr&0xff0000)>>16; 
     addrMid = (fontaddr&0xff00)>>8; 
     addrLow = fontaddr&0xff; 
 
     get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,16 );/*ȡ 16 ���ֽڵ����ݣ��浽"fontbuf[32]"*/ 
      
     display_graphic_8x16(y,x,fontbuf);/*��ʾ 8x16 �� ASCII�ֵ� LCD �ϣ�y Ϊҳ��ַ��x Ϊ�е�ַ��fontbuf[]Ϊ����*/ 
     i+=1; 
     x+=8; 
   } 
   else 
     i++;  
 } 
  
} 
 
void display_string_5x7(uchar y,uchar x,uchar *text) 
{ 
 unsigned char i= 0; 
 unsigned char addrHigh,addrMid,addrLow ; 
 while((text[i]>0x00)) 
 { 
   if((text[i]>=0x20) &&(text[i]<=0x7e))  
   {             
     unsigned char fontbuf[8];       
     fontaddr = (text[i]- 0x20); 
     fontaddr = (unsigned long)(fontaddr*8); 
     fontaddr = (unsigned long)(fontaddr+0x3bfc0);       
     addrHigh = (fontaddr&0xff0000)>>16; 
     addrMid = (fontaddr&0xff00)>>8; 
     addrLow = fontaddr&0xff; 
 
     get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,8);/*ȡ 8 ���ֽڵ����ݣ��浽"fontbuf[32]"*/ 
      
     display_graphic_5x7(y,x,fontbuf);/*��ʾ 5x7 �� ASCII �ֵ�LCD �ϣ�y Ϊҳ��ַ��x Ϊ�е�ַ��fontbuf[]Ϊ����*/ 
     i+=1; 
     x+=6; 
   } 
   else 
     i++;  
 } 
}

//eof

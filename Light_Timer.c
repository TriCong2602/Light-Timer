#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdint.h>
#include <wiringPiI2C.h>
#include <time.h>

#define LED 0

int ds3231;
unsigned char data[2];
uint8_t h,m,s;
uint8_t hex2dec(uint8_t h){
  uint8_t d;
    d = (h>>4)*10 + (h&0x0F);
   
   return d;

}
void send_data(unsigned char address, unsigned char value){
data[0] = address;
data[1] = value;
wiringPiSPIDataRW(0, data, 2);
}

void Init(void){
send_data(0x09, 0xFF);
send_data(0x0A, 0x09);
send_data(0x0B, 0x07);
send_data(0x0C, 1);
send_data(0x0F, 0);
}

void Read_Time(void){

    h = hex2dec(wiringPiI2CReadReg8(ds3231, 0x02));
    m = hex2dec(wiringPiI2CReadReg8(ds3231, 0x01));
    s = hex2dec(wiringPiI2CReadReg8(ds3231, 0x00));
    uint8_t max[8] = { h / 10, h % 10, 0x0A, m / 10, m % 10, 0x0A, s / 10, s % 10 };

    
    if (wiringPiI2CReadReg8(ds3231, 0x0F)==0x89)
        {digitalWrite(LED, HIGH);}
    else if (wiringPiI2CReadReg8(ds3231, 0x0F)==0x8A)
        {digitalWrite(LED, LOW);}

    for (int i=0; i<8; i++){
         send_data(i+1, max[7-i]);
}
}

int main(void){
wiringPiSetup();
pinMode(LED, OUTPUT);
wiringPiSPISetup(0, 8000000);
ds3231 = wiringPiI2CSetup(0x68);   //0x68 là địa chỉ của con ds3231
wiringPiI2CWriteReg8(ds3231, 0x07, 0x00);
wiringPiI2CWriteReg8(ds3231, 0x08, 0x08);
wiringPiI2CWriteReg8(ds3231, 0x09, 0x23);
wiringPiI2CWriteReg8(ds3231, 0x0A, 0x80);
wiringPiI2CWriteReg8(ds3231, 0x0B, 0x42);
wiringPiI2CWriteReg8(ds3231, 0x0C, 0x22);
wiringPiI2CWriteReg8(ds3231, 0x0D, 0x80);
// set time
wiringPiI2CWriteReg8(ds3231, 0x0E, 0x1F);
wiringPiI2CWriteReg8(ds3231, 0x0F, 0x88);

Init();
while (1){
    Read_Time();
   }   
return 0;

}

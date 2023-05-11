#include <stdio.h>
#include <stdint.h>
#include <wiringPiI2C.h>
#include <time.h>


int ds3231;
uint8_t dec2hex(uint8_t d){
  uint8_t h,ch,dv;
    ch = d/10;
    dv = d%10;
    h = (ch<<4) | dv;
   
   return h;

}

int main(void){
    while (1){
    //setup  I2C interface
    ds3231 = wiringPiI2CSetup(0x68);   //0x68 là địa chỉ của con ds3231
    //setup gpio cho module ds3231
    //read local time -> tm_sec, tm_min, tm_hour
    time_t rawtime;
    struct tm *tm;

    time (&rawtime);
    tm = localtime (&rawtime);
    
    // tm_sec = 46; tm_min = 35; tm_hour = 13
    wiringPiI2CWriteReg8(ds3231, 0x00, dec2hex(tm->tm_sec));   //Reg8 (gửi 8bit/lần); 0x00 (địa chỉ thanh ghi của ds3231; 
    wiringPiI2CWriteReg8(ds3231, 0x01, dec2hex(tm->tm_min));
    wiringPiI2CWriteReg8(ds3231, 0x02, dec2hex(tm->tm_hour));
   
  return 0;}
}



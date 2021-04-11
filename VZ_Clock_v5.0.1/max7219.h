#define DIN_PIN   13                                                                    //GPIO 13 / D7
#define CS_PIN    15                                                                    //GPIO 15 / D8
#define CLK_PIN   14                                                                    //GPIO 14 / D5
// MAX7219 commands:
#define CMD_NOOP   0
#define CMD_DIGIT0 1
#define CMD_DIGIT1 2
#define CMD_DIGIT2 3
#define CMD_DIGIT3 4
#define CMD_DIGIT4 5
#define CMD_DIGIT5 6
#define CMD_DIGIT6 7
#define CMD_DIGIT7 8
#define CMD_DECODEMODE  9
#define CMD_INTENSITY   10
#define CMD_SCANLIMIT   11
#define CMD_SHUTDOWN    12
#define CMD_DISPLAYTEST 15
byte scr[144];            // Максимально можно использовать 16 секций 8х8 (NUM_MAX *8 +8)
//======================================================================================================
void sendCmdAll(byte cmd,byte data){
  digitalWrite(CS_PIN,LOW);
  for(int i=leds.NUM_MAX0+leds.NUM_MAX1-1;i>=0;i--){
    shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,cmd);
    shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,data);
  }
  digitalWrite(CS_PIN,HIGH);
}
//======================================================================================================
void refreshAll(){ //90 90
  byte mask0=(leds.rotate0==270?0x01:0x80);
  byte mask1=(leds.rotate1==270?0x01:0x80);
  //for(int c=7;c>=0;c--){
  for(int c=0;c<8;c++){
    digitalWrite(CS_PIN,LOW);

    for(int i=(leds.rotate1==180?leds.NUM_MAX0:leds.NUM_MAX0+leds.NUM_MAX1-1);(leds.rotate1==180?(i<leds.NUM_MAX0+leds.NUM_MAX1):(i>=leds.NUM_MAX0));(leds.rotate1==180?i++:i--)){
      byte bt=0;
      if(leds.rotate1==270){
        for(int b=0;b<8;b++){
          bt<<=1;
          if(scr[i*8+b]&mask1)bt|=0x01;
        }
        shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,CMD_DIGIT0+c);
        shiftOut(DIN_PIN,CLK_PIN,(leds.mirror1?LSBFIRST:MSBFIRST),bt);
      }else if(leds.rotate1==90){
        for(int b=0;b<8;b++){
          bt>>=1;
          if(scr[i*8+b]&mask1)bt|=0x80;
        }
        shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,CMD_DIGIT0+c);
        shiftOut(DIN_PIN,CLK_PIN,(leds.mirror1?LSBFIRST:MSBFIRST),bt);
      }else {
        shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,CMD_DIGIT0+c);
        shiftOut(DIN_PIN,CLK_PIN,(leds.mirror1?LSBFIRST:MSBFIRST),scr[i*8+c]);
      }
    }
    if(leds.rotate1==270)mask1<<=1;
    if(leds.rotate1==90)mask1>>=1;

    for(int i=(leds.rotate0==180?0:leds.NUM_MAX0-1);(leds.rotate0==180?(i<leds.NUM_MAX0):(i>=0));(leds.rotate0==180?i++:i--)){  
      byte bt=0;
      if(leds.rotate0==270){
        for(int b=0;b<8;b++){
          bt<<=1;
          if(scr[i*8+b]&mask0)bt|=0x01;
        }
        shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,CMD_DIGIT0+c);
        shiftOut(DIN_PIN,CLK_PIN,(leds.mirror0?LSBFIRST:MSBFIRST),bt);
      }else if(leds.rotate0==90){
        for(int b=0;b<8;b++){
          bt>>=1;
          if(scr[i*8+b]&mask0)bt|=0x80;
        }
        shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,CMD_DIGIT0+c);
        shiftOut(DIN_PIN,CLK_PIN,(leds.mirror0?LSBFIRST:MSBFIRST),bt);
      }else {
        shiftOut(DIN_PIN,CLK_PIN,MSBFIRST,CMD_DIGIT0+c);
        shiftOut(DIN_PIN,CLK_PIN,(leds.mirror0?LSBFIRST:MSBFIRST),scr[i*8+c]);
      }
    }
    if(leds.rotate0==270)mask0<<=1;
    if(leds.rotate0==90)mask0>>=1;
    
    digitalWrite(CS_PIN,HIGH);
  }
}
//======================================================================================================
void clr(byte zone){
  for(int i=(zone?leds.NUM_MAX0*8:0);i<(zone?leds.NUM_MAX1+leds.NUM_MAX0:leds.NUM_MAX0)*8;i++)scr[i]=0;
}
//======================================================================================================
void clrAll(){
  for(int i=0;i<(leds.NUM_MAX1+leds.NUM_MAX0)*8;i++)scr[i]=0;
}
//======================================================================================================
void scrollLeft(byte zone){
  for(int i=(zone?leds.NUM_MAX0*8:0);i<(zone?leds.NUM_MAX0+leds.NUM_MAX1:leds.NUM_MAX0)*8+7;i++)scr[i]=scr[i+1];
}
//======================================================================================================
void scrollDown(byte zone){
  for(int i=(zone?leds.NUM_MAX0*8:0);i<(zone?leds.NUM_MAX0+leds.NUM_MAX1:leds.NUM_MAX0)*8+7;i++)scr[i]=scr[i]<<1;
}
//======================================================================================================
void invert(byte zone){
  for(int i=(zone?leds.NUM_MAX0*8:0);i<(zone?leds.NUM_MAX0+leds.NUM_MAX1:leds.NUM_MAX0)*8;i++)scr[i]=~scr[i];
}
//======================================================================================================
void initMAX7219(){
  pinMode(DIN_PIN,OUTPUT);
  pinMode(CLK_PIN,OUTPUT);
  pinMode(CS_PIN,OUTPUT);
  digitalWrite(CS_PIN,HIGH);
  delay(500);
  sendCmdAll(CMD_DISPLAYTEST,0);
  sendCmdAll(CMD_SCANLIMIT,7);
  sendCmdAll(CMD_DECODEMODE,0);
  sendCmdAll(CMD_INTENSITY,0);
  sendCmdAll(CMD_SHUTDOWN,0);
  delay(500);
  sendCmdAll(CMD_DISPLAYTEST,0);
  sendCmdAll(CMD_SCANLIMIT,7);
  sendCmdAll(CMD_DECODEMODE,0);
  sendCmdAll(CMD_INTENSITY,0);
  sendCmdAll(CMD_SHUTDOWN,0);
  clrAll();
  refreshAll();
}

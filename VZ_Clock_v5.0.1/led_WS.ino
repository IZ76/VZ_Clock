uint8_t saturationVal=255; // насыщенность
uint8_t ihuya = 85;
uint8_t idex;
static uint8_t hue=0;
uint64_t count_led_WS;
uint16_t delay_WS=5;

bool on_WS=true;
uint8_t demo_WS = 1;
uint64_t count_demo_WS;

/*
void updateColor(uint8_t r,uint8_t g,uint8_t b){
  for(uint8_t i = 0 ; i < LED_COUNT_WS; i++ ){
    leds_WS[i].setRGB(r,g,b);
  }
}
void select_effect_led(){
  if(!on_WS)return;
  if(count_led_WS>millis()) return;
  count_led_WS=millis()+delay_WS;
  if(sec_demo_WS!=0){
    if(count_demo_WS<millis()){
      count_demo_WS=millis()+(sec_demo_WS*1000);
      mode_WS++;
      if(mode_WS>9)mode_WS=1;
    }
  }
  switch(mode_WS){
    case 1: rainbow();           delay_WS=50; break;
    case 2: fadeToCenter();      delay_WS=5;  break;
    case 3: runnerChameleon();   delay_WS=50; break;
    case 4: rainbow_fade();      delay_WS=250;break;
    case 5: policeBlinker1();    delay_WS=5;  break;
    case 6: policeBlinker2();    delay_WS=5;  break;
    case 7: random_burst();      delay_WS=5;  break;
    case 8: random_burst2();     delay_WS=50; break;
    case 9: flicker();           delay_WS=50; break;
  }

}
// плавная вращающаяся радуга
void rainbow(){
  ihuya-=1;
  fill_rainbow(leds_WS,LED_COUNT_WS,ihuya);
  LEDS.show();
  //delay(delayValue_WS);
}
//расплывающие цвета
void fadeToCenter(){
 static uint8_t hue;
  for(uint8_t i=0;i<LED_COUNT_WS/2;i++){   
    leds_WS.fadeToBlackBy(40);
    leds_WS[i]=CHSV(hue++,255,255);
    leds_WS(LED_COUNT_WS/2,LED_COUNT_WS-1)=leds_WS(LED_COUNT_WS/2-1,0);
    //FastLED.delay(33);
  }
  LEDS.show();  
}
//бегающий по кругу хамелион c хвостом
void runnerChameleon(){
  if(ihuya>=LED_COUNT_WS-1)ihuya=0;
  else ihuya++;
  uint8_t a=ihuya==0?LED_COUNT_WS-1:ihuya-1;
  uint8_t b=a==0?LED_COUNT_WS-1:a-1;
  leds_WS[ihuya]=CHSV(hue++,255,255);
  leds_WS[a]=CHSV(hue++,255,150);
  leds_WS[b]=CHSV(hue++,255,80);
  FastLED.show(); 
  //leds_WS[ihuya]=CRGB::Black;
  updateColor(0,0,0);
}
// полицейская мигалка 
void policeBlinker1(){
  if(ihuya>=20)ihuya=0;
  else ihuya++;
  if(ihuya%2==0){
    if(ihuya<10){
      updateColor(0,0,0);
      for(uint8_t x=1;x<LED_COUNT_WS/2;x++ ){
        leds_WS[x]=CHSV(0,255,255);
      }
    }else{
      for(uint8_t x=LED_COUNT_WS-1;x>LED_COUNT_WS/2;x--){
        leds_WS[x]=CHSV(170,255,255);
      }
    }
  }else updateColor(0,0,0);
  FastLED.show(); 
}
void policeBlinker2(){
  if(ihuya>=20)ihuya=0;
  else ihuya++;
  if(ihuya%2==0){
    if(ihuya<10){
      updateColor(0,0,0);
      for(uint8_t x=5;x<16;x++ ){
        leds_WS[x]=CHSV(0,255,255);
      }
    }else{
      for(uint8_t x=0;x<20;x++ ){
        if(!(x>4&&x<14))leds_WS[19-x]=CHSV(170,255,255);
      }
    }
  }else updateColor(0,0,0);
  LEDS.show(); 
}
//плавная смена цветов
void rainbow_fade(){ 
  ihuya++;
  //  if(ihuya > 255){
  //  ihuya = 0;
  //}
  for(int i=0;i<LED_COUNT_WS;i++){
    leds_WS[i]=CHSV(ihuya,255,255);
  }
  LEDS.show();
}
// мозаика
void random_burst(){
  idex=random(0,LED_COUNT_WS);
  ihuya=random(0,255);
  leds_WS[idex]=CHSV(ihuya,saturationVal,255);
  LEDS.show();
}
// случайная смена цвета одного пикселя
void random_burst2(){
  idex=random(0,LED_COUNT_WS);
  ihuya=random(0,255);
  updateColor(0,0,0);
  leds_WS[idex]=CHSV(ihuya,saturationVal,255);
  LEDS.show();
}
// стробоскоп
void flicker(){
  ihuya++;
  int random_bright=random(0,255);
  if(ihuya%2==0){
    for(int i=0;i<LED_COUNT_WS;i++){
      leds_WS[i]=CHSV(160,50,random_bright);
    }
    count_led_WS=millis()+5;
  }else{
    updateColor(0,0,0);
    count_led_WS=millis()+random(1,300);
  }
  LEDS.show();
}
*/

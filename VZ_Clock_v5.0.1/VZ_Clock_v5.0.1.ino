/*__________________By IvanZah__________________________
                 Піни LED------NodeMCU  1.0            |
  DataIn____________________________________D7/GPIO 13 |
  LOAD/CS___________________________________D8/GPIO 15 |
  CLK_______________________________________D5/GPIO 14 |
  Кнопка____________________________________D0/GPIO 16 |
  Бaзер_____________________________________D6/GPIO 12 |
  DS18B20___________________________________D3/GPIO 0  |
  Si7021/BMP/BME280/DS3231/SGP30/AHTX0 SDA__D2/GPIO 4  |
  Si7021/BMP/BME280/DS3231/SGP30/AHTX0 SCL__D1/GPIO 5  |
  GND - Фоторезистор _______________________A0         |
  +5 (+3,3) - Резистор 10-100k _____________A0         |
  IR _______________________________________D4/GPIO 2  |
  ____________________________________________________*/
#define ver "5.0.1"

#include <Wire.h>
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <FS.h>
const char* fsName="SPIFFS";
FS* fileSystem=&SPIFFS;
File uploadFile;
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
//#include "Adafruit_HTU21DF.h"
#include "Adafruit_Si7021.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BMP085.h>
#include "BlueDot_BME280.h"
//#include <SimpleDHT.h>
#include <Adafruit_AHTX0.h>
#include <BH1750.h>
#include <MAX44009.h>
#include <Ticker.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
Ticker timeLocal;
Ticker animClock;
ESP8266HTTPUpdateServer httpUpdater;
WiFiClient ESPclient;
PubSubClient MQTTclient(ESPclient);
ESP8266WebServer server(80);
File fsUploadFile;
IPAddress apIP(192,168,4,1);
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include "fonts.h"
//#include <FastLED.h>
// ===================================================wifi
String jsonJssid="{}";
String jsonSsid="{}";
String jsonWifi="{}";
bool WIFI_connected=false;
struct{
  char ssidAP[64]="ESP-Info";
  char passAP[64]="11112222";
  char auth[64]="1234asdf5678";
  bool authOn=true;
  bool hiddenNetworks=false;
} indexs;
struct{
  uint8_t num=1;
  String ssid[10]={"IvanZah"};
  String pass[10]={"11223344"};
} ssids;
char ssid[64];
char password[64];
bool firstStart=0;
bool apStart=0;
byte amountNotStarts=0;
//-----------------------------------time
String jsonTimes={};
String jsonStime="{}";
IPAddress timeServerIP;
bool stopAlarm=false;
struct{
  String ntpServerName="ntp3.time.in.ua";
  float timeZone=2.0;    
  bool isDayLightSaving=true;
  bool rtcStat=true;
  byte alarme[5][3]{{12,30,0},{7,15,0},{22,55,0},{0,30,0},{0,0,0}}; //1-часы, 2-минуты, 3-откл(0)/1раз(11)/пон-пят(8)/пон-сб(9)/сб-вс(10)/вс(1)/пон(2)/вто(3)/сре(4)/чет(5)/пят(6)/сб(7)/всегда(12)
  byte memory_date[9][2]{{1,1},{2,1},{6,1},{7,1},{13,1},{19,1},{8,3},{1,5},{1,9}}; //1-день, 2-месяц
  char memory_date_mes[9][51]={"Новый Год!!!","ПослеНовыйГод(((","Ночь перед Рождеством","Рождество","Старый Новый Год!","Крещение","День цветов","День шашлыка)))","Отведи ребенка в школу"};
  byte memory_hour_start=9;
  byte memory_hour_end=23;
  byte nc_hour[5]{22,24,24,24,24};
  byte nc_min[5]{55,60,60,60,60};
  char nc_mess[5][51]={"IvanZah"};
} times;
const int NTP_PACKET_SIZE=48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP udp;
unsigned int localPort=2390;
unsigned long epochNM;
static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};        // Кількість днів у місяцях
#define LEAP_YEAR(Y) (((1970+Y)>0)&&!((1970+Y)%4)&&(((1970+Y)%100)||!((1970+Y)%400)))   // Високосні літа
float hourCorr;
long localEpoc=0;
long localMillisAtUpdate=0;
int hour=0,minute=30,second=45,month=1,day=21,dayOfWeek=6,year=2021;
int g_hour,g_minute,g_second,g_month=1,g_day,g_dayOfWeek,g_year;
bool statusUpdateNtpTime=0;                     // якщо не "0" - то останнє оновленя часу було вдалим
String y,mon,wd,d,h,m,s,mes;
uint8_t hourTest[3],minuteTest[3];
int lastSecond,lastMinute;
String date;
String tNow,tYear,tCurr,tPress,tPress0,tSpeed,tMin,tTom,tYour,tPoint,tIp,tPass,tWeatrNot,tWeatrTN;
byte reminder_work=false;
byte mdate_work=false;
bool stop_line_mdate_work=false;
bool alarm_work=false;
byte alarm_numer=255;
int rtcAddr=0x68;
int hour_rtc, minute_rtc, second_rtc, month_rtc, day_rtc, dayOfWeek_rtc, year_rtc;
byte errorRTC;
int secFr;
//----------------------------------------weath
String jsonWeaths{};
String weatherHost0="api.weatherbit.io";
String weatherHost1="api.openweathermap.org";
String location_name;
String location_region;
String location_country;
String location_localtime;
String location_sunrise;
String location_sunset;
float location_temp;
float location_app_temp;
int location_rh;
float location_pres;
float location_wind_spd;
String location_wind_cdir_full;
int location_clouds;
int location_vis;
int location_uv;
String location_wds;
String weatherString;
String weatherStringZ;
bool updateForecastNot=true;
int updateForecast=0;
int updateForecasttomorrow=0;
String wind[]={"","","","","","","",""};
struct{
  bool weathOn=false;
  byte weatherHost=true;
  String weatherKey0;
  String weatherKey1;
  String cityID0="Kryvyy Rih";
  String cityID1="703845";
  char personalCityName[51];
  bool displayCityName=true;
  bool displayForecastNow=true;
  bool displayForecastToday=true;
  bool displayForecastTomorrow=true;
  int displayForecastWind=1;                  // 0 - Стрелками, 1 - словами
  bool animNotWeather=true;
  byte timeStartViewWeather=6;
  byte timeEndViewWeather=23;
} weaths;
String weatherMain="";
String weatherDescription="";
String weatherLocation="";
String country;
String tempz;
int clouds;
int windDeg;
float windSpeed;
String dw, _month;
int dx=0;
int dy=0;
byte del=0;
//--------------------------------------------led
String jsonLeds={};
  bool timeD=true;
struct{
  byte NUM_VER_X=0;
  byte NUM_MAX0=4;
  byte NUM_MAX1=4;       // x2
  int rotate0=90;
  int rotate1=90;        // x2
  bool mirror0=false;
  bool mirror1=false;
  byte timeDay=5;
  byte timeNight=24;
  byte volBrightsMax=10;
  byte volBrightsMin=2;
  int volBHMax=50;
  int volBHMin=49;
  int vol44Max=50;
  int vol44Min=49;
  byte typeSwitchBrightness=0;
  byte fontCLOCK=1;      // 0-крупный, 1-крупный цифровой, 2-полу жирный, 3-полу жирный цифровой, 4-обычный, 5-обычный цифровой, 6-узкий, 7-узкий цифровой.
  byte fontSizeData=1;   // x2
  byte textDown=0;       // x2
  byte dataDown=1;       // x2
  int displayData=1;                      // 0 - Не отображать, 1 - отображать статику, 2 - отображать в бегущей строке
  byte clockAndSec=1;
  byte maxModeSec=3;
  byte animPoint=4;
  byte secLine=1;        // x2
  byte timeScrollSpeed=40;
  byte function[12]={0,1,3,4,5,6,7,8,9,10,0,2}; //0-время, 1-дата, 2-погода, 3-D1...10-D8, 11-RSS
  byte period[12]={50,5,2,2,2,2,2,2,2,2,50,5};
  byte view[12]={0,1,1,1,1,1,1,1,1,1,1,1}; // 0-всегда, 1-днем, 2-ночью
  byte dataAlig=2;
  byte clockAlig=1;
} leds;
byte clockAndSecMem=1;
byte fnCount=0;
bool runningLine=0;
bool endString=true;
bool bigClock=true;
String space="";
#define MAX_DIGITS 16
byte dig[MAX_DIGITS]={0};
byte digold[MAX_DIGITS]={0};
byte digtrans[MAX_DIGITS]={0};
byte levelBridhtness=0;
int lowLivelBrightness=0;
int upLivelBrightness=1023;
uint8_t shift;
uint64_t timerRandomMax;
String rss_mess;
uint64_t rss_count;
#include "max7219.h"
//--------------------------------------------------sens
String jsonSenss={};
#define buzzerPin 12  //GPIO 12 / D6
#define brightPin A0
#define BUT_PIN   16
byte butMode=0; // 0 - не нажата, 1 - нажата один раз, 2 - нажата два раза, 3 - 5 секунд нажата, 4 - 30 секунд нажата.
byte butFlag=0; // 1 - кнопка нажата, 0 - не нажата
int butCount=0; // счетчик времени нажатия кнопки
int butMillis=0;
struct{
  bool mqttOn=false;
  bool thingOn=false;
  bool nmonOn=false;
  byte params[8]={0,10};      //  0-темп.дом(tD), 1-темп.улица(tU), 2-темп.Н(tН), 3-(tT), 4-(tL) 
                                     // 10-влажность1(hD), 11-(h1), 12-(h2), 13-(h3), 14-(h4)
                                     // 20-давление(P), 21-целое число(С) 22-батарейка(V), 23-батарейка(A)
  byte sensors[8]={10,10};     //NONE=0, DS18B20=1, Si7021=2, BMP280=3, BME280=4, DHT=5, AHTx0=10, MQTT1-8=11-18, THING1-8=21-28, NMon1-8=31-38
  float corr[8];
  bool sgpOn=true;
  byte buzzerSet=1; // 0- не использовать, 1 - активный базер, 2- пссивный
  String messLang="uk";
  byte kuOn=7;
  byte kuOff=23;
  byte butStat=1;
  byte pressSys=1;
  bool printCom=true;
  bool updateOTA=false;
  bool beepSave=true;
} senss;
float data[8];
String tJanuary,tFebruary,tMarch,tApril,tMay,tJune,tJuly,tAugust,tSeptember,tOctober,tNovember,tDecember;
String tMonday,tTuesday,tWednesday,tThursday,tFriday,tSaturday,tSunday,tAlarm;
#include "rtc.h"
//-------------------------------------------------mqtt
String jsonMqtts={};
float tMqtt[8];
unsigned long counterMqtt[8];
boolean counterMqttTimeout[8]={true,true,true,true,true,true,true,true};
struct{
  char mqtt_server[21]="m13.cloudmqtt.com";
  int  mqtt_port=13011;
  char mqtt_user[25]="";
  char mqtt_pass[25]="";
  char mqtt_name[25]="ESP-Informer";
  char mqtt_sub_inform[25]="Inform/mess";
  char mqtt_sub[8][25]={"Ulica/temp","Ulica/hum","Ulica/press"};
  char mqtt_pub_sensors[8][25]={"Informer/temp","Informer/tempUl","Informer/tempH","Informer/hum","Informer/press","Informer/alt","Informer/eco2","Informer/tvoc"};
  int timeOutMqtt=1200000; // таймаут обновления данных - 20 минут
} mqtts;
//----------------------------------------------------thing
String jsonThings{};
String nameWriteChannel;
String nameField[8];
String nameReadChannel;
String nameReadField[8];
float readDataField[8];
struct{
  String channelIdWrite;
  String writeApiKey;
  String wrApiKey;
  byte writeFild[8]; //0-нет, 1-tD, 2-tU, 3-tH, 4-hD, 5-P, 6-sgpCo2, 7-sgpTvoc
  String channelIdRead;
  String readApiKey;
  byte statThing[8];// храним номер филда откуда приходят данные
} things;
//-------------------------------------------------------nmon
String jsonNmons{};
String macNmon;
float nMon[8];
struct{
  String uuid;
  String api_key;
  int sensors_ID[8]={0,3300,0};
  String latNmon;
  String lonNmon;
  byte periodNmon=5;
  byte writeNmon[8];
  String nameNmon="_Info";
} nmons;
//-------------------------------------------------------sgp
String jsonSgps={};
#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;
boolean sgpFound=false;
int sgpCo2=400;
byte sgpCo2Livel=0; //0=400-699, 1=700-999, 2=1000-2499, 3=2500-4999, 4=5000++
int sgpCo2Livels[4]={700,1000,2500.5000};
String sgpCo2Message[]={"","","","",""};
int sgpTvoc=0;
byte sgpTvocLivel=0; //0=0-64, 1=65-219, 2=220-659, 3=660-2199, 4=2200++
int sgpTvocLivels[4]={65,220,660,2200};
String sgpTvocMessage[]={"","","","",""};
struct{
  byte sgpCo2LivelAlarm=2;
  bool eCo2AlarmEsp=true;
  bool eCo2AlarmMqtt=false;
  bool eCo2Led=true;
  byte sgpTvocLivelAlarm=2;
  bool tvocAlarmEsp=true;
  bool tvocAlarmMqtt=false;
  bool tvocLed=true;
  byte setSgpCorr=0;  //0-нет коррекции, 1-BME-280, 2-Si7021, 3-AHTx0, 4-DHT11/22
  float sgpCorrTemp=22.1; // [°C]
  float sgpCorrHumi=45.2; // [%RH]
} sgps;
//--------------------------------------------------------ir
const uint16_t kRecvPin=2;
IRrecv irrecv(kRecvPin);
decode_results results;
String jsonIrs="{}";
uint32_t ir_key[6]={33431775,33444015,33464415,33448095,33464410,33448090};
uint32_t ir_code;
byte ir_save=0;
//--------------------------------------------------------led
#define LED_COUNT_WS 20 // число пикселей в ленте
#define LED_DT_WS 9    // пин, куда подключен DIN ленты (номера пинов ESP8266 совпадает с Arduino)  
//CRGBArray<LED_COUNT_WS> leds_WS;
uint8_t bright_WS = 200; // яркость (0 - 255)
uint8_t sec_demo_WS=10; //0-эффект работает постоянно
uint8_t mode_WS=0;
//--------------------------------------------------------------
String pathSw[12]={"","/wifis.json","/ssids.json","/times.json","/weaths.json","/leds.json","/senss.json","/mqtts.json","/things.json","/nmons.json","/sgps.json","/irs.json"};
// ----------------------------------------------------- Сенсоры
//SimpleDHT11 dht11;
//SimpleDHT22 dht22;
//int pinDHT=2;
//bool dhtFound=false;
//float tempDht;
//float humiDht;
//
OneWire  ds(0);
bool ds18b20Found=false;
float tempDs18b20;
//
Adafruit_BMP085 bmp180;
bool bmp180Found=false;
Adafruit_BMP280 bmp;
bool bmp280Found=false;
float tempBmp;
float pressBmp;
float altBmp;
//
BlueDot_BME280 bme;
bool bme280Found=false;
float tempBme;
float humiBme;
float pressBme;
float altBme;
//
//Adafruit_HTU21DF sensor = Adafruit_HTU21DF();
Adafruit_Si7021 sensor = Adafruit_Si7021();
bool si7021Found=false;
float humiSi7021;
float tempSi7021;
//
Adafruit_AHTX0 aht;
bool AHTx0Found=false;
float humiAHTx0=0;
float tempAHTx0=0;
//
BH1750 lightMeter(0x23);//    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage) - 0x5C (if ADD pin had > 0.7VCC voltage)
bool BH1750Found=false;
int volBH1750;
//
bool MAX44009Found=false;
MAX44009 light;
float volMAX44009;
//
unsigned long fnTimer;
unsigned long fireTimer;
unsigned long nmonTimer=5000;
unsigned long nmonGetTimer=7000;
unsigned long ntpTimer=1800000;
unsigned long rtcTimer;
unsigned long weathTimer=10000;
unsigned long sgpTimer=20000;
unsigned long thingTimer=40000;
unsigned long mqttTimer=50000;
unsigned long wifiTimer=30000;
unsigned long recWifiTimer;
unsigned long sensTimer;
//======================================================================================
void setup(){
  Wire.begin();
  Serial.begin(115200);
  if(senss.printCom) Serial.println("");
  delay(500);
  //LEDS.setBrightness(bright_WS);
  //LEDS.addLeds<WS2812B, LED_DT_WS, GRB>(leds_WS, LED_COUNT_WS);  // настройки для вашей ленты (ленты на WS2811, WS2812, WS2812B)
  //updateColor(0,0,0);
  //FastLED.show();
  irrecv.enableIRIn();
  macNmon=WiFi.macAddress();
  macNmon.replace(":","");
  SPIFFS.begin();{
    Dir dir=SPIFFS.openDir("/");
    while(dir.next()){
      String fileName=dir.fileName();
      size_t fileSize=dir.fileSize();
    }
  }
  loadSsids();
  loadJssids();
  loadNet();
  loadTimes();
  //saveTimes();
  loadWeaths();
  loadLeds();
  loadSenss();
  loadMqtts();
  loadThings();
  loadNmons();
  loadSgps();
  loadIr();
  lang();
  pinMode(buzzerPin,OUTPUT);
  pinMode(BUT_PIN,INPUT);
  digitalWrite(BUT_PIN,!senss.butStat);
  bip();
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN,1);
  sendCmdAll(CMD_INTENSITY,1);
  for(byte i=0;i<leds.NUM_MAX0;i++) space+="   ";
  Wire.beginTransmission(0x67);
  errorRTC=Wire.endTransmission();
  if(errorRTC==0){
    rtcAddr=0x67;
    if(senss.printCom) Serial.println("YES!!! find RTC module addr: 0x67!");
  }else{
    Wire.beginTransmission(0x68);
    errorRTC=Wire.endTransmission();
    if(errorRTC==0){
      rtcAddr=0x68;
      if(senss.printCom) Serial.println("YES!!! find RTC module addr: 0x68!");
    } else{
      if(times.rtcStat) times.rtcStat=false;
      saveTimes();
    }
  }
  if(times.rtcStat){
    if(senss.printCom) Serial.println("RTC START");
    getRTCDateTime();
    hour=hour_rtc;
    minute=minute_rtc;
    second=second_rtc;
    day=day_rtc;
    month=month_rtc;
    year=year_rtc;
    dayOfWeek=dayOfWeek_rtc;
    if (senss.printCom){
      Serial.println("RTC update: "+String(hour)+":"+String(minute)+":"+String(second)+"    "+String(day)+"."+String(month)+"."+String(year)+" D="+String(dayOfWeek));
    }
  } else{
    if(senss.printCom) Serial.println("RTC module off!");
    loadStime();
  }
  // ----------
  timeLocal.attach(0.5,timeLocale);
  localMillisAtUpdate=millis();
  localEpoc=(hour*60*60+minute*60+second);
  udp.begin(localPort);
  animClock.attach(0.05,showAnimClock);
  // ---------- Підключення до WiFi
  wifiConnect();
  // ------------------
  setup_sensors();
  server_init();
    // ***********  OTA SETUP  
  if(senss.updateOTA){
    ArduinoOTA.setPort(8266);
    ArduinoOTA.setHostname("ESP-ZAL");
    ArduinoOTA.onEnd([](){ESP.restart();});
    ArduinoOTA.onError([](ota_error_t error){
      Serial.printf("Error[%u]: ",error);
      if(error==OTA_AUTH_ERROR&&senss.printCom) Serial.println("Auth Failed");
      else if(error==OTA_BEGIN_ERROR&&senss.printCom) Serial.println("Begin Failed");
      else if(error==OTA_CONNECT_ERROR&&senss.printCom) Serial.println("Connect Failed");
      else if(error==OTA_RECEIVE_ERROR&&senss.printCom) Serial.println("Receive Failed");
      else if(error==OTA_END_ERROR&&senss.printCom) Serial.println("End Failed");
      ESP.restart();
    });
    ArduinoOTA.begin();
  }
  if(WiFi.status()==WL_CONNECTED && weaths.weathOn) weather_update();
// ---------- MQTT client
  MQTTclient.setServer(mqtts.mqtt_server,mqtts.mqtt_port);
  MQTTclient.setCallback(callback);
  MQTTclient.connect(mqtts.mqtt_name);
  MQTTclient.subscribe(mqtts.mqtt_sub_inform);
  for(byte i=0;i<8;i++){
    if(strlen(mqtts.mqtt_sub[i])>0){
      MQTTclient.subscribe(mqtts.mqtt_sub[i]);
      Serial.println("mqtts.mqtt_sub["+String(i)+"]="+String(mqtts.mqtt_sub[i]));
    }
  }
  for(byte i=0;i<8;i++){
    if(senss.sensors[i]>30 && senss.sensors[i]<40){
      narodmonGet();
      break;
    }
  }
  if(!sgp.begin()){
    if(senss.printCom){
      Serial.println("Sensor SGP30 not found :(");
    }
    sgpFound=false;
  } else {
    if(senss.printCom){
      printTime();
      Serial.print("Found SGP30 serial #");
      Serial.print(sgp.serialnumber[0],HEX);
      Serial.print(sgp.serialnumber[1],HEX);
      Serial.println(sgp.serialnumber[2],HEX);
    }   
    sgpFound=true;
  }
  thingSpeak();
  nmonTimer=millis()+nmons.periodNmon*60000;
  fnTimer=millis()+leds.period[0]*1000;
}
//======================================================================================
void callback(char* topic,byte* payload,unsigned int length){ // получаем знаковое число с десятичной плавающей запятой
  if(!senss.mqttOn) return;
  String Text;
  for(int i=0;i<length;i++) Text+=((char)payload[i]);
  if((String(topic)==mqtts.mqtt_sub_inform)&&(sgpCo2Livel<sgps.sgpCo2LivelAlarm||sgpTvocLivel<sgps.sgpTvocLivelAlarm)){
    for(byte i=0;i<leds.NUM_MAX0;i++){
      Text+="   ";
    }
    bip();
    bip();
    bip();
    bip();
    if(senss.printCom){
      printTime();
      Serial.println("MQTT Incoming Message: "+Text);
    }
    clr(leds.NUM_VER_X);
    refreshAll();
    printStringWithShift(Text.c_str(),30);
    return;
  }
  for(byte i=0;i<8;i++){
    if(String(topic)==mqtts.mqtt_sub[i]){
      tMqtt[i]=Text.substring(0,length+1).toFloat();
      if(senss.printCom){
        printTime();
        Serial.println("MQTT "+String(i+1)+" Incoming: " + String(tMqtt[i]));
      }
      counterMqtt[i]=millis();
      counterMqttTimeout[i]=false;
      sensorses();
    }
  }
}
//======================================================================================
void reconnect(){
  if(senss.printCom){
    printTime();
    Serial.println("Start Reconnect void...");
  }
  if(WiFi.status()==WL_CONNECTED){  // && !ESPclient.connected()
    if(senss.printCom){
      printTime();
      Serial.print("MQTT reconnection...");
    }
    if(MQTTclient.connect(mqtts.mqtt_name, mqtts.mqtt_user, mqtts.mqtt_pass)){
      if(senss.printCom) Serial.println("connected");
      MQTTclient.subscribe(mqtts.mqtt_sub_inform);
      for(byte i=0;i<8;i++){
        if(strlen(mqtts.mqtt_sub[i])>0){
          MQTTclient.subscribe(mqtts.mqtt_sub[i]);
          Serial.println("mqtts.mqtt_sub["+String(i)+"]="+String(mqtts.mqtt_sub[i]));
          Serial.println("strlen(mqtts.mqtt_sub[i]="+String(strlen(mqtts.mqtt_sub[i])));
        }
      }
    } else{
      if(senss.printCom){
        Serial.print("failed, rc=");
        Serial.println(MQTTclient.state());
      }
    }
  }
}

//--------------------------------------------------------------------------------------
void timeLocale(){
  timeLocal.detach();
  long curEpoch=localEpoc+((millis()-localMillisAtUpdate)/1000);
  long epoch=curEpoch+86400L;
  epoch=round(epoch);
  epoch=(epoch%86400L);
  lastMinute=minute;
  lastSecond=second;
  hour=((epoch%86400L)/3600)%24;
  minute=(epoch%3600)/60;
  second=epoch%60;
  if(second!=lastSecond){                                                            // на початку нової секунди скидаємо secFr в "0"
    if(leds.typeSwitchBrightness==1){
      int br=analogRead(brightPin);
      if(lowLivelBrightness<=upLivelBrightness){
        if(br<lowLivelBrightness) lowLivelBrightness=br;
        if(br>upLivelBrightness) upLivelBrightness=br;
      } else {
        if(br<upLivelBrightness) upLivelBrightness=br;
        if(br>lowLivelBrightness) lowLivelBrightness=br;
      }
      levelBridhtness=map(br,lowLivelBrightness, upLivelBrightness, leds.volBrightsMin, leds.volBrightsMax);
      sendCmdAll(CMD_INTENSITY, levelBridhtness);
    } else if(leds.typeSwitchBrightness==2&&BH1750Found){
      volBH1750=(int)lightMeter.readLightLevel();
      if(leds.volBHMax<volBH1750) leds.volBHMax=volBH1750;
      if(leds.volBHMin>volBH1750) leds.volBHMin=volBH1750;
      levelBridhtness=map(volBH1750,leds.volBHMax,leds.volBHMin,leds.volBrightsMax,leds.volBrightsMin);
      sendCmdAll(CMD_INTENSITY, levelBridhtness);
    } else if(leds.typeSwitchBrightness==3&&MAX44009Found){
      volMAX44009=light.get_lux();
      if(leds.vol44Max<volMAX44009) leds.vol44Max=volMAX44009;
      if(leds.vol44Min>volMAX44009) leds.vol44Min=volMAX44009;
      levelBridhtness=map(volMAX44009,leds.vol44Max,leds.vol44Min,leds.volBrightsMax,leds.volBrightsMin);
      sendCmdAll(CMD_INTENSITY, levelBridhtness);
    } else{
      if(timeD) sendCmdAll(CMD_INTENSITY,leds.volBrightsMax);
      else sendCmdAll(CMD_INTENSITY, leds.volBrightsMin);
    }
    if(lastMinute!=minute){
      if(hour==0&&minute==0&&second==0){
        day++;
        if(day==32||(day==31&&(month==4||month==6||month==9||month==11))||(month==2&&((day==29&&year%4!=0)||(day==30&&year%4==0)))){
          day=1;
          month++;
          if(month>12){
            month=1;
            year++;
          }
        }
        dayOfWeek++;
        if(dayOfWeek>7) dayOfWeek=1;
      }
      if(times.alarme[alarm_numer][2]==11){
        times.alarme[alarm_numer][2]=0; // если будильник был разовый то выключаем его
        saveTimes();
      }
      stopAlarm=false;
      if(alarm_work){
        alarm_work=false;
        leds.clockAndSec=clockAndSecMem;
      }
      reminder_work=0;
      if(minute==0){
        for(byte i=0;i<9;i++){
          if(times.memory_date[i][0]==day&&times.memory_date[i][1]==month){
            mdate_work=i+1;
            if(!endString) stop_line_mdate_work=true;
            break;
          }
        }
      } 
    }
    if(!stopAlarm){
      for(byte i=1;i<6;i++){
        if(hour==times.nc_hour[i-1]&&minute==times.nc_min[i-1]){
          reminder_work=i;
          break;
        }
      }      
      alarm_work=alarms();
    }
  }
  timeD=true;
  if(((leds.timeDay<leds.timeNight)&&(hour<leds.timeDay||hour>=leds.timeNight))||((leds.timeDay>leds.timeNight)&&(hour<leds.timeDay&&hour>=leds.timeNight))) timeD=false;
  timeLocal.attach(0.15,timeLocale);
}
//======================================================================================
//======================================================================================
void loop(){

  buttonInter();
  if(senss.updateOTA) ArduinoOTA.handle();
  server.handleClient();
  if(second!=lastSecond){
    if(!stopAlarm&&reminder_work){
      bip();bip();bip();
      String mess=times.nc_mess[reminder_work-1];
      if(mess=="IvanZah")mess="  \200\200\200 Моей любимой Заюшке!!! \200\200\200"+space;
      if(leds.NUM_VER_X)clr(leds.NUM_VER_X);
      printStringWithShift(mess.c_str(),leds.timeScrollSpeed);
    }
    if(!stopAlarm&&alarm_work){
      if(!leds.NUM_VER_X) bigClock=true;
      bip();bip();
    }
    if(mdate_work&&hour>=times.memory_hour_start&&hour<=times.memory_hour_end&&second<15&&second>2){
      bip();bip();bip();
      clr(leds.NUM_VER_X);
      String mess="  "+String(mdate_work==1?times.memory_date_mes[0]:mdate_work==2?times.memory_date_mes[1]:mdate_work==3?times.memory_date_mes[2]:mdate_work==4?times.memory_date_mes[3]:mdate_work==5?times.memory_date_mes[4]:mdate_work==6?times.memory_date_mes[5]:mdate_work==7?times.memory_date_mes[6]:mdate_work==8?times.memory_date_mes[7]:times.memory_date_mes[8])+space;
      printStringWithShift(mess.c_str(),leds.timeScrollSpeed);
    } else mdate_work=0;
  }
  if((fnTimer<millis()||fireTimer<millis())&&endString&&(!stopAlarm&&!alarm_work&&!reminder_work&&!mdate_work)){
    fireTimer=millis()+30000;
    runService();
  }
  if((fnTimer<=millis())&&endString&&(!stopAlarm&&!alarm_work&&!reminder_work&&!mdate_work)){
    fnCount++;
    if(fnCount>=12) fnCount=0;
    fnTimer=millis()+(leds.period[fnCount]*1000);    
    printTime();
    Serial.print("fnCount=  "+String(fnCount)+"   ");
    Serial.print("period[fnCount]=  "+String(leds.period[fnCount])+"     ");
    Serial.print("millis()=  "+String(millis())+"     ");
    Serial.println("fnTimer=  "+String(fnTimer)+"     ");
  }
  if(butMode==0&&endString&&(!stopAlarm&&!alarm_work&&!reminder_work&&!mdate_work)){
    if(leds.period[fnCount]==0){
      fnTimer=millis();
    }else if((leds.view[fnCount]==0)||(leds.view[fnCount]==1&&timeD==1)||(leds.view[fnCount]==2&&timeD==0)){
      if(leds.function[fnCount]==0){//время
        bigClock=true;
      }else if(leds.function[fnCount]==1){//дата
        if(leds.displayData==1){
          clr(leds.NUM_VER_X);
          showSimpleDate();
        }else if(leds.displayData==2){
          clr(leds.NUM_VER_X);
          convertDw();
          convertMonth();
          date="  "+dw+", "+String(day)+" "+_month+" "+String(year)+" "+tYear+space;
          printStringWithShift(date.c_str(),leds.timeScrollSpeed);
          fnTimer=millis();
        } else fnTimer=millis();
      }else if(leds.function[fnCount]==2){//погода
        fnTimer=millis();
        printForecast();
      }else if(leds.function[fnCount]>2&&leds.function[fnCount]<11){//датчики
        if(senss.sensors[leds.function[fnCount]-3]){
          showSimple(leds.function[fnCount]-3);
        } else fnTimer=millis();
      }else if(leds.function[fnCount]==11){//rss
        
        if(rss_mess==""||rss_count<millis()){
          fnTimer=millis();
          return;
        }
        clr(leds.NUM_VER_X);
        printStringWithShift((rss_mess+space).c_str(),leds.timeScrollSpeed);
        fnTimer=millis();
      }
    } else fnTimer=millis();
  }
  // ---------- якщо мережа WiFi доступна то виконуємо наступні функції ----------------------------
  if(WIFI_connected&&(!stopAlarm&&!alarm_work&&!reminder_work&&!mdate_work)){
    if(senss.mqttOn) MQTTclient.loop();           // перевіряємо чи намає вхідних повідомлень, як є, то кoлбек функція
  }
  if((second!=lastSecond)&&butMode!=0){ // если отработали все функции работы с кнопкой, то состояние сбросится автоматом
    Serial.println("BUT MODE RESET");
    butMode=0;
  }
}
//======================================================================================
//======================================================================================
void runService(){
  // ----------Проверка доступности WiFi--------------------------------
  if(wifiTimer<millis()){
    wifiTimer=millis()+30000;
    if(!alarm_work){
      if(apStart&&millis()>600000) apStart=0; // через 10 минут после страта если нет подключения к Wifi - можно пробовать переподключиться
      if((WiFi.status()!=WL_CONNECTED||!WIFI_connected)&&!apStart){  // повторне підк. до WiFi кожну 1, 6, 11, 16...56 хв.
        WIFI_connected=false;
        if(recWifiTimer<millis()){
          recWifiTimer=millis()+300000;
          wifiConnect();
          if(WiFi.status()==WL_CONNECTED) WIFI_connected=true;
        }
      }
    }
  }
  if(WIFI_connected&&!alarm_work){ //работаем с функциями использующими интернет
    //-----------Получение погоды-------------------------------------------
    if(weathTimer<millis()&& weaths.weathOn){
      if(hour>=weaths.timeStartViewWeather&&hour<=weaths.timeEndViewWeather){
        if(!updateForecast||!updateForecasttomorrow) weather_update();
        else{
          if(updateForecast){
            if(!weaths.weatherHost) getWeatherData0();
            else getWeatherData1();
          }
          if(updateForecasttomorrow){
            if(!weaths.weatherHost) getWeatherDataz0();
            else getWeatherDataz1();
          }
        }
        if(!updateForecast||!updateForecasttomorrow) weathTimer=millis()+1800000;// если обновление удачное то следующее через 30 минут
        else weathTimer=millis()+300000;// если не удачное то следующее через 5 минут
        if(senss.mqttOn&&!MQTTclient.connected()) {
          reconnect();
          if(!MQTTclient.connected()) reconnect();
        }
      }
    }
    //-----------Прием данных из NMon---------------------------------------
    if(nmonGetTimer<millis()){
      nmonGetTimer=millis()+nmons.periodNmon*60000;
      for(byte i=0;i<8;i++){
        if(senss.sensors[i]>30 && senss.sensors[i]<40){
          narodmonGet();
          break;
        }
      }
    }
    //-----------Отправка данных на NMon------------------------------------  
    if(nmonTimer<millis()&&senss.nmonOn){
      nmonTimer=millis()+nmons.periodNmon*60000;
      narodmonSend();
    }
    //----------Работа с ThingSpeak-----------------------------------------
    if(thingTimer<millis()&&senss.thingOn){
      thingTimer=millis()+60000;
      thingSpeak();
      if(senss.mqttOn && !MQTTclient.connected()){
        reconnect();
        if(!MQTTclient.connected()) reconnect();
      }
    }
    // ----------Работа с MQTT ---------------------------------------------
    if(mqttTimer<millis()&&senss.mqttOn){
      mqttTimer=millis()+60000;
      if(!MQTTclient.connected()) reconnect();
      if(MQTTclient.connected()){
        for(byte i=0;i<8;i++){
          if(senss.sensors[i]&&String(mqtts.mqtt_pub_sensors[i]) != ""){
            MQTTclient.publish(mqtts.mqtt_pub_sensors[i], String(data[i]).c_str());
            if(senss.printCom){
              printTime();
              Serial.println("Publish sensors["+String(i)+"]: "+String(data[i]));
            }
          }
        }
      }
      // ---------- Обработка таймаутов MQTT -------------------------------
      if(mqtts.timeOutMqtt<millis()){
        for(byte i=0;i<8;i++){
          if(millis()-counterMqtt[i]>mqtts.timeOutMqtt) counterMqttTimeout[i]=true;
          else counterMqttTimeout[i]=false;
        }
      }
    }
    //-----------Получение времени от NTP сервера---------------------------
    if(ntpTimer<millis()&&!alarm_work){
      timeUpdateNTP();
      if(statusUpdateNtpTime==0) ntpTimer=millis()+300000; //при неудачном обновлении повтор через 5 минут
      else ntpTimer=millis()+3600000; // при удачном обновлении повтор через 60 минут
    }
  }
  //-----------Функции без интернет
  //-----------Обновление данных SGP--------------------------------------
  if(sgpTimer<millis()&&sgpFound&&senss.sgpOn){
    sgpTimer=millis()+60000;
    sgp30();
  }
  //-----------Обновление времени из RTC модуля---------------------------
  if(rtcTimer<millis()){
    rtcTimer=millis()+86400000;
    if(times.rtcStat){
      printTime();
      getRTCDateTime();
      hour=hour_rtc;
      minute=minute_rtc;
      second=second_rtc;
      day=day_rtc;
      month=month_rtc;
      year=year_rtc;
      dayOfWeek=dayOfWeek_rtc;
      if(senss.printCom){
        Serial.println("RTC update: "+String(hour)+":"+String(minute)+":"+String(second)+"    "+String(day)+"."+String(month)+"."+String(year)+" D="+String(dayOfWeek));
      }
    }
  }
  if(sensTimer<millis()){
    sensTimer=millis()+60000;
    sensorsAll();
  }
}
//===========================================================================
void showSimple(byte num){
  for(byte i=0;i<8;i++){
    if(num==i){
      if(senss.params[i]<10) showSimpleTemperature(senss.params[i],data[i]);
      else if(senss.params[i]<20) showSimpleHumidity(senss.params[i],data[i]);
      else if(senss.params[i]==20) showSimplePressure(senss.params[i],data[i]);
      else if(senss.params[i]==21) showSimpleNumeric(senss.params[i],data[i]);
      else showSimpleBattery(senss.params[i],data[i]);
      break;
    }
  }
}
//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ========================================
void showSimpleTemperature(byte znakT,float temp0){
  int temp1=int(temp0);
  int temp2=int(temp0*10*(temp0>0?1:-1))%10;
  int znak=znakT%10;
  byte digPos[10] {3,4,5,6,17,18,19,20,21,22};
  byte indent=alignment();
  dx=dy=0;
  clr(leds.NUM_VER_X);
  showDigit((temp0<0.0?digPos[znak*2+1]:digPos[znak*2]),indent,leds.fontSizeData?znaki5x7:znaki5x8);
  if(temp1<=-10.0||temp1>=10) showDigit((temp1<0?(temp1*-1)/10:temp1/10),4+indent,leds.fontSizeData?dig5x7:dig5x8);
  showDigit((temp1<0?(temp1*-1)%10:temp1%10),10+indent,leds.fontSizeData?dig5x7:dig5x8);
  showDigit(2,16+indent,leds.fontSizeData?znaki5x7:znaki5x8);
  showDigit(temp2,18+indent,leds.fontSizeData?dig5x7:dig5x8);
  showDigit(0,24+indent,leds.fontSizeData?znaki5x7:znaki5x8);
  showDigit(1,27+indent,leds.fontSizeData?znaki5x7:znaki5x8);
  if(leds.dataDown) scrollDown(leds.NUM_VER_X);
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ВОЛОГОСТІ========================================
void showSimpleHumidity(byte znakT,float humi0){
  if(humi0>0){
    if(humi0>=100)humi0=99.9;
    int humi1=int(humi0);
    int humi2=int(humi0*10*(humi0>0?1:-1))%10;
    int znak=znakT%10;
    byte digPos[5] {7,23,24,25,26};
    byte indent=alignment();
    dx=dy=0;
    clr(leds.NUM_VER_X);
    showDigit(digPos[znak],indent,leds.fontSizeData?znaki5x7:znaki5x8);     // друкуємо знак вологості
    if(humi1>=10) showDigit(humi1/10,6+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit((humi1-(humi1/10)*10),12+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit(2,18+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    showDigit(humi2,20+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit(8,26+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    if(leds.dataDown) scrollDown(leds.NUM_VER_X);
    refreshAll();
  }
}
//==========ВИВІД НА ЕКРАН ТИСКУ========================================
void showSimplePressure(byte znakT,float press0){
  if(senss.pressSys==1&&press0>815) press0/=1.3332239;
  if(senss.pressSys!=1&&press0<815) press0/=0.7500615613026439;
  if(press0>0){
    int press1=(int)press0/1000;
    int press2=((int)press0-press1*1000)/100;
    int press3=((int)press0-press1*1000-press2*100)/10;
    int press4=(int)press0%10;
    byte indent=alignment();
    dx=dy=0;
    clr(leds.NUM_VER_X);
    showDigit(9,indent,leds.fontSizeData?znaki5x7:znaki5x8);
    if(press1>0) showDigit(press1,5+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit(press2,(press1>0?10:6)+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit(press3,(press1>0?16:12)+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit(press4,(press1>0?22:18)+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit((senss.pressSys==1?10:15),(press1>0?28:24)+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    showDigit((senss.pressSys==1?11:16),(press1>0?(senss.pressSys==1?33:32):(senss.pressSys==1?29:28))+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    if(leds.dataDown) scrollDown(leds.NUM_VER_X);
    refreshAll();
  }
}
//========== ВИВІД НА ЕКРАН БАТАРЕЇ ==================================================
void showSimpleBattery(byte znakT, float batt0){
  if(batt0>0){
    int batt1=int(batt0);
    int batt2=int(batt0*10*(batt0>0?1:-1))%10;
    byte indent=alignment();
    dx=dy=0;
    clr(leds.NUM_VER_X);
    showDigit(znakT+5,indent,leds.fontSizeData?znaki5x7:znaki5x8);
    if(batt1>=10) showDigit(batt1/10,6+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit((batt1-(batt1/10)*10),12+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit(2,18+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    showDigit(batt2,20+indent,leds.fontSizeData?dig5x7:dig5x8);
    showDigit(znakT+7,26+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    if(leds.dataDown) scrollDown(leds.NUM_VER_X);
    refreshAll();
  }
}
//==========ВИВІД НА ЕКРАН ДОДАТКОВИХ ДАННИХ========================================
void showSimpleNumeric(byte znakT,float numer0){
  byte indent=alignment();
  dx=dy=0;
  clr(leds.NUM_VER_X);
  showDigit((numer0<0.0?32:31),indent,leds.fontSizeData?znaki5x7:znaki5x8);
  float numer1=numer0*(numer0>=0?1:-1);
  if(numer1>=10000){
    showDigit((int)numer1%10,28+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1/10)%10,23+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1/100)%10,18+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1/1000)%10,13+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1/10000)%10,8+indent,leds.fontSizeData?dig4x7:dig4x8);
  }else if(numer1>=1000){
    showDigit((int)(numer1*10)%10,28+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit(2,26+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    showDigit((int)numer1%10,21+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1/10)%10,16+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1/100)%10,11+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1/1000)%10,6+indent,leds.fontSizeData?dig4x7:dig4x8);      
  }else{
    showDigit((int)(numer1*100)%10,28+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit((int)(numer1*10)%10,23+indent,leds.fontSizeData?dig4x7:dig4x8);
    showDigit(2,21+indent,leds.fontSizeData?znaki5x7:znaki5x8);
    showDigit((int)numer1%10,16+indent,leds.fontSizeData?dig4x7:dig4x8);
    if(numer1>=10) showDigit((int)(numer1/10)%10,11+indent,leds.fontSizeData?dig4x7:dig4x8);
    if(numer1>=100) showDigit((int)(numer1/100)%10,6+indent,leds.fontSizeData?dig4x7:dig4x8);
  }
  if(leds.dataDown) scrollDown(leds.NUM_VER_X);
  refreshAll(); 
}
//==========ВИВІД НА ЕКРАН ДАТИ=========================================================
void showSimpleDate(){
  bool nonsens=false;
  byte digPos[8] {0,5,10,12,17,22,23,28};
  if(year%10==1){
    digPos[6]++;
    if(month%10==1){digPos[3]++; digPos[2]++; digPos[1]++; digPos[0]++;}
    if(month/10==1){digPos[2]++; digPos[1]++; digPos[0]++;}
    if(day%10==1){  digPos[0]++;}
  } else {
    if(month%10==1){digPos[5]--; digPos[4]--;}
    else if(month/10==1){digPos[5]--; digPos[4]--; digPos[3]--;}
    if(month%10==1 && month/10==1){digPos[2]++; digPos[1]++; digPos[0]++;}
    if(month%10!=1 && month/10!=1&&(day%10==1||day/10==1)){digPos[5]--; digPos[4]--; digPos[3]--; digPos[2]--; digPos[1]--;}
    else if(day%10==1){digPos[0]++;}
    if(month%10!=1&&month/10!=1&&day%10!=1&&day/10==1) nonsens=true;
  }
  byte indent=alignment();
  dx=dy=0;
  clr(leds.NUM_VER_X);
  showDigit(nonsens?10:day/10,digPos[0]+indent,leds.fontSizeData?dig4x7:dig4x8);
  showDigit(day%10,digPos[1]+indent,leds.fontSizeData?dig4x7:dig4x8);
  showDigit(2,digPos[2]+indent,leds.fontSizeData?znaki5x7:znaki5x8);
  showDigit(month/10,digPos[3]+indent,leds.fontSizeData?dig4x7:dig4x8);
  showDigit(month%10,digPos[4]+indent,leds.fontSizeData?dig4x7:dig4x8);
  showDigit(2,digPos[5]+indent,leds.fontSizeData?znaki5x7:znaki5x8);
  showDigit((year-2000)/10,digPos[6]+indent,leds.fontSizeData?dig4x7:dig4x8);
  showDigit((year-2000)%10,digPos[7]+indent,leds.fontSizeData?dig4x7:dig4x8);
  if(leds.dataDown) scrollDown(leds.NUM_VER_X);
  refreshAll();
}
//==========Выравнивание данных=========================================================
byte alignment(){
  bigClock=0;
  if(!leds.dataAlig) return 0;
  byte indent=(leds.NUM_VER_X?leds.NUM_MAX1*8:leds.NUM_MAX0*8)-32;
  if(leds.dataAlig==1&&indent) indent/=2;
  return indent;
}
//==========ВИВІД НА ЕКРАН АНІМАЦІЙНОГО ГОДИННИКА=======================================
void showAnimClock(){
  //select_effect_led();
  if(!leds.NUM_VER_X&&!bigClock) return;
  animClock.detach();
  //ширина 4
  byte digPos[6] {5,10,18,23,25,11}; //дЧ, еЧ, дМ, еМ, начало секунд, точки нижние
  byte digPoint[2] {15};
  digPoint[1]=digPoint[0]+(leds.NUM_MAX0*8);
  bool num=hour<10?1:0;
  if(leds.clockAndSec==1){//с секундами
    digPos[0]-=5;digPos[1]-=4;digPos[2]-=5;digPos[3]-=4;
    if(num){digPos[1]-=4;digPos[2]-=2;digPos[3]-=1;digPos[5]-=3;}
  } else if(leds.clockAndSec==2){//макс 
    if(bigClock&&leds.NUM_VER_X){                                             // ширина 4
      if(leds.fontCLOCK<2){digPos[0]-=4;digPos[1]-=2;digPos[3]+=2;            // ширина 6 
      } else if(leds.fontCLOCK<6){digPos[0]-=2;digPos[1]-=1;digPos[3]+=1;}    // ширина 5
      if(num){digPos[1]-=3;digPos[2]-=3;digPos[3]-=3;  digPoint[0]-=3;digPoint[1]-=3;}
    } else{
      digPos[0]-=5;digPos[1]-=4;digPos[2]-=5;digPos[3]-=4;
      if(num){digPos[1]-=4;digPos[2]-=2;digPos[3]-=1;digPos[5]-=3;}
    }
  }else{  // без секунд                                                     // ширина 4
    if(leds.fontCLOCK<2){digPos[0]-=4;digPos[1]-=2;digPos[3]+=2;            // ширина 6 
    } else if(leds.fontCLOCK<6){digPos[0]-=2;digPos[1]-=1;digPos[3]+=1;}    // ширина 5
    if(num){digPos[1]-=3;digPos[2]-=3;digPos[3]-=3;  digPoint[0]-=3;digPoint[1]-=3;}
  }
  byte indent=0;
  if(leds.clockAlig){
    indent=(leds.NUM_MAX0*8)-32;
    if(leds.clockAlig==1) indent/=2;
    for(byte i=0;i<6;i++){
      digPos[i]+=indent;
      if(i<2)digPoint[i]+=indent;
    }
  }
  int digHt=16;
  int i;
  if(del==0){
    del=digHt;
    for(i=num; i<4; i++) digold[i]=dig[i];
    dig[0]=hour/10 ;
    dig[1]=hour%10;
    dig[2]=minute/10;
    dig[3]=minute%10;
    for(i=num; i<4; i++) digtrans[i]=(dig[i]==digold[i])?0:digHt;
  } else del--;
  if(bigClock) clrAll();
  else clr(0);
  // ------------------  выводим часы и минуты
  for(i=num;i<4;i++){   
    if(digtrans[i]==0){
      dy=0;
      if(!bigClock||!leds.NUM_VER_X){
        if(leds.fontCLOCK==0) showDigitH(dig[i],digPos[i],leds.clockAndSec>=1?dig5x7s:dig6x7);
        else if(leds.fontCLOCK==1) showDigitH(dig[i],digPos[i],leds.clockAndSec>=1?dig5x8s:dig6x8);
        else if(leds.fontCLOCK==2) showDigitH(dig[i],digPos[i],dig5x7rn);
        else if(leds.fontCLOCK==3) showDigitH(dig[i],digPos[i],dig5x8rn);
        else if(leds.fontCLOCK==4) showDigitH(dig[i],digPos[i],dig5x7);
        else if(leds.fontCLOCK==5) showDigitH(dig[i],digPos[i],dig5x8);
        else if(leds.fontCLOCK==6) showDigitH(dig[i],digPos[i],dig4x7);
        else if(leds.fontCLOCK==7) showDigitH(dig[i],digPos[i],dig4x8);
      } else if(leds.clockAndSec!=1){ // без секунд
        if(leds.fontCLOCK==0){showDigitH(dig[i],digPos[i],dig6x6bigH);showDigit(dig[i],digPos[i],dig6x6bigL);}
        else if(leds.fontCLOCK==1){showDigitH(dig[i],digPos[i],leds.secLine==0?dig6x7bigH:dig6x6bigH); showDigit(dig[i],digPos[i],leds.secLine==0?dig6x7bigL:dig6x6bigL);}
        else if(leds.fontCLOCK==2){showDigitH(dig[i],digPos[i],dig5x6rnH);showDigit(dig[i],digPos[i],dig5x6rnL);}
        else if(leds.fontCLOCK==3){showDigitH(dig[i],digPos[i],leds.secLine==0?dig5x7rnH:dig5x6rnH);showDigit(dig[i],digPos[i],leds.secLine==0?dig5x7rnL:dig5x6rnL);}
        else if(leds.fontCLOCK==4){showDigitH(dig[i],digPos[i],dig5x6H);showDigit(dig[i],digPos[i],dig5x6L);}
        else if(leds.fontCLOCK==5){showDigitH(dig[i],digPos[i],leds.secLine==0?dig5x7H:dig5x6H);showDigit(dig[i],digPos[i],leds.secLine==0?dig5x7L:dig5x6L);}
        else if(leds.fontCLOCK==6){showDigitH(dig[i],digPos[i],dig4x6H);showDigit(dig[i],digPos[i],dig4x6L);}
        else if(leds.fontCLOCK==7){showDigitH(dig[i],digPos[i],leds.secLine==0?dig4x7H:dig4x6H);showDigit(dig[i],digPos[i],leds.secLine==0?dig4x7L:dig4x6L);}
      } else{
        if(leds.fontCLOCK==0){showDigitH(dig[i],digPos[i],dig5x6bigH);showDigit(dig[i],digPos[i],dig5x6bigL);}
        else if(leds.fontCLOCK==1){showDigitH(dig[i],digPos[i],dig5x7bigH);showDigit(dig[i],digPos[i],dig5x7bigL);}
        else if(leds.fontCLOCK==2){showDigitH(dig[i],digPos[i],dig5x6rnH);showDigit(dig[i],digPos[i],dig5x6rnL);}
        else if(leds.fontCLOCK==3){showDigitH(dig[i],digPos[i],dig5x7rnH);showDigit(dig[i],digPos[i],dig5x7rnL);}
        else if(leds.fontCLOCK==4){showDigitH(dig[i],digPos[i],dig5x6H);showDigit(dig[i],digPos[i],dig5x6L);}
        else if(leds.fontCLOCK==5){showDigitH(dig[i],digPos[i],dig5x7H);showDigit(dig[i],digPos[i],dig5x7L);}
        else if(leds.fontCLOCK==6){showDigitH(dig[i],digPos[i],dig4x6H);showDigit(dig[i],digPos[i],dig4x6L);}
        else if(leds.fontCLOCK==7){showDigitH(dig[i],digPos[i],dig4x7H);showDigit(dig[i],digPos[i],dig4x7L);} 
      }
    } else {  //АНИМАЦИЯ ПЕРЕЛИСТЫВАНИЯ
      dy=digHt-digtrans[i];
      if(!bigClock||!leds.NUM_VER_X){
        if(leds.fontCLOCK==0) showDigitH(digold[i],digPos[i],leds.clockAndSec>=1?dig5x7s:dig6x7);
        else if(leds.fontCLOCK==1) showDigitH(digold[i],digPos[i],leds.clockAndSec>=1?dig5x8s:dig6x8);
        else if(leds.fontCLOCK==2) showDigitH(digold[i],digPos[i],dig5x7rn);
        else if(leds.fontCLOCK==3) showDigitH(digold[i],digPos[i],dig5x8rn);
        else if(leds.fontCLOCK==4) showDigitH(digold[i],digPos[i],dig5x7);
        else if(leds.fontCLOCK==5) showDigitH(digold[i],digPos[i],dig5x8);
        else if(leds.fontCLOCK==6) showDigitH(digold[i],digPos[i],dig4x7);
        else if(leds.fontCLOCK==7) showDigitH(digold[i],digPos[i],dig4x8);
      } else if(leds.clockAndSec!=1){ // без секунд
        if(leds.fontCLOCK==0){showDigitH(digold[i],digPos[i],dig6x6bigH);showDigit(digold[i],digPos[i],dig6x6bigL);}
        else if(leds.fontCLOCK==1){showDigitH(digold[i],digPos[i],leds.secLine==0?dig6x7bigH:dig6x6bigH);showDigit(digold[i],digPos[i],leds.secLine==0?dig6x7bigL:dig6x6bigL);}
        else if(leds.fontCLOCK==2){showDigitH(digold[i],digPos[i],dig5x6rnH);showDigit(digold[i],digPos[i],dig5x6rnL);}
        else if(leds.fontCLOCK==3){showDigitH(digold[i],digPos[i],leds.secLine==0?dig5x7rnH:dig5x6rnH);showDigit(digold[i],digPos[i],leds.secLine==0?dig5x7rnL:dig5x6rnL);}
        else if(leds.fontCLOCK==4){showDigitH(digold[i],digPos[i],dig5x6H);showDigit(digold[i],digPos[i],dig5x6L);}
        else if(leds.fontCLOCK==5){showDigitH(digold[i],digPos[i],leds.secLine==0?dig5x7H:dig5x6H);showDigit(digold[i],digPos[i],leds.secLine==0?dig5x7L:dig5x6L);}
        else if(leds.fontCLOCK==6){showDigitH(digold[i],digPos[i],dig4x6H);showDigit(digold[i],digPos[i],dig4x6L);}
        else if(leds.fontCLOCK==7){showDigitH(digold[i],digPos[i],leds.secLine==0?dig4x7H:dig4x6H);showDigit(digold[i],digPos[i],leds.secLine==0?dig4x7L:dig4x6L);}
      } else{
        if(leds.fontCLOCK==0){showDigitH(digold[i],digPos[i],dig5x6bigH);showDigit(digold[i],digPos[i],dig5x6bigL);}
        else if(leds.fontCLOCK==1){showDigitH(digold[i],digPos[i],dig5x7bigH);showDigit(digold[i],digPos[i],dig5x7bigL);}
        else if(leds.fontCLOCK==2){showDigitH(digold[i],digPos[i],dig5x6rnH);showDigit(digold[i],digPos[i],dig5x6rnL);}
        else if(leds.fontCLOCK==3){showDigitH(digold[i],digPos[i],dig5x7rnH);showDigit(digold[i],digPos[i],dig5x7rnL);}
        else if(leds.fontCLOCK==4){showDigitH(digold[i],digPos[i],dig5x6H);showDigit(digold[i],digPos[i],dig5x6L);}
        else if(leds.fontCLOCK==5){showDigitH(digold[i],digPos[i],dig5x7H);showDigit(digold[i],digPos[i],dig5x7L);}
        else if(leds.fontCLOCK==6){showDigitH(digold[i],digPos[i],dig4x6H);showDigit(digold[i],digPos[i],dig4x6L);}
        else if(leds.fontCLOCK==7){showDigitH(digold[i],digPos[i],dig4x7H);showDigit(digold[i],digPos[i],dig4x7L);} 
      }
      dy=-digtrans[i];
      if(!bigClock||!leds.NUM_VER_X){
       if(leds.fontCLOCK==0) showDigitH(dig[i],digPos[i],leds.clockAndSec>=1?dig5x7s:dig6x7);
        else if(leds.fontCLOCK==1) showDigitH(dig[i],digPos[i],leds.clockAndSec>=1?dig5x8s:dig6x8);
        else if(leds.fontCLOCK==2) showDigitH(dig[i],digPos[i],dig5x7rn);
        else if(leds.fontCLOCK==3) showDigitH(dig[i],digPos[i],dig5x8rn);
        else if(leds.fontCLOCK==4) showDigitH(dig[i],digPos[i],dig5x7);
        else if(leds.fontCLOCK==5) showDigitH(dig[i],digPos[i],dig5x8);
        else if(leds.fontCLOCK==6) showDigitH(dig[i],digPos[i],dig4x7);
        else if(leds.fontCLOCK==7) showDigitH(dig[i],digPos[i],dig4x8);
      } else if(leds.clockAndSec==0){ // без секунд
        if(leds.fontCLOCK==0){showDigitH(dig[i],digPos[i],dig6x6bigH);showDigit(dig[i],digPos[i],dig6x6bigL);}
        else if(leds.fontCLOCK==1){showDigitH(dig[i],digPos[i],leds.secLine==0?dig6x7bigH:dig6x6bigH); showDigit(dig[i],digPos[i],leds.secLine==0?dig6x7bigL:dig6x6bigL);}
        else if(leds.fontCLOCK==2){showDigitH(dig[i],digPos[i],dig5x6rnH);showDigit(dig[i],digPos[i],dig5x6rnL);}
        else if(leds.fontCLOCK==3){showDigitH(dig[i],digPos[i],leds.secLine==0?dig5x7rnH:dig5x6rnH);showDigit(dig[i],digPos[i],leds.secLine==0?dig5x7rnL:dig5x6rnL);}
        else if(leds.fontCLOCK==4){showDigitH(dig[i],digPos[i],dig5x6H);showDigit(dig[i],digPos[i],dig5x6L);}
        else if(leds.fontCLOCK==5){showDigitH(dig[i],digPos[i],leds.secLine==0?dig5x7H:dig5x6H);showDigit(dig[i],digPos[i],leds.secLine==0?dig5x7L:dig5x6L);}
        else if(leds.fontCLOCK==6){showDigitH(dig[i],digPos[i],dig4x6H);showDigit(dig[i],digPos[i],dig4x6L);}
        else if(leds.fontCLOCK==7){showDigitH(dig[i],digPos[i],leds.secLine==0?dig4x7H:dig4x6H);showDigit(dig[i],digPos[i],leds.secLine==0?dig4x7L:dig4x6L);}
      } else{
        if(leds.fontCLOCK==0){showDigitH(dig[i],digPos[i],dig5x6bigH);showDigit(dig[i],digPos[i],dig5x6bigL);}
        else if(leds.fontCLOCK==1){showDigitH(dig[i],digPos[i],dig5x7bigH);showDigit(dig[i],digPos[i],dig5x7bigL);}
        else if(leds.fontCLOCK==2){showDigitH(dig[i],digPos[i],dig5x6rnH);showDigit(dig[i],digPos[i],dig5x6rnL);}
        else if(leds.fontCLOCK==3){showDigitH(dig[i],digPos[i],dig5x7rnH);showDigit(dig[i],digPos[i],dig5x7rnL);}
        else if(leds.fontCLOCK==4){showDigitH(dig[i],digPos[i],dig5x6H);showDigit(dig[i],digPos[i],dig5x6L);}
        else if(leds.fontCLOCK==5){showDigitH(dig[i],digPos[i],dig5x7H);showDigit(dig[i],digPos[i],dig5x7L);}
        else if(leds.fontCLOCK==6){showDigitH(dig[i],digPos[i],dig4x6H);showDigit(dig[i],digPos[i],dig4x6L);}
        else if(leds.fontCLOCK==7){showDigitH(dig[i],digPos[i],dig4x7H);showDigit(dig[i],digPos[i],dig4x7L);}   
      }
      digtrans[i]--;
    }
  }
  //-------------------  выводим секунды или анимированные точки
  dy=0;
  int flash=millis()%2000;
  if(leds.animPoint%2) flash=flash%1000;
  else flash=flash/2;
  
  
  
  //------------------- если выводим секунды
  if(leds.clockAndSec==1||(!bigClock&&leds.clockAndSec==2)||(!leds.NUM_VER_X&&leds.clockAndSec==2)){
    if(flash<500) setCol(digPos[5]+((bigClock==1&&leds.NUM_VER_X)?leds.NUM_MAX0*8:0),0x80);
    else setCol(digPos[5]+1+((bigClock==1&&leds.NUM_VER_X)?leds.NUM_MAX0*8:0),0x80);
    if(!(!leds.NUM_VER_X&&alarm_work&&!stopAlarm)){
      if(leds.clockAndSec!=2){
        if(bigClock==1){showDigit((second/10)%10,digPos[4],leds.fontCLOCK%2==0?dig3x6:dig3x7);showDigit(second%10,digPos[4]+4,leds.fontCLOCK%2==0?dig3x6:dig3x7);}
        else{showDigitH((second/10)%10,digPos[4],leds.fontCLOCK%2==0?dig3x6:dig3x7);showDigitH(second%10,digPos[4]+4,leds.fontCLOCK%2==0?dig3x6:dig3x7);}
      }else{//maxMode
        if(leds.maxModeSec<3)shift=leds.maxModeSec;
        else if(timerRandomMax<millis()){
          timerRandomMax=millis()+(random(1,11)*1000);
          shift=random(3);
        }
        showDigitMax((second/10)%10,digPos[4],leds.fontCLOCK%2==0?max3x5:max3x6,shift);
        showDigitMax(second%10,digPos[4]+4,leds.fontCLOCK%2==0?max3x5:max3x6,shift);
      }
    }
  // ------------------  выводим точки 
  } else{
    // точки только в первой строке
    if(!bigClock||!leds.NUM_VER_X){  
      if(leds.clockAndSec!=1){
        if(!WIFI_connected){
          // полная точка
          if(flash<500){setCol(digPoint[0],leds.fontCLOCK%2==0?0x60:0xC0);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x60:0xC0);}
        } else if(!statusUpdateNtpTime){
          if(flash<149){setCol(digPoint[0],leds.fontCLOCK%2==0?0x36:0x66);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x36:0x66);}
        // Простая
        } else if(leds.animPoint<3){
          if(flash<499||leds.animPoint==0){setCol(digPoint[0],leds.fontCLOCK%2==0?0x36:0x66);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x36:0x66);}
        // MAX1
        } else if(leds.animPoint==3||leds.animPoint==4){ 
          if(flash<500){setCol(digPoint[0],leds.fontCLOCK%2==0?0x32:0x62);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x32:0x62);}
          else{setCol(digPoint[0],leds.fontCLOCK%2==0?0x26:0x46);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x26:0x46);}
        // MAX2
        } else if(leds.animPoint==5||leds.animPoint==6){ 
          if((flash>=200&&flash<400)||flash>=600){setCol(digPoint[0],leds.fontCLOCK%2==0?0x36:0x66);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x36:0x66);}
          if(flash>=0&&flash<200){setCol(digPoint[0],leds.fontCLOCK%2==0?0x14:0x24);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x14:0x24);}
          if(flash>=400&&flash<600){setCol(digPoint[0],leds.fontCLOCK%2==0?0x22:0x42);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x22:0x42);}
        // Мерцание 
        } else if(leds.animPoint==7||leds.animPoint==8){ 
          if((flash>=(leds.animPoint==5?180:200)&&flash<(leds.animPoint==5?360:400))||flash>=(leds.animPoint==5?540:600)){setCol(digPoint[0],leds.fontCLOCK%2==0?0x36:0x66);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x36:0x66);}
          if(flash>=0&&flash<(leds.animPoint==5?180:200)){setCol(digPoint[0],leds.fontCLOCK%2==0?0x14:0x24);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x22:0x42);}
          if(flash>=(leds.animPoint==5?360:400)&&flash<(leds.animPoint==5?540:600)){setCol(digPoint[0],leds.fontCLOCK%2==0?0x22:0x42);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x14:0x24);}
        // Вращение 
        } else if(leds.animPoint==9||leds.animPoint==10){ 
          if(flash<250){setCol(digPoint[0],leds.fontCLOCK%2==0?0x06:0x06);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x30:0x60);}
          else if(flash>=250&&flash<500){setCol(digPoint[0],leds.fontCLOCK%2==0?0x22:0x42);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x22:0x42);}
          else if(flash>=500&&flash<750){setCol(digPoint[0],leds.fontCLOCK%2==0?0x30:0x60);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x06:0x06);}
          else if(flash>=750){setCol(digPoint[0],leds.fontCLOCK%2==0?0x14:0x24);setCol(digPoint[0]+1,leds.fontCLOCK%2==0?0x14:0x24);}
        }
        // точки по углам
        if(updateForecast&&WIFI_connected&&weaths.weathOn&&weaths.animNotWeather) setCol(00,flash<500?0x80:0x00);
        if(updateForecasttomorrow && WIFI_connected && weaths.weathOn && weaths.animNotWeather) setCol(leds.NUM_MAX0*8-1, flash<500?0x80:0x00);
        // будильник
      }
    // точки в две строки
    } else if(leds.clockAndSec!=1){
      if(!WIFI_connected){
        if(flash<500){setCol(digPoint[1],0x30);setCol(digPoint[1]+1,0x30);}
      } else if(!statusUpdateNtpTime){
        if(flash<149){setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x0C);}
      // Простая 
      } else if(leds.animPoint<3){
        if(flash<499||leds.animPoint==0){setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x0C);}
      // MAX1
      } else if(leds.animPoint==3||leds.animPoint==4){
        if(flash<500){setCol(digPoint[0],0x10);setCol(digPoint[0]+1,0x10);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x0C);}
        else{setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x08);setCol(digPoint[1]+1,0x08);}
      // MAX2
      } else if(leds.animPoint==5||leds.animPoint==6){
          if((flash>=200&&flash<400)||flash>=600){setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x0C);}
          if(flash>=0&&flash<200){setCol(digPoint[0],0x10);setCol(digPoint[0]+1,0x10);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x0C);}
          if(flash>=400&&flash<600){setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x08);setCol(digPoint[1]+1,0x08);}
      //Мерцание 
      } else if(leds.animPoint==7||leds.animPoint==8){
          if((flash>=(leds.animPoint==5?180:200)&&flash<(leds.animPoint==5?360:400))||flash>=(leds.animPoint==5?540:600)){setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x0C);}
          if(flash>=0&&flash<(leds.animPoint==5?180:200)){setCol(digPoint[0],0x10);setCol(digPoint[0]+1,0x20);setCol(digPoint[1],0x08);setCol(digPoint[1]+1,0x04);}
          if(flash>=(leds.animPoint==5?360:400)&&flash<(leds.animPoint==5?540:600)){setCol(digPoint[0],0x20);setCol(digPoint[0]+1,0x10);setCol(digPoint[1],0x04);setCol(digPoint[1]+1,0x08);}
      // Вращение 
      } else if(leds.animPoint==9||leds.animPoint==10){ 
          if(flash<250){setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x20);setCol(digPoint[1],0x04);setCol(digPoint[1]+1,0x0C);}
          else if(flash>=250&&flash<500){setCol(digPoint[0],0x30);setCol(digPoint[0]+1,0x10);setCol(digPoint[1],0x08);setCol(digPoint[1]+1,0x0C);}
          else if(flash>=500&&flash<750){setCol(digPoint[0],0x10);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x08);}
          else if(flash>=750){setCol(digPoint[0],0x20);setCol(digPoint[0]+1,0x30);setCol(digPoint[1],0x0C);setCol(digPoint[1]+1,0x04);}
      }
      // ------------------  анимация секунд
      if(leds.secLine&&leds.NUM_VER_X){
        if(leds.secLine==1){
          for(byte s=0;s<=second;s++){
            if(s<15){byte addr=s+16+indent;setCol(addr,scr[addr]|0x01);}
            else if(s>=14&&s<45){byte addr=leds.NUM_MAX0*8+indent+(45-s);setCol(addr, scr[addr]|0x80);}
            else{byte addr=s-44+indent;setCol(addr, scr[addr]|0x01);}
          }
        } else if(leds.secLine==2){
          for(byte s=0;s<=second;s++){
            if(s<30){byte addr=s+1+indent;int asd=scr[addr]|0x01;setCol(addr,asd);}
            else{byte addr=leds.NUM_MAX0*8+indent+(60-s);int asd=scr[addr]|0x80;setCol(addr,asd);  
            }
          }
        } else{
          for(byte s=0;s<=second;s++){
            byte addr=leds.NUM_MAX0*8+(s/2)+1+indent;
            int asd=scr[addr]|0x80;
            setCol(addr, asd);
            s++;
          }
        }
      }
      // точки в углах - обновление погоды
      if(updateForecast && WIFI_connected && weaths.weathOn && weaths.animNotWeather) setCol(leds.NUM_MAX0*8, flash < 500 ? 0x80 : 0x00);
      if(updateForecasttomorrow && WIFI_connected && weaths.weathOn && weaths.animNotWeather) setCol((leds.NUM_MAX0+leds.NUM_MAX1)*8-1, flash < 500 ? 0x80 : 0x00);
    }
  }
  if(alarm_work&&!stopAlarm){
    leds.clockAndSec=1;
    if(leds.NUM_VER_X) bigClock=false;
    else bigClock=true;
    if(leds.NUM_VER_X) clr(leds.NUM_VER_X);
    if(flash%200<50){
      showDigit(13,leds.NUM_VER_X?leds.NUM_MAX0*4-3:digPos[4]+1,leds.fontCLOCK%2==0?znaki5x7:znaki5x8);
    } else{
      showDigit(12,leds.NUM_VER_X?leds.NUM_MAX0*4-3:digPos[4]+1,leds.fontCLOCK%2==0?znaki5x7:znaki5x8);
    }
  }
  refreshAll();
  animClock.attach(0.05,showAnimClock);
}

//==========ДРУКУВАННЯ БІГУЧОЇ СТРОКИ *s - текст, shiftDelay - швидкість==========================================
void printStringWithShift(const char* s,int shiftDelay){
  endString=false;
  bool oldBigCklok=bigClock;
  bigClock=0;
  while(*s){                                                  // коли працює ця функція, основний цикл зупиняється
    server.handleClient();
    printCharWithShift(*s,shiftDelay,leds.NUM_VER_X);
    s++;
    if(senss.mqttOn) MQTTclient.loop();                              // зберігаемо можливість слухати MQTT топік під час бігучої стоки
    if(butMode!=0||(!stopAlarm&&alarm_work)||stop_line_mdate_work){
      if(stop_line_mdate_work) stop_line_mdate_work=false;
      clr(leds.NUM_VER_X);
      refreshAll();
      bigClock=oldBigCklok;
      endString=true;
      if(butMode!=0)bip();
      butMode=0;
      return;
    }
    yield();
  }
  bigClock=oldBigCklok;
  endString=true;
}
//==ДРУКУВАННЯ БІГУЧОГО СИМВОЛУ с - символ, shiftDelay - швидкість=====================================
void printCharWithShift(unsigned char c,int shiftDelay,byte zone){
  c=convert_UA_RU_PL_DE(c);
  //Serial.println(c);
  if(c<' ') return;
  c-=32;
  int w=showChar(c,fontUA_RU_PL_DE,zone);
  for (int i=0;i<w+1;i++){
    delay(shiftDelay);
    scrollLeft(leds.NUM_VER_X);
    refreshAll();
    buttonInter();
    if(butMode!=0||(alarm_work&&!stopAlarm)){
      break;
      //return;
    }
  }
}
//======================================================================================
int showChar(char ch,const uint8_t *data,byte zone){
  int len=pgm_read_byte(data);
  int i,w=pgm_read_byte(data+1+ch*len);
  for(i=0;i<w;i++){
    scr[(zone?leds.NUM_MAX0+leds.NUM_MAX1:leds.NUM_MAX0)*8+i]=pgm_read_byte(data+1+ch*len+1+i)<<leds.textDown;
  }
  scr[(zone?leds.NUM_MAX0+leds.NUM_MAX1:leds.NUM_MAX0)*8+i]=0;
  return w;
}
//======================================================================================
void showDigit(char ch,int col,const uint8_t *data){
  if(dy<-8|dy>8) return;
  int len=pgm_read_byte(data);
  int w=pgm_read_byte(data+1+ch*len);
  col+=dx+(leds.NUM_VER_X?leds.NUM_MAX0*8:0);
  for(int i=0;i<w;i++){
    if(col+i>=0&&col+i<8*(leds.NUM_MAX0+(leds.NUM_VER_X?leds.NUM_MAX1:0))){
      byte v=pgm_read_byte(data+1+ch*len+1+i);
      if(!dy)scr[col+i]=v;
      else scr[col+i]|=dy>0?v>>dy:v<<-dy;
    }
  }
}
//======================================================================================
void showDigitH(char ch,int col,const uint8_t *data){
  if(dy<-8|dy>8) return;
  int len=pgm_read_byte(data);
  int w=pgm_read_byte(data+1+ch*len);
  col+=dx;
  for(int i=0;i<w;i++){
    if(col+i>=0&&col+i<leds.NUM_MAX0*8){
      byte v=pgm_read_byte(data+1+ch*len+1+i);
      if(!dy)scr[col+i]=v;
      else scr[col+i]|=dy>0?v>>dy:v<<-dy;
    }
  }
}
//======================================================================================
void showDigitMax(char ch,int col,const uint8_t *data,uint8_t shift){
  if(dy<-8|dy>8) return;
  int len=pgm_read_byte(data);
  int w=pgm_read_byte(data+1+ch*len);
  col+=dx;
  for(int i=0;i<w;i++){
    if(col+i>=0&&col+i<leds.NUM_MAX0*8){
      byte v=pgm_read_byte(data+1+ch*len+1+i);
      v=v<<shift;
      if(!dy)scr[col+i]=v;
      else scr[col+i]|=dy>0?v>>dy:v<<-dy;
    }
  }
}
//======================================================================================
void setCol(int col,byte v){
  if(dy<-8|dy>8) return;
  col+=dx;
  if(col>=0&&col<8*(leds.NUM_MAX0+(leds.NUM_VER_X?leds.NUM_MAX1:0))){
    if(!dy) scr[col]=v;
    else scr[col]|=dy>0?v>>dy:v<<-dy;
  }
}

//==========КОНВЕРТАЦІЯ СИМВОЛІВ В РАЗІ ВИКОРИСТАННЯ УКРАЇНСЬКИХ ЛІТЕР==================
uint8_t dualChar=0;
unsigned char convert_UA_RU_PL_DE(unsigned char _c){
  unsigned char c=_c;
  // конвертирование латиницы
  if(c==208){dualChar=1;return 0;}
  if(c==209){dualChar=2;return 0;}
  if(c==210){dualChar=3;return 0;}
  if(c==32&&dualChar!=4){dualChar=4;return c;}
  
  if(dualChar==1){
    if(c>=144&&c<192){c+=48;}
    switch (_c){
      case 129: c=185; break;
    }
    dualChar=0;
    return c;
  }
  if(dualChar==2){
    if(c>=128&&c<144){c+=112;}
    switch (_c){
      //case 144: c=133; break;
      case 145: c=184; break;
    }
    dualChar=0;
    return c;
  }
  if(dualChar==3){
    if(c>=128&&c<144){c+=112;}
    switch (_c){
      case 144: c=133; break;
      case 145: c=149; break;
    }
    dualChar=0;
    return c;
  }
  //URL-коды символов ISO-Latin 
  if(c==194){dualChar=5;return 0;}
    if(dualChar==5){
    switch (_c){
      case 160: c=32; break;
      case 171: c=34; break;
      case 187: c=34; break;
    }
    dualChar=0;
    return c;
  }
  // конвертирование польского и немецкого
  if(c==195){dualChar=6;return 0;}
  if(c==196){dualChar=7;return 0;}
  if(c==197){dualChar=8;return 0;}

  if(dualChar==6){
    switch (_c){
      case 132: c=177; break;
      case 147: c=166; break;
      case 150: c=179; break;
      case 156: c=181; break;
      case 159: c=183; break;
      case 164: c=178; break;
      case 179: c=167; break;
      case 182: c=180; break;
      case 188: c=182; break;
    }
    dualChar=0;
    return c;
  }
  if(dualChar==7){
    if(c>=132&&c<136){c+=26;}
    switch (_c){
      case 152: c=168; break;
      case 153: c=169; break;
    }
    dualChar=0;
    return c;
  }
  if(dualChar==8){
    if(c>=129&&c<133){c+=33;}
    if(c>=154&&c<156){c+=16;}
    if(c>=185&&c<189){c-=13;}
    dualChar=0;
    return c;
  }
  // конвертирование UTF-8
  if(c==226){dualChar=10;return 0;}
  if(dualChar==10&&c==128){dualChar=11;return 0;}
  if(dualChar==11){
    if(c>=144&&c<148){c=46;}
    if(c>=148&&c<150){c=45;}
    dualChar=0;
    return c;
  }
  
  return c;
}


//==========
void saveChrMas(String string_t, byte lenght_off, byte number_s){
  byte lenght=string_t.length();
  if (lenght > lenght_off) return;
  const char *s=string_t.c_str();

  for (int i=0; i < lenght; i++){
    //snprintf(*memory_date_mes[number_s], 1, "%s", *s);
    s++;
  }
}

//==========ОНОВЛЕННЯ МЕРЕЖЕВОГО ЧАСУ (перевірка в три проходи)====================================================
void timeUpdateNTP(){
  if (!WIFI_connected) return;
  if (senss.printCom) printTime();
  statusUpdateNtpTime=1;
  for (int timeTest=0; timeTest < 3; timeTest++){
    getNTPtime();
    if (senss.printCom){
      if (timeTest) Serial.print("          ");
      Serial.println("Proba #"+String(timeTest+1)+"   "+String(g_hour)+":"+((g_minute < 10) ? "0" : "")+String(g_minute)+":"+((g_second < 10) ? "0" : "")+String(g_second));
    }
    // updateTime();

    hourTest[timeTest]=g_hour;
    minuteTest[timeTest]=(g_minute || (g_minute == 59 ? 0 : g_minute++));
    if (statusUpdateNtpTime == 0){
      if (senss.printCom){
        printTime();
        Serial.print("ERROR TIME!!!\r\n");
      }
      return;
    }
    if (timeTest > 0){
      if ((hourTest[timeTest] != hourTest[timeTest - 1] || minuteTest[timeTest] != minuteTest[timeTest - 1])){
        statusUpdateNtpTime=0;
        if (senss.printCom){
          printTime();
          Serial.print("ERROR TIME!!!\r\n");
        }
        return;
      }
    }
  }
  hour=g_hour;
  minute=g_minute;
  second=g_second;
  day=g_day;
  dayOfWeek=g_dayOfWeek;
  month=g_month;
  year=g_year;
  if(times.rtcStat){
    hour_rtc=hour;
    minute_rtc=minute;
    second_rtc=second;
    month_rtc=month;
    day_rtc=day;
    dayOfWeek_rtc=dayOfWeek;
    year_rtc=year;
    setRTCDateTime();
  }
  localMillisAtUpdate=millis();
  localEpoc=(hour * 60 * 60+minute * 60+second);
  saveStime();
  if (senss.printCom){
    printTime();
    Serial.println((day < 10 ? "0" : "")+String(day)+"."+(month < 10 ? "0" : "")+String(month)+"."+String(year)+" DW="+String(dayOfWeek));
    Serial.println("          Time update OK.");
  }
}
//==========ОТРИМАННЯ ДАТИ ТА ЧАСУ ВІД СЕРВЕРА ТОЧНОГО ЧАСУ =============================================================
void getNTPtime(){
  WiFi.hostByName(times.ntpServerName.c_str(),timeServerIP);
  int cb;
  for (int i=0;i<3;i++){
    memset(packetBuffer,0,NTP_PACKET_SIZE);
    packetBuffer[0]=0b11100011;
    packetBuffer[1]=0;
    packetBuffer[2]=6;
    packetBuffer[3]=0xEC;
    packetBuffer[12]=49;
    packetBuffer[13]=0x4E;
    packetBuffer[14]=49;
    packetBuffer[15]=52;
    udp.beginPacket(timeServerIP,123);                     //NTP порт 123
    udp.write(packetBuffer,NTP_PACKET_SIZE);
    udp.endPacket();
    delay(800);                                             // чекаємо пів секуни
    cb=udp.parsePacket();
    if(!cb&&senss.printCom) Serial.println("          no packet yet..."+String (i+1));
    if(!cb&&i==2){                                             // якщо час не отримано
      statusUpdateNtpTime=0;
      return;                                             // вихіз з getNTPtime()
    }
    if(cb) i=3;
  }
  if(cb){                                                  // якщо отримали пакет з серверу
    udp.read(packetBuffer,NTP_PACKET_SIZE);
    unsigned long highWord=word(packetBuffer[40],packetBuffer[41]);
    unsigned long lowWord=word(packetBuffer[42],packetBuffer[43]);
    unsigned long secsSince1900=highWord<<16|lowWord;
    const unsigned long seventyYears=2208988800UL;        // Unix час станом на 1 січня 1970. в секундах, то 2208988800:
    unsigned long epoch=secsSince1900-seventyYears;
    epochNM=epoch-(millis()/1000);
    bool summerTime;
    if(month<3||month>10) summerTime=false;            // не переходимо на літній час в січні, лютому, листопаді і грудню
    if(month>3&&month<10) summerTime=true;             // Sommerzeit лічимо в квіні, травні, червені, липні, серпені, вересені
    if(month==3&&(hour+24*day)>=(3+24*(31-(5*year/4+4)%7))||month==10&&(hour+24*day)<(3+24*(31-(5*year/4+1)%7))) summerTime=true;
    epoch+=(int)(times.timeZone*3600+(3600*(times.isDayLightSaving&&summerTime)));
    hourCorr=times.timeZone+(times.isDayLightSaving&&summerTime);
    g_year=0;
    int days=0;
    uint32_t time;
    time=epoch/86400;
    g_hour=(epoch%86400L)/3600;
    g_minute=(epoch%3600)/60;
    g_second=epoch%60;
    g_dayOfWeek=(((time)+4)%7)+1;
    while((unsigned)(days+=(LEAP_YEAR(g_year)?366:365))<=time){
      g_year++;
      yield();
    }
    days-=LEAP_YEAR(g_year)?366:365;
    time-=days;
    days=0;
    g_month=0;
    uint8_t monthLength=0;
    for(g_month=0; g_month<12;g_month++){
      if(g_month==1){
        if(LEAP_YEAR(g_year)) monthLength=29;
        else monthLength=28;
      }
      else monthLength=monthDays[g_month];
      if(time>=monthLength) time-=monthLength;
      else break;
    }
    g_month++;
    g_day=time+1;
    g_year+=1970;
    return;
  }
  if(senss.printCom) Serial.println("Nie ma czasu(((");
}
//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ  https://www.weatherbit.io                                                 //
//===============================================================================================================================//
void getWeatherData0(){
  if(weaths.weatherKey0==""||!weaths.weathOn) return;
  if(!WIFI_connected){
    updateForecast++;
    return;
  }
  if(senss.printCom){
    Serial.println("======== START GET WEATHER FROM WEATHERBIT.IO =========");
    printTime();
  }  
  location_name="";
  location_region="";
  location_country="";
  location_localtime="";
  location_temp=0;
  location_app_temp=0;
  location_rh=0;
  location_pres=0;
  location_wind_spd=0;
  location_wind_cdir_full="";
  location_sunrise="";
  location_sunset="";
  location_clouds=0;
  location_vis=0;
  location_uv=0;
  location_wds="";
  if(ESPclient.connect(weatherHost0.c_str(), 80)){}
  else {
    if(senss.printCom){
      Serial.println(" Not connection server!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost0+"/v2.0/current/daily?city="+urlencode(weaths.cityID0)+"&lang="+senss.messLang+"&key="+weaths.weatherKey0;
  if(senss.printCom) Serial.print(reqline);
  if(http.begin(ESPclient, reqline)){
    int httpCode=http.GET();
    if(httpCode > 0){
      if(senss.printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
        line=http.getString();
      }
    } else {
      if(senss.printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      updateForecast++;
      return;
    }
    http.end();
  } else {
    if(senss.printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  if(line==""){
    if(senss.printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  const size_t capacity=JSON_ARRAY_SIZE(1)+JSON_OBJECT_SIZE(2)+JSON_OBJECT_SIZE(3)+JSON_OBJECT_SIZE(37)+1128; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(senss.printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  JsonObject data=doc["data"][0];
  location_rh=data["rh"]; // 69
  location_pres=data["pres"]; // 999.3
  if(senss.pressSys==1) location_pres/=1.3332239;
  const char* data_timezone=data["timezone"]; // "Europe/Kiev"
  location_region=data_timezone;
  const char* data_ob_time=data["ob_time"]; // "2019-09-19 17:57"
  location_localtime=data_ob_time;
  const char* data_country_code=data["country_code"]; // "UA"
  location_country=data_country_code;
  location_clouds=data["clouds"]; // 4
  const char*   data_city_name=data["city_name"]; // "Kiev"
  location_name=data_city_name;
  location_wind_spd=data["wind_spd"]; // 1
  const char* data_wind_cdir_full=data["wind_cdir_full"]; // "пі́вдень-пі́вдень-схід"
  location_wind_cdir_full=data_wind_cdir_full;
    const char* data_wind_cdir = data["wind_cdir"]; // направление ветра
  location_vis=data["vis"]; // 5
  const char*   data_sunset=data["sunset"]; // "16:01"
  location_sunset=data_sunset;
  location_uv=data["uv"]; // 0
  int           data_wind_dir=data["wind_dir"]; // 166
  const char*   data_sunrise=data["sunrise"]; // "03:39"
  location_sunrise=data_sunrise;   //int         data_dni=data["dni"]; // 0
  JsonObject    data_weather=data["weather"];
  const char*   data_weather_description=data_weather["description"]; // "ясного неба"
  location_wds=data_weather_description;
  location_temp=data["temp"]; // 10.6
  location_app_temp=data["app_temp"]; // 10.6
  String windDegString;
  if(data_wind_dir>=345||data_wind_dir<=22 )windDegString="\211";    //"Північний";
  if(data_wind_dir>=23 &&data_wind_dir<=68 )windDegString="\234";    //"Північно-східний";
  if(data_wind_dir>=69 &&data_wind_dir<=114)windDegString="\230";    //"Східний";
  if(data_wind_dir>=115&&data_wind_dir<=160)windDegString="\235";    //"Південно-східний";
  if(data_wind_dir>=161&&data_wind_dir<=206)windDegString="\210";    //"Південний";
  if(data_wind_dir>=207&&data_wind_dir<=252)windDegString="\232";    //"Південно-західний";
  if(data_wind_dir>=253&&data_wind_dir<=298)windDegString="\231";    //"Західний";
  if(data_wind_dir>=299&&data_wind_dir<=344)windDegString="\233";    //"Північно-західний";
  weatherString="  ";
  if(weaths.displayCityName){
    String PCN=weaths.personalCityName;
    if(PCN.length() > 0) weatherString += PCN;
    else weatherString += String(location_name);
    weatherString += ", ";
  }
  if(weaths.displayForecastNow){
    weatherString += tNow+":    \212 "+String(location_temp, 1)+" ("+String(location_app_temp,1)+")"+("\202")+"C";
    weatherString += "     \213 "+String(location_rh)+"%"; 
    weatherString += "     \215 "+String((location_pres), 0)+(senss.pressSys == 1 ? tPress : tPress0);
    if(weaths.displayForecastWind==1)weatherString += "     \214 "+String(data_wind_cdir)+ ", " +String(location_wind_spd, 1)+tSpeed;
    else weatherString += "     \214 "+windDegString+String(location_wind_spd, 1)+tSpeed;
    weatherString += "     \216 "+String(location_clouds)+"%     "+data_weather_description+space;   
  }
  updateForecast=0;
  updateForecastNot=false;
  if(senss.printCom){
    printTime();
    Serial.println("line ="+line);
    Serial.println("======== END ==========================================");
  }
}
//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ  openweathermap.org                                                     //
//===============================================================================================================================//
void getWeatherData1(){
  if(weaths.weatherKey1==""||!weaths.weathOn) return;
  if(!WIFI_connected){
    updateForecast++;
    return;
  }
  if(senss.printCom){                                                            //
    Serial.println("======== START GET WEATHER FROM OPENWEATHERMAP.ORG ====");
    printTime();
  } 
  location_name="";
  location_region="";
  location_country="";
  location_localtime="";
  location_temp=0;
  location_app_temp=0;
  location_rh=0;
  location_pres=0;
  location_wind_spd=0;
  location_wind_cdir_full="";
  location_sunrise="";
  location_sunset="";
  location_clouds=0;
  location_vis=0;
  location_uv=0;
  location_wds="";
  if(ESPclient.connect(weatherHost1.c_str(), 80)){}
  else{
    if(senss.printCom){
      Serial.println(" Not connection server!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost1+"/data/2.5/weather?id="+urlencode(weaths.cityID1)+"&lang="+senss.messLang+"&units=metric&appid="+weaths.weatherKey1;
  if(senss.printCom) Serial.print(reqline); 
  if(http.begin(ESPclient, reqline)){
    int httpCode=http.GET();
    if(httpCode > 0){
      if(senss.printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
       line=http.getString();
      }
    } else {
      if(senss.printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      updateForecast++;
      return;
    }
    http.end();
  } else {
    if(senss.printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  if(line==""){
    if(senss.printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  const size_t capacity=JSON_ARRAY_SIZE(1)+JSON_OBJECT_SIZE(1)+2*JSON_OBJECT_SIZE(2)+JSON_OBJECT_SIZE(4)+2*JSON_OBJECT_SIZE(5)+JSON_OBJECT_SIZE(13)+751; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(senss.printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  JsonObject weather_0=doc["weather"][0];
  const char*   data_weather_description=weather_0["description"]; // "fog"
  location_wds=data_weather_description;
  JsonObject main=doc["main"];
  location_temp=main["temp"]; // 10.34
  location_pres=main["pressure"]; // 1023
  if(senss.pressSys==1) location_pres /= 1.3332239;
  location_rh=main["humidity"]; // 100
  float location_temp_min=main["temp_min"]; // 7
  float location_temp_max=main["temp_max"]; // 12.22
  location_vis=doc["visibility"]; // 1000
  location_vis /= 1000;
  location_wind_spd=doc["wind"]["speed"]; // 1
  int data_wind_dir=doc["wind"]["deg"]; // 230
  location_clouds=doc["clouds"]["all"]; // 20
  JsonObject sys=doc["sys"];
  const char* data_country_code=sys["country"]; // "UA"
  location_country=data_country_code;
  const char*   data_city_name=doc["name"]; // "Kyiv"
  location_name=data_city_name;
  String windDegString;
  if(data_wind_dir>=345||data_wind_dir<=22 ){windDegString=weaths.displayForecastWind==0?"\211":"С";}//"Північний";
  if(data_wind_dir>=23 &&data_wind_dir<=68) {windDegString=weaths.displayForecastWind==0?"\234":"СВ";}//"Північно-східний";
  if(data_wind_dir>=69 &&data_wind_dir<=114){windDegString=weaths.displayForecastWind==0?"\230":"В";}//"Східний";
  if(data_wind_dir>=115&&data_wind_dir<=160){windDegString=weaths.displayForecastWind==0?"\235":"ЮВ";}//"Південно-східний";
  if(data_wind_dir>=161&&data_wind_dir<=206){windDegString=weaths.displayForecastWind==0?"\210":"Ю";}//"Південний";
  if(data_wind_dir>=207&&data_wind_dir<=252){windDegString=weaths.displayForecastWind==0?"\232":"ЮЗ";}//"Південно-західний";
  if(data_wind_dir>=253&&data_wind_dir<=298){windDegString=weaths.displayForecastWind==0?"\231":"З";}//"Західний";
  if(data_wind_dir>=299&&data_wind_dir<=344){windDegString=weaths.displayForecastWind==0?"\233":"СЗ";}//"Північно-західний";
  weatherString="  ";
  if(weaths.displayCityName){
    String PCN=weaths.personalCityName;
    if(PCN.length() > 0) weatherString += PCN;
    else weatherString += String(location_name);
    weatherString += ", ";
  }
  if(weaths.displayForecastNow){
    weatherString += tNow+":    \212 "+String(location_temp, 1)+" ("+String(location_app_temp,1)+")"+("\202")+"C";
    weatherString += "     \213 "+String(location_rh)+"%"; 
    weatherString += "     \215 "+String((location_pres), 0)+(senss.pressSys == 1 ? tPress : tPress0);
    weatherString += "     \214 "+windDegString+(weaths.displayForecastWind==1?", ":"")+String(location_wind_spd,1)+tSpeed;
    weatherString += "     \216 "+String(location_clouds)+"%     "+data_weather_description+space;   
  }
  updateForecast=0;
  updateForecastNot=false;
  if(senss.printCom){
    printTime();
    Serial.println("line ="+line);
    Serial.println("======== END ==========================================");
  }
}
// ============================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту https://www.weatherbit.io     // 
// ============================================================================//
void getWeatherDataz0(){
  if(weaths.weatherKey0==""||!weaths.displayForecastTomorrow) return;
  if(!WIFI_connected){
    updateForecasttomorrow++;
    return;
  }
  if(senss.printCom){
    Serial.println("======== START GET FORECAST FROM WEATHERBIT.IO ========");
    printTime();
  } 
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost0+"/v2.0/forecast/daily?city="+urlencode(weaths.cityID0)+"&lang="+senss.messLang+"&days=2&key="+weaths.weatherKey0;
  if(senss.printCom) Serial.print(reqline);   
  if(http.begin(ESPclient, reqline)){
   int httpCode=http.GET();
   if(httpCode > 0){
     if(senss.printCom) Serial.printf("  [HTTP] GET... code: %d\n", httpCode);
     if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
      line=http.getString();
     }
   } else {
     if(senss.printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
     http.end();
     updateForecasttomorrow++;
     return;
   }
   http.end();
  } else {
    if(senss.printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  if(line==""){
    if(senss.printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  const size_t capacity=JSON_ARRAY_SIZE(2)+2*JSON_OBJECT_SIZE(3)+JSON_OBJECT_SIZE(7)+2*JSON_OBJECT_SIZE(37)+2321;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(senss.printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  JsonObject data_0=doc["data"][0];
  JsonObject data_0_weather=data_0["weather"];
  const char* data_0_weather_description=data_0_weather["description"]; // "Помірний дощ"
  float data_0_max_temp=data_0["max_temp"]; // 13.4
  float data_0_min_temp=data_0["min_temp"]; // 10.9
  JsonObject data_1=doc["data"][1];
  int data_1_rh=data_1["rh"]; // 75
  int data_1_clouds=data_1["clouds"]; // 58
  float data_1_wind_spd=data_1["wind_spd"]; // 3.75302
  JsonObject data_1_weather=data_1["weather"];
  const char* data_1_weather_description=data_1_weather["description"]; // "Світло душ дощ"
  float data_1_max_temp=data_1["max_temp"]; // 16.3
  float data_1_min_temp=data_1["min_temp"]; // 10
  weatherStringZ="";
  if(weaths.displayForecastToday){
    if(hour<18) weatherStringZ += tCurr+":";
    if(hour<12) weatherStringZ += "   \212"+String(data_0_min_temp, 1)+"...."+String(data_0_max_temp, 1)+"\202"+"C  ";
    if(hour<18) weatherStringZ += "   "+String(data_0_weather_description)+"     ";
  }
  if(weaths.displayForecastTomorrow){
    weatherStringZ += tTom+":   \212"+String(data_1_min_temp, 1)+"...."+String(data_1_max_temp, 1)+"\202"+"C";
    weatherStringZ += "     \213 "+String(data_1_rh)+"%";
    weatherStringZ += "     \214 "+String(data_1_wind_spd, 1)+tSpeed;
    weatherStringZ += "     "+String(data_1_weather_description);
    weatherStringZ += space;
  }
  if(senss.printCom) Serial.println("          Getting weather forecast for tomorrow - is OK.");
  updateForecasttomorrow=0;
  updateForecastNot=false;
  if(senss.printCom){
    printTime();
    Serial.println("line ="+line);
    Serial.println("======== END ==========================================");
  }
}
// =======================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту openweathermap.org      //
// =======================================================================//
void getWeatherDataz1(){
  if(weaths.weatherKey1==""||!weaths.displayForecastTomorrow) return;
  if(!WIFI_connected){
    updateForecasttomorrow++;
    return;
  }
  if(senss.printCom){
    Serial.println("======== START GET FORECAST FROM OPENWEATHERMAP.ORG ===");
    printTime();
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost1+"/data/2.5/forecast/daily?id="+urlencode(weaths.cityID1)+"&units=metric&appid="+weaths.weatherKey1+"&lang="+senss.messLang+"&cnt=2";
  if(senss.printCom) Serial.print(reqline);   
  if(http.begin(ESPclient, reqline)){
   int httpCode=http.GET();
   if (httpCode > 0){
     if(senss.printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
     if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
      line=http.getString();
     }
   } else {
     if(senss.printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
     http.end();
     updateForecasttomorrow++;
     return;
   }
   http.end();
  } else {
    if(senss.printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  if(line==""){
    if(senss.printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  const size_t capacity=2*JSON_ARRAY_SIZE(1)+JSON_ARRAY_SIZE(2)+JSON_OBJECT_SIZE(2)+2*JSON_OBJECT_SIZE(4)+JSON_OBJECT_SIZE(5)+3*JSON_OBJECT_SIZE(6)+2*JSON_OBJECT_SIZE(10)+1281;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(senss.printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  JsonObject city=doc["city"];
  JsonObject list_0=doc["list"][0];
  JsonObject list_0_temp=list_0["temp"];
  float data_0_min_temp=list_0_temp["min"]; // 9.63
  float data_0_max_temp=list_0_temp["max"]; // 9.69
  JsonObject list_0_weather_0=list_0["weather"][0];
  const char* data_0_weather_description=list_0_weather_0["description"]; // "облачно"
  JsonObject list_1=doc["list"][1];
  JsonObject list_1_temp=list_1["temp"];
  float data_1_day_temp=list_1_temp["day"]; // 18.13
  float data_1_min_temp=list_1_temp["min"]; // 10.67
  float data_1_max_temp=list_1_temp["max"]; // 20.32
  int data_1_rh=list_1["humidity"]; // 56
  JsonObject list_1_weather_0=list_1["weather"][0];
  const char* data_1_weather_description=list_1_weather_0["description"]; // "слегка облачно"
  float data_1_wind_spd=list_1["speed"]; // 0.7
  int data_1_clouds=list_1["clouds"]; // 38
  weatherStringZ="";
  if(weaths.displayForecastToday){
    if(hour<18) weatherStringZ += tCurr+":";
    if(hour<12) weatherStringZ += "   \212"+String(data_0_min_temp, 1)+"...."+String(data_0_max_temp, 1)+"\202"+"C  ";
    if(hour<18) weatherStringZ += "   "+String(data_0_weather_description)+"     ";
  }
  if(weaths.displayForecastTomorrow){
    weatherStringZ += tTom+":   \212"+String(data_1_min_temp, 1)+"...."+String(data_1_day_temp, 1)+"...."+String(data_1_max_temp, 1)+"\202"+"C";
    weatherStringZ += "     \213 "+String(data_1_rh)+"%";
    weatherStringZ += "     \214 "+String(data_1_wind_spd, 1)+tSpeed;
    weatherStringZ += "     \216 "+String(data_1_clouds)+"%     " +String(data_1_weather_description);
    weatherStringZ += space;
  }
  updateForecasttomorrow=0;
  updateForecastNot=false;
  if(senss.printCom){
    printTime();
    Serial.println("line ="+line);
    Serial.println("======== END ==========================================");
  }
}
//=========================================================================================================
void printForecast(){
  if(weaths.weathOn&&butMode==0&&(weaths.timeStartViewWeather<weaths.timeEndViewWeather?(hour>=weaths.timeStartViewWeather&&hour<weaths.timeEndViewWeather):!(hour<weaths.timeStartViewWeather&&hour>=weaths.timeEndViewWeather))){
    clr(leds.NUM_VER_X);
    if(updateForecastNot) printStringWithShift(("  "+tWeatrTN+space).c_str(),leds.timeScrollSpeed);// выводится до первого обновления погоды
    else{
      if(updateForecast>30&&updateForecast<360) weatherString="  "+tWeatrTN+" - "+String(updateForecast)+"мин."+space;
      else if(updateForecast>=360) weatherString=tWeatrNot;
      String mess=weatherString;
      if(updateForecasttomorrow<30){
        mess+=weatherStringZ;
      }
      printStringWithShift(mess.c_str(),leds.timeScrollSpeed);
    }
  }
}
//=========================================================================================================
//                                  narodmon.ru
void narodmonSend(){
  if(!WIFI_connected||!senss.nmonOn) return;
  byte a=0;
  for(byte i=0;i<8;i++){
    if(nmons.writeNmon[i]!=0) a++;
  }
  if(a==0) return;
  String buf="#VZ"+macNmon+"#VZ"+nmons.nameNmon;
  buf+="#"+nmons.latNmon;
  buf+="#"+nmons.lonNmon; 
  for(int i=0;i<8;i++){
    if(nmons.writeNmon[i]){
      buf+="\n#"+macNmon+String(i)+"#";
      buf+=String(data[nmons.writeNmon[i]-1]); 
    }
  }
  buf+="\n##\r\n";
  if(!ESPclient.connect("narodmon.ru",8283)){
    if(senss.printCom)Serial.println("Not connected to narodmon.ru");
    return;
  } else{
    ESPclient.print(buf);
    if(senss.printCom)Serial.println("Connected narodmon.ru is OK!");
  }
  if(senss.printCom)Serial.println(buf);
  for(uint8_t i=0;i<50;i++){
    if(ESPclient.available())return;
    delay(100);
  }
}
//================================================================================================
//                                  narodmon.ru
void narodmonGet(){
  if(!WIFI_connected||!senss.nmonOn||nmons.uuid==""||nmons.api_key=="") return;
  byte a=0;
  for(byte i=0;i<8;i++){
    if(nmons.sensors_ID[i]!=0) a++;
  }
  if(a==0) return;
  if(senss.printCom){
    Serial.println("======== START NARODMON=================================");
    printTime();
    Serial.print("GET  ");
  }
  String line="";
  String reqline="http://narodmon.ru/api/sensorsValues?sensors=";
  for(byte i=0;i<8;i++){
    if(nmons.sensors_ID[i]){
      reqline+=String(nmons.sensors_ID[i]);
      if(a>1){
        reqline+=",";
        a--;
      }
    }
  }
  reqline+="&uuid="+nmons.uuid+"&api_key="+nmons.api_key;
  HTTPClient http;
  if(senss.printCom) Serial.print(reqline);
  if(http.begin(ESPclient, reqline)){
    int httpCode=http.GET();
    if(senss.printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
    if(httpCode>0){
      if(httpCode==HTTP_CODE_OK||httpCode==HTTP_CODE_MOVED_PERMANENTLY){
        line=http.getString();
      }
    } else {
      if(senss.printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      return;
    }
    http.end();
  } else {
    if(senss.printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  if(line==""){
    if(senss.printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  Serial.println("line="+line);
  //const size_t capacity=JSON_ARRAY_SIZE(2)+JSON_OBJECT_SIZE(1)+2*JSON_OBJECT_SIZE(6)+340; //https://arduinojson.org/v6/assistant/
  //DynamicJsonDocument doc(capacity);
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(senss.printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  long sensors_time[8];
  float value[8];
  int sensors_id[8];
  JsonObject sensors_0=doc["sensors"][0];
  sensors_id[0]=sensors_0["id"];
  value[0]=sensors_0["value"]; // 14.2
  sensors_time[0]=sensors_0["time"]; // 1571853360
  JsonObject sensors_1=doc["sensors"][1];
  sensors_id[1]=sensors_1["id"];
  value[1]=sensors_1["value"]; // 14
  sensors_time[1]=sensors_1["time"]; // 1571853000
  JsonObject sensors_2=doc["sensors"][2];
  sensors_id[2]=sensors_2["id"];
  value[2]=sensors_2["value"];
  sensors_time[2]=sensors_2["time"];
  JsonObject sensors_3=doc["sensors"][3];
  sensors_id[3]=sensors_3["id"];
  value[3]=sensors_3["value"];
  sensors_time[3]=sensors_3["time"];
  JsonObject sensors_4=doc["sensors"][4];
  sensors_id[4]=sensors_4["id"];
  value[4]=sensors_4["value"];
  sensors_time[4]=sensors_4["time"];
  JsonObject sensors_5=doc["sensors"][5];
  sensors_id[5]=sensors_5["id"];
  value[5]=sensors_5["value"];
  sensors_time[5]=sensors_5["time"];
  JsonObject sensors_6=doc["sensors"][6];
  sensors_id[6]=sensors_6["id"];
  value[6]=sensors_6["value"];
  sensors_time[6]=sensors_6["time"];
  JsonObject sensors_7=doc["sensors"][7];
  sensors_id[7]=sensors_7["id"];
  value[7]=sensors_7["value"];
  sensors_time[7]=sensors_7["time"];
  
  long timestamp=epochNM + (millis()/1000);

  for(byte j=0;j<8;j++){
    for(int i=0;i<8;i++){
      if(nmons.sensors_ID[j] && sensors_id[i]==nmons.sensors_ID[j]){
        if((timestamp-sensors_time[i])>3600){
          nMon[j]=0;
        } else nMon[j]=value[i];
        printTime();
        Serial.println("nMon"+String(j+1)+"="+String(nMon[j]));
      }
    }
  }
  if(senss.printCom) {
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}
//=========================================================================================================
void wifiConnect(){
  if(senss.printCom){
    printTime();
    Serial.println("Connecting WiFi...");
  }
  //WiFi.disconnect();
  if(!ssids.num){
    if(senss.printCom){
      printTime();
      Serial.println("No list of networks!!!");
      Serial.println("          This is your first launch");
      Serial.println("          Connect to the Wi-Fi network: "+String(indexs.ssidAP)+"   password:"+String(indexs.passAP));
      Serial.println("          Address Web page: 192.168.4.1");
      WiFi.mode(WIFI_AP);
      //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(indexs.ssidAP,indexs.passAP);
      firstStart=1;
      apStart=1;
      //updateLocalTime();
      String aaa=tPoint+" "+indexs.ssidAP;
      if(indexs.passAP!="") aaa+=",  "+tPass+": "+indexs.passAP;
      aaa+=" "+tIp+": 192.168.4.1           ";
     clr(leds.NUM_VER_X);
     printStringWithShift(aaa.c_str(),35);
     return;
    }
  } else{                                          // есть спиок сетей
    WiFi.mode(WIFI_STA);
    if(WiFi.status()!=WL_CONNECTED){
      uint8_t n=WiFi.scanNetworks();
      if(n!=0||indexs.hiddenNetworks){                                       // есть доступные сети или слепой поиск
        Serial.println("Search ssids="+String(n)+"   Hidden Networks="+String(indexs.hiddenNetworks));
        //viewRSSI(String(WiFi.SSID()));
        if(indexs.hiddenNetworks)n=1;
        for(uint8_t i=0;i<n;i++){                     // цикл по количеству доступных сетей
          for(uint8_t k=0;k<ssids.num;k++){           // цикл по количеству cохраненных сетей
            delay(1);
            if(WiFi.SSID(i)==ssids.ssid[k]||indexs.hiddenNetworks){          // найдена доступная сеть из списка
              //rssi=WiFi.RSSI(i);
              ssids.ssid[k].toCharArray(ssid,(ssids.ssid[k].length())+1);
              ssids.pass[k].toCharArray(password,(ssids.pass[k].length())+1);
              if(senss.printCom) Serial.print("Connecting to : "+String(ssid)+"("+String(password)+")");
              //WiFi.disconnect();
              WiFi.begin(ssid,password);
              byte oldBigClock=bigClock;
              for(uint8_t a=0;a<20;a++){          
                if(WiFi.status()==WL_CONNECTED){
                  if(!amountNotStarts) bigClock=oldBigClock;
                  goto connectedd;
                }
                if(senss.printCom) Serial.print(".");
                if(!amountNotStarts){
                  bigClock=false;
                  clr(leds.NUM_VER_X);
                  dy=-1;
                  showDigit(87, 0, fontUA_RU_PL_DE);
                  showDigit(73, 6, fontUA_RU_PL_DE);
                  showDigit(70, 10, fontUA_RU_PL_DE);
                  showDigit(73, 15, fontUA_RU_PL_DE);
                  dy=0;
                  if(a>9) showDigit(a/10, 21, dig5x8);
                  showDigit(a%10,27,dig5x8);
                  //scrollDown(1);
                  refreshAll();
                }
                delay(500);
              }
              if(!amountNotStarts) bigClock=oldBigClock;
              if(senss.printCom) Serial.println();
            }
          }
        }
        amountNotStarts++;
        if(senss.printCom){
          Serial.print("Amount of the unsuccessful connecting=");
          Serial.println(amountNotStarts);
        }
        if(amountNotStarts>21){
          amountNotStarts=0;
          //ESP.reset();
        }
        if(!firstStart){
          firstStart=1;
          apStart=1;
          WiFi.mode(WIFI_AP);
          //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
          WiFi.softAP(indexs.ssidAP,indexs.passAP);
          if(senss.printCom){
            printTime();
            Serial.println("Start AP mode!!!");
            Serial.print("          Wifi AP IP : ");
            Serial.println(WiFi.softAPIP());
          }
          String aaa=tPoint+" "+indexs.ssidAP;
          if(indexs.passAP!="") aaa+=",  "+tPass+": "+indexs.passAP;
          aaa+=" "+tIp+": 192.168.4.1           ";
          clr(leds.NUM_VER_X);
          printStringWithShift(aaa.c_str(),35);
        }
        return;
      }
    }
    connectedd:
    if(senss.printCom) Serial.println();
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.SSID().toCharArray(ssid,(WiFi.SSID().length())+1);
    if(senss.printCom) Serial.print("\r\nHuraaa!!! Connected to \""); Serial.print(ssid); Serial.println("\"("+WiFi.localIP().toString()+")\n");
    WIFI_connected=true;
    if(!amountNotStarts){
      String aaa=WiFi.localIP().toString()+space;
      if(leds.NUM_MAX0>4){
        for(byte j=0;j<leds.NUM_MAX0-4;j++) aaa+="  ";
      }
      clr(leds.NUM_VER_X);
      printStringWithShift(" IP: ",15);
      printStringWithShift(aaa.c_str(),25);
    }
    timeUpdateNTP();
    amountNotStarts=0;
    firstStart=1;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sensorsAll(){
  if(senss.printCom) Serial.println("======== START GET SENSORS DATA =======================");
  sensorsDs18b20();
  sensorsSi7021();
  sensorsAHTx0();
  sensorsBmp();
  sensorsBme();
  //sensorsDht();
  sensorses();
  if(senss.printCom) Serial.println("======== END ==========================================");
}
//-------------------------------------------------------------------------
void sensorses(){
  for(byte i=0;i<8;i++){
    data[i]=0;
    if(senss.sensors[i]==1 && senss.params[i]<10) data[i]=tempDs18b20;
    else if(senss.sensors[i]==2 && senss.params[i]<10)data[i]=tempSi7021;
    else if(senss.sensors[i]==2 &&senss.params[i]<20)data[i]=humiSi7021;
    else if(senss.sensors[i]==3 && senss.params[i]<10)data[i]=tempBmp;
    else if(senss.sensors[i]==3 && senss.params[i]==20)data[i]=pressBmp;
    else if(senss.sensors[i]==4 && senss.params[i]<10)data[i]=tempBme;
    else if(senss.sensors[i]==4 &&senss.params[i]<20)data[i]=humiBme;
    else if(senss.sensors[i]==4 && senss.params[i]==20)data[i]=pressBme;
    //else if(senss.sensors[i]==5 && senss.params[i]<10)data[i]=tempDht;
    //else if(senss.sensors[i]==5 &&senss.params[i]<20)data[i]=humiDht;
    else if(senss.sensors[i]==10 && senss.params[i]<10)data[i]=tempAHTx0;
    else if(senss.sensors[i]==10 && senss.params[i]<20)data[i]=humiAHTx0;
    else if(senss.sensors[i]>10 && senss.sensors[i]<20)data[i]=tMqtt[senss.sensors[i]-11];
    else if(senss.sensors[i]>20 && senss.sensors[i]<30)data[i]=readDataField[senss.sensors[i]-21];
    else if(senss.sensors[i]>30 && senss.sensors[i]<40)data[i]=nMon[senss.sensors[i]-31];
    if(senss.sensors[i]){
      data[i] += senss.corr[i];
    }
    if(senss.printCom && senss.sensors[i]){
      printTime();
      Serial.println("senss.sensors["+String(i)+"](+corr)=" + String(data[i]));
    }
  }
  if(sgps.setSgpCorr){
    if(sgps.setSgpCorr==1 && bmp280Found){
      sgps.sgpCorrTemp=tempBme;
      sgps.sgpCorrHumi=humiBme;
    }
    if(sgps.setSgpCorr==2 && si7021Found){
      sgps.sgpCorrTemp=tempSi7021;
      sgps.sgpCorrHumi=humiSi7021;
    }
    if(sgps.setSgpCorr==3 && AHTx0Found){
      sgps.sgpCorrTemp=tempAHTx0;
      sgps.sgpCorrHumi=humiAHTx0;
    }
    //if(sgps.setSgpCorr==4 && dhtFound){
    //  sgps.sgpCorrTemp=tempDht;
    //  sgps.sgpCorrHumi=humiDht;
    //}
  }
  if((senss.sensors[0]==4&&senss.sensors[2]==4&&bmp280Found)||(senss.sensors[0]==2&&senss.sensors[2]==2&&si7021Found)||(senss.sensors[0]==10&&senss.sensors[2]==10&&AHTx0Found)){//||(senss.sensors[0]==5&&senss.sensors[2]==5&&dhtFound)
    if(senss.params[4]==21&&senss.sensors[4]==99){
      float absH=(6.112*(pow(2.718281828,(17.67*data[0])/(data[0]+243.5)))*data[2]*2.1674)/(273.15+data[0]);
      if(senss.printCom){
        printTime();
        Serial.println("Absolute Humi=" + String(absH) + "g/m3");
      }
      data[4]=absH;
    }
    if(senss.params[3]==3&&senss.sensors[3]==99){
      float ans=(data[0]-(14.55+0.114*data[0])*(1-(0.01*data[2]))-pow(((2.5+0.007*data[0])*(1-(0.01*data[2]))),3)-(15.9+0.117*data[0])*pow((1-(0.01*data[2])),14));
      if(senss.printCom){
        printTime();
        Serial.println("Dew point=" + String(ans) + "*C");
      }
      data[3]=ans;
    }
  }
}
//--------------------------------------------------------------------------
void sensorsDs18b20(){  //1
  byte present=0;
  byte type_s;
  byte data[12];
  byte addr[8];
  if(!ds.search(addr)) {                                // Стартуємо функцію пошук першого або наступного датчика в addr-возвращаем ответ датчика
    ds.reset_search();                                  // Якщо добігли кінця адресного простору, то скидуємо адрес на початок простору
    delay(250);                                         // Пауза
    ds18b20Found=false;
    return;                                             // Виходимо з підпрограми
  }
  ds18b20Found=true;
  if(OneWire::crc8(addr, 7) != addr[7]) return;           // перевіряємо 7 байт в addr - он содержит crc8 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(750);
  present=ds.reset();
  ds.select(addr);    
  ds.write(0xBE);
  for(byte i=0; i < 9; i++) {
    data[i]=ds.read();
  }
  int16_t raw=(data[1] << 8) | data[0];
  if(type_s) {
    raw=raw << 3;
    if(data[7]==0x10) {
      raw=(raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg=(data[4] & 0x60);
    if(cfg==0x00) raw=raw & ~7;
    else if(cfg==0x20) raw=raw & ~3;
    else if (cfg==0x40) raw=raw & ~1;
  }
  tempDs18b20=(float)raw / 16.00;
  if(senss.printCom) {
    printTime();
    Serial.println("Temperature DS18B20: " + String(tempDs18b20) + " *C");
  }
}
//--------------------------------------------------------------------------
void sensorsSi7021() {  //2
  if(si7021Found==false) return;
  humiSi7021=sensor.readHumidity();
  tempSi7021=sensor.readTemperature();
  if(senss.printCom) {
    printTime();
    Serial.println("Temperature Si7021: " + String(tempSi7021) + " *C,  Humidity: " + String(humiSi7021) + " %");
  }
}
//--------------------------------------------------------------------------
void sensorsAHTx0() {  //2
  if(AHTx0Found==false) return;
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  humiAHTx0=humidity.relative_humidity;
  tempAHTx0=temp.temperature;
  if(senss.printCom) {
    printTime();
    Serial.println("Temperature AHTx0: " + String(tempAHTx0) + " *C,  Humidity: " + String(humiAHTx0) + " %");
  }
}
//--------------------------------------------------------------------------
void sensorsBmp() {  //3
  if(bmp280Found==true) {
    tempBmp=bmp.readTemperature();
    pressBmp=verific_press(bmp.readPressure());
    pressBmp=(int) pressBmp;
    altBmp=bmp.readAltitude(1013.25);
    if(senss.printCom) {
      printTime();
      Serial.println("Temperature BMP280: " + String(tempBmp) + " *C,  Pressure: " + String(pressBmp) + (senss.pressSys==1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBmp) + " m");
    }
  }
  if(bmp180Found==true) {
    tempBmp=bmp180.readTemperature();
    pressBmp=verific_press(bmp180.readPressure());
    pressBmp=(int) pressBmp;
    altBmp=bmp180.readAltitude(101500);
    if(senss.printCom) {
      printTime();
      Serial.println("Temperature BMP180: " + String(tempBmp) + " *C,  Pressure: " + String(pressBmp) + (senss.pressSys==1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBmp) + " m");
    }
  }
}
//--------------------------------------------------------------------------
void sensorsBme() {  //4
  if(bme280Found==false) return;
  tempBme=bme.readTempC();          //bme.readTempF()
  humiBme=bme.readHumidity();
  pressBme=verific_press(bme.readPressure());
  pressBme=(int) pressBme;
  altBme=bme.readAltitudeFeet();   //bme.readAltitudeMeter()  bme.readAltitudeFeet()
  if(senss.printCom) {
    printTime();
    Serial.println("Temperature BME280: " + String(tempBme) + " *C,  Humidity: " + String(humiBme) + " %,  Pressure: " + String(int(pressBme)) + (senss.pressSys==1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBme) + " m");
  }
}
//--------------------------------------------------------------------------
/*void sensorsDht() {   //5
  dhtFound=false;
  int err=SimpleDHTErrSuccess;
  int err2=SimpleDHTErrSuccess;
  byte temp11=0;
  byte humi11=0;
  if((err2=dht22.read2(pinDHT, &tempDht, &humiDht, NULL))==SimpleDHTErrSuccess) {
    humiDht=int(humiDht);
    if(senss.printCom) {
      printTime();
      Serial.println("Temperature DHT22: " + String(tempDht) + " *C,  Humidity: " + String(humiDht) + " %");
    }
    dhtFound=true;
  }
  if((err=dht11.read(pinDHT, &temp11, &humi11, NULL))==SimpleDHTErrSuccess) {
    tempDht=temp11;
    humiDht=humi11;
    if(senss.printCom) {
      printTime();
      Serial.println("Temperature DHT11: " + String(tempDht) + " *C,  Humidity: " + String(humiDht) + " %");
    }
    dhtFound=true;
  }
}*/
//----------------------------------------------------------------------------------------------------------------------------------
uint32_t getAbsoluteHumidity(float temperature, float humidity){
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity=216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f+temperature)) / (273.15f+temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled=static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}
//----------------------------------------------------------------------------------------------------------------------------------
void sgp30(){
  if(!senss.sgpOn) return;
  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  if(sgps.setSgpCorr)sgp.setHumidity(getAbsoluteHumidity(sgps.sgpCorrTemp,sgps.sgpCorrHumi));
  printTime();
  if(!sgp.IAQmeasure()){
    if(senss.printCom)Serial.println("Measurement failed");
    return;
  }
  sgpCo2=sgp.eCO2;
  sgpTvoc=sgp.TVOC;
  if(senss.printCom)Serial.print("TVOC="+String(sgpTvoc)+" ppb,     ");
  if(senss.printCom)Serial.print("eCO2="+String(sgpCo2)+" ppm,     ");
  if(sgpCo2<sgpCo2Livels[0]) sgpCo2Livel=0;
  else if(sgpCo2<sgpCo2Livels[1])sgpCo2Livel=1;
  else if(sgpCo2<sgpCo2Livels[2])sgpCo2Livel=2;
  else if(sgpCo2<sgpCo2Livels[3])sgpCo2Livel=3;
  else sgpCo2Livel=4;
  if(sgpTvoc<sgpTvocLivels[0])sgpTvocLivel=0;
  else if(sgpTvoc<sgpTvocLivels[1])sgpTvocLivel=1;
  else if(sgpTvoc<sgpTvocLivels[2])sgpTvocLivel=2;
  else if(sgpTvoc<sgpTvocLivels[3])sgpTvocLivel=3;
  else sgpTvocLivel=4;
  if(!sgp.IAQmeasureRaw()){
    if(senss.printCom)Serial.println("Raw Measurement failed");
  }else{
    if(senss.printCom)Serial.print("Raw H2="+String(sgp.rawH2)+",     ");
    if(senss.printCom)Serial.println("Raw Ethanol="+String(sgp.rawEthanol));
    if (minute%5==0){
      uint16_t TVOC_base,eCO2_base;
      if(!sgp.getIAQBaseline(&eCO2_base,&TVOC_base)){
        if(senss.printCom){
          printTime();
          Serial.println("Failed to get baseline readings");
        }
        return;
      }
      if(senss.printCom){
        printTime();
        Serial.print("****Baseline values: eCO2: 0x"+String(eCO2_base,HEX));
        Serial.println(" & TVOC: 0x"+String(TVOC_base,HEX));
      }
    }
  }
  String livelCo2=tAlarm+sgpCo2Message[sgpCo2Livel]+" eCO2="+String(sgpCo2)+" ppm               ";
  if(sgpCo2Livel>=sgps.sgpCo2LivelAlarm){
    if(sgps.eCo2AlarmMqtt&&WIFI_connected) MQTTclient.publish(mqtts.mqtt_sub_inform,livelCo2.c_str());
    if(sgps.eCo2AlarmEsp){
      for(byte i=0;i<4;i++){bip();}
      clr(leds.NUM_VER_X);
      printStringWithShift(("        " + livelCo2).c_str(),leds.timeScrollSpeed);
    }
  }
  String livelTvoc=tAlarm+sgpTvocMessage[sgpTvocLivel]+" TVOC="+String(sgpTvoc)+" ppb               ";
  if(sgpTvocLivel>=sgps.sgpTvocLivelAlarm){
    if(sgps.tvocAlarmMqtt&&WIFI_connected)MQTTclient.publish(mqtts.mqtt_sub_inform,livelTvoc.c_str());
    if(sgps.tvocAlarmEsp){
      for(byte i=0;i<4;i++){bip();}
      clr(leds.NUM_VER_X);
      printStringWithShift(("        "+livelTvoc).c_str(),leds.timeScrollSpeed);
    }
  }
}
//--------------------------------------------------------------------------
void printTime(){
  if(senss.printCom)Serial.print((hour<10?"0":"")+String(hour)+":"+(minute<10?"0":"")+String(minute)+":"+(second<10?"0":"")+String(second)+"  ");
}
//--------------------------------------------------------------------------
void bip(){
  if(!senss.buzzerSet) return;
  if(senss.buzzerSet==2){
    tone(buzzerPin, 2000, 40);
    delay(250);
    noTone(buzzerPin);
  } else {
    digitalWrite(buzzerPin, HIGH);
    delay(120);
    digitalWrite(buzzerPin, LOW);
    delay(120);
  }
}
//--------------------------------------------------------------------------
byte alarms(){
  for(byte i=0;i<5;i++){
    if(times.alarme[i][0]==hour&&times.alarme[i][1]==minute&&(times.alarme[i][2]==dayOfWeek||(times.alarme[i][2]==8&&(dayOfWeek>1&&dayOfWeek<7))||(times.alarme[i][2]==9&&dayOfWeek>1)||(times.alarme[i][2]==10&&(dayOfWeek==1||dayOfWeek==7))||times.alarme[i][2]>10)){
      alarm_numer=i;
      return 1;
    }
  }
  alarm_numer=255;
  return 0;
}
//------------ function urlencode for weather parameters --------------------
String urlencode(String str){    // функция взята от http://forum.amperka.ru/members/benben.19545/
  String encodedString="";
  char c;
  char code0;
  char code1;
  for(int i=0;i<str.length();i++){
    c=str.charAt(i);
    if(c==' '){
      encodedString += '+';
    }else if(isalnum(c)){
      encodedString+=c;
    }else{
      code1=(c&0xf)+'0';
      if((c&0xf)>9){
        code1=(c&0xf)-10+'A';
      }
      c=(c>>4)&0xf;
      code0=c+'0';
      if(c>9){
        code0=c-10+'A';
      }
      encodedString+='%';
      encodedString+=code0;
      encodedString+=code1;
    }
    yield();
  }
  return encodedString;
}
//------------ function chr_to_str --------------------
String chr_to_str(String str){
  String chr_to_str="";
  for (int i=0;i<str.length(); i++){
    chr_to_str+=str.charAt(i);
  }
  return chr_to_str;
}
// ===========================КОНВЕРТАЦІЯ НАЗВ ДНІВ ТИЖНЯ НА УКРАЇНСЬКУ МОВУ============================================
void convertDw(){
  switch(dayOfWeek){
    case 1 : dw=tSunday;    break;
    case 2 : dw=tMonday;    break;
    case 3 : dw=tTuesday;   break;
    case 4 : dw=tWednesday; break;
    case 5 : dw=tThursday;  break;
    case 6 : dw=tFriday;    break;
    case 7 : dw=tSaturday;  break;
  }
}
// ===========================КОНВЕРТАЦІЯ НАЗВ МІСЯЦІВ НА УКРАЇНСЬКУ МОВУ============================================
void convertMonth(){
  switch(month){
    case 1 : _month=tJanuary;      break;
    case 2 : _month=tFebruary;     break;
    case 3 : _month=tMarch;        break;
    case 4 : _month=tApril;        break;
    case 5 : _month=tMay;          break;
    case 6 : _month=tJune;         break;
    case 7 : _month=tJuly;         break;
    case 8 : _month=tAugust;       break;
    case 9 : _month=tSeptember;    break;
    case 10 : _month=tOctober;     break;
    case 11 : _month=tNovember;    break;
    case 12 : _month=tDecember;    break;
  }
}
//---------------------------------------------------------------------------
void buttonInter(){
  if(minute==0&&second==0&&(second!=lastSecond)&&(hour>=senss.kuOn&&hour<senss.kuOff)){bip();bip();}      // сигнал кожду годину 
  if(digitalRead(BUT_PIN)==senss.butStat&&butCount==0&&butFlag==0&&butMode==0){
    butCount=millis();
    butFlag=1;
  }
  if((millis()-butCount)>=30000&&butFlag==1&&butMode==0){
    butMode=4;
    if(senss.printCom)Serial.println(F("BUT MODE 4"));
    butFlag=0;
    butCount=0;
  }
  if(digitalRead(BUT_PIN)==!senss.butStat&&(millis()-butCount)>=10000&&butFlag==1&&butMode==0){
    butMode=3;
    if(senss.printCom)Serial.println(F("BUT MODE 3"));
    butFlag=0;
    butCount=0;
  }
  if(digitalRead(BUT_PIN)==!senss.butStat&&(millis()-butCount)<10000&&(millis()-butCount)>800&&(butFlag==1||butFlag==2)&&butMode==0){
    butMode=1;
    if(senss.printCom) Serial.println(F("BUT MODE 1"));
    butFlag=0;
    butCount=0;
  }
  if(digitalRead(BUT_PIN)==!senss.butStat&&(millis()-butCount)<=800&&butFlag==1&&butMode==0)butFlag=2;
  if(digitalRead(BUT_PIN)==senss.butStat&&(millis()-butCount)<=800&&butFlag==2){
    butMode=2;
    if(senss.printCom) Serial.println(F("BUT MODE 2"));
    butFlag=0;
    butCount=0;
  }
  if(!stopAlarm&&(alarm_work||reminder_work)&&butMode){ // если будильник работает, то любое нажатие выключает его
    bigClock=true;
    stopAlarm=true;
    fnTimer=millis();
    if(senss.printCom) Serial.println(F("Stop Alarm for But!"));
    if(alarm_work) butMode=0;
    leds.clockAndSec=clockAndSecMem;
  }
  if(irrecv.decode(&results)){
    ir_code=results.value;
    if(senss.printCom) Serial.println(String("ir_key code= "+String(ir_code,HEX)));
    delay(100);
    irrecv.resume();  // Receive the next value
    if(ir_save){
      ir_key[ir_save-1]=ir_code;
      //Serial.println("SAVE key="+String(ir_save)+"    code="+String(senss.ir_key[ir_save-1],HEX));
      saveIr();
      bip();
    }else{
      if(ir_code==ir_key[0]){
        if(!stopAlarm&&(alarm_work||reminder_work)){ // если будильник работает, то любое нажатие выключает его
          bigClock=true;
          stopAlarm=true;
          fnTimer=millis();
          if(senss.printCom) Serial.println("Stop Alarm for IR!");
          if(alarm_work) butMode=0;
          leds.clockAndSec=clockAndSecMem;
        }
      }
      if(ir_code==ir_key[1])butMode=1;
      if(ir_code==ir_key[2]){
        bip();
        butMode=5;
        delay(200);
        butMode=0;
        printForecast();
      }
      if(ir_code==ir_key[3]){
        leds.fontCLOCK++;
        if(leds.fontCLOCK>7)leds.fontCLOCK=0;
      }
      if(ir_code==ir_key[4]){
        bigClock=true;;
        delay(1000);
      }
    }
  }
  if(endString)buttonHandling();
}
//----------------------------------------------------------------
void buttonHandling(){
  if(butMode==4){ // если кнопка нажата была более 30 секунд то возврат к заводским установкам
    butMode=0;
    Serial.println("Ta to jest KAPUT!!!!!!");
    bip();bip();bip();bip();
    delay(3000);
    if(senss.printCom){
      printTime();
      Serial.println("ESP erase Config file(s)");
    }
    eraseConfigFiles();
  }
  if(butMode==3){ // если кнопка была нажата более 10 секунд но менее 30, то будет рестарт часов
    butMode=0;
    Serial.println("Reset ESP!!!");
    bip();
    bip();
    bip();
    ESP.reset();
  }
  if(butMode==1){
    bip();
    clr(leds.NUM_VER_X);
    refreshAll();
    for(byte i=0;i<8;i++){
      if(senss.sensors[i]){
        showSimple(i);
        delay(1500);
      }
    }
    butMode=0;
    clr(leds.NUM_VER_X);
    refreshAll();
  }
  if(butMode==2){ // При двойном нажатии на кнопку выводится прогноз погоды
    bip();
    bip();
    butMode=5;
    delay(200);
    butMode=0;
    printForecast();
  }
}

// ============================================================================//
//         Работа с сервисом https://thingspeak.com           // 
// ============================================================================//
void thingSpeak(){  
  if(!senss.thingOn || !WIFI_connected) return;
  if(things.channelIdWrite!="" || things.writeApiKey!=""){
    String line="";
    String regline="http://api.thingspeak.com/channels/"  + things.channelIdWrite + "/feeds.json?";
    if(things.wrApiKey!="") regline += "api_key=" + things.wrApiKey + "&";
    regline += "results=0";
    if(senss.printCom) {
      Serial.println("======== START SEND THINGSPEAK.COM ====================");
      printTime();
      Serial.print(regline);
    } 
    HTTPClient http;
    if(http.begin(ESPclient, regline)) { // HTTP
      int httpCode=http.GET();
      if (httpCode > 0) {
        if(senss.printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
        if (httpCode==HTTP_CODE_OK || httpCode==HTTP_CODE_MOVED_PERMANENTLY) {
          line=http.getString();
        }
      } else {
        if(senss.printCom) Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        http.end();
        return;
      }
      http.end();
    } else {
      if(senss.printCom) Serial.printf("    [HTTP] Unable to connect\n");
      return;
    }
    if(line==""){
      if(senss.printCom) Serial.printf("    [HTTP] The answer is empty\n");
      return;
    }
    const size_t capacity=JSON_ARRAY_SIZE(0) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(15) + 546; //https://arduinojson.org/v6/assistant/
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, line);
    JsonObject channel=doc["channel"];
    const char* channel_name=(channel["name"]);
    const char* channel_field1=channel["field1"];
    const char* channel_field2=channel["field2"];
    const char* channel_field3=channel["field3"];
    const char* channel_field4=channel["field4"];
    const char* channel_field5=channel["field5"];
    const char* channel_field6=channel["field6"];
    const char* channel_field7=channel["field7"];
    const char* channel_field8=channel["field8"];
    nameWriteChannel=String(channel_name);
    nameField[0]=String(channel_field1);
    nameField[1]=String(channel_field2);
    nameField[2]=String(channel_field3);
    nameField[3]=String(channel_field4);
    nameField[4]=String(channel_field5);
    nameField[5]=String(channel_field6);
    nameField[6]=String(channel_field7);
    nameField[7]=String(channel_field8);
    regline="http://api.thingspeak.com/update?api_key=" + things.writeApiKey;
    bool sendRegline=false;
    for(byte i=0;i<8;i++){
      if(String(nameField[i])!="" && things.writeFild[i]){
        if(     things.writeFild[i]==1 && senss.sensors[0]){regline+="&field"+String(i+1)+"="+String(data[0]); sendRegline=true;}
        else if(things.writeFild[i]==2 && senss.sensors[1]){regline+="&field"+String(i+1)+"="+String(data[1]); sendRegline=true;}
        else if(things.writeFild[i]==3 && senss.sensors[2]){regline+="&field"+String(i+1)+"="+String(data[2]); sendRegline=true;}
        else if(things.writeFild[i]==4 && senss.sensors[3]){regline+="&field"+String(i+1)+"="+String(data[3]); sendRegline=true;}
        else if(things.writeFild[i]==5 && senss.sensors[4]){regline+="&field"+String(i+1)+"="+String(data[4]); sendRegline=true;}
        else if(things.writeFild[i]==6 && sgpFound){regline+="&field"+String(i+1)+"="+String(sgpCo2);  sendRegline=true;}
        else if(things.writeFild[i]==7 && sgpFound){regline+="&field"+String(i+1)+"="+String(sgpTvoc); sendRegline=true;}
      }
    }
    int httpCode;
    if(sendRegline){
      HTTPClient client;
      client.begin(regline);
      httpCode=client.GET();
      client.end();
    }
    if(senss.printCom){
      printTime();
        if(sendRegline) Serial.println ("send line=" + regline +"   httpCode=" + String(httpCode));
        else Serial.println("No data to send!");
      Serial.println("======== END ==========================================");
    }
  }
  if(things.channelIdRead!=""){
    HTTPClient http;
    String line="";
    String regline="http://api.thingspeak.com/channels/"+things.channelIdRead+"/feeds.json?";
    if(things.readApiKey!="") regline += "api_key=" + things.readApiKey + "&";
    regline += "results=1";
    if(senss.printCom){
      Serial.println("======== START GET THINGSPEAK.COM =====================");
      printTime();
      Serial.print(regline);
    } 
    if (http.begin(ESPclient, regline)){
      int httpCode=http.GET();
      if (httpCode > 0){
        Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
        if (httpCode==HTTP_CODE_OK || httpCode==HTTP_CODE_MOVED_PERMANENTLY) {
          line=http.getString();
        }
      } else Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      http.end();
    } else Serial.printf("    [HTTP} Unable to connect\n");
    const size_t capacity=JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(10) + JSON_OBJECT_SIZE(15) + 797; //https://arduinojson.org/v6/assistant/
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, line);
    JsonObject channel=doc["channel"];
    const char* channel_name=channel["name"]; // "ESP-Informer"
    const char* channel_field1=channel["field1"]; // "TempDOM"
    const char* channel_field2=channel["field2"]; // "HumiDOM"
    const char* channel_field3=channel["field3"]; // "CO2"
    const char* channel_field4=channel["field4"]; // "TVOC"
    const char* channel_field5=channel["field5"]; // "Field Label 5"
    const char* channel_field6=channel["field6"]; // "Field Label 6"
    const char* channel_field7=channel["field7"]; // "Field Label 7"
    const char* channel_field8=channel["field8"]; // "Field Label 8"
    JsonObject feeds_0=doc["feeds"][0];
    const char* feeds_0_field1=feeds_0["field1"]; // "20.53"
    const char* feeds_0_field2=feeds_0["field2"]; // "44.31"
    const char* feeds_0_field3=feeds_0["field3"]; // "400"
    const char* feeds_0_field4=feeds_0["field4"]; // "0"
    const char* feeds_0_field5=feeds_0["field5"]; // "44.31"
    const char* feeds_0_field6=feeds_0["field6"]; // "44.31"
    const char* feeds_0_field7=feeds_0["field7"]; // "44.31"
    const char* feeds_0_field8=feeds_0["field8"]; // "44.31"
    nameReadChannel=channel_name;
    nameReadField[0]=channel_field1;
    nameReadField[1]=channel_field2;
    nameReadField[2]=channel_field3;
    nameReadField[3]=channel_field4;
    nameReadField[4]=channel_field5;
    nameReadField[5]=channel_field6;
    nameReadField[6]=channel_field7;
    nameReadField[7]=channel_field8;
    String aaa;
    for(byte i=0;i<8;i++){
      aaa=things.statThing[i]==0?"0":things.statThing[i]==1?String(feeds_0_field1):things.statThing[i]==2?String(feeds_0_field2):things.statThing[i]==3?String(feeds_0_field3):things.statThing[i]==4?String(feeds_0_field4):things.statThing[i]==5?String(feeds_0_field5):things.statThing[i]==6?String(feeds_0_field6):things.statThing[i]==7?String(feeds_0_field7):String(feeds_0_field8);
      readDataField[i]=aaa.substring(0, aaa.length()+1).toFloat();
    }
    if(senss.printCom){
      printTime();
      Serial.println("get line=" + line);
      Serial.println("======== END ==========================================");
    }
    sensorses();
  }
}
//-------------------------------------------------------------------------------
void weather_update(){
  if(weaths.weathOn){
    Serial.print("weaths.weatherHost=");
    Serial.println(String(weaths.weatherHost));
    if(!weaths.weatherHost){
      getWeatherData0();
      getWeatherDataz0();
    } else {
      getWeatherData1();
      getWeatherDataz1();
    }
  }
}
//-------------------------------------------------------------------------------
float verific_press(float pr){
  if(pr>64100 && pr<108700) pr=pr/100;
  if(senss.pressSys==1 && pr>815) pr /= 1.3332239;
  if(senss.pressSys!=1 && pr<815) pr /= 0.7500615613026439;
  return pr;
}
//-------------------------------------------------------------------------------
void setup_sensors(){
  //sensorsDht();
  //delay(500);
  if(bmp180.begin()) {
    if(senss.printCom) Serial.println("YES!!! Found sensor BMP180!");
    bmp180Found=true;
    sensorsBmp();
  } else if(senss.printCom) Serial.println("Did not find sensor BMP180!");
  delay(500);
  if(bmp.begin()) {
    if(senss.printCom) Serial.println("YES!!! Found sensor BMP280!");
    bmp280Found=true;
    sensorsBmp();
  } else if(senss.printCom) Serial.println("Did not find sensor BMP280!");
  delay(500);
  bme.parameter.communication=0;                            //Подключение сенсора по I2C 
  bme.parameter.I2CAddress=0x76;                            //I2C Адрес сенсора или 0x77
  bme.parameter.sensorMode=0b11;                            //0b00 спящий режим измерения не выполняются, 0b01: выполняется одно измерение, 0b11: датчик постоянно измеряет
  bme.parameter.IIRfilter=0b100;                            //высокое значение коэффициента означает меньше шума, но измерения также менее чувствительны 0b000-0(off), 0b001-2, 0b010-4, 0b011-8, 0b100-16 (default value)
  bme.parameter.humidOversampling=0b100;                    //коэффициент избыточной дискретизации для измерений влажности 0b000-0 (фильтр откл), 0b001-1, 0b010-2, 0b011-4, 0b100-8, 0b101-16 (максимум)
  bme.parameter.tempOversampling=0b101;                     //коэффициент передискретизации для измерения температуры 0b000-0 (фильтр откл), 0b001-1, 0b010-2, 0b011-4, 0b100-8, 0b101-16 (максимум)
  bme.parameter.pressOversampling=0b101;                    //коэффициент избыточной дискретизации для измерений давления. Для измерений высоты более высокий коэффициент обеспечивает более стабильные значения
  bme.parameter.pressureSeaLevel=1013.25;                   //текущее давление, скорректированное для уровня моря 1013.25 hPa. Для точных измерений высоты
  bme.parameter.tempOutsideCelsius=15;                      //средняя температура снаружи 15°C
  bme.parameter.tempOutsideFahrenheit=59;                   //средняя температура снаружи 59°F
  delay(500);
  if(bme.init()==0x60) {
    if(senss.printCom) Serial.println("YES!!! Found sensor BME280!");
    bme280Found=true;
    sensorsBme();
  } else if(senss.printCom) Serial.println("Did not find sensor BME280!");
  delay(500);
  if(sensor.begin()) {
    if(senss.printCom) Serial.println("YES!!! Found sensor Si7021!");
    si7021Found=true;
    sensorsSi7021();
  } else if(senss.printCom) Serial.println("Did not find sensor Si7021!");
  delay(500);
  if(aht.begin()) {
    if(senss.printCom) Serial.println("YES!!! Found sensor AHT10 or AHT20!");
    AHTx0Found=true;
    sensorsAHTx0();
  } else if(senss.printCom) Serial.println("Did not find sensor AHT10 or AHT20!");
  delay(500);
  if(lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 Found!!!");
    BH1750Found=true;
  } else Serial.println("BH1750 Not Found");
  delay(500);
  if(!light.begin()) {
    Serial.println("MAX44009 Found!!!");
    MAX44009Found=true;
  } else Serial.println("MAX44009 Not Found");
  sensorses();
}
//-----------------------------------------------------
void lang(){
  if(senss.messLang=="uk"){
    tNow="Зараз";
    tPress0="гПа";
    tPress="ммРс";
    tSpeed="м/с";
    tMin="хв.";
    tCurr="Сьогодні";
    tTom="Завтра";
    tYour="Ваш";
    tPoint="Підключіться до";
    tIp="та перейдіть за адресою";
    tPass="пароль";
    tWeatrNot="   Немає оновлень погоди більше 6 годин!!!   ";
    tWeatrTN="Немає оновлень погоди.";
    tYear="року";
    tJanuary="січня";
    tFebruary="лютого";
    tMarch="березня";
    tApril="квітня";
    tMay="травня";
    tJune="червня";
    tJuly="липня";
    tAugust="серпня";
    tSeptember="вересня";
    tOctober="жовтня";
    tNovember="листопада";
    tDecember="грудня";
    tMonday="Понеділок";
    tTuesday="Вівторок";
    tWednesday="Середа";
    tThursday="Четвер";
    tFriday="П'ятниця";
    tSaturday="Субота";
    tSunday="Неділя";
    tAlarm="УВАГА!!!   ";
    sgpCo2Message[0]="Допустимий рівень повітря";
    sgpCo2Message[1]="Важке повітря. Необхідно провітрювання";
    sgpCo2Message[2]="Можлива сонливість, втома, головний біль";
    sgpCo2Message[3]="Серйозне погіршення здоров'я !!!";
    sgpCo2Message[4]="Гранично допустима концентрація протягом 8 годин!!!";
    sgpTvocMessage[0]="Допустимий рівень ЛОС";
    sgpTvocMessage[1]="Рекомендується вентиляція";
    sgpTvocMessage[2]="Рекомендується інтенсивна вентиляція";
    sgpTvocMessage[3]="Інтенсивна вентиляція і провітрювання необхідні";
    sgpTvocMessage[4]="Дуже інтенсивна вентиляція суворо необхідна !!!";
    wind[0]="Північний";
    wind[1]="Північно-східний";
    wind[2]="Східний";
    wind[3]="Південно-східний";
    wind[4]="Південний";
    wind[5]="Південно-західний";
    wind[6]="Західний";
    wind[7]="Північно-західний";
  } else if(senss.messLang=="ru"){
    tNow="Сейчас";
    tPress0="гПа";
    tPress="ммРс";
    tSpeed="м/с";
    tMin="мин.";
    tCurr="Сегодня";
    tTom="Завтра";
    tYour="Ваш";
    tPoint="Подключитесь к точке доступа";
    tIp= "и введите в браузере адрес";
    tPass="пароль";
    tWeatrNot="   Нет обновления погоды более 6 часов!!!   ";
    tWeatrTN="Нет обновления погоды.";
    tYear="года";
    tJanuary="января";
    tFebruary="февраля";
    tMarch="марта";
    tApril="апреля";
    tMay="мая";
    tJune="июня";
    tJuly="июля";
    tAugust="августа";
    tSeptember="сентября";
    tOctober="октября";
    tNovember="ноября";
    tDecember="декабря";
    tMonday="Понедельник";
    tTuesday="Вторник";
    tWednesday="Среда";
    tThursday="Четверг";
    tFriday="Пятница";
    tSaturday="Суббота";
    tSunday="Воскресенье";
    tAlarm="ВНИМАНИЕ!!!   ";
    sgpCo2Message[0]="Допустимый уровень возуха";
    sgpCo2Message[1]="Тяжелый воздух. Необходимо проветривание";
    sgpCo2Message[2]="Возможно сонливость, усталость, головная боль";
    sgpCo2Message[3]="Серьезное ухудшение здоровья!!!";
    sgpCo2Message[4]="Предельно допустимая концентрация в течении 8 часов";
    sgpTvocMessage[0]="Допустимый уровень ЛОС";
    sgpTvocMessage[1]="Рекомендуется вентиляция";
    sgpTvocMessage[2]="Рекомендуется интенсивная вентиляция";
    sgpTvocMessage[3]="Интенсивная вентиляция и проветривание необходимы";
    sgpTvocMessage[4]="Очень интенсивная вентиляция строжайше необходима!!!";
    wind[0] = "Северный";
    wind[1] = "Северо-восточный";
    wind[2] = "Восточный";
    wind[3] = "Юго-восточный";
    wind[4] = "Южный";
    wind[5] = "Юго-Западный";
    wind[6] = "Западный";
    wind[7] = "Северо-Западный"; 
  } else if(senss.messLang=="en"){
    tNow="now";
    tPress0="gPa";
    tPress="mmHg";
    tSpeed="m/s";
    tMin="min.";
    tCurr="Today";
    tTom="Tomorrow";
    tYour="Your";
    tPoint="Connect to access point the";
    tIp="and enter in the browser the address";
    tPass="password";
    tWeatrNot="   There is no weather update for more than 6 hours !!!   ";
    tWeatrTN="No weather updates.";
    tYear="year";
    tJanuary="January";
    tFebruary="February";
    tMarch="March";
    tApril="April";
    tMay="May";
    tJune="June";
    tJuly="July";
    tAugust="August";
    tSeptember="September";
    tOctober="October";
    tNovember="November";
    tDecember="December";
    tMonday="Monday";
    tTuesday="Tuesday";
    tWednesday="Wednesday";
    tThursday="Thursday";
    tFriday="Friday";
    tSaturday="Saturday";
    tSunday="Sunday";
    tAlarm="ATTENTION!!!   ";
    sgpCo2Message[0]="Permissible level of air";
    sgpCo2Message[1]="Heavy air. Ventilation is necessary";
    sgpCo2Message[2]="Drowsiness, fatigue, headache is possible";
    sgpCo2Message[3]="Serious deterioration of health !!!";
    sgpCo2Message[4]="Maximum permissible concentration within 8 hours";
    sgpTvocMessage[0]="Allowable VOC Level";
    sgpTvocMessage[1]="Recommended ventilation";
    sgpTvocMessage[2]="Recommended intensive ventilation";
    sgpTvocMessage[3]="Intensive ventilation and ventilation are necessary";
    sgpTvocMessage[4]="Very intensive ventilation is strictly necessary !!!";
    wind[0] = "North";
    wind[1] = "Northeast";
    wind[2] = "East";
    wind[3] = "Southeast";
    wind[4] = "South";
    wind[5] = "Southwest";
    wind[6] = "Western";
    wind[7] = "Northwest"; 
  } else if(senss.messLang=="de"){
    tNow="jetzt";
    tPress0="gPa";
    tPress="mmHg";
    tSpeed="m/s";
    tMin="min.";
    tCurr="Heute";
    tTom="Morgen";
    tYour="Ihr";
    tPoint="Stellen Sie die Verbindung mit";
    tIp="und geben Sie im Browser die Adresse";
    tPass="Passwort";
    tWeatrNot="   Keine Wetteraktualisierungen seit 6 Stunden!!!   ";
    tWeatrTN="Keine Wetteraktualisierungen.";
    tYear="jahr";
    tJanuary="Januar";
    tFebruary="Februar";
    tMarch="März";
    tApril="April";
    tMay="Mai";
    tJune="Juni";
    tJuly="Juli";
    tAugust="August";
    tSeptember="September";
    tOctober="Oktober";
    tNovember="November";
    tDecember="Dezember";
    tMonday="Montag";
    tTuesday="Dienstag";
    tWednesday="Mittwoch";
    tThursday="Donnerstag";
    tFriday="Freitag";
    tSaturday="Samstag";
    tSunday="Sonntag";
    tAlarm="ACHTUNG !!!";
    sgpCo2Message[0]="Zulässiges Luftniveau";
    sgpCo2Message[1]="Starke Luft. Belüftung erforderlich";
    sgpCo2Message[2]="Schläfrigkeit, Müdigkeit, Kopfschmerzen sind möglich";
    sgpCo2Message[3]="Schwerwiegende Verschlechterung der Gesundheit !!!";
    sgpCo2Message[4]="Maximal zulässige Konzentration innerhalb von 8 Stunden";
    sgpTvocMessage[0]="Zulässiger VOC-Level";
    sgpTvocMessage[1]="Empfohlene Belüftung";
    sgpTvocMessage[2]="Empfohlene intensive Beatmung";
    sgpTvocMessage[3]="Intensive Beatmung und Beatmung sind notwendig";
    sgpTvocMessage[4]="Sehr intensive Belüftung ist unbedingt erforderlich !!!";
    wind[0] = "Norden";
    wind[1] = "Nordosten";
    wind[2] = "Ost";
    wind[3] = "Südosten";
    wind[4] = "Süd";
    wind[5] = "Südwesten";
    wind[6] = "Western";
    wind[7] = "Nordwesten";
  } else if(senss.messLang=="pl"){
    tNow="Teraz";
    tPress0="gPa";
    tPress="mmHg";
    tSpeed="m/s";
    tMin="min.";
    tCurr="Dzisiaj";
    tTom="Jutro";
    tYour="Twój";
    tPoint="Polacz sie z punktem dostępu";
    tIp="i wprowadz w przeglądarce adres";
    tPass="hasło";
    tWeatrNot="   Brak aktualizacji pogody przez ponad 6 godzin!!!   ";
    tWeatrTN="Brak aktualizacji pogody.";
    tYear="rok";
    tJanuary="stycznia";
    tFebruary="lutego";
    tMarch="marzca";
    tApril="kwiecnia";
    tMay="maja";
    tJune="czerwca";
    tJuly="lipca";
    tAugust="sierpnia";
    tSeptember="wrzesnia";
    tOctober="pazdziernika";
    tNovember="listopada";
    tDecember="grudznia";
    tMonday="Poniedzialek";
    tTuesday="Wtorek";
    tWednesday="Środa";
    tThursday="Czwartek";
    tFriday="Piątek";
    tSaturday="Sobota";
    tSunday="Niedziela";
    tAlarm="UWAGA !!!";
    sgpCo2Message[0]="Dopuszczalny poziom powietrza";
    sgpCo2Message[1]="Ciężkie powietrze. Konieczna jest wentylacja";
    sgpCo2Message[2]="Możliwe jest senność, zmęczenie, ból głowy";
    sgpCo2Message[3]="Poważne pogorszenie stanu zdrowia !!!";
    sgpCo2Message[4]="Maksymalne dopuszczalne stężenie w ciągu 8 godzin";
    sgpTvocMessage[0]="Dopuszczalny poziom LZO";
    sgpTvocMessage[1]="Zalecana wentylacja";
    sgpTvocMessage[2]="Zalecana intensywna wentylacja";
    sgpTvocMessage[3]="Konieczna jest intensywna wentylacja i wentylacja";
    sgpTvocMessage[4]="Bardzo intensywna wentylacja jest absolutnie konieczna !!!";
    wind[0] = "Północny";
    wind[1] = "Północno-wschodni";
    wind[2] = "Wschódni";
    wind[3] = "Południowo-wschodni";
    wind[4] = "Południowy";
    wind[5] = "Południowo-zachodni";
    wind[6] = "Pachodni";
    wind[7] = "Północno-zachodni"; 
  }
}

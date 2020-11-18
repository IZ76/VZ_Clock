/*_______By IvanUA_____________________________
 Піни LED------NodeMCU  1.0                    | 
       DataIn__________________D7/GPIO 13      |
       LOAD/CS_________________D8/GPIO 15      |
       CLK_____________________D5/GPIO 14      |
 Кнопка________________________D0/GPIO 16      |
 Бaзер_________________________D6/GPIO 12      |
 DS18B20_______________________D3/GPIO 0       |
 Si7021/BMP/BME280/DS3231 DA___D2/GPIO 4       | 
 Si7021/BMP/BME280/DS3231 CL___D1/GPIO 5       |
 GND - FotoRezistor ___________A0              |
 +3,3 - rezistor 2-100k _______A0              |
 DHT11/22______________________D4/GPIO 2       |
______________________________________________*/
#define ver "x2_4.4"

#include <Wire.h>
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include "Adafruit_Si7021.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BMP085.h>
#include "BlueDot_BME280.h"
#include <SimpleDHT.h>
#include <Adafruit_AHTX0.h>
#include <Ticker.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
Ticker blinker;
ESP8266HTTPUpdateServer httpUpdater;
WiFiClient ESPclient;
PubSubClient MQTTclient(ESPclient);
ESP8266WebServer server(80);
File fsUploadFile;
IPAddress apIP(192, 168, 4, 1);
#include "P_js.h"
#include "P_css.h"
#include "P_index.h"
#include "P_time.h"
#include "P_weath.h"
#include "P_setup.h"
#include "P_sgp.h"
#include "P_help.h"
#include "P_mqtt.h"
#include "P_thing.h"

// ===================================================
String ssid         = "IvanZah";
String password     = "";
String ssidAP       = "ESP-Info";
String passwordAP   = "11223344";
String auth         = "1234asdf5678";
String weatherHost0 = "api.weatherbit.io";
String weatherHost1 = "api.openweathermap.org";
String weatherKey0  = "";
String weatherKey1  = "";
String cityID0      = "Kryvyy Rih";
String cityID1      = "703845"; // Kryvyy Rih, "2925533"-Frankfurt
boolean authOn = true;
boolean weatherHost = 0;
char   personalCityName[51] = "";
String weatherLang = "uk";
String location_name = "";
String location_region = "";
String location_country = "";
String location_localtime = "";
float  location_temp;
float  location_app_temp;
int    location_rh;
float  location_pres;
float  location_wind_spd;
String location_wind_cdir_full = "";
String location_sunrise = "";
String location_sunset = "";
int    location_clouds;
int    location_vis;
int    location_uv;
String location_weather_description = "";
String cityName;
//int cityID0;
String weatherString;
String weatherStringZ;
bool animNotWeather = true;
// ----------змінні для роботи з mqtt сервером
char mqtt_server[21]       = "m13.cloudmqtt.com";
int  mqtt_port             = 13011;
char mqtt_user[25]         = "";
char mqtt_pass[25]         = "";
char mqtt_name[25]         = "ESP-Informer";
char mqtt_sub_inform[25]   = "Inform/mess";
char mqtt_sub1[25]         = "Ulica/temp";
char mqtt_sub2[25]         = "Ulica/hum";
char mqtt_sub3[25]         = "Ulica/press";
char mqtt_pub_sensor00[25] = "Informer/temp";
char mqtt_pub_sensor01[25] = "Informer/tempUl";
char mqtt_pub_sensor02[25] = "Informer/tempH";
char mqtt_pub_sensor03[25] = "Informer/hum";
char mqtt_pub_sensor04[25] = "Informer/press";
char mqtt_pub_eco2[25]     = "Informer/eco2";
char mqtt_pub_tvoc[25]     = "Informer/tvoc";
bool mqttOn = true;
// --------------------------------------------
String uuid = "";
String api_key = "";
int sensors_ID0 = 0;    //88733 Frankfurt
int sensors_ID1 = 3300;   //88459 Frankfurt
int sensors_ID2 = 0;
// ----------------Thing для чтения ----------------------------
bool sendThing = true;
String writeApiKey      = "aaaaaaaaaaaaaaaaa";
String wrApiKey         = "bbbbbbbbbbbbb";
String channelIdWrite   = "111111111";
String nameWriteChannel = "";
String nameField0 = "";
String nameField1 = "";
String nameField2 = "";
String nameField3 = "";
String nameField4 = "";
String nameField5 = "";
String nameField6 = "";
String nameField7 = "";
byte writeFild0 = 1; //0-нет, 1-tD, 2-tU, 3-tH, 4-hD, 5-P, 6-sgpCo2, 7-sgpTvoc
byte writeFild1 = 4;
byte writeFild2 = 6;
byte writeFild3 = 7;
byte writeFild4 = 0;
byte writeFild5 = 0;
byte writeFild6 = 0;
byte writeFild7 = 0;

bool getThing = true;
String channelIdRead = "2222222222";
String readApiKey    = "cccccccccccccccccc";
String nameReadChannel = "";
String nameReadField0 = ""; // имена филдов
String nameReadField1 = "";
String nameReadField2 = "";
String nameReadField3 = "";
String nameReadField4 = "";
String nameReadField5 = "";
String nameReadField6 = "";
String nameReadField7 = "";
float readDataField0 = 0; // получаемые данные
float readDataField1 = 0;
float readDataField2 = 0;
float readDataField3 = 0;
float readDataField4 = 0;
byte statThing0 = 0; // храним номер филда откуда приходят данные
byte statThing1 = 1;
byte statThing2 = 2;
byte statThing3 = 3;
byte statThing4 = 7;

String oldcreatedat = "";
unsigned long counterThink = 0;
bool humThinkOnOff = true;

float tempThink = 0.0; //температура с ThingSpeak со знаком и плавающей запятой
float humThink = 0.0; // влажность на улице с ThingSpeak со знаком и плавающей запятой
byte hT1 = 0;    // влажность на улице с ThingSpeak целая беззнаковая часть
byte hT2 = 0;    // влажность на улице с ThingSpeak дробная беззнаковая часть
float pressThink = 0.0; // джавление с ThingSpeak со знаком и плавающей запятой
float batThink = 0.0; // Заряд АКБ с ThingSpeak со знаком и плавающей запятой
// =====================================================================================
bool printCom = true;
#define MAX_DIGITS 16
byte NUM_MAX0 = 4;
byte NUM_MAX1 = 4;
byte fontCLOCK = 0;      // 0-крупный, 1-крупный цифровой, 2-полу жирный, 3-полу жирный цифровой, 4-обычный, 5-обычный цифровой, 6-узкий, 7-узкий цифровой. 
bool fontSizeCLOCK = 0;  // 0-8 строк, 1-7 строк
bool fontSizeData = 1;   // 0-8 строк, 1-7 строк
bool textDown = 0;
bool dataDown = 1;
byte animPoint = 0;
byte volBrightnessD = 10;
byte volBrightnessN = 2;
bool volBrightnessAuto = 0;
byte levelBridhtness = 0;
int lowLivelBrightness = 0;
int upLivelBrightness = 1023;
byte timeDay = 5;
byte timeNight = 24;
int rotate0 = 90;
int rotate1 = 90;
byte timeStartViewWeather = 6;
byte timeEndViewWeather = 23;
byte timeScrollSpeed = 40;
#define DIN_PIN   13                                                                    //GPIO 13 / D7
#define CS_PIN    15                                                                    //GPIO 15 / D8
#define CLK_PIN   14                                                                    //GPIO 14 / D5
#define buzzerPin 12                                                                    //GPIO 12 / D6
bool buzzerSet = 1;
bool stopAlarm = false;
#define brightPin A0
OneWire  ds(0);                                                                         // DS18B20 подключен к 10 пину (резистор на 4.7к обязателен)
int pinDHT = 2;
SimpleDHT11 dht11;
SimpleDHT22 dht22;
#define BUT_PIN   16
// ----------
#include "max7219.h"
#include "fonts.h"
boolean WIFI_connected = false;
// ---------- Настройка оновлення часу
IPAddress timeServerIP;
String ntpServerName = "ntp3.time.in.ua";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP udp;
unsigned int localPort = 2390;
unsigned long epochNM;
static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};        // Кількість днів у місяцях
#define LEAP_YEAR(Y) (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100)||!((1970+Y)%400)))   // Високосні літа
String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
String tempz;
int clouds;
int windDeg;
float windSpeed;
String dw, _month;
String jsonConfig = "{}";
String jsonAlarm = "{}";
String jsonTime = "{}";
// ---------- Змінні для роботи локального годинника
float timeZone = 2.0;                                                                       //  часовий пояс
float hourCorr;
bool isDayLightSaving = true;
int displayData = 1;                      // 0 - Не отображать, 1 - отображать статику, 2 - отображать в бегущей строке
long localEpoc = 0;
long localMillisAtUpdate = 0;
int hour=2,   minute=40,  second=42,  month=4,   day=6,   dayOfWeek=6,   year=2018;
int g_hour, g_minute, g_second, g_month=1, g_day, g_dayOfWeek, g_year;
bool statusUpdateNtpTime = 0;                                                               // якщо не "0" - то останнє оновленя часу було вдалим
String y, mon, wd, d, h, m, s, mes;
uint8_t hourTest[3], minuteTest[3];
int secFr, lastSecond, lastMinute;
String date;
byte kuOn = 7;
byte kuOff = 23;
byte clock2line = 2;
bool bigCklock = 0;
byte timeStopBigCklock = 5; //в секундах
unsigned long timerStopBigCklock;
byte secLine = 1;
// ----------
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int dx = 0;
int dy = 0;
byte del = 0;
int updateOTA = 1;
bool displayForecast = true;
bool displayCityName = true;
bool displayForecastNow = true;
bool displayForecastToday = true;
bool displayForecastTomorrow = true;
bool updateForecastNot = true;
int updateForecast = 0;
int updateForecasttomorrow = 0;
float data00; // данные первого датчика
float data01; // данные второго датчика
float data02; // данные третьего датчика
float data03; // данные 4 датчика
float data04;     // данные пятого датчика
float tMqtt1 = 0.0; //температура с mqtt со знаком и плавающей запятой
float tMqtt2 = 0.0; //влажность с mqtt со знаком и плавающей запятой
float tMqtt3 = 0.0; //давление с mqtt со знаком и плавающей запятой
unsigned long counterMqtt1 = 0;
unsigned long counterMqtt2 = 0;
unsigned long counterMqtt3 = 0;
boolean counterMqtt1Timeout = true; // 1 - время вышло
boolean counterMqtt2Timeout = true;
boolean counterMqtt3Timeout = true;
int timeOutMqtt = 1200000; // таймаут обновления данных - 20 минут
float tempNM = 0.0;
bool pressSys = 1;
byte humidity;                        // влажность для прогноза
float pressure;                       // давление для прогноза
float temp;                           // температура для прогноза
float tempDs18b20;                    // температура с датчика DS18B20
BlueDot_BME280 bme;
float tempBme = 0;
float humiBme = 0;
float pressBme = 0;
float altBme = 0;
Adafruit_BMP280 bmp;
Adafruit_BMP085 bmp180;
float tempBmp = 0;
float pressBmp = 0;
float altBmp = 0;
bool ds18b20Found = false;
bool dhtFound = false;
bool bmp280Found = false;
bool bmp180Found = false;
bool bme280Found = false;
bool nMonFound = false;
float tempDht = 0;
float humiDht = 0;
Adafruit_Si7021 sensor = Adafruit_Si7021();
float humiSi7021 = 0;
float tempSi7021 = 0;
bool si7021Found = false;
Adafruit_AHTX0 aht;
float humiAHTx0 = 0;
float tempAHTx0 = 0;
bool AHTx0Found = false;
float corr00 = 0;
float corr01 = 0;
float corr02 = 0;
float corr03 = 0;
float corr04 = 0;
byte sensor00 = 10;     //NONE=0, DS18B20=1, Si7021=2, BMP280=3, BME280=4, DHT=5, MQTT1=6, MQTT2=7, MQTT3=8; NMon=9, AHTx0=10, THING1=11, THING2=12, THING3=13, THING4=14, THING5=15;
byte sensor01 = 6;      //                          2                   4      5                                           10   
byte sensor02 = 7;
byte sensor03 = 10;
byte sensor04 = 0;
byte param0 = 0;      //  0-темп.дом(tD), 1-темп.улица(tU), 2-темп.Н(tН), 3-(tT), 4-(tL) 
byte param1 = 1;      // 10-влажность1(hD), 11-(h1), 12-(h2), 13-(h3), 14-(h4)
byte param2 = 2;      // 20-давление(P), 21-целое число(С) 22-батарейка(V), 23-батарейка(A)
byte param3 = 10;
byte param4 = 20;
String tNow, tCurr, tPress, tPress0, tSpeed, tMin, tTom, tYour, tPoint, tIp, tPass, tWeatrNot, tWeatrTN;
bool alarm_stat=0;
bool alarm_hold=0;
byte alarm_numer=255;
byte alarme[5][3]{{12, 30, 0},{7, 15, 0},{22, 55, 0},{0, 30, 0},{0, 0, 0}}; //1-часы, 2-минуты, 3-откл(0)/1раз(11)/пон-пят(8)/пон-сб(9)/сб-вс(10)/вс(1)/пон(2)/вто(3)/сре(4)/чет(5)/пят(6)/сб(7)/всегда(12)
byte memory_hour_start=9;
byte memory_hour_end=23;
byte m_date=0;
byte memory_date[9][2]{{1, 1},{2, 1},{6, 1},{7, 1},{13, 1},{19, 1},{8, 3},{1, 5},{1, 9}}; //1-день, 2-месяц
char memory_date_mes0[51] = "Новый Год!!!";
char memory_date_mes1[51] = "ПослеНовыйГод(((";
char memory_date_mes2[51] = "Ночь перед Рождеством";
char memory_date_mes3[51] = "Рождество";
char memory_date_mes4[51] = "Старый Новый Год!";
char memory_date_mes5[51] = "Крещение";
char memory_date_mes6[51] = "День цветов";
char memory_date_mes7[51] = "День шашлыка)))";
char memory_date_mes8[51] = "Отведи ребенка в школу";
bool firstStart=0;
bool apStart=0;
byte amountNotStarts=0;
String jsonLine = "";
bool rtcStat = true;
int rtcAddr = 0x68;
int hour_rtc, minute_rtc, second_rtc, month_rtc, day_rtc, dayOfWeek_rtc, year_rtc;
#include "rtc.h"
byte errorRTC;
bool butStat = 1;
byte butMode = 0; // 0 - не нажата, 1 - нажата один раз, 2 - нажата два раза, 3 - 5 секунд нажата, 4 - 30 секунд нажата.
byte butFlag = 0; // 1 - кнопка нажата, 0 - не нажата
int butCount = 0; // счетчик времени нажатия кнопки
int butMillis = 0;
bool runningLine = 0;
String tJanuary, tFebruary, tMarch, tApril, tMay, tJune, tJuly, tAugust, tSeptember, tOctober, tNovember, tDecember;
String tMonday, tTuesday, tWednesday, tThursday, tFriday, tSaturday, tSunday, tAlarm;

#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;
boolean sgpFound = false;
int sgpCo2 = 400;
byte sgpCo2Livel = 0; //0=400-699, 1=700-999, 2=1000-2499, 3=2500-4999, 4=5000++
int sgpCo2Livel1 =  700;
int sgpCo2Livel2 = 1000;
int sgpCo2Livel3 = 2500;
int sgpCo2Livel4 = 5000;
byte sgpCo2LivelAlarm = 2;
boolean eCo2AlarmEsp = true;
boolean eCo2AlarmMqtt = false;
boolean eCo2Led = true;
String sgpCo2Message[]={"","","","",""};
int sgpTvoc = 0;
byte sgpTvocLivel = 0; //0=0-64, 1=65-219, 2=220-659, 3=660-2199, 4=2200++
int sgpTvocLivel1 = 65;
int sgpTvocLivel2 = 220;
int sgpTvocLivel3 = 660;
int sgpTvocLivel4 = 2200;
byte sgpTvocLivelAlarm = 2;
boolean tvocAlarmEsp = true;
boolean tvocAlarmMqtt = false;
boolean tvocLed = true;
String sgpTvocMessage[]={"","","","",""};
byte setSgpCorr = 0;  //0-нет коррекции, 1-BME-280, 2-Si7021, 3-AHTx0, 4-DHT11/22
float sgpCorrTemp = 22.1; // [°C]
float sgpCorrHumi = 45.2; // [%RH]
//======================================================================================
void setup(){
  Wire.begin(); 
  Serial.begin(115200);
  if(printCom) Serial.println("");
  pinMode(BUT_PIN, INPUT);
  digitalWrite(BUT_PIN, !butStat);
  delay(500);
  SPIFFS.begin();
  loadConfig();
  loadAlarm();
  loadTime();
  lang();
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, volBrightnessN);
  Wire.beginTransmission(0x67);
  errorRTC = Wire.endTransmission();
  if(errorRTC==0){
    rtcAddr = 0x67;
    if(printCom) Serial.println("YES!!! find RTC module addr: 0x67!");
  } else {
    Wire.beginTransmission(0x68);
    errorRTC = Wire.endTransmission();
    if(errorRTC==0){
      rtcAddr = 0x68;
      if(printCom) Serial.println("YES!!! find RTC module addr: 0x68!");
    } else rtcStat = false;
  }
  if(rtcStat){
    if(printCom) Serial.println("RTC START");
    printTime();
    getRTCDateTime();
    hour=hour_rtc;
    minute=minute_rtc;
    second=second_rtc;
    day=day_rtc;
    month=month_rtc;
    year=year_rtc;
    dayOfWeek=dayOfWeek_rtc;
    if(printCom){
      Serial.println("RTC update: "+String(hour)+":"+String(minute)+":"+String(second)+"    "+String(day)+"."+String(month)+"."+String(year)+" D="+String(dayOfWeek));
    }
  } else if(printCom) Serial.println("RTC module off!");
  // ------------------
  sensorsDht();
  if(bmp.begin()) {
    if(printCom) Serial.println("YES!!! Found sensor BMP280!");
    bmp280Found = true;
    sensorsBmp();
  } else if(printCom) Serial.println("Did not find sensor BMP280!");
  if(bmp180.begin()) {
    if(printCom) Serial.println("YES!!! Found sensor BMP180!");
    bmp180Found = true;
    sensorsBmp();
  } else if(printCom) Serial.println("Did not find sensor BMP180!");
  bme.parameter.communication = 0;                            //Подключение сенсора по I2C 
  bme.parameter.I2CAddress = 0x76;                            //I2C Адрес сенсора или 0x77
  bme.parameter.sensorMode = 0b11;                            //0b00 спящий режим измерения не выполняются, 0b01: выполняется одно измерение, 0b11: датчик постоянно измеряет
  bme.parameter.IIRfilter = 0b100;                            //высокое значение коэффициента означает меньше шума, но измерения также менее чувствительны 0b000-0(off), 0b001-2, 0b010-4, 0b011-8, 0b100-16 (default value)
  bme.parameter.humidOversampling = 0b100;                    //коэффициент избыточной дискретизации для измерений влажности 0b000-0 (фильтр откл), 0b001-1, 0b010-2, 0b011-4, 0b100-8, 0b101-16 (максимум)
  bme.parameter.tempOversampling = 0b101;                     //коэффициент передискретизации для измерения температуры 0b000-0 (фильтр откл), 0b001-1, 0b010-2, 0b011-4, 0b100-8, 0b101-16 (максимум)
  bme.parameter.pressOversampling = 0b101;                    //коэффициент избыточной дискретизации для измерений давления. Для измерений высоты более высокий коэффициент обеспечивает более стабильные значения
  bme.parameter.pressureSeaLevel = 1013.25;                   //текущее давление, скорректированное для уровня моря 1013.25 hPa. Для точных измерений высоты
  bme.parameter.tempOutsideCelsius = 15;                      //средняя температура снаружи 15°C
  bme.parameter.tempOutsideFahrenheit = 59;                   //средняя температура снаружи 59°F
  if(bme.init() == 0x60) {
    if(printCom) Serial.println("YES!!! Found sensor BME280!");
    bme280Found = true;
    sensorsBme();
  } else if(printCom) Serial.println("Did not find sensor BME280!");
  if(sensor.begin()) {
    if(printCom) Serial.println("YES!!! Found sensor Si7021!");
    si7021Found = true;
    sensorsSi7021();
  } else if(printCom) Serial.println("Did not find sensor Si7021!");
  if(aht.begin()) {
    if(printCom) Serial.println("YES!!! Found sensor AHT10 or AHT20!");
    AHTx0Found = true;
    sensorsAHTx0();
  } else if(printCom) Serial.println("Did not find sensor AHT10 or AHT20!");
  sensors();
  serverInit();
// ----------
  blinker.attach(0.05, showAnimClock);
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  udp.begin(localPort);
  pinMode(buzzerPin, OUTPUT);
// ---------- Підключення до WiFi
  wifiConnect();


  if(displayData) showSimpleDate();
// ***********  OTA SETUP
  if(updateOTA) {
    ArduinoOTA.setPort(8266);
    ArduinoOTA.setHostname("ESP-Informer"); 
    ArduinoOTA.onEnd([](){ESP.restart();});
    ArduinoOTA.onError([](ota_error_t error){Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR && printCom) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR && printCom) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR && printCom) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR && printCom) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR && printCom) Serial.println("End Failed");
      ESP.restart(); 
    });
    ArduinoOTA.begin();
  }
  if(WiFi.status() == WL_CONNECTED) {
    if(displayForecast){
      if(!weatherHost) {
        getWeatherData0();
        getWeatherDataz0();
      } else {
        getWeatherData1();
        getWeatherDataz1();
      }
    }
  }
// ---------- MQTT client
  MQTTclient.setServer(mqtt_server, mqtt_port);
  MQTTclient.setCallback(callback);
  MQTTclient.connect(mqtt_name);
  MQTTclient.subscribe(mqtt_sub_inform);
  MQTTclient.subscribe(mqtt_sub1);
  MQTTclient.subscribe(mqtt_sub2);
  MQTTclient.subscribe(mqtt_sub3);
  if(sensor01==9)getNarodmon();

  if(!sgp.begin()){
    if(printCom){
      Serial.println("Sensor SGP-30 not found :(");
    }
    sgpFound = false;
  } else {
    if(printCom){
      printTime();
      Serial.print("Found SGP30 serial #");
      Serial.print(sgp.serialnumber[0], HEX);
      Serial.print(sgp.serialnumber[1], HEX);
      Serial.println(sgp.serialnumber[2], HEX);
    }   
    sgpFound = true;
  }
  if(sendThing) sendThings();
  if(getThing) getThings();
}
//==========================================
void callback(char* topic, byte* payload, unsigned int length) { // получаем знаковое число с десятичной плавающей запятой
  if(!mqttOn) return;
  String Text;
  for(int i=0; i<length; i++) Text+=((char)payload[i]);
  if((String(topic)==mqtt_sub_inform)&&(sgpCo2Livel<sgpCo2LivelAlarm||sgpTvocLivel<sgpTvocLivelAlarm)) {
    for(byte i=0; i<NUM_MAX1; i++) {
      Text+="   ";
    }
    for(int i=0; i<4; i++) bip();
    if(printCom){
      printTime();
      Serial.println("MQTT Incoming Message: " + Text);
    }
    clr(1);
    refreshAll();
    printStringWithShift(Text.c_str(), 30, 1);
    return;
  }
  if(String(topic) == mqtt_sub1) {
    tMqtt1 = Text.substring(0, length+1).toFloat();
    if(printCom){
      printTime();
      Serial.println("MQTT1 Incoming: " + String(tMqtt1));
    }
    counterMqtt1 = millis();
    counterMqtt1Timeout = false;
    sensors();
  }
  if(String(topic) == mqtt_sub2) {
    tMqtt2 = Text.substring(0, length+1).toFloat();
    if(printCom){
      printTime();
      Serial.println("MQTT2 Incoming: " + String(tMqtt2));
    }
    counterMqtt2 = millis();
    counterMqtt2Timeout = false;
    sensors();
  }
  if(String(topic) == mqtt_sub3) {
    tMqtt3 = Text.substring(0, length+1).toFloat();
    if(printCom){
      printTime();
      Serial.println("MQTT3 Incoming: " + String(tMqtt3));
    }
    counterMqtt3 = millis();
    counterMqtt3Timeout = false;
    sensors();
  }
}
//======================================================================================
void reconnect() {
  if(printCom) {
    printTime();
    Serial.println("Start Reconnect void...");
  }
  if(WiFi.status() == WL_CONNECTED) {  // && !ESPclient.connected()
    if(printCom) {
      printTime();
      Serial.print("MQTT reconnection...");
    }
    if(MQTTclient.connect(mqtt_name, mqtt_user, mqtt_pass)) {
      if(printCom) Serial.println("connected");
      MQTTclient.subscribe(mqtt_sub_inform);
      MQTTclient.subscribe(mqtt_sub1);
      MQTTclient.subscribe(mqtt_sub2);
      MQTTclient.subscribe(mqtt_sub3);
    } else {
      if(printCom){
        Serial.print("failed, rc = ");
        Serial.println(MQTTclient.state());
      }
    }
  }
}
//======================================================================================
//======================================================================================
void loop() {
  if(updateOTA) ArduinoOTA.handle();
  server.handleClient();                                                                // дозволяємо HTTP серверу відповідать на запити
  updateTime();                                                                         // оновлюємо час
  buttonInter();
  if(second != lastSecond) {                                                            // на початку нової секунди скидаємо secFr в "0"
    lastSecond = second;
    if (volBrightnessAuto) {
      //levelBridhtness = map(analogRead(brightPin), 1023, 0, 0, 15);
      int br = analogRead(brightPin);
      if(lowLivelBrightness<=upLivelBrightness){
        if(br<lowLivelBrightness) lowLivelBrightness=br;
        if(br>upLivelBrightness) upLivelBrightness=br;
      } else {
        if(br<upLivelBrightness) upLivelBrightness=br;
        if(br>lowLivelBrightness) lowLivelBrightness=br;
      }
      levelBridhtness = map(br,lowLivelBrightness, upLivelBrightness, volBrightnessN, volBrightnessD);
      sendCmdAll(CMD_INTENSITY, levelBridhtness);
    } else {
      if (hour >= timeDay && hour < timeNight) sendCmdAll(CMD_INTENSITY, volBrightnessD);
      else sendCmdAll(CMD_INTENSITY, volBrightnessN);
    }
    secFr=0;
    if(lastMinute!=minute) {
      if(hour==0 && minute==0 && second==0) {
        day++;
        if(day==32 || (day==31 && (month==4 || month==6 || month==9 || month==11)) || (month==2 && ((day==29 && year%4!=0) || (day==30 && year%4==0)))) {
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
      lastMinute=minute;
    }
  } else secFr++;                                                                       // якщо секунда ще не скінчилась то нарощуємо лічильник циклів secFr
  if(minute == 0 && second == 0 && secFr == 0 && (hour >= kuOn && hour < kuOff)) {      // сигнал кожду годину 
    bip();
    bip();
  }
  //----------- РОБОТА З БУДИЛЬНИКОМ------------------------------------------------------
  if(secFr == 0) {
    if(second > 0 && alarms()) { // если вернулась 1 тоесть совпало время и режим alarm_numer= номеру совпадения
      if(!alarm_stat && alarm_numer != 255 && !alarm_hold) alarm_stat = 1;
    } else if(alarm_stat) {
      alarm_stat = 0;
      if(alarme[alarm_numer][2] == 11) alarme[alarm_numer][2] = 0; // если будильник был разовый то выключаем его
    } else if(alarm_hold != 0);
  }
  //------------- РОБОТА ЗІ СВЯТКОВИМИ ДАТАМИ ---------------------------------------------
  if(secFr == 0) {
    if(minute == 0) {
      if(hour >= memory_hour_start && hour <= memory_hour_end && second < 15 && second > 2 && !alarm_stat){
        for(byte i = 0; i < 9; i++) {
          if(memory_date[i][0] == day && memory_date[i][1] == month){
            m_date = i;
            bip();
            bip();
            bip();
            printStringWithShift(("         "), timeScrollSpeed, 1);
            printStringWithShift((m_date==0?memory_date_mes0:m_date==1?memory_date_mes1:m_date==2?memory_date_mes2:m_date==3?memory_date_mes3:m_date==4?memory_date_mes4:m_date==5?memory_date_mes5:m_date==6?memory_date_mes6:m_date==7?memory_date_mes7:memory_date_mes8), timeScrollSpeed, 1);
            printStringWithShift(("         "), timeScrollSpeed, 1);
            break;
          }
        }
      } 
    } else m_date = 0;
  }
  buttonHandling();
  //------------- НАШ ЧАС ----------------------------------------------------------------
  if(hour == 22 && minute == 55) {                                                      // якщо наш час - то іде повідомлення для коханої
    //bip();
    //bip();
    //bip();
    printStringWithShift("       \200\200\200 Моей любимой Заюшке!!! \200\200\200", timeScrollSpeed, 1);
    return;
  }

  // ************ Все функции выполняются один раз в секунду ************************************************
  if(secFr == 0){
      // ---------- ВИВІД НА ЕКРАН ГОДИННИКА АБО ТЕМПЕРАТУРИ ЧИ ВОЛОГОСТІ------------------------
    if(!alarm_stat && butMode == 0) {
      if(second == 40 && sensor00 && timeStopBigCklock) {
        showSimple(0);
      } else if(second == 45 && sensor01 && timeStopBigCklock) {
        showSimple(1);
      } else if(second == 50 && sensor02 && timeStopBigCklock) {
        showSimple(2);
      } else if(second == 55 && sensor03 && timeStopBigCklock) {
        showSimple(3);
      } else if(second == 00 && sensor04 && timeStopBigCklock) {
        showSimple(4);
      } else if(displayData && (timeStartViewWeather<timeEndViewWeather?(hour < timeStartViewWeather || hour >= timeEndViewWeather):(hour >= timeStartViewWeather || hour < timeEndViewWeather)) && second == 10) {
         showSimpleDate();
      }
    } else if(alarm_stat && butMode == 0) {
      if(secFr == 0 && second>=1) {
        bip();
        bip();
      }
    }
    // ---------- Каждую 5 секунду обновляем данные СО2 при наличии датчика ---------------------------------
    if(second == 5 && sgpFound){
      sgp30();
    }
    // ---------- Раз в сутки берем время из модуля RTC -----------------------------------------------------
    if(second == 10 && minute == 1 && hour == 3) {  
      if(rtcStat){
        printTime();
        getRTCDateTime();
        hour=hour_rtc;
        minute=minute_rtc;
        second=second_rtc;
        day=day_rtc;
        month=month_rtc;
        year=year_rtc;
        dayOfWeek=dayOfWeek_rtc;
        if(printCom){
          Serial.println("RTC update: "+String(hour)+":"+String(minute)+":"+String(second)+"    "+String(day)+"."+String(month)+"."+String(year)+" D="+String(dayOfWeek));
        }
      }
    }
    // ---------- 10 секунда - виводимо дату/погоду----------------------------------------------------------
    if(second == 10 && !alarm_stat) {
      sensorsAll();
      if(hour >= timeStartViewWeather && hour < timeEndViewWeather && !(clock2line && (hour<timeDay || hour>=timeNight)) && butMode == 0) {        // працує тілки в дозволений час
        if(minute % 2 == 0 || !displayForecast) {
          if(displayData == 1){
            clr(1);
            showSimpleDate();
          } else if(displayData==2){
            clr(1);
            convertDw();
            convertMonth();
            date = "     " + dw + ", " + String(day) + " " + _month + " " + String(year) + "                ";
            printStringWithShift(date.c_str(), timeScrollSpeed, 1);
            if(sgpFound && eCo2Led){
              String livelCo2 = sgpCo2Message[sgpCo2Livel] + " eCO\275 = " + String(sgpCo2) + " ppm               ";
              printStringWithShift(livelCo2.c_str(), timeScrollSpeed, 1);
            }
            if(sgpFound && tvocLed){
              String livelTvoc = sgpTvocMessage[sgpTvocLivel] + " TVOC = " + String(sgpTvoc) + " ppb               ";
              printStringWithShift(livelTvoc.c_str(), timeScrollSpeed, 1);
            }
            if(sensor00) showSimple(0);
          }
        } else {
          if(updateForecastNot) printStringWithShift(("            " + tWeatrTN + "            ").c_str(), timeScrollSpeed, 1);// выводится до первого обновления погоды
          else {
            if(bigCklock) clr(1);
            if(updateForecast > 30 && updateForecast < 360) weatherString = tWeatrTN + " - " + String(updateForecast) + "мин.        ";
            else if(updateForecast >= 360) weatherString = tWeatrNot;
            printStringWithShift(weatherString.c_str(), timeScrollSpeed, 1);
            if(updateForecasttomorrow<30) printStringWithShift(weatherStringZ.c_str(), timeScrollSpeed, 1);
          }
          if(sensor00&&!bigCklock) showSimple(0);
        }
      }
    }
    // ---------- 43 секунда оновленя мережевого часу кожну хвилину або в 5 хвилину кожного часу ------------------------
    if(((statusUpdateNtpTime == 0 && second == 43) || (minute == 02 && second == 43)) && !alarm_stat) timeUpdateNTP();
    // ---------- 46 cек. две функции -----------------------------------------------------------------------------------
    if(second==46){
      //  перевірка доступності WiFi мережі ---
      if(!alarm_stat){
        if(apStart && millis()>600000) apStart=0; // через 10 минут после страта если нет подключения к Wifi - можно пробовать переподключиться
        if((WiFi.status() != WL_CONNECTED || !WIFI_connected) && !apStart) {  // повторне підк. до WiFi кожну 1, 6, 11, 16...56 хв.
          WIFI_connected = false;
          if(minute % 5 == 1) {
            wifiConnect();
            if(WiFi.status() == WL_CONNECTED) WIFI_connected = true;
          }
        }
      }
      //оновлюємо прогноз погоди (во время отображения, каждую 14 и 44 миунту или когда не получен прогноз и когда разрешен показ 
      if((hour>=timeStartViewWeather&&hour<=timeEndViewWeather&&!alarm_stat) && (minute==14||minute==44||updateForecast||updateForecasttomorrow) && displayForecast &&  WIFI_connected){
        if(minute == 14 || minute == 44) {
          if(!weatherHost) {
            getWeatherData0();
            getWeatherDataz0();
          } else {
            getWeatherData1();
            getWeatherDataz1();
          }
        } else {
          if(updateForecast){
            if(!weatherHost) {
              getWeatherData0();
            } else {
              getWeatherData1();
            }
          }
          if(updateForecasttomorrow){
            if(!weatherHost) {
              getWeatherDataz0();
            } else {
              getWeatherDataz1();
            }
          }
        }
        if(sensor01==9) getNarodmon();
        if(mqttOn && !MQTTclient.connected()) {
          reconnect();
          if(!MQTTclient.connected()) reconnect();
        }
      }
    }
    // ---------- 50 сек. перевірка доступності MQTT та публікація температури ---------
    if(second == 50 && mqttOn && !alarm_stat && WIFI_connected) {
      if(WiFi.status() != WL_CONNECTED) {
        WIFI_connected = false;
      }
      if(!MQTTclient.connected() && WIFI_connected) {
        reconnect();
      }
      if(MQTTclient.connected() && WIFI_connected) {
        if(sensor00&&String(mqtt_pub_sensor00) != ""){
          MQTTclient.publish(mqtt_pub_sensor00, String(data00).c_str());
          if(printCom){
            printTime();
            Serial.println("Publish sensor00: " + String(data00));
          }
        }
        if(sensor01&&String(mqtt_pub_sensor01) != ""){
          MQTTclient.publish(mqtt_pub_sensor01, String(data01).c_str());
          if(printCom){
            printTime();
            Serial.println("Publish sensor01: " + String(data01));
          }
        }
        if(sensor02&&String(mqtt_pub_sensor02) != ""){
          MQTTclient.publish(mqtt_pub_sensor02, String(data02).c_str());
          if(printCom){
            printTime();
            Serial.println("Publish sensor02: " + String(data02));
          }
        }
        if(sensor03&&String(mqtt_pub_sensor03)!=""){
          MQTTclient.publish(mqtt_pub_sensor03, (String(data03)).c_str());
          if(printCom){
            printTime();
            Serial.println("Publish sensor03: " + String(data03));
          }
        }
        if(sensor04&&String(mqtt_pub_sensor04)!=""){
          MQTTclient.publish(mqtt_pub_sensor04, (String(data04)).c_str());
          if(printCom){
            printTime();
            Serial.println("Publish sensor04: " + String(data04));
          }
        }
        if(sgpFound){
          if(String(mqtt_pub_eco2)!=""){
            MQTTclient.publish(mqtt_pub_eco2, String(sgpCo2).c_str());
            if(printCom){
                printTime();
                Serial.println("Publish eCO2: " + String(sgpCo2));
            }
          }
          if(String(mqtt_pub_tvoc)!=""){
            MQTTclient.publish(mqtt_pub_tvoc, String(sgpTvoc).c_str());
            if(printCom){
                printTime();
                Serial.println("Publish TVOC: " + String(sgpTvoc));
            }
          }
        }
      }
    }
    // ---------- 55 сек. работа с ThingSpeak ----------------------------
    if(second == 55 && (sendThing || getThing) && !alarm_stat && WIFI_connected) {
      if(sendThing) sendThings();
      if(getThing) getThings();
      if(mqttOn && !MQTTclient.connected()) {
        reconnect();
        if(!MQTTclient.connected()) reconnect();
      }
    }
    // ---------- Обработка таймаутов MQTT -------------------------------
    if(millis()>timeOutMqtt){
      if(millis()-counterMqtt1>timeOutMqtt) counterMqtt1Timeout = true;
      else counterMqtt1Timeout = false;
      if(millis()-counterMqtt2>timeOutMqtt) counterMqtt2Timeout = true;
      else counterMqtt2Timeout = false;
      if(millis()-counterMqtt3>timeOutMqtt) counterMqtt3Timeout = true;
      else counterMqtt3Timeout = false;
    }
  }//************** все функции выполнялись раз в секунду*******************************************

  
  // ---------- якщо мережа WiFi доступна то виконуємо наступні функції ----------------------------
  if(WIFI_connected){
    if(mqttOn) MQTTclient.loop();           // перевіряємо чи намає вхідних повідомлень, як є, то кoлбек функція
  }
  if(secFr==0 && butMode != 0){ // если отработали все функции работы с кнопкой, то состояние сбросится автоматом
    Serial.println("BUT MODE RESET");
    butMode = 0;
  }
  bool oldBigCklok = bigCklock;
  //                                                                                          10 < 8     &&   10 >= 3
  if((timerStopBigCklock+(timeStopBigCklock*1000))<millis()){
    bigCklock = ((clock2line == 1 && (timeDay<timeNight?(hour<timeDay || hour>=timeNight):(hour<timeDay && hour>=timeNight)))|| clock2line >1) && butMode == 0;
  }
  if(oldBigCklok != bigCklock){
    clr(0);
    clr(1);
    refreshAll();
  }
}
//======================================================================================
//======================================================================================
void showSimple(byte num){
  if(num==0){
    if(param0<10) showSimpleTemperature(param0, data00);
    else if(param0<20) showSimpleHumidity(param0, data00);
    else if(param0==20) showSimplePressure(param0, data00);
    else if(param0==21) showSimpleNumeric(param0, data00);
    else showSimpleBattery(param0, data00);
  } else if(num==1){
    if(param1<10) showSimpleTemperature(param1, data01);
    else if(param1<20) showSimpleHumidity(param1, data01);
    else if(param1==20) showSimplePressure(param1, data01);
    else if(param1==21) showSimpleNumeric(param1, data01);
    else showSimpleBattery(param1, data01);
  } else if(num==2){
    if(param2<10) showSimpleTemperature(param2, data02);
    else if(param2<20) showSimpleHumidity(param2, data02);
    else if(param2==20) showSimplePressure(param2, data02);
    else if(param2==21) showSimpleNumeric(param2, data02);
    else showSimpleBattery(param2, data02);
  } else if(num==3){
    if(param3<10) showSimpleTemperature(param3, data03);
    else if(param3<20) showSimpleHumidity(param3, data03);
    else if(param3==20) showSimplePressure(param3, data03);
    else if(param3==21) showSimpleNumeric(param3, data03);
    else showSimpleBattery(param3, data03);
  } else if(num==4){
    if(param4<10) showSimpleTemperature(param4, data04);
    else if(param4<20) showSimpleHumidity(param4, data04);
    else if(param4==20) showSimplePressure(param4, data04);
    else if(param4==21) showSimpleNumeric(param4, data04);
    else showSimpleBattery(param4, data04);
  }
}
//========== ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ ================================================
void showSimpleTemperature(byte znakT, float temp0) {
  bigCklock = 0;
  timerStopBigCklock=millis();
  int temp1 = int(temp0);
  int temp2 = int(temp0*10*(temp0>0?1:-1))%10;
  int znak = znakT%10;
  byte digPos[10] {3, 4, 5, 6, 17, 18, 19, 20, 21, 22};
  byte indent = (NUM_MAX1 * 8) - 32;
  dx = dy = 0;
  clr(1);
  showDigit((temp0 < 0.0 ? digPos[znak*2+1]:digPos[znak*2]), 0 + indent, (fontSizeData?znaki5x7:znaki5x8), 1); // друкуємо t+ альбо t-
  if(temp1 <= -10.0 || temp1 >= 10) showDigit((temp1 < 0 ? (temp1 * -1) / 10 : temp1 / 10), 4 + indent, (fontSizeData?dig5x7:dig5x8), 1);
  showDigit((temp1 < 0 ? (temp1 * -1) % 10 : temp1 % 10), 10 + indent, (fontSizeData?dig5x7:dig5x8), 1);
  showDigit(2, 16 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
  showDigit(temp2, 18 + indent, (fontSizeData?dig5x7:dig5x8), 1);
  showDigit(0, 24 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
  showDigit(1, 27 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
  if(dataDown) scrollDown(1);
  refreshAll();
}
//========== ВИВІД НА ЕКРАН ВОЛОГОСТІ ==================================================
void showSimpleHumidity(byte znakT, float humi0) {
  bigCklock = 0;
  timerStopBigCklock=millis();
  if(humi0>0){
    int humi1 = int(humi0);
    int humi2 = int(humi0*10*(humi0>0?1:-1))%10;
    int znak = znakT%10;
    byte digPos[5] {7, 23, 24, 25, 26};
    byte indent = (NUM_MAX1 * 8) - 32;
    dx = dy = 0;
    clr(1);
    showDigit(digPos[znak], 0 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);     // друкуємо знак вологості
    if(humi1 >= 10) showDigit(humi1/10, 6 + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit((humi1-(humi1/10)*10), 12 + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit(2, 18 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    showDigit(humi2, 20 + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit(8, 26 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    if(dataDown) scrollDown(1);
    refreshAll();
  }
}
//==========ВИВІД НА ЕКРАН ТИСКУ =================================================
void showSimplePressure(byte znakT, float press0){
  bigCklock = 0;
  timerStopBigCklock=millis();
  if(pressSys==1 && press0>815) press0 /= 1.3332239;
  if(pressSys!=1 && press0<815) press0 /= 0.7500615613026439;
  if(press0 > 0){
    int press1 = (int) press0 / 1000;
    int press2 = ((int) press0 - press1 * 1000) / 100;
    int press3 = ((int) press0 - press1 * 1000 - press2 * 100) / 10;
    int press4 = (int)press0 % 10;
    byte indent = (NUM_MAX1 * 8) - 32;
    dx = dy = 0;
    clr(1);
    showDigit(9, 0 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);     // друкуємо знак тиску
    if(press1 > 0) showDigit(press1, 5 + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit(press2 , (press1 > 0 ? 10 : 6) + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit(press3 , (press1 > 0 ? 16 : 12) + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit(press4 , (press1 > 0 ? 22 : 18) + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit((pressSys == 1 ? 10 : 15), (press1 > 0 ? 28 : 24) + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    showDigit((pressSys == 1 ? 11 : 16), (press1 > 0 ? (pressSys==1?33:32) : (pressSys==1?29:28)) + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    if(dataDown) scrollDown(1);
    refreshAll();
  }
}
//========== ВИВІД НА ЕКРАН БАТАРЕЇ ==================================================
void showSimpleBattery(byte znakT, float batt0) {
  bigCklock = 0;
  timerStopBigCklock=millis();
  if(batt0>0){
    int batt1 = int(batt0);
    int batt2 = int(batt0*10*(batt0>0?1:-1))%10;
    byte indent = (NUM_MAX1 * 8) - 32;
    dx = dy = 0;
    clr(1);
    showDigit(znakT+5, 0 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);     // друкуємо знак вологості
    if(batt1 >= 10) showDigit(batt1/10, 6 + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit((batt1-(batt1/10)*10), 12 + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit(2, 18 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    showDigit(batt2, 20 + indent, (fontSizeData?dig5x7:dig5x8), 1);
    showDigit(znakT+7, 26 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    if(dataDown) scrollDown(1);
    refreshAll();
  }
}
//==========ВИВІД НА ЕКРАН ДОДАТКОВИХ ДАННИХ========================================
void showSimpleNumeric(byte znakT, float numer0){
  bigCklock = 0;
  timerStopBigCklock=millis();
  byte indent = (NUM_MAX1 * 8) - 32;
  dx = dy = 0;
  clr(1);
  showDigit((numer0 < 0.0 ? 32 : 31), 0 + indent, (fontSizeData?znaki5x7:znaki5x8), 1); //друкуємо U+ альбо U-
  float numer1 = numer0*(numer0>=0?1:-1);
  if(numer1>=10000){
    showDigit((int)numer1%10, 28 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1/10)%10, 23 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1/100)%10, 18 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1/1000)%10, 13 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1/10000)%10, 8 + indent, (fontSizeData?dig4x7:dig4x8), 1);
  } else if(numer1>=1000){
    showDigit((int)(numer1*10)%10, 28 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit(2, 26 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    showDigit((int)numer1%10, 21 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1/10)%10, 16 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1/100)%10, 11 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1/1000)%10, 6 + indent, (fontSizeData?dig4x7:dig4x8), 1);      
  } else {
    showDigit((int)(numer1*100)%10, 28 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit((int)(numer1*10)%10, 23 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    showDigit(2, 21 + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
    showDigit((int)numer1%10, 16 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    if(numer1 >=10) showDigit((int)(numer1/10)%10, 11 + indent, (fontSizeData?dig4x7:dig4x8), 1);
    if(numer1>=100) showDigit((int)(numer1/100)%10, 6 + indent, (fontSizeData?dig4x7:dig4x8), 1);
  }
  if(dataDown) scrollDown(1);
  refreshAll();
}


//==========ВИВІД НА ЕКРАН ДАТИ=========================================================
void showSimpleDate(){
  bigCklock = 0;
  timerStopBigCklock=millis()+((8-timeStopBigCklock)*1000);
  bool nonsens = false;
  byte digPos[8] {0, 5, 10, 12, 17, 22, 23, 28};
  if(year%10 == 1){
    digPos[6]++;
    if(month%10 == 1){digPos[3]++; digPos[2]++; digPos[1]++; digPos[0]++;}
    if(month/10 == 1){digPos[2]++; digPos[1]++; digPos[0]++;}
    if(day%10 == 1){  digPos[0]++;}
  } else {
    if(month%10 == 1){digPos[5]--; digPos[4]--;}
    else if(month/10 == 1){digPos[5]--; digPos[4]--; digPos[3]--;}
    if(month%10 == 1 && month/10 == 1){digPos[2]++; digPos[1]++; digPos[0]++;}
    if(month%10 != 1 && month/10 != 1 && (day%10 == 1 || day/10 == 1)) {digPos[5]--; digPos[4]--; digPos[3]--; digPos[2]--; digPos[1]--;}
    else if(day%10 == 1){digPos[0]++;}
    if(month%10 != 1 && month/10 != 1 && day%10 != 1 && day/10 == 1) nonsens = true;
  }
  byte indent = (NUM_MAX1 * 8) - 32;
  dx = dy = 0;
  clr(1);
  showDigit(nonsens?10:day / 10, digPos[0] + indent, (fontSizeData?dig4x7:dig4x8), 1);
  showDigit(day % 10, digPos[1] + indent, (fontSizeData?dig4x7:dig4x8), 1);
  showDigit(2, digPos[2] + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
  showDigit(month / 10, digPos[3] + indent, (fontSizeData?dig4x7:dig4x8), 1);
  showDigit(month % 10, digPos[4] + indent, (fontSizeData?dig4x7:dig4x8), 1);
  showDigit(2, digPos[5] + indent, (fontSizeData?znaki5x7:znaki5x8), 1);
  showDigit((year - 2000) / 10, digPos[6] + indent, (fontSizeData?dig4x7:dig4x8), 1);
  showDigit((year - 2000) % 10, digPos[7] + indent, (fontSizeData?dig4x7:dig4x8), 1);
  if(dataDown) scrollDown(1);
  refreshAll();
}
//==========ВИВІД НА ЕКРАН АНІМАЦІЙНОГО ГОДИННИКА=======================================
void showAnimClock(){
  byte digPos[6] {5, 10, 18, 23, 15, 47,}; //digPos[0-3] первая-четвертая цифра, digPos[4] - начало точек верняя строка digPos[5] - нижнаяя строка 
  byte digPos2[6] {0, 6, 13, 19, 25, 11};
  bool num=hour<10?1:0;
  if(num){digPos[1]=7; digPos[2]=15; digPos[3]=20; digPos[4]=12; digPos[5]=44;}
  if(fontCLOCK<2 || bigCklock){
    if(num) {digPos[1]=5; digPos[2]=15; digPos[3]=22; digPos[4]=12; digPos[5]=44;}
    else{digPos[0]=1; digPos[1]=8; digPos[2]=18; digPos[3]=25; digPos[4]=15; digPos[5]=47;}
  } else if(fontCLOCK<6 && !bigCklock){
    if(num) {digPos[1]=6; digPos[2]=15; digPos[3]=21; digPos[4]=12;}
    else{digPos[0]=3; digPos[1]=9; digPos[2]=18; digPos[3]=24; digPos[4]=15;}
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
  clr(0);
  if(bigCklock) clr(1);
  // ------------------  выводим часы
  for(i=num; i<4; i++){   
    if(digtrans[i]==0){
      dy=0;
      if(!bigCklock){
        if(fontCLOCK==0) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig6x7:dig6x8), 0);
        else if(fontCLOCK==1) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig6x7dig:dig6x8dig), 0);
        else if(fontCLOCK==2) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7rn:dig5x8rn), 0);
        else if(fontCLOCK==3) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7rndig:dig5x8rndig), 0);
        else if(fontCLOCK==4) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7:dig5x8), 0);
        else if(fontCLOCK==5) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7dig:dig5x8dig), 0);
        else if(fontCLOCK==6) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig4x7:dig4x8), 0);
        else if(fontCLOCK==7) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig4x7dig:dig4x8dig), 0);
      } else if(clock2line<3){
        showDigit(dig[i], digPos[i], dig6x6bigH, 0);
        showDigit(dig[i], digPos[i], dig6x6bigL, 1);
      } else{
        showDigit(dig[i], digPos2[i], clock2line==4?dig5x7bigH:dig5x6bigH, 0);
        showDigit(dig[i], digPos2[i], clock2line==4?dig5x7bigL:dig5x6bigL, 1);
      }
    } else {
      dy=digHt-digtrans[i];
      if(!bigCklock){
        if(fontCLOCK==0) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig6x7:dig6x8), 0);
        else if(fontCLOCK==1) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig6x7dig:dig6x8dig), 0);
        else if(fontCLOCK==2) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig5x7rn:dig5x8rn), 0);
        else if(fontCLOCK==3) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig5x7rndig:dig5x8rndig), 0);
        else if(fontCLOCK==4) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig5x7:dig5x8), 0);
        else if(fontCLOCK==5) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig5x7dig:dig5x8dig), 0);
        else if(fontCLOCK==6) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig4x7:dig4x8), 0);
        else if(fontCLOCK==7) showDigit(digold[i], digPos[i], (fontSizeCLOCK?dig4x7dig:dig4x8dig), 0);
      } else if(clock2line<3){
        showDigit(digold[i], digPos[i], dig6x6bigH, 0);
        showDigit(digold[i], digPos[i], dig6x6bigL, 1);
      } else{
        showDigit(digold[i], digPos2[i], clock2line==4?dig5x7bigH:dig5x6bigH, 0);
        showDigit(digold[i], digPos2[i], clock2line==4?dig5x7bigL:dig5x6bigL, 1);
      }
      dy=-digtrans[i];
      if(!bigCklock){
        if(fontCLOCK==0) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig6x7:dig6x8), 0);
        else if(fontCLOCK==1) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig6x7dig:dig6x8dig), 0);
        else if(fontCLOCK==2) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7rn:dig5x8rn), 0);
        else if(fontCLOCK==3) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7rndig:dig5x8rndig), 0);
        else if(fontCLOCK==4) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7:dig5x8), 0);
        else if(fontCLOCK==5) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig5x7dig:dig5x8dig), 0);
        else if(fontCLOCK==6) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig4x7:dig4x8), 0);
        else if(fontCLOCK==7) showDigit(dig[i], digPos[i], (fontSizeCLOCK?dig4x7dig:dig4x8dig), 0);
      } else if(clock2line<3){
        showDigit(dig[i], digPos[i], dig6x6bigH, 0);
        showDigit(dig[i], digPos[i], dig6x6bigL, 1);
      } else{
        showDigit(dig[i], digPos2[i], clock2line==4?dig5x7bigH:dig5x6bigH, 0);
        showDigit(dig[i], digPos2[i], clock2line==4?dig5x7bigL:dig5x6bigL, 1);
      }
      digtrans[i]--;
    }
  }
  // ------------------  выводим точки
  dy=0;
  int flash=millis()%2000;
  if(animPoint%2) flash=flash%1000;
  else flash=flash/2;
  // точки только в первой строке
  if(!bigCklock) {  
    if(!alarm_stat){
      if(!WIFI_connected){
        if(flash<500){
          setCol(digPos[4], fontSizeCLOCK?0x60:0xC0); // полная точка
          setCol(digPos[4]+1, fontSizeCLOCK?0x60:0xC0);
        }
      } else if(!statusUpdateNtpTime){
        if(flash<149){
          setCol(digPos[4], fontSizeCLOCK?0x36:0x66);
          setCol(digPos[4]+1, fontSizeCLOCK?0x36:0x66);
        }
      } else if(animPoint<3){      // Простая 
        if(flash<499 || animPoint==0){
          setCol(digPos[4], fontSizeCLOCK?0x36:0x66);
          setCol(digPos[4]+1, fontSizeCLOCK?0x36:0x66);
        }
      } else if(animPoint==3 || animPoint==4){ // MAX1
        if(flash<500){
          setCol(digPos[4], fontSizeCLOCK?0x32:0x62);
          setCol(digPos[4]+1, fontSizeCLOCK?0x32:0x62);
        } else{
          setCol(digPos[4], fontSizeCLOCK?0x26:0x46);
          setCol(digPos[4]+1, fontSizeCLOCK?0x26:0x46);
        }
      } else if(animPoint==5 || animPoint==6){ // MAX2
        if((flash>=200 && flash<400) || flash>=600){
          setCol(digPos[4], fontSizeCLOCK?0x36:0x66);
          setCol(digPos[4]+1, fontSizeCLOCK?0x36:0x66);
        }
        if(flash>=0 && flash<200){
          setCol(digPos[4], fontSizeCLOCK?0x14:0x24);
          setCol(digPos[4]+1, fontSizeCLOCK?0x14:0x24);
        }
        if(flash>=400 && flash<600){
          setCol(digPos[4], fontSizeCLOCK?0x22:0x42);
          setCol(digPos[4]+1, fontSizeCLOCK?0x22:0x42);
        }
      } else if(animPoint==7 || animPoint==8){ // Мерцание 
        if((flash>=(animPoint==5?180:200) && flash<(animPoint==5?360:400)) || flash>=(animPoint==5?540:600)){
          setCol(digPos[4], fontSizeCLOCK?0x36:0x66);
          setCol(digPos[4]+1, fontSizeCLOCK?0x36:0x66);
        }
        if(flash>=0 && flash<(animPoint==5?180:200)){
          setCol(digPos[4], fontSizeCLOCK?0x14:0x24);
          setCol(digPos[4]+1, fontSizeCLOCK?0x22:0x42);
        }
        if(flash>=(animPoint==5?360:400) && flash<(animPoint==5?540:600)){
          setCol(digPos[4], fontSizeCLOCK?0x22:0x42);
          setCol(digPos[4]+1, fontSizeCLOCK?0x14:0x24);
        }
      } else if(animPoint==9 || animPoint==10){ // Вращение 
        if(flash<250){
          setCol(digPos[4], fontSizeCLOCK?0x06:0x06);
          setCol(digPos[4]+1, fontSizeCLOCK?0x30:0x60);
        } else if(flash>=250 && flash<500){
          setCol(digPos[4], fontSizeCLOCK?0x22:0x42);
          setCol(digPos[4]+1, fontSizeCLOCK?0x22:0x42);
        } else if(flash>=500 && flash<750){
          setCol(digPos[4], fontSizeCLOCK?0x30:0x60);
          setCol(digPos[4]+1, fontSizeCLOCK?0x06:0x06);
        } else if(flash>=750){
          setCol(digPos[4], fontSizeCLOCK?0x14:0x24);
          setCol(digPos[4]+1, fontSizeCLOCK?0x14:0x24);
        }
      }
      if(updateForecast && WIFI_connected && displayForecast && animNotWeather) setCol(00, flash<500?0x80:0x00);
      if(updateForecasttomorrow && WIFI_connected && displayForecast && animNotWeather) setCol(NUM_MAX0*8-1, flash<500?0x80:0x00);
    } else if(!runningLine){
      if(flash>=360 && flash<540){
        clr(1);
        setCol(digPos[4], (fontSizeCLOCK ? 0x36 : 0x66));
        setCol(digPos[4]+1, (fontSizeCLOCK ? 0x36 : 0x66));
        showDigit(12, (NUM_MAX0*4-3), (fontSizeData?znaki5x7:znaki5x8), 1);
      } else{
        showDigit(13, (NUM_MAX0*4-3), (fontSizeData?znaki5x7:znaki5x8), 1);
      }
    }
  // точки в две строки
  } else if(clock2line<3){
    if(!alarm_stat){
      if(!WIFI_connected){
        if(flash < 500){
          setCol(digPos[5], 0x30);
          setCol(digPos[5]+1, 0x30);
        }
      } else if(!statusUpdateNtpTime){
        if(flash<149){
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x0C);
        }
      } else if(animPoint<3){      // Простая 
        if(flash<499 || animPoint==0){
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x0C);
        }
      } else if(animPoint==3 || animPoint==4){ // MAX1
        if(flash<500){
          setCol(digPos[4], 0x10);
          setCol(digPos[4]+1, 0x10);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x0C);
        } else{
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x08);
          setCol(digPos[5]+1, 0x08);
        }
      } else if(animPoint==5 || animPoint==6){ // MAX2
        if((flash>=200 && flash<400) || flash>=600){
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x0C);
        }
        if(flash>=0 && flash<200) {
          setCol(digPos[4], 0x10);
          setCol(digPos[4]+1, 0x10);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x0C);
        }
        if(flash>=400 && flash<600){
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x08);
          setCol(digPos[5]+1, 0x08);
        }
      } else if(animPoint==7 || animPoint==8){ // Мерцание 
        if((flash>=(animPoint==5?180:200) && flash<(animPoint==5?360:400)) || flash>=(animPoint==5?540:600)){
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x0C);
        }
        if(flash>=0 && flash<(animPoint==5?180:200)){
          setCol(digPos[4], 0x10);
          setCol(digPos[4]+1, 0x20);
          setCol(digPos[5], 0x08);
          setCol(digPos[5]+1, 0x04);
        }
        if(flash>=(animPoint==5?360:400) && flash<(animPoint==5?540:600)){
          setCol(digPos[4], 0x20);
          setCol(digPos[4]+1, 0x10);
          setCol(digPos[5], 0x04);
          setCol(digPos[5]+1, 0x08);
        }
      } else if(animPoint==9 || animPoint==10){ // Вращение 
        if(flash<250) {
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x20);
          setCol(digPos[5], 0x04);
          setCol(digPos[5]+1, 0x0C);
        } else if(flash>=250 && flash<500){
          setCol(digPos[4], 0x30);
          setCol(digPos[4]+1, 0x10);
          setCol(digPos[5], 0x08);
          setCol(digPos[5]+1, 0x0C);
        } else if(flash>=500 && flash<750){
          setCol(digPos[4], 0x10);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x08);
        } else if(flash>=750){
          setCol(digPos[4], 0x20);
          setCol(digPos[4]+1, 0x30);
          setCol(digPos[5], 0x0C);
          setCol(digPos[5]+1, 0x04);
        }
      }
      // ------------------  анимация секунд
      if(secLine){
        if(secLine == 1){
          for(int s=0; s<=second; s++){
            if(s<15){
              int asd = scr[s+16]|0x01;
              setCol(s+16, asd);
            } else if(s>=14 && s<45){
              int asd = scr[(NUM_MAX0+NUM_MAX1)*8-(s-NUM_MAX1*8+19)]|0x80;
              setCol((NUM_MAX0+NUM_MAX1)*8-(s-NUM_MAX1*8+19), asd);  
            } else {
              int asd = scr[s-44]|0x01;
              setCol(s-44, asd);
            }
          }
        } else if(secLine == 2){
          for(int s=0; s<=second; s++){
            if(s<30){
              int asd = scr[s+1]|0x01;
              setCol(s+1, asd);  
            } else {
              int asd = scr[(NUM_MAX0+NUM_MAX1)*8-(s-NUM_MAX1*8+4)]|0x80;
              setCol((NUM_MAX0+NUM_MAX1)*8-(s-NUM_MAX1*8+4), asd);  
            }
          }
        } else {
          for(int s=0; s<=second; s++){
            int asd = scr[NUM_MAX0*8+(s/2)+1]|0x80;
            setCol(NUM_MAX0*8+(s/2)+1, asd);
            s++;
          }
        }
      }
      // точки в углах - обновление погоды
      if(updateForecast && WIFI_connected && displayForecast && animNotWeather) setCol(NUM_MAX0*8, flash < 500 ? 0x80 : 0x00);
      if(updateForecasttomorrow && WIFI_connected && displayForecast && animNotWeather) setCol((NUM_MAX0+NUM_MAX1)*8-1, flash < 500 ? 0x80 : 0x00);
    } else { // - кода сработал будильник
      if(flash >= 0 && flash < 500) {
        setCol(digPos[4], 0x30);
        setCol(digPos[4]+1, 0x30);
        setCol(digPos[5], 0x0C);
        setCol(digPos[5]+1, 0x0C);
      }
    }
  } else{
    if(flash < 500) setCol(digPos2[5]+(NUM_MAX0*8), clock2line==4?0x80:0x40);
    else setCol(digPos2[5]+1+(NUM_MAX0*8), clock2line==4?0x80:0x40);
    showDigit((second/10)%10, digPos2[4], clock2line==4?dig3x7:dig3x6, 1);
    showDigit(second%10, digPos2[4]+4, clock2line==4?dig3x7:dig3x6, 1);
  }
  refreshAll();
}
//==========ДРУКУВАННЯ БІГУЧОЇ СТРОКИ *s - текст, shiftDelay - швидкість, zone - зона екрану==================
void printStringWithShift(const char* s, int shiftDelay, byte zone) {
  bool oldBigCklok = bigCklock;
  bigCklock = 0;
  runningLine = 1;
  while(*s) {
    printCharWithShift(*s, shiftDelay, zone);
    s++;
    server.handleClient();
    if(mqttOn) MQTTclient.loop();
    buttonInter();
    if(butMode != 0) {
      clr(1);
      refreshAll();
      runningLine = 0;
      return;
    }
  }
  runningLine = 0;
  bigCklock = oldBigCklok;
}
//==========ДРУКУВАННЯ БІГУЧОГО СИМВОЛУ с - символ, shiftDelay - швидкість, zone - зона екрану================
void printCharWithShift(unsigned char c, int shiftDelay, byte zone) {
  c = convert_fonts(c);
  if(c < ' ') return;
  c -= 32;
  int w = showChar(c, fontUA_RU_PL_DE, zone);
  for(int i = 0; i < w + 1; i++){
    delay(shiftDelay);
    scrollLeft(zone);
    refreshAll();
  }
}
//==========ВИІД СИМВОЛУ НА ЕКРАН ch -  код символу, *data -  посилання на шрифт, zone - зона екрану===========
int showChar(char ch, const uint8_t *data, byte zone) {
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for(i = 0; i < w; i++){
    scr[(zone?NUM_MAX0+NUM_MAX1:NUM_MAX0) * 8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i) << textDown ;
  }
  scr[(zone?NUM_MAX0+NUM_MAX1:NUM_MAX0) * 8 + i] = 0;
  return w;
}
//==========ВИІД СИМВОЛУ НА ЕКРАН ch -  код символу, col- позиція, *data -  посилання на шрифт, zone - зона екрану===========
void showDigit(char ch, int col, const uint8_t *data, byte zone) {
  if(dy < -8 | dy > 8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx + (zone ? NUM_MAX0*8:0);
  for(int i = 0; i < w; i++) {
    if(col + i >= 0 && col + i < 8 * (NUM_MAX0 + NUM_MAX1)) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v;
      else scr[col + i] |= dy > 0 ? v >> dy : v << -dy;
    }
  }
}
//======================================================================================
void setCol(int col, byte v) {
  if(dy < -8 | dy > 8) return;
  col += dx;
  if(col >= 0 && col < 8 * (NUM_MAX0 + NUM_MAX1)) {
    if(!dy) scr[col] = v; else scr[col] |= dy > 0 ? v >> dy : v << -dy;
  }
}

//==========КОНВЕРТАЦІЯ СИМВОЛІВ В РАЗІ ВИКОРИСТАННЯ УКРАЇНСЬКИХ ЛІТЕР==================
byte dualChar = 0;
unsigned char convert_fonts(unsigned char _c) {
  unsigned char c = _c;
  // конвертирование латиницы
  if(c == 208) {
    dualChar = 1;
    return 0;
  }
  else if(c == 209 || c == 210) {
    dualChar = 2;
    return 0;
  }
  if(c == 32 && dualChar != 3) {
    dualChar = 3;
    return c;
  }
  if(dualChar == 1) {
    if(c >= 144 && c < 192) {c += 48;}
    dualChar = 0;
    return c;
  }
  if(dualChar == 2) {
    if(c >= 128 && c < 144) {
      c += 112;
    } 
    switch(_c) {
      case 144: c = 133; break;
      case 145: c = 149; break;
    }
    dualChar = 0;
    return c;
  }
  // конвертирование польского и немецкого
  if(c == 195) {
    dualChar = 4;
    return 0;
  }
  if(c == 196) {
    dualChar = 5;
    return 0;
  }
  if(c == 197) {
    dualChar = 6;
    return 0;
  }
  if(dualChar == 4) {
    switch(_c) {
      case 132: c = 177; break;
      case 147: c = 166; break;
      case 150: c = 179; break;
      case 156: c = 181; break;
      case 159: c = 183; break;
      case 164: c = 178; break;
      case 179: c = 167; break;
      case 182: c = 180; break;
      case 188: c = 182; break;
    }
    dualChar = 0;
    return c;
  }
  if(dualChar == 5) {
    if(c >= 132 && c < 136) {
      c += 26;
    }
    switch(_c) {
      case 152: c = 168; break;
      case 153: c = 169; break;
    }
    dualChar = 0;
    return c;
  }
  if(dualChar == 6) {
    if(c >= 129 && c < 133) {
      c += 33;
    }
    if(c >= 154 && c < 156) {
      c += 16;
    }
    if(c >= 185 && c < 189) {
      c -= 13;
    }  
    dualChar = 0;
    return c;
  }
}
//==========
void saveChrMas(String string_t, byte lenght_off, byte number_s) {
  byte lenght = string_t.length();
  if(lenght>lenght_off) return;
  const char *s = string_t.c_str();
  
  for(int i=0; i<lenght; i++) {
    //snprintf(*memory_date_mes[number_s], 1, "%s", *s);
    s++;
  }
}
//==========ОНОВЛЕННЯ ЛОКАЛЬНОГО ЧАСУ (ЛОКАЛЬНІ ЧАСИ)===============================================================
void updateTime() {
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  long epoch = round(curEpoch + 86400L);
  epoch = (epoch % 86400L);
  hour = ((epoch % 86400L) / 3600) % 24;
  minute = (epoch % 3600) / 60;
  second = epoch % 60;
}
//==========ОНОВЛЕННЯ МЕРЕЖЕВОГО ЧАСУ (перевірка в три проходи)====================================================
void timeUpdateNTP(){
  if(!WIFI_connected) return;
  if(printCom){
    Serial.println("======== START UPDATE NTP =============================");
  }  
  statusUpdateNtpTime = 1;
  for(int timeTest = 0; timeTest < 3; timeTest++) {
    getNTPtime();
    if(printCom) {
      if(timeTest) Serial.print("          ");
      else printTime();
      Serial.println("Proba #"+String(timeTest+1)+"   "+String(g_hour)+":"+((g_minute<10)?"0":"")+String(g_minute)+":"+((g_second<10)?"0":"")+String(g_second));
    }
   // updateTime();

    hourTest[timeTest] = g_hour;
    minuteTest[timeTest] = (g_minute || (g_minute==59?0:g_minute++));
    if(statusUpdateNtpTime == 0) {
      if(printCom) {
        printTime();
        Serial.print("ERROR TIME!!!\r\n");
      }
        return;
    }
    if(timeTest > 0) {
      if((hourTest[timeTest] != hourTest[timeTest - 1]||minuteTest[timeTest] != minuteTest[timeTest - 1])) {
        statusUpdateNtpTime = 0;
        if(printCom) {
          printTime();
          Serial.print("ERROR TIME!!!\r\n");
          Serial.println("======== END ==========================================");
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
  if(rtcStat){
    hour_rtc = hour;
    minute_rtc = minute;
    second_rtc = second;
    month_rtc = month;
    day_rtc = day;
    dayOfWeek_rtc = dayOfWeek;
    year_rtc = year;
    printTime();
    setRTCDateTime();
  }
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  if(printCom) {
    printTime();
    Serial.println("localEpoc = " + String(localEpoc));
    Serial.println("======== END ==========================================");
  }
  saveTime();
}
//==========ОТРИМАННЯ ДАТИ ТА ЧАСУ ВІД СЕРВЕРА ТОЧНОГО ЧАСУ =============================================================
void getNTPtime(){
  WiFi.hostByName(ntpServerName.c_str(), timeServerIP); 
  int cb;
  for(int i = 0; i < 3; i++){
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    udp.beginPacket(timeServerIP, 123);                     //NTP порт 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
    delay(800);                                             // чекаємо пів секуни
    cb = udp.parsePacket();
    if(!cb && printCom) Serial.println("          no packet yet..." + String (i + 1)); 
    if(!cb && i == 2) {                                              // якщо час не отримано
      statusUpdateNtpTime = 0;
      return;                                             // вихіз з getNTPtime()
    }
    if(cb) i = 3;
  }
  if(cb) {                                                   // якщо отримали пакет з серверу
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;        // Unix час станом на 1 січня 1970. в секундах, то 2208988800:
    unsigned long epoch = secsSince1900 - seventyYears;
    epochNM = epoch - (millis()/1000);
    boolean summerTime;
    if(month < 3 || month > 10) summerTime = false;             // не переходимо на літній час в січні, лютому, листопаді і грудню
    if(month > 3 && month < 10) summerTime = true;              // Sommerzeit лічимо в квіні, травні, червені, липні, серпені, вересені
    if(month == 3 && (hour + 24 * day) >= (3 + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (3 + 24 * (31 - (5 * year / 4 + 1) % 7))) summerTime = true; 
    epoch = epoch + (int)(timeZone*3600 + (3600*(isDayLightSaving && summerTime)));
    hourCorr = timeZone + (isDayLightSaving && summerTime);
    g_year = 0;
    int days = 0;
    uint32_t time;
    time = epoch/86400;
    g_hour = (epoch % 86400L) / 3600;
    g_minute = (epoch % 3600) / 60;
    g_second = epoch % 60;
    g_dayOfWeek = (((time) + 4) % 7) + 1;
    while((unsigned)(days += (LEAP_YEAR(g_year) ? 366 : 365)) <= time) {
      g_year++;
    }
    days -= LEAP_YEAR(g_year) ? 366 : 365;
    time -= days;
    days = 0;
    g_month = 0;
    uint8_t monthLength = 0;
    for(g_month = 0; g_month < 12; g_month++){
      if(g_month == 1){
        if(LEAP_YEAR(g_year)) monthLength = 29;
        else monthLength = 28;
      }
      else monthLength = monthDays[g_month];
      if(time >= monthLength) time -= monthLength;
      else break;
    }
    g_month++;
    g_day = time + 1;
    g_year += 1970;
    return;
  }
  if(printCom) Serial.println("Nie ma czasu(((");
}
//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ  https://www.weatherbit.io                                                 //
//===============================================================================================================================//
void getWeatherData0() {
  if(weatherKey0=="" || !displayForecast) return;
  if(!WIFI_connected) {
    updateForecast++;
    return;
  }
  if(printCom) {
    Serial.println("======== START GET WEATHER FROM WEATHERBIT.IO =========");
    printTime();
  }  
  location_name = "";
  location_region = "";
  location_country = "";
  location_localtime = "";
  location_temp = 0;
  location_app_temp = 0;
  location_rh = 0;
  location_pres = 0;
  location_wind_spd = 0;
  location_wind_cdir_full = "";
  location_sunrise = "";
  location_sunset = "";
  location_clouds = 0;
  location_vis = 0;
  location_uv = 0;
  location_weather_description = "";
  if(ESPclient.connect(weatherHost0.c_str(), 80)){}
  else {
    if(printCom){
      Serial.println(" Not connection server!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost0+"/v2.0/current/daily?city="+urlencode(cityID0)+"&lang="+weatherLang+"&key="+weatherKey0;
  if(printCom) Serial.print(reqline);
  if(http.begin(ESPclient, reqline)){
    int httpCode = http.GET();
    if(httpCode > 0) {
      if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {
      if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      updateForecast++;
      return;
    }
    http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(37) + 1128; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  JsonObject data = doc["data"][0];
  location_rh = data["rh"]; // 69
  location_pres = data["pres"]; // 999.3
  if(pressSys == 1) location_pres /= 1.3332239;
  const char* data_timezone = data["timezone"]; // "Europe/Kiev"
  location_region = data_timezone;
  const char* data_ob_time = data["ob_time"]; // "2019-09-19 17:57"
  location_localtime = data_ob_time;
  const char* data_country_code = data["country_code"]; // "UA"
  location_country = data_country_code;
  location_clouds = data["clouds"]; // 4
  const char*   data_city_name = data["city_name"]; // "Kiev"
  location_name = data_city_name;
  location_wind_spd = data["wind_spd"]; // 1
  const char* data_wind_cdir_full = data["wind_cdir_full"]; // "пі́вдень-пі́вдень-схід"
  location_wind_cdir_full = data_wind_cdir_full;
  location_vis = data["vis"]; // 5
  const char*   data_sunset = data["sunset"]; // "16:01"
  location_sunset = data_sunset;
  location_uv = data["uv"]; // 0
  int           data_wind_dir = data["wind_dir"]; // 166
  const char*   data_sunrise = data["sunrise"]; // "03:39"
  location_sunrise = data_sunrise;   //int         data_dni = data["dni"]; // 0
  JsonObject    data_weather = data["weather"];
  const char*   data_weather_description = data_weather["description"]; // "ясного неба"
  location_weather_description = data_weather_description;
  location_temp = data["temp"]; // 10.6
  location_app_temp = data["app_temp"]; // 10.6
  String windDegString;
  if(data_wind_dir >= 345 || data_wind_dir <= 22)  windDegString = "\211";    //"Північний";
  if(data_wind_dir >= 23  && data_wind_dir <= 68)  windDegString = "\234";    //"Північно-східний";
  if(data_wind_dir >= 69  && data_wind_dir <= 114) windDegString = "\230";    //"Східний";
  if(data_wind_dir >= 115 && data_wind_dir <= 160) windDegString = "\235";    //"Південно-східний";
  if(data_wind_dir >= 161 && data_wind_dir <= 206) windDegString = "\210";    //"Південний";
  if(data_wind_dir >= 207 && data_wind_dir <= 252) windDegString = "\232";    //"Південно-західний";
  if(data_wind_dir >= 253 && data_wind_dir <= 298) windDegString = "\231";    //"Західний";
  if(data_wind_dir >= 299 && data_wind_dir <= 344) windDegString = "\233";    //"Північно-західний";
  weatherString = "         ";
  if(displayCityName){
    String PCN=personalCityName;
    if(PCN.length() > 0) weatherString += PCN;
    else weatherString += String(location_name);
    weatherString += ", ";
  }
  if(displayForecastNow){
    weatherString += tNow + ":    \212 " + String(location_temp, 1) + ("\202") + "C";
    weatherString += "     \213 " + String(location_rh) + "%"; 
    weatherString += "     \215 " + String((location_pres), 0) + (pressSys == 1 ? tPress : tPress0);
    weatherString += "     \214 " + windDegString + String(location_wind_spd, 1) + tSpeed;
    weatherString += "     \216 " + String(location_clouds) + "%     " + data_weather_description + "                ";
  }
  updateForecast = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}
//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ  openweathermap.org                                                     //
//===============================================================================================================================//
void getWeatherData1(){
  if(weatherKey1=="" || !displayForecast) return;
  if(!WIFI_connected){
    updateForecast++;
    return;
  }
  if(printCom){                                                            //
    Serial.println("======== START GET WEATHER FROM OPENWEATHERMAP.ORG ====");
    printTime();
  } 
  location_name = "";
  location_region = "";
  location_country = "";
  location_localtime = "";
  location_temp = 0;
  location_app_temp = 0;
  location_rh = 0;
  location_pres = 0;
  location_wind_spd = 0;
  location_wind_cdir_full = "";
  location_sunrise = "";
  location_sunset = "";
  location_clouds = 0;
  location_vis = 0;
  location_uv = 0;
  location_weather_description = "";
  if(ESPclient.connect(weatherHost1.c_str(), 80)){}
  else{
    if(printCom){
      Serial.println(" Not connection server!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost1+"/data/2.5/weather?id="+urlencode(cityID1)+"&lang="+weatherLang+"&units=metric&appid="+weatherKey1;
  if(printCom) Serial.print(reqline); 
  if(http.begin(ESPclient, reqline)){
    int httpCode = http.GET();
    if(httpCode > 0) {
      if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
       line = http.getString();
      }
    } else {
      if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      updateForecast++;
      return;
    }
    http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(13) + 751; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  JsonObject weather_0 = doc["weather"][0];
  const char*   data_weather_description = weather_0["description"]; // "fog"
  location_weather_description = data_weather_description;
  JsonObject main = doc["main"];
  location_temp = main["temp"]; // 10.34
  location_pres = main["pressure"]; // 1023
  if(pressSys == 1) location_pres /= 1.3332239;
  location_rh = main["humidity"]; // 100
  float location_temp_min = main["temp_min"]; // 7
  float location_temp_max = main["temp_max"]; // 12.22
  location_vis = doc["visibility"]; // 1000
  location_vis /= 1000;
  location_wind_spd = doc["wind"]["speed"]; // 1
  int data_wind_dir = doc["wind"]["deg"]; // 230
  location_clouds = doc["clouds"]["all"]; // 20
  JsonObject sys = doc["sys"];
  const char* data_country_code = sys["country"]; // "UA"
  location_country = data_country_code;
  const char*   data_city_name = doc["name"]; // "Kyiv"
  location_name = data_city_name;
  String windDegString;
  if(data_wind_dir >= 345 || data_wind_dir <= 22)  windDegString = "\211";    //"Північний";
  if(data_wind_dir >= 23  && data_wind_dir <= 68)  windDegString = "\234";    //"Північно-східний";
  if(data_wind_dir >= 69  && data_wind_dir <= 114) windDegString = "\230";    //"Східний";
  if(data_wind_dir >= 115 && data_wind_dir <= 160) windDegString = "\235";    //"Південно-східний";
  if(data_wind_dir >= 161 && data_wind_dir <= 206) windDegString = "\210";    //"Південний";
  if(data_wind_dir >= 207 && data_wind_dir <= 252) windDegString = "\232";    //"Південно-західний";
  if(data_wind_dir >= 253 && data_wind_dir <= 298) windDegString = "\231";    //"Західний";
  if(data_wind_dir >= 299 && data_wind_dir <= 344) windDegString = "\233";    //"Північно-західний";
  weatherString = "         ";
  if(displayCityName){
    String PCN=personalCityName;
    if(PCN.length() > 0) weatherString += PCN;
    else weatherString += String(location_name);
    weatherString += ", ";
  }
  if(displayForecastNow){
    weatherString += tNow + ":    \212 "+String(location_temp, 1)+" ("+String(location_temp_min,1)+"..."+String(location_temp_max,1)+")"+("\202")+"C";
    weatherString += "     \213 " + String(location_rh) + "%";
    weatherString += "     \215 " + String((location_pres), 0) + (pressSys == 1 ? tPress : tPress0) ;
    weatherString += "     \214 " + windDegString + String(location_wind_spd, 1) + tSpeed;
    weatherString += "     \216 " + String(location_clouds) + "%     " + data_weather_description + "                ";
  }
  updateForecast = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}
// ============================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту https://www.weatherbit.io     // 
// ============================================================================//
void getWeatherDataz0() {
  if(weatherKey0=="" || !displayForecast) return;
  if(!WIFI_connected) {
    updateForecasttomorrow++;
    return;
  }
  if(printCom){
    Serial.println("======== START GET FORECAST FROM WEATHERBIT.IO ========");
    printTime();
  } 
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost0+"/v2.0/forecast/daily?city="+urlencode(cityID0)+"&lang="+weatherLang+"&days=2&key="+weatherKey0;
  if(printCom) Serial.print(reqline);   
  if(http.begin(ESPclient, reqline)){
   int httpCode = http.GET();
   if(httpCode > 0) {
     if(printCom) Serial.printf("  [HTTP] GET... code: %d\n", httpCode);
     if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      line = http.getString();
     }
   } else {
     if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
     http.end();
     updateForecasttomorrow++;
     return;
   }
   http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(7) + 2*JSON_OBJECT_SIZE(37) + 2321;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  JsonObject data_0 = doc["data"][0];
  JsonObject data_0_weather = data_0["weather"];
  const char* data_0_weather_description = data_0_weather["description"]; // "Помірний дощ"
  float data_0_max_temp = data_0["max_temp"]; // 13.4
  float data_0_min_temp = data_0["min_temp"]; // 10.9
  JsonObject data_1 = doc["data"][1];
  int data_1_rh = data_1["rh"]; // 75
  int data_1_clouds = data_1["clouds"]; // 58
  float data_1_wind_spd = data_1["wind_spd"]; // 3.75302
  JsonObject data_1_weather = data_1["weather"];
  const char* data_1_weather_description = data_1_weather["description"]; // "Світло душ дощ"
  float data_1_max_temp = data_1["max_temp"]; // 16.3
  float data_1_min_temp = data_1["min_temp"]; // 10
  weatherStringZ = "";
  if(displayForecastToday){
    if(hour<18) weatherStringZ += tCurr + ":";
    if(hour<12) weatherStringZ += "   \212" + String(data_0_min_temp, 1) + "...." + String(data_0_max_temp, 1) + "\202" + "C  ";
    if(hour<18) weatherStringZ += "   " + String(data_0_weather_description) + "     ";
  }
  if(displayForecastTomorrow) {
    weatherStringZ += tTom + ":   \212" + String(data_1_min_temp, 1) + "...." + String(data_1_max_temp, 1) + "\202" + "C";
    weatherStringZ += "     \213 " + String(data_1_rh) + "%";
    weatherStringZ += "     \214 " + String(data_1_wind_spd, 1) + tSpeed;
    weatherStringZ += "     " + String(data_1_weather_description);
    weatherStringZ += "                   ";
  }
  if(printCom) Serial.println("          Getting weather forecast for tomorrow - is OK.");
  updateForecasttomorrow = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}
// =======================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту openweathermap.org      //
// =======================================================================//
void getWeatherDataz1(){
  if(weatherKey1=="" || !displayForecast) return;
  if(!WIFI_connected) {
    updateForecasttomorrow++;
    return;
  }
  if(printCom){
    Serial.println("======== START GET FORECAST FROM OPENWEATHERMAP.ORG ===");
    printTime();
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost1+"/data/2.5/forecast/daily?id="+urlencode(cityID1)+"&units=metric&appid="+weatherKey1+"&lang="+weatherLang+"&cnt=2";
  if(printCom) Serial.print(reqline);   
  if(http.begin(ESPclient, reqline)){
   int httpCode = http.GET();
   if (httpCode > 0) {
     if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
     if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      line = http.getString();
     }
   } else {
     if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
     http.end();
     updateForecasttomorrow++;
     return;
   }
   http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  const size_t capacity = 2*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 3*JSON_OBJECT_SIZE(6) + 2*JSON_OBJECT_SIZE(10) + 1281;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  JsonObject city = doc["city"];
  JsonObject list_0 = doc["list"][0];
  JsonObject list_0_temp = list_0["temp"];
  float data_0_min_temp = list_0_temp["min"]; // 9.63
  float data_0_max_temp = list_0_temp["max"]; // 9.69
  JsonObject list_0_weather_0 = list_0["weather"][0];
  const char* data_0_weather_description = list_0_weather_0["description"]; // "облачно"
  JsonObject list_1 = doc["list"][1];
  JsonObject list_1_temp = list_1["temp"];
  float data_1_day_temp = list_1_temp["day"]; // 18.13
  float data_1_min_temp = list_1_temp["min"]; // 10.67
  float data_1_max_temp = list_1_temp["max"]; // 20.32
  int data_1_rh = list_1["humidity"]; // 56
  JsonObject list_1_weather_0 = list_1["weather"][0];
  const char* data_1_weather_description = list_1_weather_0["description"]; // "слегка облачно"
  float data_1_wind_spd = list_1["speed"]; // 0.7
  int data_1_clouds = list_1["clouds"]; // 38
  weatherStringZ = "";
  if(displayForecastToday){
    if(hour<18) weatherStringZ += tCurr + ":";
    if(hour<12) weatherStringZ += "   \212" + String(data_0_min_temp, 1) + "...." + String(data_0_max_temp, 1) + "\202" + "C  ";
    if(hour<18) weatherStringZ += "   " + String(data_0_weather_description) + "     ";
  }
  if(displayForecastTomorrow) {
    weatherStringZ += tTom + ":   \212" + String(data_1_min_temp, 1) + "...." + String(data_1_day_temp, 1) + "...." + String(data_1_max_temp, 1) + "\202" + "C";
    weatherStringZ += "     \213 " + String(data_1_rh) + "%";
    weatherStringZ += "     \214 " + String(data_1_wind_spd, 1) + tSpeed;
    weatherStringZ += "     \216 " + String(data_1_clouds) + "%     "  + String(data_1_weather_description);
    weatherStringZ += "                   ";
  }
  updateForecasttomorrow = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}
//=========================================================================================================
//                                  narodmon.ru
void getNarodmon(){
  nMonFound = false;
  if(!WIFI_connected)  return;
  if(printCom){
    Serial.println("======== START ========================================");
    printTime();
    Serial.print("GET NARODMON: ");
  }
  if(ESPclient.connect("http://narodmon.ru", 80)) {
    if(printCom){
      Serial.println("No connection to server!!!");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  if(!sensors_ID0){
    if(printCom){
      Serial.println("No ID sensor(0)");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  String line="";
  String reqline="http://narodmon.ru/api/sensorsValues?sensors=";
  if(sensors_ID0) reqline+=String(sensors_ID0);
  if(sensors_ID1) reqline+=","+String(sensors_ID1);
  if(sensors_ID2) reqline+=","+String(sensors_ID2);
  reqline+="&uuid="+uuid+"&api_key="+api_key;
  HTTPClient http;
  if(printCom) Serial.print(reqline);
  if(http.begin(ESPclient, reqline)){
    int httpCode = http.GET();
    if(httpCode>0){
      if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
        line = http.getString();
      }
    } else {
      if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      return;
    }
    http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(6) + 340; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    return;
  }
  JsonObject sensors_0 = doc["sensors"][0];
  float sensors_0_value = sensors_0["value"]; // 14.2
  long sensors_0_time = sensors_0["time"]; // 1571853360
  JsonObject sensors_1 = doc["sensors"][1];
  float sensors_1_value = sensors_1["value"]; // 14
  long sensors_1_time = sensors_1["time"]; // 1571853000
  JsonObject sensors_2 = doc["sensors"][2];
  float sensors_2_value = sensors_2["value"];
  long sensors_2_time = sensors_2["time"];
  long timestamp = epochNM + (millis()/1000);
  /*if(printCom){
    printTime();
    Serial.println("sensors_0 = "+String(sensors_0_value, 1)+"'C    sensors_1 = "+String(sensors_1_value, 1)+"'C    sensors_2 = "+String(sensors_2_value, 1)+"'C");
    Serial.println("time_0 = "+String(timestamp-sensors_0_time)+"      time_1 = "+String(timestamp-sensors_1_time)+"      time_2 = "+String(timestamp-sensors_2_time));
  }*/
  tempNM=0;
  if(sensors_ID0){
    if((timestamp-sensors_0_time)>3600){
      sensors_0_value=99;
    } else {
      tempNM=sensors_0_value;
      nMonFound = true;
    }
  }
  if(sensors_ID1){
    if((timestamp-sensors_1_time)>3600) {
      sensors_1_value=99;
    } else if(tempNM>sensors_1_value){
      tempNM=sensors_1_value;
      nMonFound = true;
    }
  }
  if(sensors_ID2){
    if((timestamp-sensors_2_time)>3600){
      sensors_2_value=99;
    } else if(tempNM>sensors_2_value){
      tempNM=sensors_2_value;
      nMonFound = true;
    }
  }
  if(!tempNM&&!updateForecast) tempNM=location_temp;
  if(printCom) {
    printTime();
    Serial.println("line =" + line);
    printTime();
    Serial.println("tempNM = "+String(tempNM, 1)+"'C");
    Serial.println("======== END ==========================================");
  }
}
//=========================================================================================================
void wifiConnect(){
  if(printCom) {
    printTime();
    Serial.print("Connecting WiFi (ssid="+String(ssid.c_str())+"  pass="+String(password.c_str())+") ");
  }
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  delay (1000);
  clr(1);
  for(int i=1; i<21; i++){
    if(WiFi.status()==WL_CONNECTED){
      WIFI_connected=true;
      if(printCom) {
        Serial.print(" IP adress : ");
        Serial.println(WiFi.localIP());
      }
      if(!amountNotStarts && !bigCklock){
        String aaa = WiFi.localIP().toString() + "                ";
        if(NUM_MAX1>4){
          for(int j=0; j<NUM_MAX1-4; j++) aaa+="  ";
        }
        clr(1);
        printStringWithShift(" IP: ", 15, 1);
        printStringWithShift(aaa.c_str(), 25, 1);
      }
      timeUpdateNTP();
      amountNotStarts=0;
      firstStart = 1;
      return;
    }
    if(printCom) Serial.print(".");
    if(!amountNotStarts && !bigCklock){
      clr(1);
      showDigit(87, 0, fontUA_RU_PL_DE, 1);
      showDigit(73, 6, fontUA_RU_PL_DE, 1);
      showDigit(70, 10, fontUA_RU_PL_DE, 1);
      showDigit(73, 15, fontUA_RU_PL_DE, 1);
      if(i>9) showDigit(i / 10, 21, dig5x7, 1);
      showDigit(i % 10, 27, dig5x7, 1);
      scrollDown(1);
      refreshAll();
    }
    delay (1000);
  }
  amountNotStarts++;
  if(printCom) {
    Serial.print("Amount of the unsuccessful connecting = ");
    Serial.println(amountNotStarts);
  }
  if(amountNotStarts>21) {
    amountNotStarts=0;
    //ESP.reset();
  }
  if(!firstStart) {
    firstStart = 1;
    apStart = 1;
    WiFi.disconnect();
    if(printCom) Serial.println(" Not connected!!!");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());
    updateTime();
    if(printCom) {
      printTime();
      Serial.println("Start AP mode!!!");
      Serial.print("          Wifi AP IP : ");
      Serial.println(WiFi.softAPIP());
    }
    String aaa = tPoint + " " + ssidAP;
    if(passwordAP != "") aaa += ",  " + tPass + ": " + passwordAP;
    aaa += " " + tIp + ": 192.168.4.1           ";
    clr(1);
    printStringWithShift(aaa.c_str(), 35, 1);
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sensorsAll() {
  if(printCom) Serial.println("======== START GET SENSORS DATA =======================");
  sensorsDs18b20();
  sensorsSi7021();
  sensorsAHTx0();
  sensorsBmp();
  sensorsBme();
  sensorsDht();
  sensors();
  if(printCom) Serial.println("======== END ==========================================");
}
//-------------------------------------------------------------------------
void sensors() {
  data00 = (sensor00==0?0:sensor00==1?tempDs18b20:sensor00==2?(param0>4?humiSi7021:tempSi7021):sensor00==3?(param0>4?pressBmp:tempBmp):sensor00==4?(param0<5?tempBme:param0<20?humiBme:pressBme):sensor00==5?(param0>4?humiDht:tempDht):sensor00==6?tMqtt1:sensor00==7?tMqtt2:sensor00==8?tMqtt3:sensor00==9?tempNM:sensor00==10?(param0>4?humiAHTx0:tempAHTx0):sensor00==11?readDataField0:sensor00==12?readDataField1:sensor00==13?readDataField2:sensor00==14?readDataField3:sensor00==15?readDataField4:0);
  if(sensor00) data00 += corr00;
  data01 = (sensor01==0?0:sensor01==1?tempDs18b20:sensor01==2?(param1>4?humiSi7021:tempSi7021):sensor01==3?(param1>4?pressBmp:tempBmp):sensor01==4?(param1<5?tempBme:param1<20?humiBme:pressBme):sensor01==5?(param1>4?humiDht:tempDht):sensor01==6?tMqtt1:sensor01==7?tMqtt2:sensor01==8?tMqtt3:sensor01==9?tempNM:sensor01==10?(param1>4?humiAHTx0:tempAHTx0):sensor01==11?readDataField0:sensor01==12?readDataField1:sensor01==13?readDataField2:sensor01==14?readDataField3:sensor01==15?readDataField4:0);
  if(sensor01) data01 += corr01;
  data02 = (sensor02==0?0:sensor02==1?tempDs18b20:sensor02==2?(param2>4?humiSi7021:tempSi7021):sensor02==3?(param2>4?pressBmp:tempBmp):sensor02==4?(param2<5?tempBme:param2<20?humiBme:pressBme):sensor02==5?(param2>4?humiDht:tempDht):sensor02==6?tMqtt1:sensor02==7?tMqtt2:sensor02==8?tMqtt3:sensor02==9?tempNM:sensor02==10?(param2>4?humiAHTx0:tempAHTx0):sensor02==11?readDataField0:sensor02==12?readDataField1:sensor02==13?readDataField2:sensor02==14?readDataField3:sensor02==15?readDataField4:0);
  if(sensor02) data02 += corr02;
  data03 = (sensor03==0?0:sensor03==1?tempDs18b20:sensor03==2?(param3>4?humiSi7021:tempSi7021):sensor03==3?(param3>4?pressBmp:tempBmp):sensor03==4?(param3<5?tempBme:param3<20?humiBme:pressBme):sensor03==5?(param3>4?humiDht:tempDht):sensor03==6?tMqtt1:sensor03==7?tMqtt2:sensor03==8?tMqtt3:sensor03==9?tempNM:sensor03==10?(param3>4?humiAHTx0:tempAHTx0):sensor03==11?readDataField0:sensor03==12?readDataField1:sensor03==13?readDataField2:sensor03==14?readDataField3:sensor03==15?readDataField4:0);
  if(sensor03) data03 += corr03;
  data04 = (sensor04==0?0:sensor04==1?tempDs18b20:sensor04==2?(param4>4?humiSi7021:tempSi7021):sensor04==3?(param4>4?pressBmp:tempBmp):sensor04==4?(param4<5?tempBme:param4<20?humiBme:pressBme):sensor04==5?(param4>4?humiDht:tempDht):sensor04==6?tMqtt1:sensor04==7?tMqtt2:sensor04==8?tMqtt3:sensor04==9?tempNM:sensor04==10?(param4>4?humiAHTx0:tempAHTx0):sensor04==11?readDataField0:sensor04==12?readDataField1:sensor04==13?readDataField2:sensor04==14?readDataField3:sensor04==15?readDataField4:0);
  if(sensor04) data04 += corr04;
  if(printCom){
    if(sensor00){
      printTime();
      Serial.println("sensor00(+corr) = " + String(data00));
    }
    if(sensor01){
      printTime();
      Serial.println("sensor01(+corr) = " + String(data01));
    }
    if(sensor02){
      printTime();
      Serial.println("sensor02(+corr) = " + String(data02));
    }
    if(sensor03){
      printTime();
      Serial.println("sensor03(+corr) = " + String(data03));
    }
    if(sensor04){
      printTime();
      Serial.println("sensor04(+corr) = " + String(data04));
    }
  }
  if(setSgpCorr){
    if(setSgpCorr==1 && bmp280Found){
      sgpCorrTemp = tempBme;
      sgpCorrHumi = humiBme;
    }
    if(setSgpCorr==2 && si7021Found){
      sgpCorrTemp = tempSi7021;
      sgpCorrHumi = humiSi7021;
    }
    if(setSgpCorr==3 && AHTx0Found){
      sgpCorrTemp = tempAHTx0;
      sgpCorrHumi = humiAHTx0;
    }
    if(setSgpCorr==4 && dhtFound){
      sgpCorrTemp = tempDht;
      sgpCorrHumi = humiDht;
    }
  }
  if((sensor00==4&&sensor02==4&&bmp280Found)||(sensor00==2&&sensor02==2&&si7021Found)||(sensor00==10&&sensor02==10&&AHTx0Found)||(sensor00==5&&sensor02==5&&dhtFound)){
    if(param4==21&&sensor04==99){
      float absH=(6.112*(pow(2.718281828,(17.67*data00)/(data00+243.5)))*data02*2.1674)/(273.15+data00);
      if(printCom){
        printTime();
        Serial.println("Absolute Humi = " + String(absH) + "g/m3");
      }
      data04=absH;
    }
    if(param3==3&&sensor03==99){
      float ans=(data00-(14.55+0.114*data00)*(1-(0.01*data02))-pow(((2.5+0.007*data00)*(1-(0.01*data02))),3)-(15.9+0.117*data00)*pow((1-(0.01*data02)),14));
      if(printCom){
        printTime();
        Serial.println("Dew point = " + String(ans) + "*C");
      }
      data03=ans;
    }
  }
}
//--------------------------------------------------------------------------
void sensorsDs18b20(){  //1
  byte present = 0;
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
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);
  for(byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if(type_s) {
    raw = raw << 3;
    if(data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if(cfg == 0x00) raw = raw & ~7;
    else if(cfg == 0x20) raw = raw & ~3;
    else if (cfg == 0x40) raw = raw & ~1;
  }
  tempDs18b20 = (float)raw / 16.00;
  if(printCom) {
    printTime();
    Serial.println("Temperature DS18B20: " + String(tempDs18b20) + " *C");
  }
}
//--------------------------------------------------------------------------
void sensorsSi7021() {  //2
  if(si7021Found == false) return;
  humiSi7021 = sensor.readHumidity();
  tempSi7021 = sensor.readTemperature();
  if(printCom) {
    printTime();
    Serial.println("Temperature Si7021: " + String(tempSi7021) + " *C,  Humidity: " + String(humiSi7021) + " %");
  }
}
//--------------------------------------------------------------------------
void sensorsAHTx0() {  //2
  if(AHTx0Found == false) return;
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  humiAHTx0 = humidity.relative_humidity;
  tempAHTx0 = temp.temperature;
  if(printCom) {
    printTime();
    Serial.println("Temperature AHTx0: " + String(tempAHTx0) + " *C,  Humidity: " + String(humiAHTx0) + " %");
  }
}
//--------------------------------------------------------------------------
void sensorsBmp() {  //3
  if(bmp280Found == true) {
    tempBmp = bmp.readTemperature();
    pressBmp = bmp.readPressure() * (pressSys == 1 ? 0.7500613026439 : 1);
    pressBmp = (int) pressBmp;
    altBmp = bmp.readAltitude(1013.25);
    if(printCom) {
      printTime();
      Serial.println("Temperature BMP280: " + String(tempBmp) + " *C,  Pressure: " + String(pressBmp) + (pressSys == 1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBmp) + " m");
    }
  }
  if(bmp180Found == true) {
    tempBmp = bmp180.readTemperature();
    pressBmp = bmp180.readPressure() * (pressSys == 1 ? 0.7500613026439 : 1);
    pressBmp = (int) pressBmp;
    altBmp = bmp180.readAltitude(101500);
    if(printCom) {
      printTime();
      Serial.println("Temperature BMP180: " + String(tempBmp) + " *C,  Pressure: " + String(pressBmp) + (pressSys == 1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBmp) + " m");
    }
  }
}
//--------------------------------------------------------------------------
void sensorsBme() {  //4
  if(bme280Found == false) return;
  tempBme = bme.readTempC();          //bme.readTempF()
  humiBme = bme.readHumidity();
  pressBme = bme.readPressure() / (pressSys == 1 ? 1.3332239 : 1);
  pressBme = (int) pressBme;
  altBme = bme.readAltitudeFeet();   //bme.readAltitudeMeter()  bme.readAltitudeFeet()
  if(printCom) {
    printTime();
    Serial.println("Temperature BME280: " + String(tempBme) + " *C,  Humidity: " + String(humiBme) + " %,  Pressure: " + String(int(pressBme)) + (pressSys == 1 ? " мм рт.ст." : " hPa") + ",  Approx altitude: " + String(altBme) + " m");
  }
}
//--------------------------------------------------------------------------
void sensorsDht() {   //5
  dhtFound=false;
  int err = SimpleDHTErrSuccess;
  int err2 = SimpleDHTErrSuccess;
  byte temp11 = 0;
  byte humi11 = 0;
  if((err2 = dht22.read2(pinDHT, &tempDht, &humiDht, NULL)) == SimpleDHTErrSuccess) {
    humiDht = int(humiDht);
    if(printCom) {
      printTime();
      Serial.println("Temperature DHT22: " + String(tempDht) + " *C,  Humidity: " + String(humiDht) + " %");
    }
    dhtFound=true;
  }
  if((err = dht11.read(pinDHT, &temp11, &humi11, NULL)) == SimpleDHTErrSuccess) {
    tempDht = temp11;
    humiDht = humi11;
    if(printCom) {
      printTime();
      Serial.println("Temperature DHT11: " + String(tempDht) + " *C,  Humidity: " + String(humiDht) + " %");
    }
    dhtFound=true;
  }
}
//--------------------------------------------------------------------------
void printTime() {
  if(printCom) Serial.print((hour < 10 ? "0" : "") + String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second) + "  ");
}
//--------------------------------------------------------------------------
void bip(){
  if(!buzzerSet){
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
byte alarms() {
  for(byte i = 0; i < 5; i++) {
    if(alarme[i][0]==hour&&alarme[i][1]==minute&&(alarme[i][2]==dayOfWeek||(alarme[i][2]==8&&(dayOfWeek>1&&dayOfWeek<7))||(alarme[i][2]==9&&dayOfWeek>1)||(alarme[i][2]==10&&(dayOfWeek==1||dayOfWeek==7))||alarme[i][2]>10)){
      alarm_numer=i;
      return 1;
    }
  }
  alarm_numer=255;
  return 0;
}
//------------ function urlencode for weather parameters --------------------
String urlencode(String str) {    // функция взята от http://forum.amperka.ru/members/benben.19545/
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
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
String chr_to_str(String str) {
  String chr_to_str = "";
  for (int i =0; i < str.length(); i++){
    chr_to_str += str.charAt(i);
  }
  return chr_to_str;
}
// ===========================КОНВЕРТАЦІЯ НАЗВ ДНІВ ТИЖНЯ НА УКРАЇНСЬКУ МОВУ============================================
void convertDw(){
  switch(dayOfWeek){
    case 1 : dw = tSunday;    break;
    case 2 : dw = tMonday;    break;
    case 3 : dw = tTuesday;   break;
    case 4 : dw = tWednesday; break;
    case 5 : dw = tThursday;  break;
    case 6 : dw = tFriday;    break;
    case 7 : dw = tSaturday;  break;
  }
}
// ===========================КОНВЕРТАЦІЯ НАЗВ МІСЯЦІВ НА УКРАЇНСЬКУ МОВУ============================================
void convertMonth(){
  switch(month){
    case 1 : _month = tJanuary;      break;
    case 2 : _month = tFebruary;     break;
    case 3 : _month = tMarch;        break;
    case 4 : _month = tApril;        break;
    case 5 : _month = tMay;          break;
    case 6 : _month = tJune;         break;
    case 7 : _month = tJuly;         break;
    case 8 : _month = tAugust;       break;
    case 9 : _month = tSeptember;    break;
    case 10 : _month = tOctober;     break;
    case 11 : _month = tNovember;    break;
    case 12 : _month = tDecember;    break;
  }
}
//-------------------------------------------------------------------------
void buttonInter(){
  if(digitalRead(BUT_PIN) == butStat && butCount == 0 && butFlag == 0  && butMode == 0){
    butCount = millis();
    butFlag = 1;
  }
  if((millis() - butCount) >= 30000 && butFlag == 1 && butMode == 0){
    butMode = 4;
    if(printCom) Serial.println("BUT MODE 4");
    butFlag = 0;
    butCount = 0;
  }
  if(digitalRead(BUT_PIN) == !butStat && (millis() - butCount) >= 10000 && butFlag == 1 && butMode == 0) {
    butMode = 3;
    if(printCom) Serial.println("BUT MODE 3");
    butFlag = 0;
    butCount = 0;
  }
  if(digitalRead(BUT_PIN) == !butStat && (millis() - butCount) < 10000 && (millis() - butCount) > 800 && (butFlag == 1 || butFlag == 2) && butMode == 0){
    butMode = 1;
    if(printCom) Serial.println("BUT MODE 1");
    butFlag = 0;
    butCount = 0;
  }
  if(digitalRead(BUT_PIN) == !butStat && (millis() - butCount) <= 800 && butFlag == 1 && butMode == 0) butFlag = 2;
  if(digitalRead(BUT_PIN) == butStat && (millis() - butCount) <= 800  && butFlag == 2){
    butMode = 2;
    if(printCom) Serial.println("BUT MODE 2");
    butFlag = 0;
    butCount = 0;
  }
}
//------------- Обробка функцій кнопки -------------------------------------------------------
void buttonHandling() {
  if(alarm_stat && (butMode||stopAlarm)) { // если будильник работает, то любое нажатие выключает его
    alarm_stat = 0;
    alarm_hold = 1;
    stopAlarm = false;
    if(printCom) Serial.println("BUT MODE 0");
    butMode = 0;
    if(alarme[alarm_numer][2] == 11) alarme[alarm_numer][2] = 0;
  }
  if(butMode == 4){ // если кнопка нажата была более 30 секунд то возврат к заводским установкам
    if(printCom) Serial.println("BUT MODE 0");
    butMode = 0;
    if(printCom) Serial.println("Ta to jest KAPUT!!!!!!");
    bip();
    bip();
    bip();
    bip();
    SPIFFS.remove("/config.json");
    if(printCom) {
      printTime();
      Serial.println("ESP erase Config file");
    }
    delay(3000);
    ESP.reset();
  }
  if(butMode == 3){ // если кнопка была нажата более 10 секунд но менее 30, то будет рестарт часов
    if(printCom) Serial.println("BUT MODE 0");
    butMode = 0;
    if(printCom) Serial.println("Reset ESP!!!");
    bip();
    bip();
    bip();
    ESP.reset();
  }
  if(butMode == 1) {
    bip();
    clr(1);
    refreshAll();
    if(sensor00) {
      showSimple(0);
      delay(1500);
    }
    if(sensor01) {
      showSimple(1);
      delay(1500);
    }
    if(sensor02) {
      showSimple(2);
      delay(1500);
    }
    if(sensor03) {
      showSimple(3);
      delay(1500);
    }
    if(sensor04) {
      showSimple(4);
      delay(1500);
    }
    butMode = 0;
    if(printCom) Serial.println("BUT MODE 0");
    clr(1);
    refreshAll();
  }
  if(butMode == 2){ // При двойном нажатии на кнопку выводится прогноз погоды
    bip();
    bip();
    butMode = 0;
    if(printCom) Serial.println("BUT MODE 0");
    clr(1);
    refreshAll();
    printStringWithShift(weatherString.c_str(), timeScrollSpeed, 1);
    printStringWithShift(weatherStringZ.c_str(), timeScrollSpeed, 1);
    clr(1);
    refreshAll();
  }
}
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

void sgp30() {
  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  if(setSgpCorr) sgp.setHumidity(getAbsoluteHumidity(sgpCorrTemp, sgpCorrHumi));
  printTime();
  if(!sgp.IAQmeasure()){
    if(printCom) Serial.println("Measurement failed");
    return;
  }
  sgpCo2 = sgp.eCO2;
  sgpTvoc = sgp.TVOC;
  if(printCom) Serial.print("TVOC = " + String(sgpTvoc) + " ppb,     ");
  if(printCom) Serial.print("eCO2 = " + String(sgpCo2) + " ppm,     ");
  if(sgpCo2<sgpCo2Livel1) sgpCo2Livel = 0;
  else if(sgpCo2<sgpCo2Livel2) sgpCo2Livel = 1;
  else if(sgpCo2<sgpCo2Livel3) sgpCo2Livel = 2;
  else if(sgpCo2<sgpCo2Livel4) sgpCo2Livel = 3;
  else sgpCo2Livel = 4;
  if(sgpTvoc<sgpTvocLivel1) sgpTvocLivel = 0;
  else if(sgpTvoc<sgpTvocLivel2) sgpTvocLivel = 1;
  else if(sgpTvoc<sgpTvocLivel3) sgpTvocLivel = 2;
  else if(sgpTvoc<sgpTvocLivel4) sgpTvocLivel = 3;
  else sgpTvocLivel = 4;
  if(!sgp.IAQmeasureRaw()) {
    if(printCom) Serial.println("Raw Measurement failed");
  } else {
    if(printCom) Serial.print("Raw H2 = " + String(sgp.rawH2) + ",     ");
    if(printCom) Serial.println("Raw Ethanol = " + String(sgp.rawEthanol));
    if (minute%5 == 0) {
      uint16_t TVOC_base, eCO2_base;
      if(!sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
        if(printCom){
          printTime();
          Serial.println("Failed to get baseline readings");
        }
        return;
      }
      if(printCom){
        printTime();
        Serial.print("****Baseline values: eCO2: 0x" + String (eCO2_base, HEX));
        Serial.println(" & TVOC: 0x" + String(TVOC_base, HEX));
      }
    }
  }
  String livelCo2 = tAlarm + sgpCo2Message[sgpCo2Livel] + " eCO2 = " + String(sgpCo2) + " ppm               ";
  if(sgpCo2Livel>=sgpCo2LivelAlarm){
    if(eCo2AlarmMqtt&&WIFI_connected) MQTTclient.publish(mqtt_sub_inform, livelCo2.c_str());
    if(eCo2AlarmEsp){
      for(int i=0; i<4; i++){
        bip();
      }
      clr(1);
      printStringWithShift(("        " + livelCo2).c_str(), timeScrollSpeed, 1);
    }
  }
  String livelTvoc = tAlarm + sgpTvocMessage[sgpTvocLivel] + " TVOC = " + String(sgpTvoc) + " ppb               ";
  if(sgpTvocLivel>=sgpTvocLivelAlarm){
    if(tvocAlarmMqtt&&WIFI_connected) MQTTclient.publish(mqtt_sub_inform, livelTvoc.c_str());
    if(tvocAlarmEsp){
      for(int i=0; i<4; i++){
        bip();
      }
      clr(1);
      printStringWithShift(("        " + livelTvoc).c_str(), timeScrollSpeed, 1);
    }
  }
}
//-----------------------------------------------------
void lang() {
  if (weatherLang == "uk") {
    tNow = "Зараз";
    tPress0 = "гПа";
    tPress = "ммРс";
    tSpeed = "м/с";
    tMin = "хв.";
    tCurr = "Сьогодні";
    tTom = "Завтра";
    tYour = "Ваш";
    tPoint = "Підключіться до";
    tIp = "та перейдіть за адресою";
    tPass = "пароль";
    tWeatrNot = "   Немає оновлень погоди більше 6 годин!!!   ";
    tWeatrTN = "Немає оновлень погоди.";
    tJanuary = "січня";
    tFebruary = "лютого";
    tMarch = "березня";
    tApril = "квітня";
    tMay = "травня";
    tJune = "червня";
    tJuly = "липня";
    tAugust = "серпня";
    tSeptember = "вересня";
    tOctober = "жовтня";
    tNovember = "листопада";
    tDecember = "грудня";
    tMonday = "Понеділок";
    tTuesday = "Вівторок";
    tWednesday = "Середа";
    tThursday = "Четвер";
    tFriday = "П'ятниця";
    tSaturday = "Субота";
    tSunday = "Неділя";
    tAlarm = "УВАГА!!!   ";
    sgpCo2Message[0] = "Допустимий рівень повітря";
    sgpCo2Message[1] = "Важке повітря. Необхідно провітрювання";
    sgpCo2Message[2] = "Можлива сонливість, втома, головний біль";
    sgpCo2Message[3] = "Серйозне погіршення здоров'я !!!";
    sgpCo2Message[4] = "Гранично допустима концентрація протягом 8 годин!!!";
    sgpTvocMessage [0] = "Допустимий рівень ЛОС";
    sgpTvocMessage [1] = "Рекомендується вентиляція";
    sgpTvocMessage [2] = "Рекомендується інтенсивна вентиляція";
    sgpTvocMessage [3] = "Інтенсивна вентиляція і провітрювання необхідні";
    sgpTvocMessage [4] = "Дуже інтенсивна вентиляція суворо необхідна !!!";
  } else if (weatherLang == "ru") {
    tNow = "Сейчас";
    tPress0 = "гПа";
    tPress = "ммРс";
    tSpeed = "м/с";
    tMin = "мин.";
    tCurr = "Сегодня";
    tTom = "Завтра";
    tYour = "Ваш";
    tPoint = "Подключитесь к точке доступа";
    tIp =  "и введите в браузере адрес";
    tPass = "пароль";
    tWeatrNot = "   Нет обновления погоды более 6 часов!!!   ";
    tWeatrTN = "Нет обновления погоды.";
    tJanuary = "января";
    tFebruary = "февраля";
    tMarch = "марта";
    tApril = "апреля";
    tMay = "мая";
    tJune = "июня";
    tJuly = "июля";
    tAugust = "августа";
    tSeptember = "сентября";
    tOctober = "октября";
    tNovember = "ноября";
    tDecember = "декабря";
    tMonday = "Понедельник";
    tTuesday = "Вторник";
    tWednesday = "Среда";
    tThursday = "Четверг";
    tFriday = "Пятница";
    tSaturday = "Суббота";
    tSunday = "Воскресенье";
    tAlarm = "ВНИМАНИЕ!!!   ";
    sgpCo2Message[0] = "Допустимый уровень возуха";
    sgpCo2Message[1] = "Тяжелый воздух. Необходимо проветривание";
    sgpCo2Message[2] = "Возможно сонливость, усталость, головная боль";
    sgpCo2Message[3] = "Серьезное ухудшение здоровья!!!";
    sgpCo2Message[4] = "Предельно допустимая концентрация в течении 8 часов";
    sgpTvocMessage[0] = "Допустимый уровень ЛОС";
    sgpTvocMessage[1] = "Рекомендуется вентиляция";
    sgpTvocMessage[2] = "Рекомендуется интенсивная вентиляция";
    sgpTvocMessage[3] = "Интенсивная вентиляция и проветривание необходимы";
    sgpTvocMessage[4] = "Очень интенсивная вентиляция строжайше необходима!!!";
  } else if (weatherLang == "en") {
    tNow = "now";
    tPress0 = "gPa";
    tPress = "mmHg";
    tSpeed = "m/s";
    tMin = "min.";
    tCurr = "Today";
    tTom = "Tomorrow";
    tYour = "Your";
    tPoint = "Connect to access point the";
    tIp = "and enter in the browser the address";
    tPass = "password";
    tWeatrNot = "   There is no weather update for more than 6 hours !!!   ";
    tWeatrTN = "No weather updates.";
    tJanuary = "January";
    tFebruary = "February";
    tMarch = "Martha";
    tApril = "April";
    tMay = "May";
    tJune = "June";
    tJuly = "July";
    tAugust = "August";
    tSeptember = "September";
    tOctober = "October";
    tNovember = "November";
    tDecember = "December";
    tMonday = "Monday";
    tTuesday = "Tuesday";
    tWednesday = "Wednesday";
    tThursday = "Thursday";
    tFriday = "Friday";
    tSaturday = "Saturday";
    tSunday = "Sunday";
    tAlarm = "ATTENTION!!!   ";
    sgpCo2Message[0] = "Permissible level of air";
    sgpCo2Message[1] = "Heavy air. Ventilation is necessary";
    sgpCo2Message[2] = "Drowsiness, fatigue, headache is possible";
    sgpCo2Message[3] = "Serious deterioration of health !!!";
    sgpCo2Message[4] = "Maximum permissible concentration within 8 hours";
    sgpTvocMessage [0] = "Allowable VOC Level";
    sgpTvocMessage [1] = "Recommended ventilation";
    sgpTvocMessage [2] = "Recommended intensive ventilation";
    sgpTvocMessage [3] = "Intensive ventilation and ventilation are necessary";
    sgpTvocMessage [4] = "Very intensive ventilation is strictly necessary !!!";
  } else if (weatherLang == "de") {
    tNow = "jetzt";
    tPress0 = "gPa";
    tPress = "mmHg";
    tSpeed = "m/s";
    tMin = "min.";
    tCurr = "Heute";
    tTom = "Morgen";
    tYour = "Ihr";
    tPoint = "Stellen Sie die Verbindung mit";
    tIp = "und geben Sie im Browser die Adresse";
    tPass = "Passwort";
    tWeatrNot = "   Keine Wetteraktualisierungen seit 6 Stunden!!!   ";
    tWeatrTN = "Keine Wetteraktualisierungen.";
    tJanuary = "Januar";
    tFebruary = "Februar";
    tMarch = "März";
    tApril = "April";
    tMay = "Mai";
    tJune = "Juni";
    tJuly = "Juli";
    tAugust = "August";
    tSeptember = "September";
    tOctober = "Oktober";
    tNovember = "November";
    tDecember = "Dezember";
    tMonday = "Montag";
    tTuesday = "Dienstag";
    tWednesday = "Mittwoch";
    tThursday = "Donnerstag";
    tFriday = "Freitag";
    tSaturday = "Samstag";
    tSunday = "Sonntag";
    tAlarm = "ACHTUNG !!!";
    sgpCo2Message[0] = "Zulässiges Luftniveau";
    sgpCo2Message[1] = "Starke Luft. Belüftung erforderlich";
    sgpCo2Message[2] = "Schläfrigkeit, Müdigkeit, Kopfschmerzen sind möglich";
    sgpCo2Message[3] = "Schwerwiegende Verschlechterung der Gesundheit !!!";
    sgpCo2Message[4] = "Maximal zulässige Konzentration innerhalb von 8 Stunden";
    sgpTvocMessage [0] = "Zulässiger VOC-Level";
    sgpTvocMessage [1] = "Empfohlene Belüftung";
    sgpTvocMessage [2] = "Empfohlene intensive Beatmung";
    sgpTvocMessage [3] = "Intensive Beatmung und Beatmung sind notwendig";
    sgpTvocMessage [4] = "Sehr intensive Belüftung ist unbedingt erforderlich !!!";
  } else if (weatherLang == "pl") {
    tNow = "Teraz";
    tPress0 = "gPa";
    tPress = "mmHg";
    tSpeed = "m/s";
    tMin = "min.";
    tCurr = "Dzisiaj";
    tTom = "Jutro";
    tYour = "Twój";
    tPoint = "Polacz sie z punktem dostępu";
    tIp = "i wprowadz w przeglądarce adres";
    tPass = "hasło";
    tWeatrNot = "   Brak aktualizacji pogody przez ponad 6 godzin!!!   ";
    tWeatrTN = "Brak aktualizacji pogody.";
    tJanuary = "stycznia";
    tFebruary = "lutego";
    tMarch = "marzca";
    tApril = "kwiecnia";
    tMay = "maja";
    tJune = "czerwca";
    tJuly = "lipca";
    tAugust = "sierpnia";
    tSeptember = "wrzesnia";
    tOctober = "pazdziernika";
    tNovember = "listopada";
    tDecember = "grudznia";
    tMonday = "Poniedzialek";
    tTuesday = "Wtorek";
    tWednesday = "Środa";
    tThursday = "Czwartek";
    tFriday = "Piątek";
    tSaturday = "Sobota";
    tSunday = "Niedziela";
    tAlarm = "UWAGA !!!";
    sgpCo2Message[0] = "Dopuszczalny poziom powietrza";
    sgpCo2Message[1] = "Ciężkie powietrze. Konieczna jest wentylacja";
    sgpCo2Message[2] = "Możliwe jest senność, zmęczenie, ból głowy";
    sgpCo2Message[3] = "Poważne pogorszenie stanu zdrowia !!!";
    sgpCo2Message[4] = "Maksymalne dopuszczalne stężenie w ciągu 8 godzin";
    sgpTvocMessage [0] = "Dopuszczalny poziom LZO";
    sgpTvocMessage [1] = "Zalecana wentylacja";
    sgpTvocMessage [2] = "Zalecana intensywna wentylacja";
    sgpTvocMessage [3] = "Konieczna jest intensywna wentylacja i wentylacja";
    sgpTvocMessage [4] = "Bardzo intensywna wentylacja jest absolutnie konieczna !!!";
  }
}

// ============================================================================//
//         Отправляем данные датчиков на сайт https://thingspeak.com           // 
// ============================================================================//
void sendThings(){  
  if(!sendThing || !WIFI_connected || channelIdWrite=="" || writeApiKey=="") return;
  // https://api.thingspeak.com/channels/1111111/feeds.json?api_key=AAAAAAAAAAAAAAA&results=0
  // https://api.thingspeak.com/channels/2222222/feeds.json?results=0 - для публичных каналов
  String line="";
  String regline="http://api.thingspeak.com/channels/"  + channelIdWrite + "/feeds.json?";
  if(wrApiKey!="") regline += "api_key=" + wrApiKey + "&";
  regline += "results=0";
  if(printCom) {
    Serial.println("======== START SEND THINGSPEAK.COM ====================");
    printTime();
    Serial.print(regline);
  } 
  HTTPClient http;
  if(http.begin(ESPclient, regline)) { // HTTP
    int httpCode = http.GET();
    if (httpCode > 0) {
      if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {
      if(printCom) Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      http.end();
      return;
    }
    http.end();
  } else {
    if(printCom) Serial.printf("    [HTTP] Unable to connect\n");
    return;
  }
  if(line==""){
    if(printCom) Serial.printf("    [HTTP] The answer is empty\n");
    return;
  }
  //if(printCom) Serial.print("          line =" + line);
  
  const size_t capacity = JSON_ARRAY_SIZE(0) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(15) + 546; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  JsonObject channel = doc["channel"];
  const char* channel_name = (channel["name"]);
  const char* channel_field1 = channel["field1"];
  const char* channel_field2 = channel["field2"];
  const char* channel_field3 = channel["field3"];
  const char* channel_field4 = channel["field4"];
  const char* channel_field5 = channel["field5"];
  const char* channel_field6 = channel["field6"];
  const char* channel_field7 = channel["field7"];
  const char* channel_field8 = channel["field8"];
  nameWriteChannel = String(channel_name);
  nameField0 = String(channel_field1);
  nameField1 = String(channel_field2);
  nameField2 = String(channel_field3);
  nameField3 = String(channel_field4);
  nameField4 = String(channel_field5);
  nameField5 = String(channel_field6);
  nameField6 = String(channel_field7);
  nameField7 = String(channel_field8);

  // https://api.thingspeak.com/update?api_key=AAAAAAAAAAAAAAAAAAAAAAAA&field1=0
  regline = "http://api.thingspeak.com/update?api_key=" + writeApiKey;
  bool sendRegline = false;
  if(String(nameField0)!="" && writeFild0){
    if(     writeFild0==1 && sensor00){ regline += "&field1=" + String (data00); sendRegline = true;}
    else if(writeFild0==2 && sensor01){  regline += "&field1=" + String (data01); sendRegline = true;}
    else if(writeFild0==3 && sensor02){regline += "&field1=" + String (data02); sendRegline = true;}
    else if(writeFild0==4 && sensor03){regline += "&field1=" + String (data03); sendRegline = true;}
    else if(writeFild0==5 && sensor04){regline += "&field1=" + String (data04); sendRegline = true;}
    else if(writeFild0==6 && sgpFound){  regline += "&field1=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild0==7 && sgpFound){  regline += "&field1=" + String (sgpTvoc); sendRegline = true;}
  }
  if(String(nameField1)!="" && writeFild1){
    if(     writeFild1==1 && sensor00){ regline += "&field2=" + String (data00); sendRegline = true;}
    else if(writeFild1==2 && sensor01){  regline += "&field2=" + String (data01); sendRegline = true;}
    else if(writeFild1==3 && sensor02){regline += "&field2=" + String (data02); sendRegline = true;} 
    else if(writeFild1==4 && sensor03){regline += "&field2=" + String (data03); sendRegline = true;}
    else if(writeFild1==5 && sensor04){regline += "&field2=" + String (data04); sendRegline = true;}
    else if(writeFild1==6 && sgpFound){  regline += "&field2=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild1==7 && sgpFound){  regline += "&field2=" + String (sgpTvoc); sendRegline = true;}
  }
  if(String(nameField2)!="" && writeFild2){
    if(     writeFild2==1 && sensor00){ regline += "&field3=" + String (data00); sendRegline = true;}
    else if(writeFild2==2 && sensor01){  regline += "&field3=" + String (data01); sendRegline = true;}
    else if(writeFild2==3 && sensor02){regline += "&field3=" + String (data02); sendRegline = true;} 
    else if(writeFild2==4 && sensor03){regline += "&field3=" + String (data03); sendRegline = true;}
    else if(writeFild2==5 && sensor04){regline += "&field3=" + String (data04); sendRegline = true;}
    else if(writeFild2==6 && sgpFound){  regline += "&field3=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild2==7 && sgpFound){  regline += "&field3=" + String (sgpTvoc); sendRegline = true;}
  }
  if(String(nameField3)!="" && writeFild3){
    if(     writeFild3==1 && sensor00){ regline += "&field4=" + String (data00); sendRegline = true;}
    else if(writeFild3==2 && sensor01){  regline += "&field4=" + String (data01); sendRegline = true;}
    else if(writeFild3==3 && sensor02){regline += "&field4=" + String (data02); sendRegline = true;} 
    else if(writeFild3==4 && sensor03){regline += "&field4=" + String (data03); sendRegline = true;}
    else if(writeFild3==5 && sensor04){regline += "&field4=" + String (data04); sendRegline = true;}
    else if(writeFild3==6 && sgpFound){  regline += "&field4=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild3==7 && sgpFound){  regline += "&field4=" + String (sgpTvoc); sendRegline = true;}
  }
  if(String(nameField4)!="" && writeFild4){
    if(     writeFild4==1 && sensor00){ regline += "&field5=" + String (data00); sendRegline = true;}
    else if(writeFild4==2 && sensor01){  regline += "&field5=" + String (data01); sendRegline = true;}
    else if(writeFild4==3 && sensor02){regline += "&field5=" + String (data02); sendRegline = true;} 
    else if(writeFild4==4 && sensor03){regline += "&field5=" + String (data03); sendRegline = true;}
    else if(writeFild4==5 && sensor04){regline += "&field5=" + String (data04); sendRegline = true;}
    else if(writeFild4==6 && sgpFound){  regline += "&field5=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild4==7 && sgpFound){  regline += "&field5=" + String (sgpTvoc); sendRegline = true;}
  }
  if(String(nameField5)!="" && writeFild5){
    if(     writeFild5==1 && sensor00){ regline += "&field6=" + String (data00); sendRegline = true;}
    else if(writeFild5==2 && sensor01){  regline += "&field6=" + String (data01); sendRegline = true;}
    else if(writeFild5==3 && sensor02){regline += "&field6=" + String (data02); sendRegline = true;} 
    else if(writeFild5==4 && sensor03){regline += "&field6=" + String (data03); sendRegline = true;}
    else if(writeFild5==5 && sensor04){regline += "&field6=" + String (data04); sendRegline = true;}
    else if(writeFild5==6 && sgpFound){  regline += "&field6=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild5==7 && sgpFound){  regline += "&field6=" + String (sgpTvoc); sendRegline = true;}
  }
  if(String(nameField6)!="" && writeFild6){
    if(     writeFild6==1 && sensor00){ regline += "&field7=" + String (data00); sendRegline = true;}
    else if(writeFild6==2 && sensor01){  regline += "&field7=" + String (data01); sendRegline = true;}
    else if(writeFild6==3 && sensor02){regline += "&field7=" + String (data02); sendRegline = true;} 
    else if(writeFild6==4 && sensor03){regline += "&field7=" + String (data03); sendRegline = true;}
    else if(writeFild6==5 && sensor04){regline += "&field7=" + String (data04); sendRegline = true;}
    else if(writeFild6==6 && sgpFound){  regline += "&field7=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild6==7 && sgpFound){  regline += "&field7=" + String (sgpTvoc); sendRegline = true;}
  }
  if(String(nameField7)!="" && writeFild7){
    if(     writeFild7==1 && sensor00){ regline += "&field8=" + String (data00); sendRegline = true;}
    else if(writeFild7==2 && sensor01){  regline += "&field8=" + String (data01); sendRegline = true;}
    else if(writeFild7==3 && sensor02){regline += "&field8=" + String (data02); sendRegline = true;} 
    else if(writeFild7==4 && sensor03){regline += "&field8=" + String (data03); sendRegline = true;}
    else if(writeFild7==5 && sensor04){regline += "&field8=" + String (data04); sendRegline = true;}
    else if(writeFild7==6 && sgpFound){  regline += "&field8=" + String (sgpCo2); sendRegline = true;}
    else if(writeFild7==7 && sgpFound){  regline += "&field8=" + String (sgpTvoc); sendRegline = true;}
  }
  int httpCode;
  if(sendRegline){
    HTTPClient client;
    client.begin(regline);
    httpCode=client.GET();
    client.end();
  }
  if(printCom){
    printTime();
      if(sendRegline) Serial.println ("send line = " + regline +"   httpCode = " + String(httpCode));
      else Serial.println("No data to send!");
    Serial.println("======== END ==========================================");
  }
}
// ============================================================================//
//               Получаем данные датчиков с сайта https://thingspeak.com          // 
// ============================================================================//
void getThings(){
  if(!getThing || !WIFI_connected || channelIdRead=="") return;
  HTTPClient http;
  String line = "";
  String regline = "http://api.thingspeak.com/channels/"+channelIdRead+"/feeds.json?";
  if(readApiKey!="") regline += "api_key=" + readApiKey + "&";
  regline += "results=1";
  if(printCom){
    Serial.println("======== START GET THINGSPEAK.COM =====================");
    printTime();
    Serial.print(regline);
  } 
  if (http.begin(ESPclient, regline)){
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {
      Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.printf("    [HTTP} Unable to connect\n");
  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(10) + JSON_OBJECT_SIZE(15) + 797; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  
  JsonObject channel = doc["channel"];
  const char* channel_name = channel["name"]; // "ESP-Informer"
  const char* channel_field1 = channel["field1"]; // "TempDOM"
  const char* channel_field2 = channel["field2"]; // "HumiDOM"
  const char* channel_field3 = channel["field3"]; // "CO2"
  const char* channel_field4 = channel["field4"]; // "TVOC"
  const char* channel_field5 = channel["field5"]; // "Field Label 5"
  const char* channel_field6 = channel["field6"]; // "Field Label 6"
  const char* channel_field7 = channel["field7"]; // "Field Label 7"
  const char* channel_field8 = channel["field8"]; // "Field Label 8"
  //const char* channel_created_at = channel["created_at"]; // "2020-05-12T08:19:16Z"
  //const char* channel_updated_at = channel["updated_at"]; // "2020-10-28T07:12:39Z"
  //int channel_last_entry_id = channel["last_entry_id"]; // 273
  
  JsonObject feeds_0 = doc["feeds"][0];
  //const char* feeds_0_created_at = feeds_0["created_at"]; // "2020-10-28T07:13:14Z"
  //int feeds_0_entry_id = feeds_0["entry_id"]; // 273
  const char* feeds_0_field1 = feeds_0["field1"]; // "20.53"
  const char* feeds_0_field2 = feeds_0["field2"]; // "44.31"
  const char* feeds_0_field3 = feeds_0["field3"]; // "400"
  const char* feeds_0_field4 = feeds_0["field4"]; // "0"
  const char* feeds_0_field5 = feeds_0["field5"]; // "44.31"
  const char* feeds_0_field6 = feeds_0["field6"]; // "44.31"
  const char* feeds_0_field7 = feeds_0["field7"]; // "44.31"
  const char* feeds_0_field8 = feeds_0["field8"]; // "44.31"

  nameReadChannel = channel_name;
  nameReadField0 = channel_field1;
  nameReadField1 = channel_field2;
  nameReadField2 = channel_field3;
  nameReadField3 = channel_field4;
  nameReadField4 = channel_field5;
  nameReadField5 = channel_field6;
  nameReadField6 = channel_field7;
  nameReadField7 = channel_field8;

  String aaa;
  aaa = statThing0==0?"0":statThing0==1?String(feeds_0_field1):statThing0==2?String(feeds_0_field2):statThing0==3?String(feeds_0_field3):statThing0==4?String(feeds_0_field4):statThing0==5?String(feeds_0_field5):statThing0==6?String(feeds_0_field6):statThing0==7?String(feeds_0_field7):String(feeds_0_field8);
  readDataField0 = aaa.substring(0, aaa.length()+1).toFloat();
  aaa = statThing1==0?"0":statThing1==1?String(feeds_0_field1):statThing1==2?String(feeds_0_field2):statThing1==3?String(feeds_0_field3):statThing1==4?String(feeds_0_field4):statThing1==5?String(feeds_0_field5):statThing1==6?String(feeds_0_field6):statThing1==7?String(feeds_0_field7):String(feeds_0_field8);
  readDataField1 = aaa.substring(0, aaa.length()+1).toFloat();
  aaa = statThing2==0?"0":statThing2==1?String(feeds_0_field1):statThing2==2?String(feeds_0_field2):statThing2==3?String(feeds_0_field3):statThing2==4?String(feeds_0_field4):statThing2==5?String(feeds_0_field5):statThing2==6?String(feeds_0_field6):statThing2==7?String(feeds_0_field7):String(feeds_0_field8);
  readDataField2 = aaa.substring(0, aaa.length()+1).toFloat();
  aaa = statThing3==0?"0":statThing3==1?String(feeds_0_field1):statThing3==2?String(feeds_0_field2):statThing3==3?String(feeds_0_field3):statThing3==4?String(feeds_0_field4):statThing3==5?String(feeds_0_field5):statThing3==6?String(feeds_0_field6):statThing3==7?String(feeds_0_field7):String(feeds_0_field8);
  readDataField3 = aaa.substring(0, aaa.length()+1).toFloat();
  aaa = statThing4==0?"0":statThing4==1?String(feeds_0_field1):statThing4==2?String(feeds_0_field2):statThing4==3?String(feeds_0_field3):statThing4==4?String(feeds_0_field4):statThing4==5?String(feeds_0_field5):statThing4==6?String(feeds_0_field6):statThing4==7?String(feeds_0_field7):String(feeds_0_field8);
  readDataField4 = aaa.substring(0, aaa.length()+1).toFloat();
  if(printCom){
    printTime();
    Serial.println("get line = " + line);
    Serial.println("======== END ==========================================");
  }
  sensors();
}

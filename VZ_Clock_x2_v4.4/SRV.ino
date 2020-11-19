void serverInit(void) {
  server.on("/", auth_auth);
  server.on("/index.vz", auth_index);  //server.on("/index.vz", [](){server.send_P(200, "text/html", P_index);});
  server.on("/time.vz", auth_time);  //server.on("/time.vz", [](){server.send_P(200, "text/html", P_time);}); 
  server.on("/weather.vz", auth_weather);  //server.on("/weather.vz", [](){server.send_P(200, "text/html", P_weath);});
  server.on("/mqtt.vz", auth_mqtt);  //server.on("/mqtt.vz", [](){server.send_P(200, "text/html", P_mqtt);});
  server.on("/thing.vz", auth_thing);
  server.on("/setup.vz", auth_setup);  //server.on("/setup.vz", [](){server.send_P(200, "text/html", P_setup);});
  server.on("/sgp.vz", auth_sgp);  //server.on("/sgp.vz", [](){server.send_P(200, "text/html", P_sgp);});
  server.on("/help.vz", auth_help);  //server.on("/help.vz", [](){server.send_P(200, "text/html", P_help);});
  server.on("/style.css", [](){server.send_P ( 200, "text/css", P_css);});
  server.on("/function.js", [](){server.send_P ( 200, "text/plain", P_js);});
  server.on("/favicon.ico", [](){server.send(200, "text/html", "");});
  server.onNotFound([]() {(404, "text/plain", "FileNotFound");});
  server.on("/configs.json", handle_ConfigJSON);    // формування configs.json сторінки для передачі данних в web інтерфейс
  server.on("/configs_wifi.json", handle_ConfigWifiJson);
  server.on("/configs_time.json", handle_ConfigTimeJson);
  server.on("/configs_mqtt.json", handle_ConfigMqttJson);
  server.on("/configs_weath.json", handle_ConfigWeathJson);
  server.on("/configs_setup.json", handle_ConfigSetupJson);
  server.on("/configs_sgp.json", handle_ConfigSgpJson);
  server.on("/configs_thing.json", handle_ConfigThingJson);
  server.on("/ssid", handle_Set_Ssid);
  server.on("/ntp", handle_ntp);         // Установка часової зони по запиту типа http://192.168.2.100/timeZone?timeZone=3
  server.on("/set_time", handle_set_time);
  server.on("/timepc", handle_timepc);
  server.on("/weatherUpdate", handle_weather_update);
  server.on("/weather", handle_weather);    // Установка сервера погоди по запиту типа http://192.168.2.100/weatherHost?weatherHost=api.openweathermap.org
  server.on("/mqttUst", handle_mqtt_ust);
  server.on("/mqttOn", handle_mqtt_on);
  server.on("/weathOn", handle_weath_on);
  server.on("/thing_on", handle_thing_on);
  server.on("/set_thing", handle_set_thing);
  server.on("/setup", handle_setup);
  server.on("/sgp", handle_sgp);
  server.on("/mess", handle_message);
  server.on("/restart", handle_Restart);            // перезавантаження можуля по запиту типу http://192.168.1.11/restart?device=ok
  server.on("/stopalarm", handle_stopAlarm);
  server.on("/resetConfig", handle_resetConfig);
  server.on("/printCom", handle_set_printCom);
  httpUpdater.setup(&server);
  server.begin();
}
//======================================================================================================
void auth_auth(){
  if(!authOn) server.send_P(200, "text/html", P_index);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_index(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_index);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_time(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_time);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_weather(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_weath);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_mqtt(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_mqtt);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_thing(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_thing);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_setup(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_setup);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_sgp(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_sgp);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void auth_help(){
  if(server.arg("auth")==auth || !authOn) server.send_P(200, "text/html", P_help);
  else server.send_P(200, "text/html", P_auth);
}
//======================================================================================================
void handle_ConfigJSON() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"ip\":\"";
  json += WiFi.localIP().toString();
  json += "\",\"printCom\":\"";
  json += (printCom==1?"checked":"");
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigWifiJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"auth\":\"";
  json += auth;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"ssid\":\"";
  json += ssid;
  json += "\",\"password\":\"";
  json += password;
  json += "\",\"ssidAP\":\"";
  json += ssidAP;
  json += "\",\"passwordAP\":\"";
  json += passwordAP;
  json += "\",\"authOn\":\"";
  json += (authOn==1?"checked":"");
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigTimeJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"ntpServerName\":\"";
  json += ntpServerName;
  json += "\",\"timeZone\":\"";
  json += timeZone;
  json += "\",\"isDayLightSaving\":\"";
  json += (isDayLightSaving==1?"checked":"");
  json += "\",\"rtcStat\":\"";
  json += (rtcStat==1?"checked":"");
  json += "\",\"al_0_0\":\"";
  json += alarme[0][0];
  json += "\",\"al_0_1\":\"";
  json += alarme[0][1];
  json += "\",\"al_0_2\":\"";
  json += alarme[0][2];
  json += "\",\"al_1_0\":\"";
  json += alarme[1][0];
  json += "\",\"al_1_1\":\"";
  json += alarme[1][1];
  json += "\",\"al_1_2\":\"";
  json += alarme[1][2];
  json += "\",\"al_2_0\":\"";
  json += alarme[2][0];
  json += "\",\"al_2_1\":\"";
  json += alarme[2][1];
  json += "\",\"al_2_2\":\"";
  json += alarme[2][2];
  json += "\",\"al_3_0\":\"";
  json += alarme[3][0];
  json += "\",\"al_3_1\":\"";
  json += alarme[3][1];
  json += "\",\"al_3_2\":\"";
  json += alarme[3][2];
  json += "\",\"al_4_0\":\"";
  json += alarme[4][0];
  json += "\",\"al_4_1\":\"";
  json += alarme[4][1];
  json += "\",\"al_4_2\":\"";
  json += alarme[4][2];
  json += "\",\"md_0_0\":\"";
  json += memory_date[0][0];
  json += "\",\"md_0_1\":\"";
  json += memory_date[0][1];
  json += "\",\"md_0_2\":\"";
  json += memory_date_mes0;
  json += "\",\"md_1_0\":\"";
  json += memory_date[1][0];
  json += "\",\"md_1_1\":\"";
  json += memory_date[1][1];
  json += "\",\"md_1_2\":\"";
  json += memory_date_mes1;
  json += "\",\"md_2_0\":\"";
  json += memory_date[2][0];
  json += "\",\"md_2_1\":\"";
  json += memory_date[2][1];
  json += "\",\"md_2_2\":\"";
  json += memory_date_mes2;
  json += "\",\"md_3_0\":\"";
  json += memory_date[3][0];
  json += "\",\"md_3_1\":\"";
  json += memory_date[3][1];
  json += "\",\"md_3_2\":\"";
  json += memory_date_mes3;
  json += "\",\"md_4_0\":\"";
  json += memory_date[4][0];
  json += "\",\"md_4_1\":\"";
  json += memory_date[4][1];
  json += "\",\"md_4_2\":\"";
  json += memory_date_mes4;
  json += "\",\"md_5_0\":\"";
  json += memory_date[5][0];
  json += "\",\"md_5_1\":\"";
  json += memory_date[5][1];
  json += "\",\"md_5_2\":\"";
  json += memory_date_mes5;
  json += "\",\"md_6_0\":\"";
  json += memory_date[6][0];
  json += "\",\"md_6_1\":\"";
  json += memory_date[6][1];
  json += "\",\"md_6_2\":\"";
  json += memory_date_mes6;
  json += "\",\"md_7_0\":\"";
  json += memory_date[7][0];
  json += "\",\"md_7_1\":\"";
  json += memory_date[7][1];
  json += "\",\"md_7_2\":\"";
  json += memory_date_mes7;
  json += "\",\"md_8_0\":\"";
  json += memory_date[8][0];
  json += "\",\"md_8_1\":\"";
  json += memory_date[8][1];
  json += "\",\"md_8_2\":\"";
  json += memory_date_mes8;
  json += "\",\"md_start\":\"";
  json += memory_hour_start;
  json += "\",\"md_stop\":\"";
  json += memory_hour_end;
  json += "\",\"t0\":\"";
  json += hour;
  json += "\",\"t1\":\"";
  json += minute;
  json += "\",\"d0\":\"";
  json += day;
  json += "\",\"d1\":\"";
  json += month;
  json += "\",\"d2\":\"";
  json += year;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigMqttJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"mqttOn\":\"";
  json += (mqttOn==1?"checked":"");
  json += "\",\"mqtt_server\":\"";
  json += mqtt_server;
  json += "\",\"mqtt_port\":\"";
  json += mqtt_port;
  json += "\",\"mqtt_user\":\"";
  json += mqtt_user;
  json += "\",\"mqtt_pass\":\"";
  json += mqtt_pass;
  json += "\",\"mqtt_name\":\"";
  json += mqtt_name;
  json += "\",\"mqtt_sub1\":\"";
  json += mqtt_sub1;
  json += "\",\"mqtt_sub2\":\"";
  json += mqtt_sub2;
  json += "\",\"mqtt_sub3\":\"";
  json += mqtt_sub3;
  json += "\",\"mqtt_sub_inform\":\"";
  json += mqtt_sub_inform;
  json += "\",\"mqtt_pub_sensor00\":\"";
  json += mqtt_pub_sensor00;
  json += "\",\"mqtt_pub_sensor01\":\"";
  json += mqtt_pub_sensor01;
  json += "\",\"mqtt_pub_sensor02\":\"";
  json += mqtt_pub_sensor02;
  json += "\",\"mqtt_pub_sensor03\":\"";
  json += mqtt_pub_sensor03;
  json += "\",\"mqtt_pub_sensor04\":\"";
  json += mqtt_pub_sensor04;
  json += "\",\"mqtt_pub_eco2\":\"";
  json += mqtt_pub_eco2;
  json += "\",\"mqtt_pub_tvoc\":\"";
  json += mqtt_pub_tvoc;
  json += "\",\"timeOutMqtt\":\"";
  json += timeOutMqtt;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================  
void handle_ConfigWeathJson() {
  if(verific()) return;
  int sr = location_sunrise.substring(0, 2).toInt() + (int)hourCorr;
  if(sr>23) sr -= 24;
  if(sr<0) sr += 24;
  String sunrise = String(sr) + location_sunrise.substring(2, 5);
  int ss = location_sunset.substring(0, 2).toInt() + (int)hourCorr;
  if(ss>23) ss -= 24;
  if(ss<0) ss += 24;
  String sunset = String(ss) + location_sunset.substring(2, 5);
  int st = location_localtime.substring(11, 13).toInt() + (int)hourCorr;
  int ly = location_localtime.substring(0, 4).toInt();
  byte lm = location_localtime.substring(5, 7).toInt();
  byte ld = location_localtime.substring(8, 10).toInt(); 
  if(st>23) {
    st -= 24;
    ld++;
    if(ld==32 || (ld==31 && (lm==4 || lm==6 || lm==9 || lm==11)) || (lm==2 && ((ld==29 && ly%4!=0) || (ld==30 && ly%4==0)))) {
      ld=1;
      lm++;
      if(lm>12){
        lm=1;
        ly++;
      }
    }
  }
  if(st<0) {
    st += 24;
    ld--;
    if(ld<1) {
      ld = 0 + ((lm==5 || lm==7 || lm==10 || lm==12 || (lm==3 && ly%4==0))?30:(lm==3 && ly%4!=0)?29:31);
      lm--;
      if(lm<1){
        lm=12;
        ly--;
      }
    }
  }
  String lt = String(ly) + "-" + (lm<10?"0":"") + String(lm) + "-" + (ld<10?"0":"") + String(ld) + " " + (st<10?"0":"") + String(st) + location_localtime.substring(13, 16);
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"weatherKey0\":\"";
  json += weatherKey0;
  json += "\",\"weatherKey1\":\"";
  json += weatherKey1;
  json += "\",\"weatherHost\":\"";
  json += weatherHost;
  json += "\",\"cityID0\":\"";
  json += cityID0;
  json += "\",\"cityID1\":\"";
  json += cityID1;
  json += "\",\"weatherLang\":\"";
  json += weatherLang;
  json += "\",\"displayForecast\":\"";
  json += (displayForecast==1?"checked":"");
  json += "\",\"displayCityName\":\"";
  json += (displayCityName==1?"checked":"");
  json += "\",\"displayForecastNow\":\"";
  json += (displayForecastNow==1?"checked":"");
  json += "\",\"displayForecastToday\":\"";
  json += (displayForecastToday==1?"checked":"");
  json += "\",\"displayForecastTomorrow\":\"";
  json += (displayForecastTomorrow==1?"checked":"");
  json += "\",\"animNotWeather\":\"";
  json += (animNotWeather==1?"checked":"");
  json += "\",\"timeStartViewWeather\":\"";
  json += timeStartViewWeather;
  json += "\",\"timeEndViewWeather\":\"";
  json += timeEndViewWeather;
  json += "\",\"location_name\":\"";
  json += location_name;
  json += "\",\"location_region\":\"";
  json += location_region;
  json += "\",\"location_country\":\"";
  json += location_country;
  json += "\",\"location_localtime\":\"";
  json += lt;
  json += "\",\"location_temp\":\"";
  json += location_temp;
  json += "\",\"location_app_temp\":\"";
  json += location_app_temp;
  json += "\",\"location_rh\":\"";
  json += location_rh;
  json += "\",\"location_pres\":\"";
  json += location_pres;
  json += "\",\"location_wind_spd\":\"";
  json += location_wind_spd;
  json += "\",\"location_wind_cdir_full\":\"";
  json += location_wind_cdir_full;
  json += "\",\"location_sunrise\":\"";
  json += sunrise;
  json += "\",\"location_sunset\":\"";
  json += sunset;
  json += "\",\"location_clouds\":\"";
  json += location_clouds;
  json += "\",\"location_vis\":\"";
  json += location_vis;
  json += "\",\"location_uv\":\"";
  json += location_uv;
  json += "\",\"location_weather_description\":\"";
  json += location_weather_description;
  json += "\",\"uuid\":\"";
  json += uuid;
  json += "\",\"api_key\":\"";
  json += api_key;
  json += "\",\"sensors_ID0\":\"";
  json += sensors_ID0;
  json += "\",\"sensors_ID1\":\"";
  json += sensors_ID1;
  json += "\",\"sensors_ID2\":\"";
  json += sensors_ID2;
  json += "\",\"personalCityName\":\"";
  json += personalCityName;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigSetupJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"kuOn\":\"";
  json += kuOn;
  json += "\",\"kuOff\":\"";
  json += kuOff;
  json += "\",\"tbd\":\"";
  json += timeDay;
  json += "\",\"tbn\":\"";
  json += timeNight;
  json += "\",\"vbd\":\"";
  json += volBrightnessD;
  json += "\",\"vbn\":\"";
  json += volBrightnessN;
  json += "\",\"clock2line\":\"";
  json += clock2line;
  json += "\",\"secLine\":\"";
  json += secLine;
  json += "\",\"timeScrollSpeed\":\"";
  json += 100 - timeScrollSpeed;
  json += "\",\"vba\":\"";
  json += (volBrightnessAuto==1?"checked":"");
  json += "\",\"lowLivelBrightness\":\""; 
  json += lowLivelBrightness;
  json += "\",\"upLivelBrightness\":\"";
  json += upLivelBrightness;
  json += "\",\"lba\":\"";
  json += levelBridhtness;
  json += "\",\"buzzerSet\":\"";
  json += (buzzerSet==1?"checked":"");
  json += "\",\"sensor00\":\"";
  json += sensor00;
  json += "\",\"corr00\":\"";
  json += corr00;
  float Td=data00;
  if(param0==20){
    if(pressSys==1 && Td>815) Td /= 1.3332239;
    if(pressSys!=1 && Td<815) Td /= 0.7500615613026439;
  }
  json += "\",\"Td\":\"";
  json += Td;
  json += "\",\"sensor01\":\"";
  json += sensor01;
  json += "\",\"corr01\":\"";
  json += corr01;
  float Tu=data01;
  if(param1==20){
    if(pressSys==1 && Tu>815) Tu /= 1.3332239;
    if(pressSys!=1 && Tu<815) Tu /= 0.7500615613026439;
  }
  json += "\",\"Tu\":\"";
  json += Tu;
  json += "\",\"sensor02\":\"";
  json += sensor02;
  json += "\",\"corr02\":\"";
  json += corr02;
  float Th=data02;
  if(param2==20){
    if(pressSys==1 && Th>815) Th /= 1.3332239;
    if(pressSys!=1 && Th<815) Th /= 0.7500615613026439;
  }
  json += "\",\"Th\":\"";
  json += Th;
  json += "\",\"sensor03\":\"";
  json += sensor03;
  json += "\",\"corr03\":\"";
  json += corr03;
  float Hd=data03;
  if(param3==20){
    if(pressSys==1 && Hd>815) Hd /= 1.3332239;
    if(pressSys!=1 && Hd<815) Hd /= 0.7500615613026439;
  }
  json += "\",\"Hd\":\"";
  json += Hd;
  json += "\",\"sensor04\":\"";
  json += sensor04;
  json += "\",\"corr04\":\"";
  json += corr04;
  float Pu=data04;
  if(param4==20){
    if(pressSys==1 && Pu>815) Pu /= 1.3332239;
    if(pressSys!=1 && Pu<815) Pu /= 0.7500615613026439;
  }
  json += "\",\"Pu\":\"";
  json += Pu;
  json += "\",\"pressSys\":\"";
  json += pressSys;
  json += "\",\"param0\":\"";
  json += param0;
  json += "\",\"param1\":\"";
  json += param1;
  json += "\",\"param2\":\"";
  json += param2;
  json += "\",\"param3\":\"";
  json += param3;
  json += "\",\"param4\":\"";
  json += param4;
  json += "\",\"rotate0\":\"";
  json += rotate0;
  json += "\",\"rotate1\":\"";
  json += rotate1;
  json += "\",\"NUM_MAX0\":\"";
  json += NUM_MAX0;
  json += "\",\"NUM_MAX1\":\"";
  json += NUM_MAX1;
  json += "\",\"fontCLOCK\":\"";
  json += fontCLOCK;
  json += "\",\"fontSizeCLOCK\":\"";
  json += fontSizeCLOCK;
  json += "\",\"fontSizeData\":\"";
  json += fontSizeData;
  json += "\",\"dataDown\":\"";
  json += dataDown;
  json += "\",\"animPoint\":\"";
  json += animPoint;
  json += "\",\"timeStopBigCklock\":\"";
  json += timeStopBigCklock;
  json += "\",\"textDown\":\"";
  json += textDown;
  json += "\",\"butStat\":\"";
  json += butStat;
  json += "\",\"displayData\":\"";
  json += displayData;
  json += "\",\"sgp\":\"";
  if(sgpFound){
    json += "SGP30";
  } else json += "";
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigSgpJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"sgpCo2\":\"";
  json += sgpCo2;
  json += "\",\"textCo2\":\"";
  json += sgpCo2Message[sgpCo2Livel];
  json += "\",\"sgpCo2LivelAlarm\":\"";
  json += sgpCo2LivelAlarm;
  json += "\",\"eCo2AlarmEsp\":\"";
  json += (eCo2AlarmEsp==1?"checked":"");
  json += "\",\"eCo2AlarmMqtt\":\"";
  json += (eCo2AlarmMqtt==1?"checked":"");
  json += "\",\"eCo2Led\":\"";
  json += (eCo2Led==1?"checked":"");
  json += "\",\"sgpTvoc\":\"";
  json += sgpTvoc;
  json += "\",\"textTvoc\":\"";
  json += sgpTvocMessage[sgpTvocLivel];
  json += "\",\"sgpTvocLivelAlarm\":\"";
  json += sgpTvocLivelAlarm;
  json += "\",\"tvocAlarmEsp\":\"";
  json += (tvocAlarmEsp==1?"checked":"");
  json += "\",\"tvocAlarmMqtt\":\"";
  json += (tvocAlarmMqtt==1?"checked":"");
  json += "\",\"tvocLed\":\"";
  json += (tvocLed==1?"checked":"");
  json += "\",\"setSgpCorr\":\"";
  json += setSgpCorr;
  json += "\",\"sgpCorrTemp\":\"";
  json += sgpCorrTemp;
  json += "\",\"sgpCorrHumi\":\"";
  json += sgpCorrHumi;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigThingJson() {
  if(verific()) return;
  String json = "{";
  json += "\"ver\":\"";
  json += ver;
  json += "\",\"time\":\"";
  json += (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  json += "\",\"sendThing\":\"";
  json += (sendThing==1?"checked":"");
  json += "\",\"channelIdWrite\":\"";
  json += channelIdWrite;
  json += "\",\"writeApiKey\":\"";
  json += writeApiKey;
  json += "\",\"wrApiKey\":\"";
  json += wrApiKey;
  json += "\",\"nameWriteChannel\":\"";
  json += nameWriteChannel;
  json += "\",\"nameField0\":\"";
  json += (nameField0!=""?nameField0:"Отключен");
  json += "\",\"nameField1\":\"";
  json += (nameField1!=""?nameField1:"Отключен");
  json += "\",\"nameField2\":\"";
  json += (nameField2!=""?nameField2:"Отключен");
  json += "\",\"nameField3\":\"";
  json += (nameField3!=""?nameField3:"Отключен");
  json += "\",\"nameField4\":\"";
  json += (nameField4!=""?nameField4:"Отключен");
  json += "\",\"nameField5\":\"";
  json += (nameField5!=""?nameField5:"Отключен");
  json += "\",\"nameField6\":\"";
  json += (nameField6!=""?nameField6:"Отключен");
  json += "\",\"nameField7\":\"";
  json += (nameField7!=""?nameField7:"Отключен");
  json += "\",\"writeFild0\":\"";
  json += writeFild0;
  json += "\",\"writeFild1\":\"";
  json += writeFild1;
  json += "\",\"writeFild2\":\"";
  json += writeFild2;
  json += "\",\"writeFild3\":\"";
  json += writeFild3;
  json += "\",\"writeFild4\":\"";
  json += writeFild4;
  json += "\",\"writeFild5\":\"";
  json += writeFild5;
  json += "\",\"writeFild6\":\"";
  json += writeFild6;
  json += "\",\"writeFild7\":\"";
  json += writeFild7;
  json += "\",\"getThing\":\"";
  json += (getThing==1?"checked":"");
  json += "\",\"channelIdRead\":\"";
  json += channelIdRead;
  json += "\",\"readApiKey\":\"";
  json += readApiKey;
  json += "\",\"nameReadChannel\":\"";
  json += nameReadChannel;
  json += "\",\"nameReadField0\":\"";
  json += (nameReadField0!=""?nameReadField0:"Откл.");
  json += "\",\"nameReadField1\":\"";
  json += (nameReadField1!=""?nameReadField1:"Откл.");
  json += "\",\"nameReadField2\":\"";
  json += (nameReadField2!=""?nameReadField2:"Откл.");
  json += "\",\"nameReadField3\":\"";
  json += (nameReadField3!=""?nameReadField3:"Откл.");
  json += "\",\"nameReadField4\":\"";
  json += (nameReadField4!=""?nameReadField4:"Откл.");
  json += "\",\"nameReadField5\":\"";
  json += (nameReadField5!=""?nameReadField5:"Откл.");
  json += "\",\"nameReadField6\":\"";
  json += (nameReadField6!=""?nameReadField6:"Откл.");
  json += "\",\"nameReadField7\":\"";
  json += (nameReadField7!=""?nameReadField7:"Откл.");
  json += "\",\"statThing0\":\"";
  json += statThing0;
  json += "\",\"statThing1\":\"";
  json += statThing1;
  json += "\",\"statThing2\":\"";
  json += statThing2;
  json += "\",\"statThing3\":\"";
  json += statThing3;
  json += "\",\"statThing4\":\"";
  json += statThing4;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_Set_Ssid() {
  if(verific()) return;
  if(server.arg("ssid")!="") ssid = server.arg("ssid").c_str();
  password = server.arg("password").c_str();
  if(server.arg("ssidAP")!="") ssidAP = server.arg("ssidAP").c_str();
  passwordAP = server.arg("passwordAP").c_str();
  if(server.arg("auth")!="") auth = server.arg("auth").c_str();
  if(server.arg("authOn")!="") authOn = server.arg("authOn").toInt();
  saveConfig();
  if(printCom) {
    printTime();
    Serial.println("Set SSID: " + ssid + ",  Set password: " + password + ",  Set SSID AP: " + ssidAP + ",  Set AP password: " + passwordAP + ",  Set auth: " + auth + ",  Set authOn: " + authOn);
  }
  server.send(200, "text/plain", "OK");
  ESP.reset();
}
//======================================================================================================
void handle_ntp(){
  if(verific()) return;
  if(server.arg("ntpServerName")!="") ntpServerName = server.arg("ntpServerName").c_str();
  if(server.arg("timeZone")!="") timeZone = server.arg("timeZone").toFloat();
  if(server.arg("isDayLightSaving")!="") isDayLightSaving = server.arg("isDayLightSaving").toInt();
  if(server.arg("rtcStat")!="") rtcStat = server.arg("rtcStat").toInt();
  if(server.arg("al_0_0")!="") alarme[0][0]=server.arg("al_0_0").toInt();
  if(server.arg("al_0_1")!="") alarme[0][1]=server.arg("al_0_1").toInt();
  if(server.arg("al_0_2")!="") alarme[0][2]=server.arg("al_0_2").toInt();
  if(server.arg("al_1_0")!="") alarme[1][0]=server.arg("al_1_0").toInt();
  if(server.arg("al_1_1")!="") alarme[1][1]=server.arg("al_1_1").toInt();
  if(server.arg("al_1_2")!="") alarme[1][2]=server.arg("al_1_2").toInt();
  if(server.arg("al_2_0")!="") alarme[2][0]=server.arg("al_2_0").toInt();
  if(server.arg("al_2_1")!="") alarme[2][1]=server.arg("al_2_1").toInt();
  if(server.arg("al_2_2")!="") alarme[2][2]=server.arg("al_2_2").toInt();
  if(server.arg("al_3_0")!="") alarme[3][0]=server.arg("al_3_0").toInt();
  if(server.arg("al_3_1")!="") alarme[3][1]=server.arg("al_3_1").toInt();
  if(server.arg("al_3_2")!="") alarme[3][2]=server.arg("al_3_2").toInt();
  if(server.arg("al_4_0")!="") alarme[4][0]=server.arg("al_4_0").toInt();
  if(server.arg("al_4_1")!="") alarme[4][1]=server.arg("al_4_1").toInt();
  if(server.arg("al_4_2")!="") alarme[4][2]=server.arg("al_4_2").toInt();
  if(server.arg("md_0_0")!="") memory_date[0][0]=server.arg("md_0_0").toInt();
  if(server.arg("md_0_1")!="") memory_date[0][1]=server.arg("md_0_1").toInt();
  if(server.arg("md_1_0")!="") memory_date[1][0]=server.arg("md_1_0").toInt();
  if(server.arg("md_1_1")!="") memory_date[1][1]=server.arg("md_1_1").toInt();
  if(server.arg("md_2_0")!="") memory_date[2][0]=server.arg("md_2_0").toInt();
  if(server.arg("md_2_1")!="") memory_date[2][1]=server.arg("md_2_1").toInt();
  if(server.arg("md_3_0")!="") memory_date[3][0]=server.arg("md_3_0").toInt();
  if(server.arg("md_3_1")!="") memory_date[3][1]=server.arg("md_3_1").toInt();
  if(server.arg("md_4_0")!="") memory_date[4][0]=server.arg("md_4_0").toInt();
  if(server.arg("md_4_1")!="") memory_date[4][1]=server.arg("md_4_1").toInt();
  if(server.arg("md_5_0")!="") memory_date[5][0]=server.arg("md_5_0").toInt();
  if(server.arg("md_5_1")!="") memory_date[5][1]=server.arg("md_5_1").toInt();
  if(server.arg("md_6_0")!="") memory_date[6][0]=server.arg("md_6_0").toInt();
  if(server.arg("md_6_1")!="") memory_date[6][1]=server.arg("md_6_1").toInt();
  if(server.arg("md_7_0")!="") memory_date[7][0]=server.arg("md_7_0").toInt();
  if(server.arg("md_7_1")!="") memory_date[7][1]=server.arg("md_7_1").toInt();
  if(server.arg("md_8_0")!="") memory_date[8][0]=server.arg("md_8_0").toInt();
  if(server.arg("md_8_1")!="") memory_date[8][1]=server.arg("md_8_1").toInt();
  if(server.arg("md_0_2")!="") snprintf(memory_date_mes0, 51, "%s", server.arg("md_0_2").c_str());
  if(server.arg("md_1_2")!="") snprintf(memory_date_mes1, 51, "%s", server.arg("md_1_2").c_str());
  if(server.arg("md_2_2")!="") snprintf(memory_date_mes2, 51, "%s", server.arg("md_2_2").c_str());
  if(server.arg("md_3_2")!="") snprintf(memory_date_mes3, 51, "%s", server.arg("md_3_2").c_str());
  if(server.arg("md_4_2")!="") snprintf(memory_date_mes4, 51, "%s", server.arg("md_4_2").c_str());
  if(server.arg("md_5_2")!="") snprintf(memory_date_mes5, 51, "%s", server.arg("md_5_2").c_str());
  if(server.arg("md_6_2")!="") snprintf(memory_date_mes6, 51, "%s", server.arg("md_6_2").c_str());
  if(server.arg("md_7_2")!="") snprintf(memory_date_mes7, 51, "%s", server.arg("md_7_2").c_str());
  if(server.arg("md_8_2")!="") snprintf(memory_date_mes8, 51, "%s", server.arg("md_8_2").c_str());
  if(server.arg("md_start")!="") memory_hour_start = server.arg("md_start").toInt();
  if(server.arg("md_stop")!="") memory_hour_end = server.arg("md_stop").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set NTP Server Name: " + ntpServerName + ",  NTP Time Zone: " + String(timeZone) + ",  isDayLightSaving: " + String(isDayLightSaving));
  }
  alarm_hold=0;
  saveAlarm();
  timeUpdateNTP();
  server.send(200, "text/plain", "OK"); 
}
//======================================================================================================
void handle_set_time(){
  if(verific()) return;
  if(server.arg("t0")!="") hour = server.arg("t0").toInt();
  if(server.arg("t1")!="") minute = server.arg("t1").toInt();
  if(server.arg("d0")!="") day = server.arg("d0").toInt();
  if(server.arg("d1")!="") month = server.arg("d1").toInt();
  if(server.arg("d2")!="") year = server.arg("d2").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set manual time: " + String(hour) + ":" + String(minute) + "      " + String(day) + "-" + String(month) + "-" + String(year));
  }
  server.send(200, "text/plain", "OK");
  localEpoc = (hour * 60 * 60 + minute * 60);
  saveTime();
  showSimpleDate();
  hour_rtc = hour;
  minute_rtc = minute;
  second_rtc = 0;
  month_rtc = month;
  day_rtc = day;
  year_rtc = year;
  printTime();
  setRTCDateTime();
}
//======================================================================================================
void handle_timepc(){
  if(verific()) return;
  if(server.arg("hours")!="") hour = server.arg("hours").toInt();
  if(server.arg("minute")!="") minute = server.arg("minute").toInt();
  if(server.arg("sec")!="") second = server.arg("sec").toInt();
  if(server.arg("tz")!="") timeZone = server.arg("tz").toFloat();
  if(server.arg("day")!="") day = server.arg("day").toInt();
  if(server.arg("month")!="") month = server.arg("month").toInt();
  if(server.arg("year")!="") year = server.arg("year").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set Date/Time from PC - "+String(day)+"."+String(month)+"."+String(year)+"     "+String(hour)+":"+String(minute)+":"+String(second)+"    timeZone="+String(timeZone));
  }
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  saveConfig();
  server.send(200, "text/plain", "OK"); 
}
//======================================================================================================
void weather_update(){
  if(displayForecast) {
    if(!weatherHost){
      getWeatherData0();
      getWeatherDataz0();
    } else {
      getWeatherData1();
      getWeatherDataz1();
    }
  }
}
//======================================================================================================
void handle_weath_on(){
  if(verific()) return;
  if(server.arg("displayForecast")!="") displayForecast = server.arg("displayForecast").toInt(); 
  if(printCom) {
    printTime();
    Serial.println("displayForecast = " + String(displayForecast));
  }
  if(displayForecast) weather_update();
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_weather_update(){
  if(verific()) return;
  if(server.arg("update") == "ok") {
    weather_update();
    if(sensor01==7)getNarodmon();
    bip();
    server.send(200, "text/plain", "OK");
  }
}
//======================================================================================================
void handle_weather(){
  if(verific()) return;
  if(server.arg("weatherHost")!="") weatherHost = server.arg("weatherHost").toInt();
  if(server.arg("weatherKey0")!="") weatherKey0 = server.arg("weatherKey0").c_str();
  if(server.arg("weatherKey1")!="") weatherKey1 = server.arg("weatherKey1").c_str();
  if(server.arg("cityID0")!="") cityID0 = server.arg("cityID0").c_str();
  if(server.arg("cityID1")!="") cityID1 = server.arg("cityID1").c_str();
  if(server.arg("weatherLang")!="") weatherLang = server.arg("weatherLang").c_str();
  if(server.arg("displayCityName")!="") displayCityName = server.arg("displayCityName").toInt();
  if(server.arg("displayForecastNow")!="") displayForecastNow = server.arg("displayForecastNow").toInt();
  if(server.arg("displayForecastToday")!="") displayForecastToday = server.arg("displayForecastToday").toInt();
  if(server.arg("displayForecastTomorrow")!="") displayForecastTomorrow = server.arg("displayForecastTomorrow").toInt();
  if(server.arg("animNotWeather")!="") animNotWeather = server.arg("animNotWeather").toInt();
  if(server.arg("timeStartViewWeather")!="") timeStartViewWeather = server.arg("timeStartViewWeather").toInt();
  if(server.arg("timeEndViewWeather")!="") timeEndViewWeather = server.arg("timeEndViewWeather").toInt();
  if(server.arg("uuid")!="") uuid = server.arg("uuid").c_str();
  if(server.arg("api_key")!="") api_key = server.arg("api_key").c_str();
  if(server.arg("sensors_ID0")!="") sensors_ID0 = server.arg("sensors_ID0").toInt();
  if(server.arg("sensors_ID1")!="") sensors_ID1 = server.arg("sensors_ID1").toInt();
  if(server.arg("sensors_ID2")!="") sensors_ID2 = server.arg("sensors_ID2").toInt();
  if(server.arg("personalCityName")!="") snprintf(personalCityName, 51, "%s", server.arg("personalCityName").c_str());
  if(printCom) {
    printTime();
    Serial.print("Set Weather Server: ");
    if(!weatherHost) {
      Serial.print(weatherHost0);
    } else  Serial.print(weatherHost1);
    Serial.println(", Key0: " + weatherKey0 + ", Key1: " + weatherKey1 + ", City ID0: " + cityID0 + ", City ID1: " + cityID1 + ", weatherLang: " + weatherLang);
    Serial.println("          displayCityName: " + String(displayCityName) + ", displayForecastNow: " + String(displayForecastNow) + ", displayForecastTomorrow: " + String(displayForecastTomorrow));
    Serial.println("          timeStartViewWeather: " + String(timeStartViewWeather) + ", timeEndViewWeather: " + String(timeEndViewWeather) + ", timeScrollSpeed: " + String(timeScrollSpeed) + ", uuid: " + String(uuid) + ", api_key: " + String(api_key) + ", sensors_ID0: " + String(sensors_ID0) + ", sensors_ID1: " + String(sensors_ID1) + ", sensors_ID2: " + String(sensors_ID2));
  }
  saveConfig();
  lang();
  weather_update();
  if(sensor01==7) getNarodmon();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_setup(){
  if(verific()) return;
  if(server.arg("tbd")!="") timeDay = server.arg("tbd").toInt();
  if(server.arg("vbd")!="") volBrightnessD = server.arg("vbd").toInt();
  if(server.arg("tbn")!="") timeNight = server.arg("tbn").toInt();
  if(server.arg("vbn")!="") volBrightnessN = server.arg("vbn").toInt();
  if(server.arg("vba")!="") volBrightnessAuto = server.arg("vba").toInt();
  if(server.arg("lowLivelBrightness")!="") lowLivelBrightness = server.arg("lowLivelBrightness").toInt();
  if(server.arg("upLivelBrightness")!="") upLivelBrightness = server.arg("upLivelBrightness").toInt();
  if(server.arg("clock2line")!="") clock2line = server.arg("clock2line").toInt();
  if(server.arg("secLine")!="") secLine = server.arg("secLine").toInt();
  if(server.arg("buzzerSet")!="") buzzerSet = server.arg("buzzerSet").toInt();
  if(server.arg("rotate0")!="") rotate0 = server.arg("rotate0").toInt();
  if(server.arg("rotate1")!="") rotate1 = server.arg("rotate1").toInt();
  if(server.arg("NUM_MAX0")!="") NUM_MAX0 = server.arg("NUM_MAX0").toInt();
  if(server.arg("NUM_MAX1")!="") NUM_MAX1 = server.arg("NUM_MAX1").toInt();
  if(server.arg("kuOn")!="") kuOn = server.arg("kuOn").toInt();
  if(server.arg("kuOff")!="") kuOff = server.arg("kuOff").toInt();

  if(server.arg("sensor00")!="" && server.arg("param0")!=""){
    int sens = server.arg("sensor00").toInt();
    int param = server.arg("param0").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<16))&&param<24)){
      sensor00 = server.arg("sensor00").toInt();
      param0 = server.arg("param0").toInt();
    }
  }
  if(server.arg("sensor01")!="" && server.arg("param1")!=""){
    int sens = server.arg("sensor01").toInt();
    int param = server.arg("param1").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<16))&&param<24)){
      sensor01 = sens;
      param1 = param;
    }
  }
  if(server.arg("sensor02")!="" && server.arg("param2")!=""){
    int sens = server.arg("sensor02").toInt();
    int param = server.arg("param2").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<16))&&param<24)){
      sensor02 = server.arg("sensor02").toInt();
      param2 = server.arg("param2").toInt();
    }
  }
  if(server.arg("sensor03")!="" && server.arg("param3")!=""){
    int sens = server.arg("sensor03").toInt();
    int param = server.arg("param3").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<16))&&param<24)||(sens==99&&param==3)){
      sensor03 = server.arg("sensor03").toInt();
      param3 = server.arg("param3").toInt();
    }
  }
  if(server.arg("sensor04")!="" && server.arg("param4")!=""){
    int sens = server.arg("sensor04").toInt();
    int param = server.arg("param4").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<16))&&param<24)||(sens==99&&param==21)){
      sensor04 = server.arg("sensor04").toInt();
      param4 = server.arg("param4").toInt();
    }
  }
  if(server.arg("pressSys")!="") pressSys = server.arg("pressSys").toInt();
  if(server.arg("fontCLOCK")!="") fontCLOCK = server.arg("fontCLOCK").toInt();
  if(server.arg("fontSizeCLOCK")!="") fontSizeCLOCK = server.arg("fontSizeCLOCK").toInt();
  if(server.arg("fontSizeData")!="") fontSizeData = server.arg("fontSizeData").toInt();
  if(server.arg("dataDown")!="") dataDown = server.arg("dataDown").toInt();
  if(server.arg("animPoint")!="") animPoint = server.arg("animPoint").toInt();
  if(server.arg("timeStopBigCklock")!="") timeStopBigCklock = server.arg("timeStopBigCklock").toInt();
  if(server.arg("textDown")!="") textDown = server.arg("textDown").toInt();
  if(server.arg("timeScrollSpeed")!="") timeScrollSpeed = 100 - server.arg("timeScrollSpeed").toInt();
  if(server.arg("corr00")!="") corr00 = server.arg("corr00").toFloat();
  if(server.arg("corr01")!="") corr01 = server.arg("corr01").toFloat();
  if(server.arg("corr02")!="") corr02 = server.arg("corr02").toFloat();
  if(server.arg("corr03")!="") corr03 = server.arg("corr03").toFloat();
  if(server.arg("corr04")!="") corr04 = server.arg("corr04").toInt();
  if(server.arg("displayData")!="") displayData = server.arg("displayData").toInt();
  if(server.arg("butStat")!="") butStat = server.arg("butStat").toInt();
  saveConfig();
  if(printCom) {
    printTime();
    Serial.println("TBD: "+String(timeDay)+", VBD: "+String(volBrightnessD)+", TBN: "+String(timeNight)+", VBN: "+String(volBrightnessN)+",  kuOn: "+String(kuOn)+",  kuOff: "+String(kuOff)+",  rotate0: "+String(rotate0)+", rotate1: "+String(rotate1)+", clock2line: "+String(clock2line)+", buzzerSet: "+String(buzzerSet));
    Serial.println("          sensor00: "+String(sensor00)+", sensor01: "+String(sensor01)+", sensor02: "+String(sensor02)+", sensor03: "+String(sensor03)+",  sensor04: "+String(sensor04));
    Serial.println("          param0: "+String(param0)+", param1: "+String(param1)+", param2: "+String(param2)+", param3: "+String(param3)+",  param4: "+String(param4));
  }
  sensorsAll();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_sgp(){
  if(verific()) return;
  if(server.arg("eCo2Led")!="") sgpCo2 = server.arg("eCo2Led").toInt();
  if(server.arg("sgpCo2LivelAlarm")!="") sgpCo2LivelAlarm = server.arg("sgpCo2LivelAlarm").toInt();
  if(server.arg("eCo2AlarmEsp")!="") eCo2AlarmEsp = server.arg("eCo2AlarmEsp").toInt();
  if(server.arg("eCo2AlarmMqtt")!="") eCo2AlarmMqtt = server.arg("eCo2AlarmMqtt").toInt();
  if(server.arg("tvocLed")!="") tvocLed = server.arg("tvocLed").toInt();
  if(server.arg("sgpTvocLivelAlarm")!="") sgpTvocLivelAlarm = server.arg("sgpTvocLivelAlarm").toInt();
  if(server.arg("tvocAlarmEsp")!="") tvocAlarmEsp = server.arg("tvocAlarmEsp").toInt();
  if(server.arg("tvocAlarmMqtt")!="") tvocAlarmMqtt = server.arg("tvocAlarmMqtt").toInt();
  if(server.arg("setSgpCorr")!="") setSgpCorr = server.arg("setSgpCorr").toInt();
  if(setSgpCorr == 99){
    if(server.arg("sgpCorrTemp")!="") sgpCorrTemp = server.arg("sgpCorrTemp").toFloat();
    if(server.arg("sgpCorrHumi")!="") sgpCorrHumi = server.arg("sgpCorrHumi").toFloat();
  }
  if(printCom){
    printTime();
    Serial.println("Set eCo2Led: " + String(eCo2Led) + ",  sgpCo2LivelAlarm: " + String(sgpCo2LivelAlarm) + ",  eCo2AlarmEsp: " + String(eCo2AlarmEsp) + ",  eCO2AlatmMqtt: " + String(eCo2AlarmMqtt));
    Serial.print("          tvocLed: " + String(tvocLed) + ",  sgpTvocLivelAlarm: " + String(sgpTvocLivelAlarm) + ",  tvocAlarmEsp: " + String(tvocAlarmEsp)+ ",  tvocAlarmMqtt: " + String(tvocAlarmMqtt) + ",  setSgpCorr: " + String(setSgpCorr));
    if(setSgpCorr == 99){
      if(printCom) Serial.println(",  sgpCorrTemp: " + String(sgpCorrTemp) + ",  sgpCorrMan: " + String(sgpCorrHumi));
    } else if(printCom) Serial.println("");
  }
  saveConfig(); 
  server.send(200, "text/plain", "OK");
  if(mqttOn) reconnect();
}
//======================================================================================================
void handle_mqtt_ust(){
  if(verific()) return;
  if(server.arg("mqtt_server")!="") snprintf(mqtt_server, 24, "%s", server.arg("mqtt_server").c_str());
  if(server.arg("mqtt_port")!="") mqtt_port = server.arg("mqtt_port").toInt();
  snprintf(mqtt_user, 24, "%s", server.arg("mqtt_user").c_str());
  snprintf(mqtt_pass, 24, "%s", server.arg("mqtt_pass").c_str());
  if(server.arg("mqtt_name")!="") snprintf(mqtt_name, 24, "%s", server.arg("mqtt_name").c_str());
  snprintf(mqtt_sub1, 24, "%s", server.arg("mqtt_sub1").c_str());
  snprintf(mqtt_sub2, 24, "%s", server.arg("mqtt_sub2").c_str());
  snprintf(mqtt_sub3, 24, "%s", server.arg("mqtt_sub3").c_str());
  snprintf(mqtt_sub_inform, 24, "%s", server.arg("mqtt_sub_inform").c_str());
  snprintf(mqtt_pub_sensor00, 24, "%s", server.arg("mqtt_pub_sensor00").c_str());
  snprintf(mqtt_pub_sensor01, 24, "%s", server.arg("mqtt_pub_sensor01").c_str());
  snprintf(mqtt_pub_sensor02, 24, "%s", server.arg("mqtt_pub_sensor02").c_str());
  snprintf(mqtt_pub_sensor03, 24, "%s", server.arg("mqtt_pub_sensor03").c_str());
  snprintf(mqtt_pub_sensor04, 24, "%s", server.arg("mqtt_pub_sensor04").c_str());
  snprintf(mqtt_pub_eco2, 24, "%s", server.arg("mqtt_pub_eco2").c_str());
  snprintf(mqtt_pub_tvoc, 24, "%s", server.arg("mqtt_pub_tvoc").c_str());
  if(server.arg("timeOutMqtt")!="") timeOutMqtt = server.arg("timeOutMqtt").toInt();
  if(printCom){
    printTime();
    Serial.println("Set mqtt_server: " + String(mqtt_server) + ",  mqtt_port: " + String(mqtt_port) + ",  mqtt_user: " + String(mqtt_user) + ",  mqtt_pass: " + String(mqtt_pass) + ", mqtt_name: " + String(mqtt_name));
    Serial.println("          mqtt_sub1: " + String(mqtt_sub1) + ",  mqtt_sub2: " + String(mqtt_sub2) + ",  mqtt_sub3: " + String(mqtt_sub3)+ ",  mqtt_sub_inform: " + String(mqtt_sub_inform) + ", mqtt_pub_eco2: " + String(mqtt_pub_eco2) + ",  mqtt_pub_tvoc: " + String(mqtt_pub_tvoc));
    Serial.println("          mqtt_pub_sensor00: " + String(mqtt_pub_sensor00) + ",  mqtt_pub_sensor01: " + String(mqtt_pub_sensor01) + ",  mqtt_pub_sensor02: " + String(mqtt_pub_sensor02) + ",  mqtt_pub_sensor03: " + String(mqtt_pub_sensor03)) + ",  mqtt_pub_sensor04: " + String(mqtt_pub_sensor04);
  }
  saveConfig(); 
  server.send(200, "text/plain", "OK");
  if(mqttOn) reconnect();
}
//======================================================================================================
void handle_mqtt_on(){
  if(verific()) return;
  if(server.arg("mqttOn")!="") mqttOn = server.arg("mqttOn").toInt(); 
  if(printCom){
    printTime();
    Serial.println(server.arg("mqttOn"));
  }
  if(mqttOn) reconnect();
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_thing_on(){
  if(verific()) return;
  if(server.arg("sendThing")!=""){
    sendThing = server.arg("sendThing").toInt();
    if(sendThing) sendThings();
  }
  if(server.arg("getThing")!=""){
    getThing = server.arg("getThing").toInt();
    if(getThing) getThings();
  }
  if(printCom){
    printTime();
    Serial.println("sendThing = " + String(sendThing) + ",  getThing = " + String(getThing));
  }
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_set_thing(){
  if(verific()) return;
  channelIdWrite = server.arg("channelIdWrite").c_str();
  writeApiKey = server.arg("writeApiKey").c_str();
  wrApiKey = server.arg("wrApiKey").c_str();
  if(server.arg("writeFild0")!="") writeFild0 = server.arg("writeFild0").toInt();
  if(server.arg("writeFild1")!="") writeFild1 = server.arg("writeFild1").toInt();
  if(server.arg("writeFild2")!="") writeFild2 = server.arg("writeFild2").toInt();
  if(server.arg("writeFild3")!="") writeFild3 = server.arg("writeFild3").toInt();
  if(server.arg("writeFild4")!="") writeFild4 = server.arg("writeFild4").toInt();
  if(server.arg("writeFild5")!="") writeFild5 = server.arg("writeFild5").toInt();
  if(server.arg("writeFild6")!="") writeFild6 = server.arg("writeFild6").toInt();
  if(server.arg("writeFild7")!="") writeFild7 = server.arg("writeFild7").toInt();
  channelIdRead = server.arg("channelIdRead").c_str();
  readApiKey = server.arg("readApiKey").c_str();
  if(server.arg("statThing0")!="") statThing0 = server.arg("statThing0").toInt();
  if(server.arg("statThing1")!="") statThing1 = server.arg("statThing1").toInt();
  if(server.arg("statThing2")!="") statThing2 = server.arg("statThing2").toInt();
  if(server.arg("statThing3")!="") statThing3 = server.arg("statThing3").toInt();
  if(server.arg("statThing4")!="") statThing4 = server.arg("statThing4").toInt();
  if(printCom){
    printTime();
    Serial.println("channelIdWrite= " + channelIdWrite + ", writeApiKey= " + writeApiKey + ", wrApiKey= " + wrApiKey);
    Serial.println("             writeFild0 = " + String(writeFild0) + ", writeFild1 = " + String(writeFild1) + ", writeFild2 = " + String(writeFild2) + ", writeFild3 = " + String(writeFild3) + ", writeFild4 = " + String(writeFild4)  + ", writeFild5 = " + String(writeFild5)  + ", writeFild6 = " + String(writeFild6) + ", writeFild7 = " + String(writeFild7));
    Serial.println("             channelIdRead= " + channelIdRead + ", readApiKey= " + readApiKey + ", statThing0 = " + String(statThing0) + ", statThing1 = " + String(statThing1) + ", statThing2 = " + String(statThing2) + ", statThing3 = " + String(statThing3) + ", statThing4 = " + String(statThing4));

  }
  saveConfig();
  if(sendThing) sendThings();
  if(getThing) getThings();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_message(){
  if(verific()) return;
  if(server.arg("text")!=""){
    server.send(200, "text/plain", "OK");
    String text = server.arg("text").c_str();
    for(int i = 0; i < 3; i++){
      bip();
    }
    printStringWithShift(("        " + String(text) + "           ").c_str(), timeScrollSpeed, 1);
    if(printCom) {
      printTime();
      Serial.println(text);
    }
  }
}
//======================================================================================================
void handle_stopAlarm(){
  if(verific()) return;
  if(server.arg("stopAlarm")=="ok"){ 
    if(alarm_stat){
      stopAlarm = true;
      if(printCom){
        printTime();
        Serial.println("STOP ALARM");
      }
      bip();
    }
  }
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_resetConfig(){
  if(verific()) return;
  if(server.arg("device") == "ok"){
    //SPIFFS.format();
    SPIFFS.remove("/config.json");
    if(printCom){
      printTime();
      Serial.println("ESP erase Config file");
    }
    delay(3000);
    server.send(200, "text/plain", "OK");
    delay(3000);
    bip();
    ESP.reset();
  }
}
//======================================================================================================
void handle_set_printCom(){
  if(verific()) return;
  if(server.arg("printCom")!=""){
    printCom = server.arg("printCom").toInt();
    if(printCom){
      printTime();
      Serial.println("Set printCom: " + String(printCom));
    }
    saveConfig();
  }
  server.send(200, "text/plain", "OK"); 
}
//======================================================================================================
void handle_Restart(){
  if(verific()) return;
  if(server.arg("device") == "ok"){
    server.send(200, "text/plain", "OK"); 
    ESP.reset();
  }
}
//====================================== Тут функції для роботи з файловою системою
String getContentType(String filename){
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".vz")) return "text/html";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//======================================= Читання файлу
bool handleFileRead(String path){
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
//======================================== Завантаження файлу
void handleFileUpload(){
  if (server.uri() != "/edit") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
  }
}
//======================================== Видалення файлу
void handleFileDelete(){
  if (server.args() == 0) return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}
//========================================= Створення файлу
void handleFileCreate(){
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return server.send(500, "text/plain", "CREATE FAILED");
  server.send(200, "text/plain", "");
  path = String();
}
//========================================== Список файлів 
void handleFileList(){
  if (!server.hasArg("dir")){
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = server.arg("dir");
  Dir dir = SPIFFS.openDir(path);
  path = String();
  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  output += "]";
  server.send(200, "text/json", output);
}
//==========================================
bool verific(){
  if(server.arg("auth")!=auth && authOn){
    server.send_P(404, "text/html", "404, you are not authorized!");
    return true;
  }
  return false;
}

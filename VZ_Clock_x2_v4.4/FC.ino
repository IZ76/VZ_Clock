bool loadConfig() {                                                // Завантаження даних збереженних в файлі config.json
  File configFile = SPIFFS.open("/config.json", "r");              // Відкриваемо файл на читання
  if(!configFile) {                                                // якщо файл не знайдено ствоюємого його та записуємо в ньго данні з наших змінних
    if(printCom) Serial.println("Failed to open config file");
    saveConfig();                                                  // Створюємо файл
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  size_t size = configFile.size();                                 // Перевіряємо ромір файлу, будемо використовувати файл довжиною в 1024 байта
  if(printCom) Serial.println("size config.file = " + (String) size);
  if(size > 4096) {
    if(printCom) Serial.println("Config file size is too large");
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  jsonConfig = configFile.readString();                            // завантажуємо файл конфігурації в глобальну змінну
  DynamicJsonDocument doc(4096);                                   // Резервуємо память для json обекту буфер може розти по мірі необхідності переважно для ESP8266 
  deserializeJson(doc, jsonConfig);
  configFile.close();
  ssidAP = doc["ssidAP"].as<String>();
  passwordAP = doc["passwordAP"].as<String>();                     // Так отримуємо рядок
  ssid = doc["ssid"].as<String>();
  password = doc["password"].as<String>();
  auth = doc["auth"].as<String>();
  authOn = doc["authOn"];
  kuOn = doc["kuOn"];
  kuOff = doc["kuOff"];
  weatherHost = doc["weatherHost"];
  weatherKey0 = doc["weatherKey0"].as<String>();
  weatherKey1 = doc["weatherKey1"].as<String>();
  cityID0 = doc["cityID0"].as<String>();
  cityID1 = doc["cityID1"].as<String>();
  snprintf(personalCityName, 51, "%s", (doc["personalCityName"].as<String>()).c_str());
  weatherLang = doc["weatherLang"].as<String>();
  displayForecast = doc["displayForecast"];
  displayCityName = doc["displayCityName"];
  displayForecastNow = doc["displayForecastNow"];
  displayForecastToday = doc["displayForecastToday"];
  displayForecastTomorrow = doc["displayForecastTomorrow"];
  animNotWeather = doc["animNotWeather"];
  timeStartViewWeather = doc["timeStartViewWeather"];
  timeEndViewWeather = doc["timeEndViewWeather"];
  timeScrollSpeed = doc["timeScrollSpeed"];
  rotate0 = doc["rotate0"];
  rotate1 = doc["rotate1"];
  updateOTA = doc["updateOTA"];
  volBrightnessD = doc["volBrightnessD"];
  volBrightnessN = doc["volBrightnessN"];
  timeDay = doc["timeDay"];
  timeNight = doc["timeNight"];
  volBrightnessAuto = doc["volBrightnessAuto"];
  clock2line = doc["clock2line"];
  secLine = doc["secLine"];
  buzzerSet = doc["buzzerSet"];
  mqttOn = doc["mqttOn"];
  snprintf(mqtt_server, 24, "%s", (doc["mqtt_server"].as<String>()).c_str());
  mqtt_port = doc["mqtt_port"];
  snprintf(mqtt_user, 24, "%s", (doc["mqtt_user"].as<String>()).c_str());
  snprintf(mqtt_pass, 24, "%s", (doc["mqtt_pass"].as<String>()).c_str());
  snprintf(mqtt_name, 24, "%s", (doc["mqtt_name"].as<String>()).c_str());
  snprintf(mqtt_pub_sensor00, 24, "%s", (doc["mqtt_pub_sensor00"].as<String>()).c_str());
  snprintf(mqtt_pub_sensor01, 24, "%s", (doc["mqtt_pub_sensor01"].as<String>()).c_str());
  snprintf(mqtt_pub_sensor02, 24, "%s", (doc["mqtt_pub_sensor02"].as<String>()).c_str());
  snprintf(mqtt_pub_sensor03, 24, "%s", (doc["mqtt_pub_sensor03"].as<String>()).c_str());
  snprintf(mqtt_pub_sensor04, 24, "%s", (doc["mqtt_pub_sensor04"].as<String>()).c_str());
  snprintf(mqtt_pub_eco2, 24, "%s", (doc["mqtt_pub_eco2"].as<String>()).c_str());
  snprintf(mqtt_pub_eco2, 24, "%s", (doc["mqtt_pub_eco2"].as<String>()).c_str());
  snprintf(mqtt_pub_tvoc, 24, "%s", (doc["mqtt_pub_tvoc"].as<String>()).c_str());
  snprintf(mqtt_sub1, 24, "%s", (doc["mqtt_sub1"].as<String>()).c_str());
  snprintf(mqtt_sub2, 24, "%s", (doc["mqtt_sub2"].as<String>()).c_str());
  snprintf(mqtt_sub3, 24, "%s", (doc["mqtt_sub3"].as<String>()).c_str());
  snprintf(mqtt_sub_inform, 24, "%s", (doc["mqtt_sub_inform"].as<String>()).c_str());
  printCom = doc["printCom"];
  sensor00 = doc["sensor00"];
  sensor01 = doc["sensor01"];
  sensor02 = doc["sensor02"];
  sensor03 = doc["sensor03"];
  sensor04 = doc["sensor04"];
  param0 = doc["param0"];
  param1 = doc["param1"];
  param2 = doc["param2"];
  param3 = doc["param3"];
  param4 = doc["param4"];
  pressSys = doc["pressSys"];
  fontCLOCK = doc["fontCLOCK"];
  fontSizeCLOCK = doc["fontSizeCLOCK"];
  fontSizeData = doc["fontSizeData"];
  textDown = doc["textDown"];
  timeStopBigCklock = doc["timeStopBigCklock"];
  dataDown = doc["dataDown"];
  animPoint = doc["animPoint"];
  corr00 = doc["corr00"];
  corr01 = doc["corr01"];
  corr02 = doc["corr02"];
  corr03  = doc["corr03"];
  corr04 = doc["corr04"];
  NUM_MAX0 = doc["NUM_MAX0"];
  NUM_MAX1 = doc["NUM_MAX1"];
  uuid  = doc["uuid"].as<String>();
  api_key = doc["api_key"].as<String>();
  sensors_ID0 = doc["sensors_ID0"];
  sensors_ID1 = doc["sensors_ID1"];
  sensors_ID2 = doc["sensors_ID2"];
  displayData = doc["displayData"];
  timeOutMqtt = doc["timeOutMqtt"];
  butStat = doc["butStat"];
  sgpCo2LivelAlarm = doc["sgpCo2LivelAlarm"];
  eCo2AlarmEsp = doc["eCo2AlarmEsp"];
  eCo2AlarmMqtt = doc["eCo2AlarmMqtt"];
  eCo2Led = doc["eCo2Led"];
  sgpTvocLivelAlarm = doc["sgpTvocLivelAlarm"];
  tvocAlarmEsp = doc["tvocAlarmEsp"];
  tvocAlarmMqtt = doc["tvocAlarmMqtt"];
  tvocLed = doc["tvocLed"];
  setSgpCorr = doc["setSgpCorr"];
  sendThing = doc["sendThing"];
  channelIdWrite = doc["channelIdWrite"].as<String>();
  writeApiKey = doc["writeApiKey"].as<String>();
  wrApiKey = doc["wrApiKey"].as<String>();
  writeFild0 = doc["writeFild0"];
  writeFild1 = doc["writeFild1"];
  writeFild2 = doc["writeFild2"];
  writeFild3 = doc["writeFild3"];
  writeFild4 = doc["writeFild4"];
  writeFild5 = doc["writeFild5"];
  writeFild6 = doc["writeFild6"];
  writeFild7 = doc["writeFild7"];
  getThing = doc["getThing"];
  channelIdRead = doc["channelIdRead"].as<String>();
  readApiKey = doc["readApiKey"].as<String>();
  statThing0 = doc["statThing0"];
  statThing1 = doc["statThing1"];
  statThing2 = doc["statThing2"];
  statThing3 = doc["statThing3"];
  statThing4 = doc["statThing4"];
  
  if(printCom) {
    printTime();
    Serial.print("Load Config(config): ");
    Serial.println(jsonConfig);
  }
  return true;
}
//================================================================================================================================================
bool loadAlarm() {                                                // Завантаження даних збереженних в файлі config.json
  File configFile = SPIFFS.open("/alarm.json", "r");              // Відкриваемо файл на читання
  if(!configFile) {                                                // якщо файл не знайдено ствоюємого його та записуємо в ньго данні з наших змінних
    if(printCom) Serial.println("Failed to open alarm file");
    saveAlarm();                                                  // Створюємо файл
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  size_t size = configFile.size();                                 // Перевіряємо ромір файлу, будемо використовувати файл довжиною в 1024 байта
  if(printCom) Serial.println("size alarme.file = " + (String) size);
  if(size > 2048) {
    if(printCom) Serial.println("Config file size is too large");
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  jsonAlarm = configFile.readString();                            // завантажуємо файл конфігурації в глобальну змінну
  DynamicJsonDocument doc(2048);                                   // Резервуємо память для json обекту буфер може розти по мірі необхідності переважно для ESP8266 
  deserializeJson(doc, jsonAlarm);
  configFile.close();
  ntpServerName = doc["ntpServerName"].as<String>();
  timeZone = doc["timeZone"];                                      // Так отримуємо число
  isDayLightSaving = doc["isDayLightSaving"];
  alarme[0][0] = doc["al_0_0"];
  alarme[0][1] = doc["al_0_1"];
  alarme[0][2] = doc["al_0_2"];
  alarme[1][0] = doc["al_1_0"];
  alarme[1][1] = doc["al_1_1"];
  alarme[1][2] = doc["al_1_2"];
  alarme[2][0] = doc["al_2_0"];
  alarme[2][1] = doc["al_2_1"];
  alarme[2][2] = doc["al_2_2"];
  alarme[3][0] = doc["al_3_0"];
  alarme[3][1] = doc["al_3_1"];
  alarme[3][2] = doc["al_3_2"];
  alarme[4][0] = doc["al_4_0"];
  alarme[4][1] = doc["al_4_1"];
  alarme[4][2] = doc["al_4_2"];
  memory_date[0][0] = doc["md_0_0"];
  memory_date[0][1] = doc["md_0_1"];
  memory_date[1][0] = doc["md_1_0"];
  memory_date[1][1] = doc["md_1_1"];
  memory_date[2][0] = doc["md_2_0"];
  memory_date[2][1] = doc["md_2_1"];
  memory_date[3][0] = doc["md_3_0"];
  memory_date[3][1] = doc["md_3_1"];
  memory_date[4][0] = doc["md_4_0"];
  memory_date[4][1] = doc["md_4_1"];
  memory_date[5][0] = doc["md_5_0"];
  memory_date[5][1] = doc["md_5_1"];
  memory_date[6][0] = doc["md_6_0"];
  memory_date[6][1] = doc["md_6_1"];
  memory_date[7][0] = doc["md_7_0"];
  memory_date[7][1] = doc["md_7_1"];
  memory_date[8][0] = doc["md_8_0"];
  memory_date[8][1] = doc["md_8_1"];
  snprintf(memory_date_mes0, 51, "%s", (doc["md_0_2"].as<String>()).c_str());
  snprintf(memory_date_mes1, 51, "%s", (doc["md_1_2"].as<String>()).c_str());
  snprintf(memory_date_mes2, 51, "%s", (doc["md_2_2"].as<String>()).c_str());
  snprintf(memory_date_mes3, 51, "%s", (doc["md_3_2"].as<String>()).c_str());
  snprintf(memory_date_mes4, 51, "%s", (doc["md_4_2"].as<String>()).c_str());
  snprintf(memory_date_mes5, 51, "%s", (doc["md_5_2"].as<String>()).c_str());
  snprintf(memory_date_mes6, 51, "%s", (doc["md_6_2"].as<String>()).c_str());
  snprintf(memory_date_mes7, 51, "%s", (doc["md_7_2"].as<String>()).c_str());
  snprintf(memory_date_mes8, 51, "%s", (doc["md_8_2"].as<String>()).c_str());
  memory_hour_start = doc["memory_hour_start"];
  memory_hour_end = doc["memory_hour_end"];
  rtcStat = doc["rtcStat"];
  if(printCom) {
    printTime();
    Serial.print("Load Alarm(config): ");
    Serial.println(jsonAlarm);
  }
  return true;
}
//======================================================
bool loadTime() {
  File timeFile = SPIFFS.open("/time.json", "r");
  if(!timeFile) {
    if(printCom) Serial.println("Failed to open time file");
    saveTime();
    timeFile.close();
    return false;
  }
  size_t size = timeFile.size();
  if(printCom) Serial.println("size time.file = " + (String) size);
  if(size > 512) {
    if(printCom) Serial.println("Time file size is too large");
    timeFile.close();
    return false;
  }
  jsonTime = timeFile.readString();
  DynamicJsonDocument doc(512);
  deserializeJson(doc, jsonTime);
  timeFile.close();
  hour = doc["hour"];
  minute = doc["minute"];
  second = doc["second"]; 
  year = doc["year"];
  month = doc["month"];
  day = doc["day"]; 
  if(printCom) {
    printTime();
    Serial.print("Load Time(config): ");
    Serial.println(jsonTime);
  }
  return true;
}
//=================================================================
bool saveConfig() {
  DynamicJsonDocument doc(4096);
  deserializeJson(doc, jsonConfig);
  doc["ssidAP"] = ssidAP;
  doc["passwordAP"] = passwordAP;
  doc["ssid"] = ssid;
  doc["password"] = password;
  doc["auth"] = auth;
  doc["authOn"] = authOn;
  doc["kuOn"] = kuOn;
  doc["kuOff"] = kuOff;
  doc["weatherHost"] = weatherHost;
  doc["weatherKey0"] = weatherKey0;
  doc["weatherKey1"] = weatherKey1;
  doc["cityID0"] = cityID0;
  doc["cityID1"] = cityID1;
  doc["personalCityName"] = personalCityName;
  doc["weatherLang"] = weatherLang;
  doc["displayForecast"] = displayForecast;
  doc["displayCityName"] = displayCityName;
  doc["displayForecastNow"] = displayForecastNow;
  doc["displayForecastToday"] = displayForecastToday;
  doc["displayForecastTomorrow"] = displayForecastTomorrow;
  doc["animNotWeather"] = animNotWeather;
  doc["timeStartViewWeather"] = timeStartViewWeather;
  doc["timeEndViewWeather"] = timeEndViewWeather;
  doc["timeScrollSpeed"] = timeScrollSpeed;
  doc["rotate0"] = rotate0;
  doc["rotate1"] = rotate1;
  doc["updateOTA"] = updateOTA;
  doc["volBrightnessD"] = volBrightnessD;
  doc["volBrightnessN"] = volBrightnessN;
  doc["timeDay"] = timeDay;
  doc["timeNight"] = timeNight;
  doc["secLine"] = secLine;
  doc["volBrightnessAuto"] = volBrightnessAuto;
  doc["clock2line"] = clock2line;
  doc["buzzerSet"] = buzzerSet;
  doc["mqttOn"] = mqttOn;
  doc["mqtt_server"] = mqtt_server;
  doc["mqtt_port"] = mqtt_port;
  doc["mqtt_user"] = mqtt_user;
  doc["mqtt_pass"] = mqtt_pass;
  doc["mqtt_name"] = mqtt_name;
  doc["mqtt_pub_sensor00"] = mqtt_pub_sensor00;
  doc["mqtt_pub_sensor01"] = mqtt_pub_sensor01;
  doc["mqtt_pub_sensor02"] = mqtt_pub_sensor02;
  doc["mqtt_pub_sensor03"] = mqtt_pub_sensor03;
  doc["mqtt_pub_sensor04"] = mqtt_pub_sensor04;
  doc["mqtt_pub_eco2"] = mqtt_pub_eco2;
  doc["mqtt_pub_eco2"] = mqtt_pub_eco2;
  doc["mqtt_pub_tvoc"] = mqtt_pub_tvoc;
  doc["mqtt_sub1"] = mqtt_sub1;
  doc["mqtt_sub2"] = mqtt_sub2;
  doc["mqtt_sub3"] = mqtt_sub3;
  doc["mqtt_sub_inform"] = mqtt_sub_inform;
  doc["printCom"] = printCom;
  doc["sensor00"] = sensor00;
  doc["sensor01"] = sensor01;
  doc["sensor02"] = sensor02;
  doc["sensor03"] = sensor03;
  doc["sensor04"] = sensor04;
  doc["param0"] = param0; 
  doc["param1"] = param1;
  doc["param2"] = param2;
  doc["param3"] = param3;
  doc["param4"] = param4;
  doc["pressSys"] = pressSys;
  doc["fontCLOCK"] = fontCLOCK;
  doc["fontSizeCLOCK"] = fontSizeCLOCK;
  doc["fontSizeData"] = fontSizeData;
  doc["textDown"] = textDown;
  doc["timeStopBigCklock"] = timeStopBigCklock;
  doc["dataDown"] = dataDown;
  doc["animPoint"] = animPoint;
  doc["corr00"] = corr00;
  doc["corr01"] = corr01;
  doc["corr02"] = corr02;
  doc["corr03"] = corr03;
  doc["corr04"] = corr04;
  doc["NUM_MAX0"] = NUM_MAX0;
  doc["NUM_MAX1"] = NUM_MAX1;
  doc["uuid"] = uuid;
  doc["api_key"] = api_key;
  doc["sensors_ID0"] = sensors_ID0;
  doc["sensors_ID1"] = sensors_ID1;
  doc["sensors_ID2"] = sensors_ID2;
  doc["displayData"] = displayData;
  doc["timeOutMqtt"] = timeOutMqtt;
  doc["butStat"] = butStat;
  doc["sgpCo2LivelAlarm"] = sgpCo2LivelAlarm;
  doc["eCo2AlarmEsp"] = eCo2AlarmEsp;
  doc["eCo2AlarmMqtt"] = eCo2AlarmMqtt;
  doc["eCo2Led"] = eCo2Led;
  doc["sgpTvocLivelAlarm"] = sgpTvocLivelAlarm;
  doc["tvocAlarmEsp"] = tvocAlarmEsp;
  doc["tvocAlarmMqtt"] = tvocAlarmMqtt;
  doc["tvocLed"] = tvocLed;
  doc["setSgpCorr"] = setSgpCorr;
  doc["sendThing"] = sendThing;
  doc["channelIdWrite"] = channelIdWrite;
  doc["writeApiKey"] = writeApiKey;
  doc["wrApiKey"] = wrApiKey;
  doc["writeFild0"] = writeFild0;
  doc["writeFild1"] = writeFild1;
  doc["writeFild2"] = writeFild2;
  doc["writeFild3"] = writeFild3;
  doc["writeFild4"] = writeFild4;
  doc["writeFild5"] = writeFild5;
  doc["writeFild6"] = writeFild6;
  doc["writeFild7"] = writeFild7;
  doc["getThing"] = getThing;
  doc["channelIdRead"] = channelIdRead;
  doc["readApiKey"] = readApiKey;
  doc["statThing0"] = statThing0;
  doc["statThing1"] = statThing1;
  doc["statThing2"] = statThing2;
  doc["statThing3"] = statThing3;
  doc["statThing4"] = statThing4;
  jsonConfig = "";
  if(serializeJson(doc, jsonConfig)==0){
    if(printCom) Serial.println(F("Failed to write to jsonConfig"));
  }  
  File configFile = SPIFFS.open("/config.json", "w");                // Відкриваємо файл для запису
  if(!configFile) {
    configFile.close();
    return false;
  }
  if(serializeJson(doc, configFile)==0){
    if(printCom) Serial.println(F("Failed to write to file"));
  } 
  if(printCom) {
    printTime();
    Serial.print("Save Config : ");
    Serial.println(jsonConfig);
  }
  configFile.close();
  clrAll();
  bip();
  return true;
}
//=================================================================
bool saveAlarm() {
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, jsonAlarm);
  doc["ntpServerName"] = ntpServerName;
  doc["timeZone"] = timeZone;
  doc["isDayLightSaving"] = isDayLightSaving;
  doc["al_0_0"] = alarme[0][0];
  doc["al_0_1"] = alarme[0][1];
  doc["al_0_2"] = alarme[0][2];
  doc["al_1_0"] = alarme[1][0];
  doc["al_1_1"] = alarme[1][1];
  doc["al_1_2"] = alarme[1][2];
  doc["al_2_0"] = alarme[2][0];
  doc["al_2_1"] = alarme[2][1];
  doc["al_2_2"] = alarme[2][2];
  doc["al_3_0"] = alarme[3][0];
  doc["al_3_1"] = alarme[3][1];
  doc["al_3_2"] = alarme[3][2];
  doc["al_4_0"] = alarme[4][0];
  doc["al_4_1"] = alarme[4][1];
  doc["al_4_2"] = alarme[4][2];
  doc["md_0_0"] = memory_date[0][0];
  doc["md_0_1"] = memory_date[0][1];
  doc["md_1_0"] = memory_date[1][0];
  doc["md_1_1"] = memory_date[1][1];
  doc["md_2_0"] = memory_date[2][0];
  doc["md_2_1"] = memory_date[2][1];
  doc["md_3_0"] = memory_date[3][0];
  doc["md_3_1"] = memory_date[3][1];
  doc["md_4_0"] = memory_date[4][0];
  doc["md_4_1"] = memory_date[4][1];
  doc["md_5_0"] = memory_date[5][0];
  doc["md_5_1"] = memory_date[5][1];
  doc["md_6_0"] = memory_date[6][0];
  doc["md_6_1"] = memory_date[6][1];
  doc["md_7_0"] = memory_date[7][0];
  doc["md_7_1"] = memory_date[7][1];
  doc["md_8_0"] = memory_date[8][0];
  doc["md_8_1"] = memory_date[8][1];
  doc["md_0_2"] = memory_date_mes0;
  doc["md_1_2"] = memory_date_mes1;
  doc["md_2_2"] = memory_date_mes2;
  doc["md_3_2"] = memory_date_mes3;
  doc["md_4_2"] = memory_date_mes4;
  doc["md_5_2"] = memory_date_mes5;
  doc["md_6_2"] = memory_date_mes6;
  doc["md_7_2"] = memory_date_mes7;
  doc["md_8_2"] = memory_date_mes8;
  doc["memory_hour_start"] = memory_hour_start;
  doc["memory_hour_end"] = memory_hour_end;
  doc["rtcStat"] = rtcStat;
  jsonAlarm = "";
  if(serializeJson(doc, jsonAlarm)==0){
    if(printCom) Serial.println(F("Failed to write to jsonConfig"));
  }  
  File configFile = SPIFFS.open("/alarm.json", "w");                // Відкриваємо файл для запису
  if(!configFile) {
    configFile.close();
    return false;
  }
  if(serializeJson(doc, configFile)==0){
    if(printCom) Serial.println(F("Failed to write to file"));
  } 
  if(printCom) {
    printTime();
    Serial.print("Save Alarm : ");
    Serial.println(jsonAlarm);
  }
  configFile.close();
  bip();
  return true;
}
//=================================================================
bool saveTime() {
  DynamicJsonDocument doc(512);
  deserializeJson(doc, jsonTime);
  doc["hour"] = hour;
  doc["minute"] = minute;
  doc["second"] = second;
  doc["year"] = year;
  doc["month"] = month;
  doc["day"] = day; 
  jsonTime = "";
  if(serializeJson(doc, jsonTime)==0){
    if(printCom) Serial.println(F("Failed to write to jsonTime"));
  }  
  File timeFile = SPIFFS.open("/time.json", "w");
  if(!timeFile) {
    timeFile.close();
    return false;
  }
  if(serializeJson(doc, timeFile)==0){
    if(printCom) Serial.println(F("Failed to write to timeFile "));
  } 
  if(printCom) {
    printTime();
    Serial.print("Save Time: ");
    Serial.println(jsonTime);
  }
  timeFile.close();
  return true;
}

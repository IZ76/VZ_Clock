//=====================================================================
void loadSsids(){
  File ssidFile=SPIFFS.open("/ssids.json","r");
  if(!ssidFile){
    ssidFile.close();
    saveSsids();
    return;
  }
  size_t size=ssidFile.size();
  if(size>512 && senss.printCom) Serial.println("AHTUNG!!! /ssids.json - size>512");
  jsonSsid=ssidFile.readString();
  DynamicJsonDocument doc(1024);
  deserializeJson(doc,jsonSsid);
  ssidFile.close();
  ssids.num=doc["num"];
  for(uint8_t i=0;i<ssids.num;i++){
    ssids.ssid[i]=doc["nets"][i*2].as<String>();
    ssids.pass[i]=doc["nets"][i*2+1].as<String>();
  }
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /ssids.json("+String(size)+"): "+jsonSsid);
  }
}
//=================================================================
void saveSsids(){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc,jsonSsid);
  doc["num"]=ssids.num;
  JsonArray nets=doc.createNestedArray("nets");
  for(uint8_t i=0;i<ssids.num;i++){
    nets.add(ssids.ssid[i]);
    nets.add(ssids.pass[i]);
  }
  jsonSsid="";
  serializeJson(doc,jsonSsid);
  File ssidFile=SPIFFS.open("/ssids.json","w");
  if(!ssidFile || serializeJson(doc, ssidFile)==0){
    if(senss.printCom) Serial.println(F("Failed to write to vz/ssids.json"));
    ssidFile.close();
    return;
  }
  ssidFile.close();
  bip();
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /ssids.json="+jsonSsid);
  }
}
//=====================================================================
void loadJssids(){
  File jssidFile=SPIFFS.open("/jssids.json","r");
  if(!jssidFile){
    jssidFile.close();
    DynamicJsonDocument doc(1024);
    for(uint8_t i=0;i<ssids.num;i++) doc[ssids.ssid[i]]=ssids.pass[i];
    jsonJssid="";
    serializeJson(doc,jsonJssid);
    File jssidFile=SPIFFS.open("/jssids.json", "w");
    jssidFile.print(jsonJssid);
    jssidFile.close();
    if(senss.printCom){
      printTime();
      Serial.println("SAVE /jssids.json="+jsonJssid);
    }
    return;
  }
  size_t size=jssidFile.size();
  jsonJssid=jssidFile.readString();
  jssidFile.close();
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /jssids.json("+String(size)+"): "+jsonJssid);
  }
}
//=====================================================================
void loadNet(){
  File wifiFile=SPIFFS.open("/wifis.json","r");
  if(!wifiFile){
    wifiFile.close();
    saveNet();
    return;
  }
  size_t size=wifiFile.size();
  if(size>512 && senss.printCom) Serial.println("AHTUNG!!! /wifis.json - size>512");
  jsonWifi=wifiFile.readString();
  DynamicJsonDocument doc(1024);
  deserializeJson(doc,jsonWifi);
  wifiFile.close();
  if(doc["APSSID"].as<String>()!="") doc["APSSID"].as<String>().toCharArray(indexs.ssidAP,(doc["APSSID"].as<String>().length())+1);
  doc["APPASS"].as<String>().toCharArray(indexs.passAP,(doc["APPASS"].as<String>().length())+1);
  doc["AUTH"].as<String>().toCharArray(indexs.auth,(doc["AUTH"].as<String>().length())+1);
  indexs.authOn=doc["AUTHON"];
  indexs.hiddenNetworks=doc["HIDDENNETWORKS"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /wifis.json("+String(size)+"): "+jsonWifi);
  }
}
//=================================================================
void saveNet(){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc,jsonWifi);
  doc["APSSID"]=indexs.ssidAP;
  doc["APPASS"]=indexs.passAP;
  doc["AUTH"]=indexs.auth;
  doc["AUTHON"]=indexs.authOn;
  doc["HIDDENNETWORKS"]=indexs.hiddenNetworks;
  jsonWifi="";
  serializeJson(doc,jsonWifi);
  File wifiFile=SPIFFS.open("/wifis.json","w");
  if(!wifiFile || serializeJson(doc, wifiFile)==0){
    if(senss.printCom) Serial.println(F("Failed to write to /wifis.json"));
    wifiFile.close();
    return;
  }
  wifiFile.close();
  //bip(1);
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /wifis.json="+jsonWifi);
  }
}
//==================================================================
void loadTimes(){
  File timesFile=SPIFFS.open("/times.json","r");
  if(!timesFile){
    saveTimes();
    timesFile.close();
    return;
  }
  size_t size=timesFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! times.json - size>2048");
  jsonTimes=timesFile.readString();
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonTimes);
  timesFile.close();
  times.ntpServerName=doc["ntpServerName"].as<String>();
  times.timeZone=doc["timeZone"];
  times.isDayLightSaving=doc["isDayLightSaving"];
  times.rtcStat=doc["rtcStat"];
  times.alarme[0][0]=doc["al_0_0"];
  times.alarme[0][1]=doc["al_0_1"];
  times.alarme[0][2]=doc["al_0_2"];
  times.alarme[1][0]=doc["al_1_0"];
  times.alarme[1][1]=doc["al_1_1"];
  times.alarme[1][2]=doc["al_1_2"];
  times.alarme[2][0]=doc["al_2_0"];
  times.alarme[2][1]=doc["al_2_1"];
  times.alarme[2][2]=doc["al_2_2"];
  times.alarme[3][0]=doc["al_3_0"];
  times.alarme[3][1]=doc["al_3_1"];
  times.alarme[3][2]=doc["al_3_2"];
  times.alarme[4][0]=doc["al_4_0"];
  times.alarme[4][1]=doc["al_4_1"];
  times.alarme[4][2]=doc["al_4_2"];
  times.memory_date[0][0]=doc["md_0_0"];
  times.memory_date[0][1]=doc["md_0_1"];
  times.memory_date[1][0]=doc["md_1_0"];
  times.memory_date[1][1]=doc["md_1_1"];
  times.memory_date[2][0]=doc["md_2_0"];
  times.memory_date[2][1]=doc["md_2_1"];
  times.memory_date[3][0]=doc["md_3_0"];
  times.memory_date[3][1]=doc["md_3_1"];
  times.memory_date[4][0]=doc["md_4_0"];
  times.memory_date[4][1]=doc["md_4_1"];
  times.memory_date[5][0]=doc["md_5_0"];
  times.memory_date[5][1]=doc["md_5_1"];
  times.memory_date[6][0]=doc["md_6_0"];
  times.memory_date[6][1]=doc["md_6_1"];
  times.memory_date[7][0]=doc["md_7_0"];
  times.memory_date[7][1]=doc["md_7_1"];
  times.memory_date[8][0]=doc["md_8_0"];
  times.memory_date[8][1]=doc["md_8_1"];
  snprintf(times.memory_date_mes[0],51,"%s",(doc["md_0_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[1],51,"%s",(doc["md_1_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[2],51,"%s",(doc["md_2_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[3],51,"%s",(doc["md_3_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[4],51,"%s",(doc["md_4_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[5],51,"%s",(doc["md_5_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[6],51,"%s",(doc["md_6_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[7],51,"%s",(doc["md_7_2"].as<String>()).c_str());
  snprintf(times.memory_date_mes[8],51,"%s",(doc["md_8_2"].as<String>()).c_str());
  times.nc_hour[0]=doc["nc_hour_0"];
  times.nc_hour[1]=doc["nc_hour_1"];
  times.nc_hour[2]=doc["nc_hour_2"];
  times.nc_hour[3]=doc["nc_hour_3"];
  times.nc_hour[4]=doc["nc_hour_4"];
  times.nc_min[0]=doc["nc_min_0"];
  times.nc_min[1]=doc["nc_min_1"];
  times.nc_min[2]=doc["nc_min_2"];
  times.nc_min[3]=doc["nc_min_3"];
  times.nc_min[4]=doc["nc_min_4"];
  snprintf(times.nc_mess[0],51,"%s",(doc["nc_mess_0"].as<String>()).c_str());
  snprintf(times.nc_mess[1],51,"%s",(doc["nc_mess_1"].as<String>()).c_str());
  snprintf(times.nc_mess[2],51,"%s",(doc["nc_mess_2"].as<String>()).c_str());
  snprintf(times.nc_mess[3],51,"%s",(doc["nc_mess_3"].as<String>()).c_str());
  snprintf(times.nc_mess[4],51,"%s",(doc["nc_mess_4"].as<String>()).c_str());
  times.memory_hour_start=doc["memory_hour_start"];
  times.memory_hour_end=doc["memory_hour_end"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /times.json("+String(size)+"): "+jsonTimes);
  }
}
//=================================================================
void saveTimes(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonTimes);
  doc["ntpServerName"]=times.ntpServerName;
  doc["timeZone"]=times.timeZone;
  doc["isDayLightSaving"]=times.isDayLightSaving;
  doc["rtcStat"]=times.rtcStat;
  doc["al_0_0"]=times.alarme[0][0];
  doc["al_0_1"]=times.alarme[0][1];
  doc["al_0_2"]=times.alarme[0][2];
  doc["al_1_0"]=times.alarme[1][0];
  doc["al_1_1"]=times.alarme[1][1];
  doc["al_1_2"]=times.alarme[1][2];
  doc["al_2_0"]=times.alarme[2][0];
  doc["al_2_1"]=times.alarme[2][1];
  doc["al_2_2"]=times.alarme[2][2];
  doc["al_3_0"]=times.alarme[3][0];
  doc["al_3_1"]=times.alarme[3][1];
  doc["al_3_2"]=times.alarme[3][2];
  doc["al_4_0"]=times.alarme[4][0];
  doc["al_4_1"]=times.alarme[4][1];
  doc["al_4_2"]=times.alarme[4][2];
  doc["md_0_0"]=times.memory_date[0][0];
  doc["md_0_1"]=times.memory_date[0][1];
  doc["md_1_0"]=times.memory_date[1][0];
  doc["md_1_1"]=times.memory_date[1][1];
  doc["md_2_0"]=times.memory_date[2][0];
  doc["md_2_1"]=times.memory_date[2][1];
  doc["md_3_0"]=times.memory_date[3][0];
  doc["md_3_1"]=times.memory_date[3][1];
  doc["md_4_0"]=times.memory_date[4][0];
  doc["md_4_1"]=times.memory_date[4][1];
  doc["md_5_0"]=times.memory_date[5][0];
  doc["md_5_1"]=times.memory_date[5][1];
  doc["md_6_0"]=times.memory_date[6][0];
  doc["md_6_1"]=times.memory_date[6][1];
  doc["md_7_0"]=times.memory_date[7][0];
  doc["md_7_1"]=times.memory_date[7][1];
  doc["md_8_0"]=times.memory_date[8][0];
  doc["md_8_1"]=times.memory_date[8][1];
  doc["md_0_2"]=times.memory_date_mes[0];
  doc["md_1_2"]=times.memory_date_mes[1];
  doc["md_2_2"]=times.memory_date_mes[2];
  doc["md_3_2"]=times.memory_date_mes[3];
  doc["md_4_2"]=times.memory_date_mes[4];
  doc["md_5_2"]=times.memory_date_mes[5];
  doc["md_6_2"]=times.memory_date_mes[6];
  doc["md_7_2"]=times.memory_date_mes[7];
  doc["md_8_2"]=times.memory_date_mes[8];
  doc["nc_hour_0"]=times.nc_hour[0];
  doc["nc_hour_1"]=times.nc_hour[1];
  doc["nc_hour_2"]=times.nc_hour[2];
  doc["nc_hour_3"]=times.nc_hour[3];
  doc["nc_hour_4"]=times.nc_hour[4];
  doc["nc_min_0"]=times.nc_min[0];
  doc["nc_min_1"]=times.nc_min[1];
  doc["nc_min_2"]=times.nc_min[2];
  doc["nc_min_3"]=times.nc_min[3];
  doc["nc_min_4"]=times.nc_min[4];
  doc["nc_mess_0"]=times.nc_mess[0];
  doc["nc_mess_1"]=times.nc_mess[1];
  doc["nc_mess_2"]=times.nc_mess[2];
  doc["nc_mess_3"]=times.nc_mess[3];
  doc["nc_mess_4"]=times.nc_mess[4];
  doc["memory_hour_start"]=times.memory_hour_start;
  doc["memory_hour_end"]=times.memory_hour_end;
  jsonTimes="";
  if(serializeJson(doc,jsonTimes)==0 && senss.printCom) Serial.println(F("Failed to write to jsonTimes"));
  File timesFile=SPIFFS.open("/times.json","w");
  if(!timesFile){
    timesFile.close();
    return;
  }
  if(serializeJson(doc, timesFile)==0 && senss.printCom) Serial.println(F("Failed to write to file"));
  timesFile.close();
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /times.json="+jsonTimes);
  }
}

//=================================================================
void loadWeaths(){
  File weathsFile=SPIFFS.open("/weaths.json","r");
  if(!weathsFile){
    saveWeaths();
    weathsFile.close();
    return;
  }
  size_t size=weathsFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! /weaths.json - size>2048");
  jsonWeaths=weathsFile.readString();
  DynamicJsonDocument doc(size*2);
  deserializeJson(doc,jsonWeaths);
  weathsFile.close();
  weaths.weathOn=doc["weathOn"];
  weaths.weatherHost=doc["weatherHost"];
  weaths.weatherKey0=doc["weatherKey0"].as<String>();
  weaths.weatherKey1=doc["weatherKey1"].as<String>();
  weaths.cityID0=doc["cityID0"].as<String>();
  weaths.cityID1=doc["cityID1"].as<String>();
  snprintf(weaths.personalCityName,51,"%s",(doc["personalCityName"].as<String>()).c_str());
  weaths.displayCityName=doc["displayCityName"];
  weaths.displayForecastNow=doc["displayForecastNow"];
  weaths.displayForecastToday=doc["displayForecastToday"];
  weaths.displayForecastTomorrow=doc["displayForecastTomorrow"];
  weaths.displayForecastWind=doc["displayForecastWind"];
  weaths.animNotWeather=doc["animNotWeather"];
  weaths.timeStartViewWeather=doc["timeStartViewWeather"];
  weaths.timeEndViewWeather=doc["timeEndViewWeather"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /weaths.json("+String(size)+"): "+jsonWeaths);
  }
}
//=================================================================
void saveWeaths(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonWeaths);
  doc["weathOn"]=weaths.weathOn;
  doc["weatherHost"]=weaths.weatherHost;
  doc["weatherKey0"]=weaths.weatherKey0;
  doc["weatherKey1"]=weaths.weatherKey1;
  doc["cityID0"]=weaths.cityID0;
  doc["cityID1"]=weaths.cityID1;
  doc["personalCityName"]=weaths.personalCityName;
  doc["displayCityName"]=weaths.displayCityName;
  doc["displayForecastNow"]=weaths.displayForecastNow;
  doc["displayForecastToday"]=weaths.displayForecastToday;
  doc["displayForecastTomorrow"]=weaths.displayForecastTomorrow;
  doc["displayForecastWind"]=weaths.displayForecastWind;
  doc["animNotWeather"]=weaths.animNotWeather;
  doc["timeStartViewWeather"]=weaths.timeStartViewWeather;
  doc["timeEndViewWeather"]=weaths.timeEndViewWeather;
  jsonWeaths="";
  if(serializeJson(doc,jsonWeaths)==0 && senss.printCom) Serial.println(F("Failed to write to jsonWeaths"));
  File weathsFile=SPIFFS.open("/weaths.json","w");
  if(!weathsFile){
    weathsFile.close();
    return;
  }
  if(serializeJson(doc,weathsFile)==0 && senss.printCom) Serial.println(F("Failed to write to file"));
  weathsFile.close();
  //bip(1);
    if(senss.printCom) {
    printTime();
    Serial.println("SAVE /weaths.json="+jsonWeaths);
  }
}
//=====================================================================
void loadLeds(){
  File ledsFile=SPIFFS.open("/leds.json","r");
  if(!ledsFile){
    saveLeds();
    ledsFile.close();
    return;
  }
  size_t size=ledsFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! /leds.json - size>2048");
  jsonLeds=ledsFile.readString();
  ledsFile.close();  
  DynamicJsonDocument doc(size*2);
  deserializeJson(doc,jsonLeds);
  leds.NUM_VER_X=doc["NUM_VER_X"];
  leds.NUM_MAX0=doc["NUM_MAX0"];
  leds.NUM_MAX1=doc["NUM_MAX1"];
  leds.rotate0=doc["rotate0"];
  leds.rotate1=doc["rotate1"];
  leds.mirror0=doc["mirror0"];
  leds.mirror1=doc["mirror1"];
  leds.timeDay=doc["timeDay"];
  leds.timeNight=doc["timeNight"];
  leds.volBrightsMax=doc["volBrightsMax"];
  leds.volBrightsMin=doc["volBrightsMin"];
  leds.typeSwitchBrightness=doc["typeSwitchBrightness"];
  leds.fontCLOCK=doc["fontCLOCK"];
  leds.fontSizeData=doc["fontSizeData"];
  leds.textDown=doc["textDown"];
  leds.dataDown=doc["dataDown"];
  leds.displayData=doc["displayData"];
  leds.clockAndSec=doc["clockAndSec"];
  leds.maxModeSec=doc["maxModeSec"];
  clockAndSecMem=leds.clockAndSec;
  leds.animPoint=doc["animPoint"];
  leds.secLine=doc["secLine"];
  leds.timeScrollSpeed=doc["timeScrollSpeed"];
  leds.function[0]=doc["function00"];
  leds.function[1]=doc["function01"];
  leds.function[2]=doc["function02"];
  leds.function[3]=doc["function03"];
  leds.function[4]=doc["function04"];
  leds.function[5]=doc["function05"];
  leds.function[6]=doc["function06"];
  leds.function[7]=doc["function07"];
  leds.function[8]=doc["function08"];
  leds.function[9]=doc["function09"];
  leds.function[10]=doc["function10"];
  leds.function[11]=doc["function11"];
  leds.period[0]=doc["period00"];
  leds.period[1]=doc["period01"];
  leds.period[2]=doc["period02"];
  leds.period[3]=doc["period03"];
  leds.period[4]=doc["period04"];
  leds.period[5]=doc["period05"];
  leds.period[6]=doc["period06"];
  leds.period[7]=doc["period07"];
  leds.period[8]=doc["period08"];
  leds.period[9]=doc["period09"];
  leds.period[10]=doc["period10"];
  leds.period[11]=doc["period11"];
  leds.view[0]=doc["view00"];
  leds.view[1]=doc["view01"];
  leds.view[2]=doc["view02"];
  leds.view[3]=doc["view03"];
  leds.view[4]=doc["view04"];
  leds.view[5]=doc["view05"];
  leds.view[6]=doc["view06"];
  leds.view[7]=doc["view07"];
  leds.view[8]=doc["view08"];
  leds.view[9]=doc["view09"];
  leds.view[10]=doc["view10"];
  leds.view[11]=doc["view11"];
  leds.dataAlig=doc["dataAlig"];
  leds.clockAlig=doc["clockAlig"];
    if(senss.printCom){
    printTime();
    Serial.println("LOAD /leds.json("+String(size)+"): "+jsonLeds);
  }
}
//=================================================================
void saveLeds(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,"");
  doc["NUM_VER_X"]=leds.NUM_VER_X;
  doc["NUM_MAX0"]=leds.NUM_MAX0;
  doc["NUM_MAX1"]=leds.NUM_MAX1;
  doc["rotate0"]=leds.rotate0;
  doc["rotate1"]=leds.rotate1;
  doc["mirror0"]=leds.mirror0;
  doc["mirror1"]=leds.mirror1;
  doc["timeDay"]=leds.timeDay;
  doc["timeNight"]=leds.timeNight;
  doc["volBrightsMax"]=leds.volBrightsMax;
  doc["volBrightsMin"]=leds.volBrightsMin;
  doc["typeSwitchBrightness"]=leds.typeSwitchBrightness;
  doc["fontCLOCK"]=leds.fontCLOCK;
  doc["fontSizeData"]=leds.fontSizeData;
  doc["textDown"]=leds.textDown;
  doc["dataDown"]=leds.dataDown;
  doc["displayData"]=leds.displayData;
  doc["clockAndSec"]=leds.clockAndSec;
  doc["maxModeSec"]=leds.maxModeSec;
  doc["animPoint"]=leds.animPoint;
  doc["secLine"]=leds.secLine;
  doc["timeScrollSpeed"]=leds.timeScrollSpeed;
  doc["function00"]=leds.function[0];
  doc["function01"]=leds.function[1];
  doc["function02"]=leds.function[2];
  doc["function03"]=leds.function[3];
  doc["function04"]=leds.function[4];
  doc["function05"]=leds.function[5];
  doc["function06"]=leds.function[6];
  doc["function07"]=leds.function[7];
  doc["function08"]=leds.function[8];
  doc["function09"]=leds.function[9];
  doc["function10"]=leds.function[10];
  doc["function11"]=leds.function[11];
  doc["period00"]=leds.period[0];
  doc["period01"]=leds.period[1];
  doc["period02"]=leds.period[2];
  doc["period03"]=leds.period[3];
  doc["period04"]=leds.period[4];
  doc["period05"]=leds.period[5];
  doc["period06"]=leds.period[6];
  doc["period07"]=leds.period[7];
  doc["period08"]=leds.period[8];
  doc["period09"]=leds.period[9];
  doc["period10"]=leds.period[10];
  doc["period11"]=leds.period[11];
  doc["view00"]=leds.view[0];
  doc["view01"]=leds.view[1];
  doc["view02"]=leds.view[2];
  doc["view03"]=leds.view[3];
  doc["view04"]=leds.view[4];
  doc["view05"]=leds.view[5];
  doc["view06"]=leds.view[6];
  doc["view07"]=leds.view[7];
  doc["view08"]=leds.view[8];
  doc["view09"]=leds.view[9];
  doc["view10"]=leds.view[10];
  doc["view11"]=leds.view[11];
  doc["dataAlig"]=leds.dataAlig;
  doc["clockAlig"]=leds.clockAlig;
  jsonLeds="";
  if(serializeJson(doc,jsonLeds)==0){
    if(senss.printCom) Serial.println(F("Failed to write to jsonLeds"));
  }
  File ledsFile=SPIFFS.open("/leds.json","w");
  if(!ledsFile){
    ledsFile.close();
    return;
  }
  ledsFile.print(jsonLeds);
  ledsFile.close();
  //bip(1);
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /leds.json="+jsonLeds);
  }
}
//=====================================================================
void loadSenss(){
  File senssFile=SPIFFS.open("/senss.json","r");
  if(!senssFile){
    saveSenss();
    senssFile.close();
    return;
  }
  size_t size=senssFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! /senss.json - size>2048");
  jsonSenss=senssFile.readString();
  DynamicJsonDocument doc(size*2);
  deserializeJson(doc,jsonSenss);
  senssFile.close();
  senss.mqttOn=doc["mqttOn"];
  senss.thingOn=doc["thingOn"];
  senss.nmonOn=doc["nmonOn"];
  senss.sgpOn=doc["sgpOn"];
  senss.buzzerSet=doc["buzzerSet"];
  senss.kuOn=doc["kuOn"];
  senss.kuOff=doc["kuOff"];
  senss.butStat=doc["butStat"];
  senss.messLang=doc["messLang"].as<String>();
  senss.params[0]=doc["params0"];
  senss.params[1]=doc["params1"];
  senss.params[2]=doc["params2"];
  senss.params[3]=doc["params3"];
  senss.params[4]=doc["params4"];
  senss.params[5]=doc["params5"];
  senss.params[6]=doc["params6"];
  senss.params[7]=doc["params7"];
  senss.sensors[0]=doc["sensors0"];
  senss.sensors[1]=doc["sensors1"];
  senss.sensors[2]=doc["sensors2"];
  senss.sensors[3]=doc["sensors3"];
  senss.sensors[4]=doc["sensors4"];
  senss.sensors[5]=doc["sensors5"];
  senss.sensors[6]=doc["sensors6"];
  senss.sensors[7]=doc["sensors7"];
  senss.corr[0]=doc["corr0"];
  senss.corr[1]=doc["corr1"];
  senss.corr[2]=doc["corr2"];
  senss.corr[3]=doc["corr3"];
  senss.corr[4]=doc["corr4"];
  senss.corr[5]=doc["corr5"];
  senss.corr[6]=doc["corr6"];
  senss.corr[7]=doc["corr7"];
  senss.pressSys=doc["pressSys"];
  senss.updateOTA=doc["updateOTA"];
  senss.printCom=doc["printCom"];
  senss.beepSave=doc["beepSave"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /senss.json("+String(size)+"): "+jsonSenss);
  }
}
//=================================================================
void saveSenss(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonSenss);
  doc["mqttOn"]=senss.mqttOn;
  doc["thingOn"]=senss.thingOn;
  doc["nmonOn"]=senss.nmonOn;
  doc["sgpOn"]=senss.sgpOn;
  doc["buzzerSet"]=senss.buzzerSet;
  doc["kuOn"]=senss.kuOn;
  doc["kuOff"]=senss.kuOff;
  doc["butStat"]=senss.butStat;
  doc["messLang"]=senss.messLang;
  doc["params0"]=senss.params[0]; 
  doc["params1"]=senss.params[1];
  doc["params2"]=senss.params[2];
  doc["params3"]=senss.params[3];
  doc["params4"]=senss.params[4];
  doc["params5"]=senss.params[5];
  doc["params6"]=senss.params[6];
  doc["params7"]=senss.params[7];
  doc["sensors0"]=senss.sensors[0];
  doc["sensors1"]=senss.sensors[1];
  doc["sensors2"]=senss.sensors[2];
  doc["sensors3"]=senss.sensors[3];
  doc["sensors4"]=senss.sensors[4];
  doc["sensors5"]=senss.sensors[5];
  doc["sensors6"]=senss.sensors[6];
  doc["sensors7"]=senss.sensors[7];
  doc["corr0"]=senss.corr[0];
  doc["corr1"]=senss.corr[1];
  doc["corr2"]=senss.corr[2];
  doc["corr3"]=senss.corr[3];
  doc["corr4"]=senss.corr[4];
  doc["corr5"]=senss.corr[5];
  doc["corr6"]=senss.corr[6];
  doc["corr7"]=senss.corr[7];
  doc["pressSys"]=senss.pressSys;
  doc["updateOTA"]=senss.updateOTA;
  doc["printCom"]=senss.printCom;
  doc["beepSave"]=senss.beepSave;
  jsonSenss="";
  if(serializeJson(doc,jsonSenss)==0 && senss.printCom) Serial.println(F("Failed to write to jsonSenss"));
  File senssFile=SPIFFS.open("/senss.json","w");
  if(!senssFile){
    senssFile.close();
    return;
  }
  if(serializeJson(doc,senssFile)==0 && senss.printCom) Serial.println(F("Failed to write to senssFile"));
  senssFile.close();
  //bip(1);
  if(senss.printCom) {
    printTime();
    Serial.println("SAVE /senss.json="+jsonSenss);
  }
}
//=====================================================================
void loadMqtts(){
  File mqttsFile=SPIFFS.open("/mqtts.json","r");
  if(!mqttsFile){
    saveMqtts();
    mqttsFile.close();
    return;
  }
  size_t size=mqttsFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! /mqtts.json - size>2048");
  jsonMqtts=mqttsFile.readString();
  DynamicJsonDocument doc(size*2);
  deserializeJson(doc,jsonMqtts);
  mqttsFile.close();
  snprintf(mqtts.mqtt_server,24,"%s",(doc["mqtt_server"].as<String>()).c_str());
  mqtts.mqtt_port=doc["mqtt_port"];
  snprintf(mqtts.mqtt_user,24,"%s",(doc["mqtt_user"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pass,24,"%s",(doc["mqtt_pass"].as<String>()).c_str());
  snprintf(mqtts.mqtt_name,24,"%s",(doc["mqtt_name"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[0],24,"%s",(doc["mqtt_sub0"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[1],24,"%s",(doc["mqtt_sub1"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[2],24,"%s",(doc["mqtt_sub2"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[3],24,"%s",(doc["mqtt_sub3"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[4],24,"%s",(doc["mqtt_sub4"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[5],24,"%s",(doc["mqtt_sub5"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[6],24,"%s",(doc["mqtt_sub6"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub[7],24,"%s",(doc["mqtt_sub7"].as<String>()).c_str());
  snprintf(mqtts.mqtt_sub_inform,24,"%s",(doc["mqtt_sub_inform"].as<String>()).c_str()); 
  snprintf(mqtts.mqtt_pub_sensors[0],24,"%s",(doc["mqtt_pub_sensors0"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pub_sensors[1],24,"%s",(doc["mqtt_pub_sensors1"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pub_sensors[2],24,"%s",(doc["mqtt_pub_sensors2"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pub_sensors[3],24,"%s",(doc["mqtt_pub_sensors3"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pub_sensors[4],24,"%s",(doc["mqtt_pub_sensors4"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pub_sensors[5],24,"%s",(doc["mqtt_pub_sensors5"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pub_sensors[6],24,"%s",(doc["mqtt_pub_sensors6"].as<String>()).c_str());
  snprintf(mqtts.mqtt_pub_sensors[7],24,"%s",(doc["mqtt_pub_sensors7"].as<String>()).c_str());
  mqtts.timeOutMqtt=doc["timeOutMqtt"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /mqtts.json("+String(size)+"): "+jsonMqtts);
  }
}
//=================================================================
void saveMqtts(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonMqtts);
  doc["mqtt_server"]=mqtts.mqtt_server;
  doc["mqtt_port"]=mqtts.mqtt_port;
  doc["mqtt_user"]=mqtts.mqtt_user;
  doc["mqtt_pass"]=mqtts.mqtt_pass;
  doc["mqtt_name"]=mqtts.mqtt_name;
  doc["mqtt_sub0"]=mqtts.mqtt_sub[0];
  doc["mqtt_sub1"]=mqtts.mqtt_sub[1];
  doc["mqtt_sub2"]=mqtts.mqtt_sub[2];
  doc["mqtt_sub3"]=mqtts.mqtt_sub[3];
  doc["mqtt_sub4"]=mqtts.mqtt_sub[4];
  doc["mqtt_sub5"]=mqtts.mqtt_sub[5];
  doc["mqtt_sub6"]=mqtts.mqtt_sub[6];
  doc["mqtt_sub7"]=mqtts.mqtt_sub[7];
  doc["mqtt_sub_inform"]=mqtts.mqtt_sub_inform;
  doc["mqtt_pub_sensors0"]=mqtts.mqtt_pub_sensors[0];
  doc["mqtt_pub_sensors1"]=mqtts.mqtt_pub_sensors[1];
  doc["mqtt_pub_sensors2"]=mqtts.mqtt_pub_sensors[2];
  doc["mqtt_pub_sensors3"]=mqtts.mqtt_pub_sensors[3];
  doc["mqtt_pub_sensors4"]=mqtts.mqtt_pub_sensors[4];
  doc["mqtt_pub_sensors5"]=mqtts.mqtt_pub_sensors[5];
  doc["mqtt_pub_sensors6"]=mqtts.mqtt_pub_sensors[6];
  doc["mqtt_pub_sensors7"]=mqtts.mqtt_pub_sensors[7];
  doc["timeOutMqtt"]=mqtts.timeOutMqtt;
  jsonMqtts="";
  if(serializeJson(doc,jsonMqtts)==0 && senss.printCom) Serial.println(F("Failed to write to jsonMqtts"));
  File mqttsFile=SPIFFS.open("/mqtts.json","w");
  if(!mqttsFile){
    mqttsFile.close();
    return;
  }
  if(serializeJson(doc,mqttsFile)==0 && senss.printCom) Serial.println(F("Failed to write to file"));
  mqttsFile.close();
  //bip(1);
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /mqtts.json="+jsonMqtts);
  }
}
//=====================================================================
void loadThings(){
  File thingsFile=SPIFFS.open("/things.json","r");
  if(!thingsFile){
    saveThings();
    thingsFile.close();
    return;
  }
  size_t size=thingsFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! /things.json - size>2048");
  jsonThings=thingsFile.readString();
  DynamicJsonDocument doc(size*2);
  deserializeJson(doc,jsonThings);
  thingsFile.close();
  things.channelIdWrite=doc["channelIdWrite"].as<String>();
  things.writeApiKey=doc["writeApiKey"].as<String>();
  things.wrApiKey=doc["wrApiKey"].as<String>();
  things.writeFild[0]=doc["writeFild0"];
  things.writeFild[1]=doc["writeFild1"];
  things.writeFild[2]=doc["writeFild2"];
  things.writeFild[3]=doc["writeFild3"];
  things.writeFild[4]=doc["writeFild4"];
  things.writeFild[5]=doc["writeFild5"];
  things.writeFild[6]=doc["writeFild6"];
  things.writeFild[7]=doc["writeFild7"];
  things.channelIdRead=doc["channelIdRead"].as<String>();
  things.readApiKey=doc["readApiKey"].as<String>();
  things.statThing[0]=doc["statThing0"];
  things.statThing[1]=doc["statThing1"];
  things.statThing[2]=doc["statThing2"];
  things.statThing[3]=doc["statThing3"];
  things.statThing[4]=doc["statThing4"];
  things.statThing[5]=doc["statThing5"];
  things.statThing[6]=doc["statThing6"];
  things.statThing[7]=doc["statThing7"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /things.json("+String(size)+"): "+jsonThings);
  }
}
//=================================================================
void saveThings(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonThings);
  doc["channelIdWrite"]=things.channelIdWrite;
  doc["writeApiKey"]=things.writeApiKey;
  doc["wrApiKey"]=things.wrApiKey;
  doc["writeFild0"]=things.writeFild[0];
  doc["writeFild1"]=things.writeFild[1];
  doc["writeFild2"]=things.writeFild[2];
  doc["writeFild3"]=things.writeFild[3];
  doc["writeFild4"]=things.writeFild[4];
  doc["writeFild5"]=things.writeFild[5];
  doc["writeFild6"]=things.writeFild[6];
  doc["writeFild7"]=things.writeFild[7];
  doc["channelIdRead"]=things.channelIdRead;
  doc["readApiKey"]=things.readApiKey;
  doc["statThing0"]=things.statThing[0];
  doc["statThing1"]=things.statThing[1];
  doc["statThing2"]=things.statThing[2];
  doc["statThing3"]=things.statThing[3];
  doc["statThing4"]=things.statThing[4];
  doc["statThing5"]=things.statThing[5];
  doc["statThing6"]=things.statThing[6];
  doc["statThing7"]=things.statThing[7];
  jsonThings="";
  if(serializeJson(doc,jsonThings)==0 && senss.printCom) Serial.println(F("Failed to write to jsonThings"));
  File thingsFile=SPIFFS.open("/things.json","w");
  if(!thingsFile){
    thingsFile.close();
    return;
  }
  if(serializeJson(doc,thingsFile)==0 && senss.printCom) Serial.println(F("Failed to write to file"));
  thingsFile.close();
  //bip(1);
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /things.json="+jsonThings);
  }
}
//=====================================================================
void loadNmons(){
  File nmonsFile=SPIFFS.open("/nmons.json","r");
  if(!nmonsFile){
    saveNmons();
    nmonsFile.close();
    return;
  }
  size_t size=nmonsFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! /nmons.json - size>2048");
  jsonNmons=nmonsFile.readString();
  DynamicJsonDocument doc(size*2);
  deserializeJson(doc,jsonNmons);
  nmonsFile.close();
  nmons.uuid =doc["uuid"].as<String>();
  nmons.api_key=doc["api_key"].as<String>();
  nmons.sensors_ID[0]=doc["sensors_ID0"];
  nmons.sensors_ID[1]=doc["sensors_ID1"];
  nmons.sensors_ID[2]=doc["sensors_ID2"];
  nmons.sensors_ID[3]=doc["sensors_ID3"];
  nmons.sensors_ID[4]=doc["sensors_ID4"];
  nmons.sensors_ID[5]=doc["sensors_ID5"];
  nmons.sensors_ID[6]=doc["sensors_ID6"];
  nmons.sensors_ID[7]=doc["sensors_ID7"];
  nmons.latNmon=doc["latNmon"].as<String>();
  nmons.lonNmon=doc["lonNmon"].as<String>();
  nmons.periodNmon=doc["periodNmon"];
  nmons.writeNmon[0]=doc["writeNmon0"];
  nmons.writeNmon[1]=doc["writeNmon1"];
  nmons.writeNmon[2]=doc["writeNmon2"];
  nmons.writeNmon[3]=doc["writeNmon3"];
  nmons.writeNmon[4]=doc["writeNmon4"];
  nmons.writeNmon[5]=doc["writeNmon5"];
  nmons.writeNmon[6]=doc["writeNmon6"];
  nmons.writeNmon[7]=doc["writeNmon7"];
  nmons.nameNmon=doc["nameNmon"].as<String>();
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /nmons.json("+String(size)+"): "+jsonNmons);
  }
}
//=================================================================
void saveNmons(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonNmons);
  doc["uuid"]=nmons.uuid;
  doc["api_key"]=nmons.api_key;
  doc["sensors_ID0"]=nmons.sensors_ID[0];
  doc["sensors_ID1"]=nmons.sensors_ID[1];
  doc["sensors_ID2"]=nmons.sensors_ID[2];
  doc["sensors_ID3"]=nmons.sensors_ID[3];
  doc["sensors_ID4"]=nmons.sensors_ID[4];
  doc["sensors_ID5"]=nmons.sensors_ID[5];
  doc["sensors_ID6"]=nmons.sensors_ID[6];
  doc["sensors_ID7"]=nmons.sensors_ID[7];
  doc["latNmon"]=nmons.latNmon;
  doc["lonNmon"]=nmons.lonNmon;
  doc["periodNmon"]=nmons.periodNmon;
  doc["writeNmon0"]=nmons.writeNmon[0];
  doc["writeNmon1"]=nmons.writeNmon[1];
  doc["writeNmon2"]=nmons.writeNmon[2];
  doc["writeNmon3"]=nmons.writeNmon[3];
  doc["writeNmon4"]=nmons.writeNmon[4];
  doc["writeNmon5"]=nmons.writeNmon[5];
  doc["writeNmon6"]=nmons.writeNmon[6];
  doc["writeNmon7"]=nmons.writeNmon[7];
  doc["nameNmon"]=nmons.nameNmon;
  jsonNmons="";
  if(serializeJson(doc,jsonNmons)==0 && senss.printCom) Serial.println(F("Failed to write to jsonNmons"));
  File nmonsFile=SPIFFS.open("/nmons.json","w");
  if(!nmonsFile){
    nmonsFile.close();
    return;
  }
  if(serializeJson(doc,nmonsFile)==0 && senss.printCom) Serial.println(F("Failed to write to file"));
  nmonsFile.close();
  //bip(1);
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /nmons.json="+jsonNmons);
  }
}
//=====================================================================
void loadSgps(){
  File sgpsFile=SPIFFS.open("/sgps.json","r");
  if(!sgpsFile){
    saveSgps();
    sgpsFile.close();
    return;
  }
  size_t size=sgpsFile.size();
  if(size>2048 && senss.printCom) Serial.println("AHTUNG!!! /sgps.json - size>2048");
  jsonSgps=sgpsFile.readString();
  DynamicJsonDocument doc(size*2);
  deserializeJson(doc,jsonSgps);
  sgpsFile.close();
  sgps.sgpCo2LivelAlarm=doc["sgpCo2LivelAlarm"];
  sgps.eCo2AlarmEsp=doc["eCo2AlarmEsp"];
  sgps.eCo2AlarmMqtt=doc["eCo2AlarmMqtt"];
  sgps.eCo2Led=doc["eCo2Led"];
  sgps.sgpTvocLivelAlarm=doc["sgpTvocLivelAlarm"];
  sgps.tvocAlarmEsp=doc["tvocAlarmEsp"];
  sgps.tvocAlarmMqtt=doc["tvocAlarmMqtt"];
  sgps.tvocLed=doc["tvocLed"];
  sgps.setSgpCorr=doc["setSgpCorr"];
  sgps.sgpCorrTemp=doc["sgpCorrTemp"];
  sgps.sgpCorrHumi=doc["sgpCorrHumi"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /sgps.json("+String(size)+"): "+jsonSgps);
  }
}
//=================================================================
void saveSgps(){
  DynamicJsonDocument doc(2048);
  deserializeJson(doc,jsonSgps);
  doc["sgpCo2LivelAlarm"]=sgps.sgpCo2LivelAlarm;
  doc["eCo2AlarmEsp"]=sgps.eCo2AlarmEsp;
  doc["eCo2AlarmMqtt"]=sgps.eCo2AlarmMqtt;
  doc["eCo2Led"]=sgps.eCo2Led;
  doc["sgpTvocLivelAlarm"]=sgps.sgpTvocLivelAlarm;
  doc["tvocAlarmEsp"]=sgps.tvocAlarmEsp;
  doc["tvocAlarmMqtt"]=sgps.tvocAlarmMqtt;
  doc["tvocLed"]=sgps.tvocLed;
  doc["setSgpCorr"]=sgps.setSgpCorr;
  doc["sgpCorrTemp"]=sgps.sgpCorrTemp;
  doc["sgpCorrHumi"]=sgps.sgpCorrHumi;
  jsonSgps="";
  if(serializeJson(doc,jsonSgps)==0&&senss.printCom)Serial.println(F("Failed to write to jsonSgps"));
  File sgpsFile=SPIFFS.open("/sgps.json","w");
  if(!sgpsFile){
    sgpsFile.close();
    return;
  }
  if(serializeJson(doc,sgpsFile)==0&&senss.printCom)Serial.println(F("Failed to write to file"));
  sgpsFile.close();
  //bip(1);
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /sgps.json="+jsonSgps);
  }
}
//======================================================ir
void loadIr(){
  File irFile=SPIFFS.open("/irs.json","r");
  if(!irFile){
    saveIr();
    irFile.close();
    return;
  }
  size_t size=irFile.size();
  if(size>512 && senss.printCom) Serial.println("AHTUNG!!! /irs.json - size>512");
  jsonIrs=irFile.readString();
  DynamicJsonDocument doc(512);
  deserializeJson(doc,jsonSenss);
  irFile.close();
  ir_key[0]=doc["ir_key0"];
  ir_key[1]=doc["ir_key1"];
  ir_key[2]=doc["ir_key2"];
  ir_key[3]=doc["ir_key3"];
  ir_key[4]=doc["ir_key4"];
  ir_key[5]=doc["ir_key5"];
  if(senss.printCom){
    printTime();
    Serial.println("LOAD /irs.json("+String(size)+"): "+jsonIrs);
  }
}
//============================================================
void saveIr(){
  DynamicJsonDocument doc(512);
  deserializeJson(doc,jsonIrs);
  doc["ir_key0"]=ir_key[0];
  doc["ir_key1"]=ir_key[1];
  doc["ir_key2"]=ir_key[2];
  doc["ir_key3"]=ir_key[3];
  doc["ir_key4"]=ir_key[4];
  doc["ir_key5"]=ir_key[5];
  jsonIrs="";
  if(serializeJson(doc,jsonIrs)==0&&senss.printCom)Serial.println(F("Failed to write to jsonIrs"));
  File irFile=SPIFFS.open("/irs.json","w");
  if(!irFile){
    irFile.close();
    return;
  }
  if(serializeJson(doc,irFile)==0&&senss.printCom)Serial.println(F("Failed to write to riFile"));
  irFile.close();
  //bip(1);
  if(senss.printCom){
    printTime();
    Serial.println("SAVE /irs.json="+jsonIrs);
  }
}
//======================================================
bool loadStime(){
  File stimeFile=SPIFFS.open("/stime.json", "r");
  if(!stimeFile){
    if(senss.printCom) Serial.println("Failed to open time file");
    saveStime();
    stimeFile.close();
    return false;
  }
  size_t size=stimeFile.size();
  if(senss.printCom) Serial.println("size time.file="+(String) size);
  if(size > 512){
    if(senss.printCom) Serial.println("Time file size is too large");
    stimeFile.close();
    return false;
  }
  jsonStime=stimeFile.readString();
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonStime);
  stimeFile.close();
  hour=doc["hour"];
  minute=doc["minute"];
  second=doc["second"]; 
  year=doc["year"];
  month=doc["month"];
  day=doc["day"]; 
  if(senss.printCom){
    printTime();
    Serial.print("Load Stime(config): ");
    Serial.println(jsonStime);
  }
  return true;
}

//=================================================================
bool saveStime(){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc,jsonStime);
  doc["hour"]=hour;
  doc["minute"]=minute;
  doc["second"]=second;
  doc["year"]=year;
  doc["month"]=month;
  doc["day"]=day; 
  jsonStime="";
  if(serializeJson(doc,jsonStime)==0){
    if(senss.printCom) Serial.println(F("Failed to write to jsonStime"));
  }  
  File stimeFile=SPIFFS.open("/stime.json", "w");
  if(!stimeFile){
    stimeFile.close();
    return false;
  }
  if(serializeJson(doc,stimeFile)==0){
    if(senss.printCom)Serial.println(F("Failed to write to stimeFile "));
  } 
  if(senss.printCom){
    printTime();
    Serial.print("Save /stime.json: ");
    Serial.println(jsonStime);
  }
  stimeFile.close();
  return true;
}
// -----------------------------------------------------------------------------
void eraseConfigFiles(){
  for(uint8_t i=1;i<12;i++){
    SPIFFS.remove(pathSw[i]);
    if(senss.printCom){printTime();Serial.println("Erase "+pathSw[i]);}
  }
  SPIFFS.remove("/jssids.json");
  delay(3000);
  bip();bip();bip();bip();
  ESP.reset();
}

void server_init(void){
  //=========== index
  server.on("/",HTTP_GET,[](){
    if(indexs.authOn)handleFileRead(F("/index.html"));
    else handleFileRead(F("/wifi.html"));
    server.send(200,"text/plain","OK");
  });
  server.on("/index.html",HTTP_GET,[](){
    if(indexs.authOn)handleFileRead(F("/index.html"));
    else handleFileRead(F("/wifi.html"));
    server.send(200,"text/plain","OK");
  });
  server.on("/auth.php",HTTP_GET,[](){
    Serial.println(server.arg("auth"));
    if((indexs.authOn&&server.arg("auth")==indexs.auth)||!indexs.authOn){
      if(handleFileRead(F("/wifi.html"))){return server.send(200,"text/plain","OK");}
    }
    server.send(400,"text/plain","Error tocken");
  });
  server.on("/esp/ssid.php",HTTP_POST,[](){
    String json="{";
    uint8_t n=WiFi.scanNetworks();
    for(uint8_t i=0;i<n;i++){
      json+="\""; 
      json+=WiFi.SSID(i);
      json+="\":\"";
      json+=abs(WiFi.RSSI(i));
      if(i==n-1) json+="\"}";
      else json+="\",";
    }
    server.send(200,"text/json",json);
  });
  server.on("/esp/connect_wifi.php",HTTP_POST,[](){
    String json="{";
    json+="\"ssid\":\"";      json+=WiFi.SSID();                  json+="\",";
    json+="\"ch\":\"";        json+=WiFi.channel();               json+="\",";
    json+="\"sig\":\"";       json+=WiFi.RSSI();                  json+="dB\",";
    json+="\"mac\":\"";       json+=WiFi.macAddress();            json+="\",";
    json+="\"ip\":\"";        json+=WiFi.localIP().toString();    json+="\",";
    json+="\"gw\":\"";        json+=WiFi.gatewayIP().toString();  json+="\",";
    json+="\"mask\":\"";      json+=WiFi.subnetMask().toString(); json+="\"}";
    server.send(200,"text/plain",json);
  });
  server.on("/wifis.jsone",[](){server.send(200,"text/json",jsonWifi);});
  server.on("/jssids.jsone",[](){server.send(200,"text/json",jsonJssid);});
  server.on("/esp/save.php",HTTP_POST,[](){
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("/wifis.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/wifis.json","w");
        if(file){
          file.print(server.arg("JS"));
          file.close();
          loadNet();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Получены не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
    if(server.hasArg("JSSIDS")){
      File file=SPIFFS.open("/jssids.json","w");
      if(file){
        file.print(server.arg("JSSIDS").c_str());
        file.close();
      }
      loadJssids();
    }
    if(server.hasArg("SSIDS")){
      File file=SPIFFS.open("/ssids.json","w");
      if(file){
        file.print(server.arg("SSIDS").c_str());
        file.close();
      }
      loadSsids();
    }  
  });
  //=========== time
  server.on("/times.jsone",[](){server.send(200,"text/json",jsonTimes);}); 
  server.on("/esp/times.php",HTTP_POST,[](){
    if(indexs.authOn && verific()) return;
    if(server.hasArg("JS")){
      delay(500);
      String line=server.arg("JS").c_str();
      Serial.println("vz/times.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/times.json","w");
        if(file){
          file.print(line);
          file.close();
          jsonTimes=line;
          loadTimes();
          timeUpdateNTP();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Полученны не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  server.on("/stop_alarm.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.arg("SA")=="1"){
      if(alarm_work){
        stopAlarm=true;
        leds.clockAndSec=clockAndSecMem;
        if(senss.printCom){
          printTime();
          Serial.println("STOP ALARM For WEB");
        }
        //bip();
        server.send(200,"text/plain","OK");
      } else server.send(200,"text/plain","NOT ALARM");
    }
  });
  server.on("/set_time.php",HTTP_POST,[](){
    if(verific()) return;
    bip();
    if(server.hasArg("JS")){
      server.send(200,"text/plain","OK");
      String set_time=server.arg("JS").c_str();
      DynamicJsonDocument doc(512);
      deserializeJson(doc, set_time);
      hour=doc["t0"];
      minute=doc["t1"];
      day=doc["d0"];
      month=doc["d1"];
      year=doc["d2"];
      if(senss.printCom){
        printTime();
        Serial.println("Set manual time: "+String(hour)+":"+String(minute)+"      "+String(day)+"-"+String(month)+"-"+String(year));
      }
      server.send(200, "text/plain", "OK");
      localEpoc=(hour*60*60+minute*60);
      if(!times.rtcStat) saveStime();
      showSimpleDate();
      hour_rtc=hour;
      minute_rtc=minute;
      second_rtc=0;
      month_rtc=month;
      day_rtc=day;
      year_rtc=year;
      printTime();
      setRTCDateTime();
    } else server.send(400,"text/plain","ERROR");
  });
  server.on("/weaths.jsone",[](){server.send(200,"text/json",jsonWeaths);}); 
  server.on("/esp/weaths.php",HTTP_POST,[](){
    if(indexs.authOn && verific()) return;
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("vz/weath.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/weaths.json","w");
        if(file){
          file.print(line);
          file.close();
          loadWeaths();
          weather_update();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Получены не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  server.on("/esp/weaths_var.json",HTTP_POST,[](){
    if(verific()) return;
    int sr=location_sunrise.substring(0,2).toInt()+(int)hourCorr;
    if(sr>23) sr-=24;
    if(sr<0) sr+=24;
    String sunrise=String(sr)+location_sunrise.substring(2,5);
    int ss=location_sunset.substring(0,2).toInt()+(int)hourCorr;
    if(ss>23) ss-=24;
    if(ss<0) ss+=24;
    String sunset=String(ss)+location_sunset.substring(2,5);
    int st=location_localtime.substring(11,13).toInt()+(int)hourCorr;
    int ly=location_localtime.substring(0,4).toInt();
    byte lm=location_localtime.substring(5,7).toInt();
    byte ld=location_localtime.substring(8,10).toInt(); 
    if(st>23){
      st-=24;
      ld++;
      if(ld==32||(ld==31&&(lm==4||lm==6||lm==9||lm==11))||(lm==2&&((ld==29&&ly%4!=0)||(ld==30&&ly%4==0)))){
        ld=1;
        lm++;
        if(lm>12){
          lm=1;
          ly++;
        }
      }
    }
    if(st<0){
      st+=24;
      ld--;
      if(ld<1){
        ld=0+((lm==5||lm==7||lm==10||lm==12||(lm==3&&ly%4==0))?30:(lm==3&&ly%4!=0)?29:31);
        lm--;
        if(lm<1){
          lm=12;
          ly--;
        }
      }
    }
    String lt=String(ly)+"-"+(lm<10?"0":"")+String(lm)+"-"+(ld<10?"0":"")+String(ld)+" "+(st<10?"0":"")+String(st)+location_localtime.substring(13,16);
    String json="{";
    json+="\"location_name\":\"";                   json+=location_name;
    json+="\",\"location_region\":\"";              json+=location_region;
    json+="\",\"location_country\":\"";             json+=location_country;
    json+="\",\"location_localtime\":\"";           json+=lt;
    json+="\",\"location_temp\":\"";                json+=location_temp;
    json+="\",\"location_app_temp\":\"";            json+=location_app_temp;
    json+="\",\"location_rh\":\"";                  json+=location_rh;
    json+="\",\"location_pres\":\"";                json+=location_pres;
    json+="\",\"location_wind_spd\":\"";            json+=location_wind_spd;
    json+="\",\"location_wind_cdir_full\":\"";      json+=location_wind_cdir_full;
    json+="\",\"location_sunrise\":\"";             json+=sunrise;
    json+="\",\"location_sunset\":\"";              json+=sunset;
    json+="\",\"location_clouds\":\"";              json+=location_clouds;
    json+="\",\"location_vis\":\"";                 json+=location_vis;
    json+="\",\"location_uv\":\"";                  json+=location_uv;
    json+="\",\"location_wds\":\"";                 json+=location_wds;
    json+="\"}";
    server.send(200,"text/plain",json);
  });
  server.on("/send_onWeath.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.hasArg("SW") && server.hasArg("SN")){
      weaths.weathOn=server.arg("SW").toInt();
      saveWeaths();
      if(senss.printCom){
        printTime();
        Serial.println("weaths.weathOn="+String(weaths.weathOn));
      }
      server.send(200,"text/plain","OK");
    } else server.send(400,"text/plain","ERROR");
  });
  //=========== led
  server.on("/leds.jsone",[](){server.send(200,"text/json",jsonLeds);}); 
  server.on("/esp/leds.php",HTTP_POST,[](){
    if(indexs.authOn&&verific()) return;
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("vz/leds.json="+line);
      if(line.endsWith("}")){
        File fileLeds=SPIFFS.open("/leds.json","w");
        if(fileLeds){
          fileLeds.print(line);
          fileLeds.close();
          loadLeds();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Получены не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  server.on("/esp/leds_var.json",HTTP_POST,[](){
    //sensorsAll();
    String json="{";
    json+="\"lba\":\"";
    if(leds.typeSwitchBrightness==1) json+=map(analogRead(brightPin),1023,0,15,0);
    else if(leds.typeSwitchBrightness==2) json+=volBH1750;
    else if(leds.typeSwitchBrightness==3) json+=volMAX44009;
    json+="\",\"BH\":\"";       json+=BH1750Found;
    json+="\",\"MAX\":\"";       json+=MAX44009Found;
    if(leds.NUM_MAX1>4) json+="\",\"dataAlig\":\"1";
    if(leds.NUM_MAX0>4) json+="\",\"clockAlig\":\"1";
    json+="\"}";
    server.send(200,"text/plain",json);
  });
  server.on("/mess.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.hasArg("TXT")){
      server.send(200,"text/plain","OK");
      String text=server.arg("TXT").c_str();
      if(senss.printCom){
        printTime();
        Serial.println(text);
      }
      bip();
      bip();
      bip();
      clr(leds.NUM_VER_X);
      printStringWithShift(("  "+String(text)+space).c_str(),leds.timeScrollSpeed);
    }
  });
  server.on("/rss.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.hasArg("TXT")){
      rss_mess=server.arg("TXT").c_str();
      rss_count=millis()+(6*3600000); // 6 часов будет выводиться сообщение
      if(senss.printCom){
        printTime();
        Serial.println("rss="+rss_mess);
      }
      server.send(200,"text/plain","OK");
    }
  });
  //=========== sens
  server.on("/senss.jsone",[](){server.send(200,"text/json",jsonSenss);}); 
  server.on("/esp/senss.php",HTTP_POST,[](){
    if(indexs.authOn && verific()) return;
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("vz/senss.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/senss.json","w");
        if(file){
          file.print(line);
          file.close();
          loadSenss();
          lang();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Полученны не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  server.on("/esp/sens_val.json",HTTP_POST,[](){
    sensorsAll();
    String json="{";
    for(byte i=0;i<8;i++){
      float volxx=data[i];
      if(senss.params[i]==20) verific_press(volxx);
      json+="\"vol0"+String(i)+"\":\""+volxx+"\"";
      if(i<7) json+=",";
    }
    json+="}";
    server.send(200,"text/plain",json);
  });
  server.on("/esp/sens_var.json",HTTP_POST,[](){
    //sensorsAll();
    String json="{";
    json+="\"DS\":\"";           json+=ds18b20Found;
    //json+="\",\"DHT\":\"";       json+=dhtFound;
    json+="\",\"BMP\":\"";       json+=(bmp180Found||bmp280Found); 
    json+="\",\"BME\":\"";       json+=bme280Found;
    json+="\",\"SI\":\"";        json+=si7021Found;
    json+="\",\"AHT\":\"";       json+=AHTx0Found;
    for(byte i=0;i<8;i++){
      if(senss.mqttOn&&strlen(mqtts.mqtt_sub[i])) json+="\",\"MQ" + String(i) + "\":\"1";
      if(senss.thingOn&&things.statThing[i]) json+="\",\"TH" + String(i) + "\":\"1";
      if(senss.nmonOn&&nmons.sensors_ID[i]) json+="\",\"NM" + String(i) + "\":\"1";
    }
    json+="\"}";
    server.send(200,"text/plain",json);
  });
  //=========== mqtt
  server.on("/mqtts.jsone",[](){server.send(200,"text/json",jsonMqtts);}); 
  server.on("/esp/mqtts.php",HTTP_POST,[](){
    if(indexs.authOn&&verific()) return;
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("vz/mqtts.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/mqtts.json","w");
        if(file){
          file.print(line);
          file.close();
          loadMqtts();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Полученны не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  //=========== thing
  server.on("/things.jsone",[](){server.send(200,"text/json",jsonThings);}); 
  server.on("/esp/things.php",HTTP_POST,[](){
    if(indexs.authOn && verific()) return;
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("vz/things.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/things.json","w");
        if(file){
          file.print(line);
          file.close();
          loadThings();
          thingSpeak();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Полученны не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  server.on("/esp/things_var.json",HTTP_POST,[](){
    String json="{";
    json+="\"nameWriteChannel\":\"";      json+=nameWriteChannel;
    json+="\",\"nameField0\":\"";         json+=nameField[0];
    json+="\",\"nameField1\":\"";         json+=nameField[1];
    json+="\",\"nameField2\":\"";         json+=nameField[2]; 
    json+="\",\"nameField3\":\"";         json+=nameField[3];
    json+="\",\"nameField4\":\"";         json+=nameField[4];
    json+="\",\"nameField5\":\"";         json+=nameField[5];
    json+="\",\"nameField6\":\"";         json+=nameField[6];
    json+="\",\"nameField7\":\"";         json+=nameField[7];
    json+="\",\"nameReadChannel\":\"";    json+=nameReadChannel;
    json+="\",\"nameReadField0\":\"";     json+=nameReadField[0];
    json+="\",\"nameReadField1\":\"";     json+=nameReadField[1];
    json+="\",\"nameReadField2\":\"";     json+=nameReadField[2];
    json+="\",\"nameReadField3\":\"";     json+=nameReadField[3];
    json+="\",\"nameReadField4\":\"";     json+=nameReadField[4];
    json+="\",\"nameReadField5\":\"";     json+=nameReadField[5];
    json+="\",\"nameReadField6\":\"";     json+=nameReadField[6];
    json+="\",\"nameReadField7\":\"";     json+=nameReadField[7];
    json+="\"}";
    server.send(200,"text/plain",json);
  });
  //=========== nmon
  server.on("/nmons.jsone",[](){server.send(200,"text/json",jsonNmons);}); 
  server.on("/esp/nmons.php",HTTP_POST,[](){
    if(indexs.authOn && verific()) return;
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("vz/nmons.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/nmons.json","w");
        if(file){
          file.print(line);
          file.close();
          server.send(200,"text/plain","OК :)");
          loadNmons();
          if(senss.beepSave) bip();
          narodmonGet();
          nmonTimer=millis()+nmons.periodNmon*60000;
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Полученны не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  server.on("/esp/nmons_var.json",HTTP_POST,[](){
    String json="{";
    json+="\"macNmon\":\"VZ"+macNmon+"\"";
    json+="}";
    server.send(200,"text/plain",json);
  });
  //=========== sgp
  server.on("/sgps.jsone",[](){server.send(200,"text/json",jsonSgps);}); 
  server.on("/esp/sgps.php",HTTP_POST,[](){
    if(indexs.authOn && verific()) return;
    if(server.hasArg("JS")){
      String line=server.arg("JS").c_str();
      Serial.println("sgps.json="+line);
      if(line.endsWith("}")){
        File file=SPIFFS.open("/sgps.json","w");
        if(file){
          file.print(line);
          file.close();
          loadSgps();
          if(senss.beepSave) bip();
          server.send(200,"text/plain","OК :)");
        }else server.send(200,"text/plain","Ошибка открытия файла для записи");
      }else server.send(200,"text/plain","Полученны не полные данные");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  server.on("/esp/sgps_var.json",HTTP_POST,[](){
    String json="{";
    json+="\"sgpCo2\":\"";       json+=sgpCo2;
    json+="\",\"textCo2\":\"";   json+=sgpCo2Message[sgpCo2Livel];
    json+="\",\"sgpTvoc\":\"";   json+=sgpTvoc;
    json+="\",\"textTvoc\":\"";  json+=sgpTvocMessage[sgpTvocLivel];
    //json+="\",\"DHT\":\"";       json+=dhtFound;
    json+="\",\"BME\":\"";       json+=bme280Found;
    json+="\",\"SI\":\"";        json+=si7021Found;
    json+="\",\"AHT\":\"";       json+=AHTx0Found; 
    json+="\"}";
    server.send(200,"text/plain",json);
  });
  //=========== ir
  server.on("/esp/ir_val.json",HTTP_POST,[](){server.send(200,"text/json",jsonIrs);});
  server.on("/send_stop_scan.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.arg("stop")=="1"){
      server.send(200,"text/plain","OK");
      if(senss.printCom) Serial.println("Stop scan code for key="+String(ir_save));
      ir_save=0;
      bip();
      return;
    }
    server.send(400,"text/plain","ERROR");
  });
  server.on("/send_start_scan.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.hasArg("start")){
      ir_save=server.arg("start").toInt();
      server.send(200,"text/plain","OK");
      bip();
      if(senss.printCom) Serial.println("Start scan & save code for key="+String(ir_save));
      return;
    }
    server.send(400,"text/plain","ERROR");
  });
  //=========== manage
  server.on("/status.php",HTTP_POST,[](){
    if(verific()) return;
    FSInfo fs_info;
    String json;
    json="{\"type\":\"";
    json+=fsName;
    json+="\", \"isOk\":";
    fileSystem->info(fs_info);
    json+=F("\"true\", \"totalBytes\":\"");
    json+=fs_info.totalBytes;
    json+=F("\", \"usedBytes\":\"");
    json+=fs_info.usedBytes;
    json+="\"}";
    server.send(200,"text/plain",json);
  });
  server.on("/send_restart.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.arg("restart")=="1"){
      server.send(200,"text/plain","OK");
      bip();bip();bip();
      ESP.reset();
    }
    server.send(400,"text/plain","ERROR");
  });
  server.on("/send_memory_res.php",HTTP_POST,[](){
    if(verific()) return;
    if(server.hasArg("resetConfig")){
      uint8_t num_select=server.arg("resetConfig").toInt();
      if(num_select>0&&num_select<12){
        SPIFFS.remove(pathSw[num_select]);
        if(senss.printCom){printTime();Serial.println("Erase "+pathSw[num_select]);}
        server.send(200,"text/plain","OK");
        delay(3000);
        bip();bip();bip();bip();
        ESP.reset();
      }
      if(num_select==12){
        server.send(200,"text/plain","OK");
        eraseConfigFiles();
      }
    }
    server.send(400,"text/plain","ERROR");
  });
  server.on("/loadConfig.php",HTTP_POST,replyOK,handleLoadFile);
  //=========== edit
  /////////// Загрузка списка файлов в памяти
  server.on("/list",HTTP_GET,[](){
    if(!server.hasArg("dir")){return server.send(400, "text/plain","BAD PATH\r\n");}
    String path = server.arg("dir");
    if(path!="/"&&!fileSystem->exists(path)){return server.send(400,"text/plain","BAD PATH\r\n");}
    Serial.println(String("handleFileList: ")+path);
    Dir dir=fileSystem->openDir(path);
    path.clear();
    // use HTTP/1.1 Chunked response to avoid building a huge temporary string
    if(!server.chunkedResponseModeStart(200,"text/json")){return server.send(505, F("text/html"), F("HTTP1.1 required"));}
    // use the same string for every line
    String output;
    output.reserve(64);
    while (dir.next()){
      #ifdef USE_SPIFFS
        String error=checkForUnsupportedPath(dir.fileName());
        if(error.length()>0){
          Serial.println(String("Ignoring ") + error + dir.fileName());
          continue;
        }
      #endif
      if(output.length()){server.sendContent(output);output=',';}// send string from previous iteration as an HTTP chunk
      else{output='[';}
      output += "{\"type\":\"";
      if(dir.isDirectory()){output+="dir";}
      else{output+=F("file\",\"size\":\"");output+=dir.fileSize();}
      output += F("\",\"name\":\"");
      if(dir.fileName()[0]=='/'){output += &(dir.fileName()[1]);}   // Always return names without leading "/"
      else{output+=dir.fileName();}
      output+="\"}";
    }
    // send last string
    output+="]";
    server.sendContent(output);
    server.chunkedResponseFinalize();
  });
  /////////// Load editor
  server.on("/edit", HTTP_GET, [](){
    if(handleFileRead(F("/edit/index.html"))){return;}
    #ifdef INCLUDE_FALLBACK_INDEX_HTM
      server.sendHeader(F("Content-Encoding"), "gzip");
      server.send(200,"text/html",index_htm_gz,index_htm_gz_len);
    #else
      return server.send(404,"text/plain","FILE_NOT_FOUND");
    #endif
  });
  /////////// Create file
  server.on("/edit",  HTTP_PUT, [](){
    String path = server.arg("path");
    if(path.isEmpty()){return server.send(400,"text/plain","PATH ARG MISSING");}
    #ifdef USE_SPIFFS
      if(checkForUnsupportedPath(path).length()>0){return server.send(400,"text/plain","INVALID FILENAME");}
    #endif
    if(path=="/"){return server.send(400,"text/plain","BAD PATH");}
    if(fileSystem->exists(path)){return server.send(400, "text/plain","PATH FILE EXISTS");}
    String src=server.arg("src");
    if(src.isEmpty()){
      // No source specified: creation
      Serial.println(String("handleFileCreate: ")+path);
      if(path.endsWith("/")){
        // Создание папки
        path.remove(path.length()-1);
        if(!fileSystem->mkdir(path)){return server.send(500,"text/plain","MKDIR FAILED\r\n");}
      }else{
        // Создание файла
        File file=fileSystem->open(path,"w");
        if(file){
          file.write((const char *)0);
          file.close();
        }else{return server.send(500,"text/plain","CREATE FAILED\r\n");}
      }
      if(path.lastIndexOf('/')>-1){path=path.substring(0,path.lastIndexOf('/'));}
      server.send(200,"text/plain",path);
    }else{
      // Source specified: rename
      if(src=="/"){
        return server.send(400,"text/plain","BAD SRC");
      }
      if(!fileSystem->exists(src)){return server.send(404, "text/plain","FILE_NOT_FOUND");}
      Serial.println(String("handleFileCreate: ")+path+" from "+src);
      if(path.endsWith("/")){
        path.remove(path.length()-1);
      }
      if(src.endsWith("/")){src.remove(src.length() - 1);}
      if(!fileSystem->rename(src,path)){
        return server.send(500,"text/plain","RENAME FAILED\r\n");
      }
      while(!src.isEmpty()&&!fileSystem->exists(src)){
        if(src.lastIndexOf('/')>0){
          src=path.substring(0, src.lastIndexOf('/'));
        }else{src=String();}  // No slash => the top folder does not exist
      }
      server.send(200,"text/plain",src);
    }
  });
  /////////// Delete file
  server.on("/edit",  HTTP_DELETE, [](){
    String path = server.arg(0);
    if (path.isEmpty() || path == "/") {
      return server.send(400, "text/plain","BAD PATH");
    }
    Serial.println(String("handleFileDelete: ") + path);
    if (!fileSystem->exists(path)) {
      return server.send(404, "text/plain","FILE_NOT_FOUND");
    }
    deleteRecursive(path);
    while (!path.isEmpty() && !fileSystem->exists(path)) {
      if (path.lastIndexOf('/') > 0) {
        path = path.substring(0, path.lastIndexOf('/'));
      } else {
        path = String();  // No slash => the top folder does not exist
      }
    }
    server.send(200,"text/plain",path);
  });
  /////////// Upload file
  // - first callback is called after the request has ended with all parsed arguments
  // - second callback handles file upload at that location
  server.on("/edit",HTTP_POST,replyOK,handleFileUpload);
  //=========== WSled
  server.on("/wsled.jsone",HTTP_POST,[](){
    String json="{\"mode_WS\":\"";
    if(sec_demo_WS==0)json+=mode_WS;
    else json+="0"; 
    json+="\"}";
    server.send(200,"text/plain",json);
  });
  server.on("/wsled.php",HTTP_POST,[](){
    if(indexs.authOn && verific()) return;
    if(server.hasArg("mode_WS")){
      uint8_t a=server.arg("mode_WS").toInt();
      if(a==0)sec_demo_WS=10;
      else{
        sec_demo_WS=0;
        mode_WS=a;
      }
      server.send(200,"text/plain","OK");
    }else server.send(200,"text/plain","Ошибочный запрос");
  });
  //=========== end
  server.onNotFound(handletFoundAuth);
  server.on("/img/favicon.ico",HTTP_GET,handletFoundNotAuth);
  server.on("/style.css",HTTP_GET,handletFoundNotAuth);
  server.on("/start.js",HTTP_GET,handletFoundNotAuth);
  server.on("/esp/time_ver.php",HTTP_POST,[](){
    String json="{";
    json+="\"ver\":\"";       json+=ver;                          json+="\",";
    json+="\"day\":\"";       json+=String(day);                  json+="\",";
    json+="\"month\":\"";     json+=String(month);                json+="\",";
    json+="\"year\":\"";      json+=String(year);                 json+="\",";
    json+="\"hour\":\"";      json+=String(hour);                 json+="\",";
    json+="\"min\":\"";       json+=(minute<10?"0":"")+String(minute); json+="\",";
    json+="\"sec\":\"";       json+=(second<10?"0":"")+String(second); json+="\"}";
    server.send(200,"text/plain",json);
  });
  httpUpdater.setup(&server);
  server.begin();
}
//==========================================
void replyOK() {
  server.send(200, "text/plain", "");
}
//------------------------------------====
void handleFileUpload() {
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    // Убедитесь, что пути всегда начинаются с "/"
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    Serial.println(String("handleFileUpload Name: ") + filename);
    uploadFile = fileSystem->open(filename, "w");
    if (!uploadFile) {
      return server.send(500, "text/plain","CREATE FAILED\r\n");
    }
    Serial.println(String("Upload: START, filename: ") + filename);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      size_t bytesWritten = uploadFile.write(upload.buf, upload.currentSize);
      if (bytesWritten != upload.currentSize) {
        return server.send(500, "text/plain","WRITE FAILED\r\n");
      }
    }
    Serial.println(String("Upload: WRITE, Bytes: ") + upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
    }
    Serial.println(String("Upload: END, Size: ") + upload.totalSize);
  }
}
//----------------------------------------------------------------------------
void handleLoadFile(){
  if(verific()) return;
  HTTPUpload& upload = server.upload();
  if(upload.status==UPLOAD_FILE_START) {
    String filename=upload.filename;
    // Убедитесь, что пути всегда начинаются с "/"
    if(!filename.startsWith("/")){
      filename="/"+filename;
    }
    Serial.println(String("handleFileUpload Name: ")+filename);
    uploadFile=fileSystem->open(filename,"w");
    if(!uploadFile){return server.send(500,"text/plain","CREATE FAILED\r\n");}
    Serial.println(String("Upload: START, filename: ")+filename);
  }else if(upload.status==UPLOAD_FILE_WRITE){
    if(uploadFile){
      size_t bytesWritten=uploadFile.write(upload.buf, upload.currentSize);
      if(bytesWritten!=upload.currentSize){return server.send(500,"text/plain","WRITE FAILED\r\n");}
    }
    Serial.println(String("Upload: WRITE, Bytes: ")+upload.currentSize);
  }else if(upload.status==UPLOAD_FILE_END){
    if(uploadFile){uploadFile.close();}
    Serial.println(String("Upload: END, Size: ")+upload.totalSize);
    bip();
  }
}
//--------------------------------------------------------------------------
void handletFoundAuth(){
  String uri = ESP8266WebServer::urlDecode(server.uri()); // required to read paths with blanks
  if(uri.endsWith(".js")){
    handletFoundNotAuth();
    return;
  }
  if(verific()) return;
  if(handleFileRead(uri)){return;}
  String message;
  message.reserve(100);
  message=F("Error: File not found\n\nURI: ");
  message+=uri;
  message+=F("\nMethod: ");
  message+=(server.method()==HTTP_GET)?"GET":"POST";
  message+=F("\nArguments: ");
  message+=server.args();
  message+='\n';
  for(uint8_t i=0;i<server.args();i++){
    message+=F(" NAME:");
    message+=server.argName(i);
    message+=F("\n VALUE:");
    message+=server.arg(i);
    message+='\n';
  }
  message+="path=";
  message+=server.arg("path");
  message+='\n';
  Serial.print(message);
  return server.send(404,"text/plain",message);
}
//--------------------------------------------------------------------------
void handletFoundNotAuth(){
  if(handleFileRead(ESP8266WebServer::urlDecode(server.uri()))){return server.send(200,"text/plain","OK");}
  server.send(404,"text/plain","Not Found CSS/JS");
}
//-----------------------------------------
bool handleFileRead(String path){
  Serial.println(String("handleFileRead: ")+path);
  if(path.endsWith("/")){path+="index.html";}
  String contentType;
  if(server.hasArg("download")){contentType=F("application/octet-stream");}
  else{contentType = mime::getContentType(path);}
  if(!fileSystem->exists(path)){path=path+".gz";} // File not found, try gzip version
  if(fileSystem->exists(path)){
    File file=fileSystem->open(path,"r");
    if(server.streamFile(file,contentType)!=file.size()){Serial.println("Sent less data than expected!");}
    file.close();
    return true;
  }
  return false;
}
//----------------------------------------------------
void deleteRecursive(String path) {
  File file = fileSystem->open(path, "r");
  bool isDir = file.isDirectory();
  file.close();
  if (!isDir) {
    fileSystem->remove(path);
    return;
  }
  Dir dir = fileSystem->openDir(path);
  while (dir.next()) {
    deleteRecursive(path + '/' + dir.fileName());
  }
     // Then delete the folder itself
  fileSystem->rmdir(path);
}
//------------------------------------------------------
bool verific(){
  if(server.arg("auth")!=indexs.auth && indexs.authOn){
    server.send_P(404, "text/html", "404, you are not authorized!");
    return true;
  }
  return false;
}

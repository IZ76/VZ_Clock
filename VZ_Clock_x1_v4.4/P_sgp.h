const char P_sgp[] PROGMEM =  R"=====(
<html>
    <head>
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер VZ_Clock_x1 (by IvanZah)</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function set_sgp(submit) {
                server = "/sgp?eCo2Led=" + val_sw('eCo2Led') + "&sgpCo2LivelAlarm=" + val('sgpCo2LivelAlarm') + "&eCo2AlarmEsp=" + val_sw('eCo2AlarmEsp') + "&eCo2AlarmMqtt=" + val_sw('eCo2AlarmMqtt');
                server += "&tvocLed=" + val_sw('tvocLed') + "&sgpTvocLivelAlarm=" + val('sgpTvocLivelAlarm') + "&tvocAlarmEsp=" + val_sw('tvocAlarmEsp') + "&tvocAlarmMqtt=" + val_sw('tvocAlarmMqtt');
                server += "&setSgpCorr=" + val('setSgpCorr') + "&sgpCorrTemp=" + val('sgpCorrTemp') + "&sgpCorrHumi=" + val('sgpCorrHumi');
                send_request(submit,server);
            }
        </script>
    </head>
    <body onload = "load('/configs_sgp');">
        <header>
            <span class="opt_cn">Часы информер VZ_Clock_{{ver}} (by IvanZah)</span><br>
            <span class="opt_cn">Время на ESP:</span>
            <span id="time">{{time}}</span>
        </header>       
        <central-unit>
            <div class="menu_unit">
                <span class="ico__text" onclick="get_html('index.vz');">Wifi</span>
                <span class="ico__text" onclick="get_html('time.vz');">Время</span>
                <span class="ico__text" onclick="get_html('mqtt.vz');">Mqtt</span>
                <span class="ico__text" onclick="get_html('thing.vz');">Thing</span>
                <span class="ico__text" onclick="get_html('weather.vz');">Погода</span>
                <span class="ico__text" onclick="get_html('setup.vz');">Настройки</span>
                <span class="ico__text" onclick="get_html('help.vz');">Помощь</span>
            </div>
            <titles>
                <span class="opt_cn">Настройки SGP-30</span>
            </titles>
            <div id="main_unit">
                <paragraph><span class="opt_cn">Настройка параметров eCO₂:</span></paragraph>
                <div class="informer">
                    <span class="opt_cn"><b>SGP30 измеряет два параметра: TVOC и eCO₂.</b></span>
                    <span class="opt_cn">eCO₂ - эквивалент диоксида углерода СО₂-экв: единица, используемая для сравнения излучающей способности парниковых газов с диоксидом углерода.</span>
                    <a href="http://piccy.info/view3/13864970/061d0af592b2841b0ffbcc8a04a5153a/" target="_blank"><img src="http://i.piccy.info/i9/c88497dcdc24329655424209daa90a32/1592663986/27636/1367845/eCO2_500.jpg" width="440" alt="Картинка из интернета" border="0" /></a><a href="http://i.piccy.info/a3c/2020-06-20-14-39/i9-13864970/500x326-r" target="_blank"><img src="http://i.piccy.info/a3/2020-06-20-14-39/i9-13864970/500x326-r/i.gif" alt="" border="0" /></a>
                </div>
                <br>
                <span class="opt_cn">Текущий уровень eCO₂ (400-5000++): <b>{{sgpCo2}}ppm</b></span>
                <br>
                <span class="opt_cn">({{textCo2}})</span>
                <br>
                <br>
                <center>
                  <table>
                    <tr>
                        <td align = "right"><span class="opt_cn">Отображать уровень eCO₂ на экране</span></td>
                        <td><input type="checkbox" class="checkbox" id="eCo2Led" name="switch-btn"></td>
                    </tr>
                  </table>
                  <div class="informer">
                      <span class="opt_cn">Уровни оповещения eCO₂: <b>0</b>=(400-699), <b>1</b>=(700-999), <b>2</b>=(1000-2499), <b>3</b>=(2500-4999), <b>4</b>=(5000++)</span>
                  </div>
                  <table>
                      <tr>
                          <td align = "right"><span class="opt_cn">Установить уровень оповещения:</span></td>
                          <td><select id = "sgpCo2LivelAlarm">
                              <option value = "0"> 0 </option>
                              <option value = "1"> 1 </option>
                              <option value = "2"> 2 </option>
                              <option value = "3"> 3 </option>
                              <option value = "4"> 4 </option>
                          </select></td>
                      </tr>
                      <tr>
                          <td align = "right"><span class="opt_cn">Выводить оповещение на экран</span></td>
                          <td><input type="checkbox" class="checkbox" id="eCo2AlarmEsp" name="switch-btn" ></td>
                      </tr>
                      <tr>
                          <td align = "right"><span class="opt_cn">Отправлять Alarm в информ топик MQTT </span></td>
                          <td><input type="checkbox" class="checkbox" id="eCo2AlarmMqtt" name="switch-btn"></td>
                      </tr>
                  </table>
                </center>
                <br>

                <paragraph><span class="opt_cn">Настройка параметров TVOC:</span></paragraph>
                <div class="informer">
                    <span class="opt_cn">TVOC - совокупные летучие органические соединения. В некоторых случаях измерение одного только CO₂ не показывает превышения VOC.</span>
                    <a href="http://piccy.info/view3/13865133/19c9f4eb269e095f2168eff76ded6f61/" target="_blank"><img src="http://i.piccy.info/i9/7a041b5e7861475556d28c9faa6e61d2/1592670426/8368/1367845/iAQ_core_AMS_2.png" width="440" alt="Картинка из интернета" border="0" /></a><a href="http://i.piccy.info/a3c/2020-06-20-16-27/i9-13865133/500x235-r" target="_blank"><img src="http://i.piccy.info/a3/2020-06-20-16-27/i9-13865133/500x235-r/i.gif" alt="" border="0" /></a>
                </div>
                <br>
                <span class="opt_cn">Текущий уровень TVOC (0-2200++): <b>{{sgpTvoc}}ppb</b></span>
                <br>
                <span class="opt_cn">({{textTvoc}})</span>
                <br>
                <br>
                <center>
                  <table>
                    <tr>
                      <td align = "right"><span class="opt_cn">Отображать уровень TVOC на экране</span></td>
                      <td><input type="checkbox" class="checkbox" id="tvocLed" name="switch-btn"></td>
                    </tr>
                  </table>
                  <div class="informer">
                      <span class="opt_cn">Уровни оповещения TVCO:  <b>0</b>=(0-64),  <b>1</b>=(65-219), <b>2</b>=(220-659), <b>3</b>=(660-2199), <b>4</b>=(2200++)</span>
                  </div>
                  <table>
                      <tr>
                          <td align = "right"><span class="opt_cn">Установить уровень оповещения:</span></td>
                          <td><select id = "sgpTvocLivelAlarm">
                              <option value = "0"> 0 </option>
                              <option value = "1"> 1 </option>
                              <option value = "2"> 2 </option>
                              <option value = "3"> 3 </option>
                              <option value = "4"> 4 </option>
                          </select></td>
                      </tr>
                      <tr>
                          <td align = "right"><span class="opt_cn">Выводить оповещение на экран</span></td>
                          <td><input type="checkbox" class="checkbox" id="tvocAlarmEsp" name="switch-btn" ></td>
                      </tr>
                      <tr>
                          <td align = "right"><span class="opt_cn">Отправлять Alarm в информ топик MQTT</span></td>
                          <td><input type="checkbox" class="checkbox" id="tvocAlarmMqtt" name="switch-btn"></td>
                      </tr>
                  </table>
                </center>  
                <br>
                <br>
                <paragraph><span class="opt_cn">Настройка коррекци данных SGP-30:</span></paragraph>
                <center>
                    <div class="informer">
                        <span class="opt_cn">Во избежание ошибок измерения необходимо указать датчику текущую абсолютную влажность воздуха, которая будет учтена при расчёте концентрации. Для этого, в паре с SGP30, рекомендуется использовать датчик влажности/температуры.</span>
                    </div>
                    <select id = "setSgpCorr">
                        <option value = "0"> Не учитывать </option>
                        <option value = "1"> BME-280 </option>
                        <option value = "2"> Si7021 </option>
                        <option value = "3"> AHTx0 </option>
                        <option value = "4"> DHT11/22 </option>
                        <option value = "99"> Ручной ввод </option>
                    </select>
                    <br>
                    <span class="opt_cn">Данные ручного ввода:</span>
                    <br>
                    <table>
                        <tr>
                            <td><span class="opt_cn">Коррекция температуры (°С): </span></td>
                            <td><input type="text" class="field form-control" id="sgpCorrTemp" value="{{sgpCorrTemp}}" pattern="\- \d+(\.\d{2})?"></td>
                        </tr>
                        <tr>
                            <td><span class="opt_cn">Коррекция влажности (%): </span></td>
                            <td><input type="text"class="field form-control" id="sgpCorrHumi" value="{{sgpCorrHumi}}" pattern="\- \d+(\.\d{2})?"></td>
                        </tr>
                    </table>
                </center>  
                <br>
                <br>
                <span class="save_booton" onclick="set_sgp(this);">Сохранить настройки</span>
                <br>
                <br>
            </div>
            <div>
                <a href="http://forum.amperka.ru/members/al_nai_ex.27406/"><font size="2">Поддержка SGP30 внедрена при содействии Al_Nai_ex</font></a>
                <a href="https://github.com/IZ76/VZ_Clocke" class = "right"><font size="2">GitHub</font></a>
            </div>
        </central-unit>
    </body>           
</html>
)=====";

const char P_mqtt[] PROGMEM =  R"=====(
<html>
    <head>
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер VZ_Clock_x2 (by IvanZah)</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function set_mqtt_ust(submit) {
                server = "/mqttUst?mqttOn=" + val('mqttOn') + "&mqtt_server=" + val('mqtt_server') + "&mqtt_port=" + (val('mqtt_port')) + "&mqtt_user=" + (val('mqtt_user')) + "&mqtt_pass=" + (val('mqtt_pass')) + "&timeOutMqtt=" + val('timeOutMqtt');
                server += "&mqtt_name=" + val('mqtt_name') + "&mqtt_sub1=" + val('mqtt_sub1') + "&mqtt_sub2=" + val('mqtt_sub2') + "&mqtt_sub3=" + val('mqtt_sub3') + "&mqtt_sub_inform=" + val('mqtt_sub_inform') + "&mqtt_pub_tvoc=" + val('mqtt_pub_tvoc');
                server += "&mqtt_pub_sensor00=" + val('mqtt_pub_sensor00') + "&mqtt_pub_sensor01=" + val('mqtt_pub_sensor01') + "&mqtt_pub_sensor02=" + val('mqtt_pub_sensor02') + "&mqtt_pub_sensor03=" + val('mqtt_pub_sensor03') + "&mqtt_pub_sensor04=" + val('mqtt_pub_sensor04') + "&mqtt_pub_eco2=" + val('mqtt_pub_eco2');
                send_request(submit,server);
            }
            function set_mqtt_on(submit) {
                server = "/mqttOn?mqttOn=" + val_sw("mqttOn");
                send_request(submit, server);
            }
        </script>
    </head>
    <body onload = "load('/configs_mqtt');">
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
                <span class="opt_cn">Настройки MQTT</span>
            </titles>
            <div class="informer">
                <span class="opt_cn">Конфигурация MQTT сервера. Вы можете использовать внешний сервер, например:</span>
                <a href="https://www.cloudmqtt.com" align="center" target="_blank"> cloudmqtt.com</a>
            </div>            
            <div id="main_unit">
                <hr>
                <label class="switch">
                    <span class="opt_cn">Использовать MQTT сервер</span>
                    <input type="checkbox" class="checkbox" id="mqttOn" name="switch-btn" onclick="set_mqtt_on(this);">
                    <span class="switch-btn"></span>
                </label>
                <hr>
                <paragraph><span class="opt_cn">Параметры MQTT сервера:</span></paragraph>
                <center>
                <table>
                    <tr>
                        <td align = "right"><span class="opt_cn">Адрес сервера</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_server" value="{{mqtt_server}}" pattern="[- 0-9a-zA-Z._/]{1,20}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Порт сервера</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_port" value="{{mqtt_port}}" pattern="[\d]{4,5}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Имя пользователя</span></td>
                        <td><input class="field form-control" id="mqtt_user" value="{{mqtt_user}}" pattern="[-\w\s#@/]{0,20}" onfocus="this.type='text'" type="password"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Пароль пользователя</span></td>
                        <td><input class="field form-control" id="mqtt_pass" value="{{mqtt_pass}}" pattern="[-\w\s#@/]{0,20}" onfocus="this.type='text'" type="password"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Имя устройства (ESP)</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_name" value="{{mqtt_name}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                </table>
                </center>  
                <paragraph><span class="opt_cn">Топики входящих сообщений</span></paragraph>
                <center>
                <table>
                    <tr>
                        <td align = "right"><span class="opt_cn">MQTT1</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_sub1" value="{{mqtt_sub1}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">MQTT2</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_sub2" value="{{mqtt_sub2}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">MQTT3</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_sub3" value="{{mqtt_sub3}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Информ.</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_sub_inform" value="{{mqtt_sub_inform}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                </table>
                </center> 
                <paragraph><span class="opt_cn">Топики исходящих сообщений</span></paragraph>
                <center>
                <table>                    
                    <tr>
                        <td align = "right"><span class="opt_cn">Датчик 1</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_pub_sensor00" value="{{mqtt_pub_sensor00}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Датчик 2</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_pub_sensor01" value="{{mqtt_pub_sensor01}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Датчик 3</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_pub_sensor02" value="{{mqtt_pub_sensor02}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Датчик 4</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_pub_sensor03" value="{{mqtt_pub_sensor03}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Датчик 5</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_pub_sensor04" value="{{mqtt_pub_sensor04}}" pattern="[-\w\s#@/]{1,24}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Данные eCO₂</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_pub_eco2" value="{{mqtt_pub_eco2}}" pattern="[-\w\s#@/]{0,20}"></td>
                    </tr>
                    <tr>
                        <td align = "right"><span class="opt_cn">Данные TVOC</span></td>
                        <td><input type="text" class="field form-control" id="mqtt_pub_tvoc" value="{{mqtt_pub_tvoc}}" pattern="[-\w\s#@/]{0,20}"></td>
                    </tr>
                </table>
                <br>
                <span class="opt_cn">Таймаут данных</span>
                    <select id = "timeOutMqtt">
                    <option value = "300000"> 5 минут</option>
                    <option value = "600000"> 10 минут</option>
                    <option value = "900000"> 15 минут</option>
                    <option value = "1200000"> 20 минут</option>
                    <option value = "1800000"> 30 минут</option>
                    <option value = "3600000"> 1 час </option>
                    <option value = "7200000"> 2 часа </option>
                    <option value = "10800000"> 3 часа </option>
                </select>
                <br>
                </center>
                <br>
                <span class="save_booton" onclick="set_mqtt_ust(this);">Сохранить настройки MQTT</span>
                <br>
                <br>
            </div>
            <div>
                <a href="http://forum.amperka.ru/threads/%D0%A7%D0%B0%D1%81%D1%8B-%D0%BD%D0%B0-esp-01-nodemcu-max7219-web-%D0%B8%D0%BD%D1%82%D0%B5%D1%80%D1%84%D0%B5%D0%B9%D1%81.12767/">Forum</a>
                <a href="https://github.com/IZ76/ESP8266_Clock_Weather-station_Informer_with_WEB_interface" class = "right">GitHub</a>
            </div>
        </central-unit>
    </body>           
</html>
)=====";

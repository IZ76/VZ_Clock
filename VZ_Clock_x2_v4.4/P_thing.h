const char P_thing[] PROGMEM =  R"=====(
<html>
    <head>
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер VZ_Clock_x2 (by IvanZah)</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function set_sendThing_on(submit) {
                server = "/thing_on?sendThing=" + val_sw("sendThing");
                send_request(submit, server);
            }
            function set_getThing_on(submit) {
                server = "/thing_on?getThing=" + val_sw("getThing");
                send_request(submit, server);
            }
            function set_thing_ust(submit) {
                server = "/set_thing?channelIdWrite=" + val('channelIdWrite') + "&writeApiKey=" + val('writeApiKey') + "&wrApiKey=" + val('wrApiKey');
                server += "&writeFild0=" + val('writeFild0') + "&writeFild1=" + val('writeFild1') + "&writeFild2=" + val('writeFild2') + "&writeFild3=" + val('writeFild3');
                server += "&writeFild4=" + val('writeFild4') + "&writeFild5=" + val('writeFild5') + "&writeFild6=" + val('writeFild6') + "&writeFild7=" + val('writeFild7');
                server += "&channelIdRead=" + val('channelIdRead') + "&readApiKey=" + val('readApiKey');
                server += "&statThing0=" + val('statThing0') + "&statThing1=" + val('statThing1') + "&statThing2=" + val('statThing2') + "&statThing3=" + val('statThing3') + "&statThing4=" + val('statThing4');
                send_request(submit,server);
            }
        </script>
    </head>
    <body onload = "load('/configs_thing');">
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
                <span class="opt_cn">Настройки ThingSpeak</span>
            </titles>
            <div class="informer">
                <span class="opt_cn">Для использования данного сервиса необходимо пройти регистрацию на сайте: </span>
                <a href="https://thingspeak.com" align="center" target="_blank"> thingspeak.com</a>
                <span class="opt_cn">и получить необходимые для работы ключи</span>
            </div>            
            <div id="main_unit">
                <paragraph><span class="opt_cn">Параметры отправки данных:</span></paragraph>
                <center>
                    <hr>
                    <label class="switch">
                        <span class="opt_cn">Отправлять данные на ThingSpeak</span>
                        <input type="checkbox" class="checkbox" id="sendThing" name="switch-btn" onclick="set_sendThing_on(this);">
                        <span class="switch-btn"></span>
                    </label>
                    <hr>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Channel ID</span></td>
                            <td><input class="field form-control" id="channelIdWrite" value="{{channelIdWrite}}" pattern="[\d]{1,20}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Write API Key</span></td>
                            <td><input type="text" class="field form-control" id="writeApiKey" value="{{writeApiKey}}" pattern="[- 0-9a-zA-Z._/]{1,20}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Read API Keys</span></td>
                            <td><input type="text" class="field form-control" id="wrApiKey" value="{{wrApiKey}}" pattern="[- 0-9a-zA-Z._/]{1,20}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Channel Name: </span></td>
                            <td><span>{{nameWriteChannel}}</span></td>
                        </tr>
                    </table>
                    <table id="table__font" class = "table">
                        <tr>
                            <td><span class="opt_cn"><b> Field </b></span></td>
                            <td><span class="opt_cn"><b> Имя/Статус</b></span></td>
                            <td><span class="opt_cn"><b> Данные </b></span></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">1</span></td>
                            <td><span class="opt_cn"> {{nameField0}} </span></td>
                            <td><select id = "writeFild0">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">2</span></td>
                            <td><span class="opt_cn"> {{nameField1}} </span></td>
                            <td><select id = "writeFild1">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">3</span></td>
                            <td><span class="opt_cn"> {{nameField2}} </span></td>
                            <td><select id = "writeFild2">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">4</span></td>
                            <td><span class="opt_cn"> {{nameField3}} </span></td>
                            <td><select id = "writeFild3">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">5</span></td>
                            <td><span class="opt_cn"> {{nameField4}} </span></td>
                            <td><select id = "writeFild4">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">6</span></td>
                            <td><span class="opt_cn"> {{nameField5}} </span></td>
                            <td><select id = "writeFild5">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">7</span></td>
                            <td><span class="opt_cn"> {{nameField6}} </span></td>
                            <td><select id = "writeFild6">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">8</span></td>
                            <td><span class="opt_cn"> {{nameField7}} </span></td>
                            <td><select id = "writeFild7">
                                <option value = "0"> НЕТ </option>
                                <option value = "1"> Датчик 1 </option>
                                <option value = "2"> Датчик 2 </option>
                                <option value = "3"> Датчик 3 </option>
                                <option value = "4"> Датчик 4 </option>
                                <option value = "5"> Датчик 5 </option>
                                <option value = "6"> Данные eCO2 </option>
                                <option value = "7"> Данные TVOC </option>
                            </select></td>
                        </tr>
                    </table>
                    <br>
                </center>  
                <paragraph><span class="opt_cn">Параметры получения данных:</span></paragraph>
                <center>
                    <hr>
                    <label class="switch">
                        <span class="opt_cn">Получать данные от ThingSpeak</span>
                        <input type="checkbox" class="checkbox" id="getThing" name="switch-btn" onclick="set_getThing_on(this);">
                        <span class="switch-btn"></span>
                    </label>
                    <hr>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Channel ID</span></td>
                            <td><input class="field form-control" id="channelIdRead" value="{{channelIdRead}}" pattern="[\d]{1,20}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Read API Keys</span></td>
                            <td><input type="text" class="field form-control" id="readApiKey" value="{{readApiKey}}" pattern="[- 0-9a-zA-Z._/]{1,20}"></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Channel Name: </span></td>
                            <td><span>{{nameReadChannel}}</span></td>
                        </tr>
                    </table>
                    <table id="table__font" class = "table">
                        <tr>
                            <td><span class="opt_cn"><b> Переменная</b></span></td>
                            <td><span class="opt_cn"><b> Имя(номер) </b></span></td>
                        </tr>
                        <tr>
                            <td><span class="opt_cn"> THING1 </span></td>
                            <td><select id = "statThing0">
                                <option value = "0"> Не использовать </option>
                                <option value = "1"> {{nameReadField0}} (1) </option>
                                <option value = "2"> {{nameReadField1}} (2) </option>
                                <option value = "3"> {{nameReadField2}} (3) </option>
                                <option value = "4"> {{nameReadField3}} (4) </option>
                                <option value = "5"> {{nameReadField4}} (5) </option>
                                <option value = "6"> {{nameReadField5}} (6) </option>
                                <option value = "7"> {{nameReadField6}} (7) </option>
                                <option value = "8"> {{nameReadField7}} (8) </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td><span class="opt_cn"> THING2 </span></td>
                            <td><select id = "statThing1">
                                <option value = "0"> Не использовать </option>
                                <option value = "1"> {{nameReadField0}} (1) </option>
                                <option value = "2"> {{nameReadField1}} (2) </option>
                                <option value = "3"> {{nameReadField2}} (3) </option>
                                <option value = "4"> {{nameReadField3}} (4) </option>
                                <option value = "5"> {{nameReadField4}} (5) </option>
                                <option value = "6"> {{nameReadField5}} (6) </option>
                                <option value = "7"> {{nameReadField6}} (7) </option>
                                <option value = "8"> {{nameReadField7}} (8) </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td><span class="opt_cn"> THING3 </span></td>
                            <td><select id = "statThing2">
                                <option value = "0"> Не использовать </option>
                                <option value = "1"> {{nameReadField0}} (1) </option>
                                <option value = "2"> {{nameReadField1}} (2) </option>
                                <option value = "3"> {{nameReadField2}} (3) </option>
                                <option value = "4"> {{nameReadField3}} (4) </option>
                                <option value = "5"> {{nameReadField4}} (5) </option>
                                <option value = "6"> {{nameReadField5}} (6) </option>
                                <option value = "7"> {{nameReadField6}} (7) </option>
                                <option value = "8"> {{nameReadField7}} (8) </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td><span class="opt_cn"> THING4 </span></td>
                            <td><select id = "statThing3">
                                <option value = "0"> Не использовать </option>
                                <option value = "1"> {{nameReadField0}} (1) </option>
                                <option value = "2"> {{nameReadField1}} (2) </option>
                                <option value = "3"> {{nameReadField2}} (3) </option>
                                <option value = "4"> {{nameReadField3}} (4) </option>
                                <option value = "5"> {{nameReadField4}} (5) </option>
                                <option value = "6"> {{nameReadField5}} (6) </option>
                                <option value = "7"> {{nameReadField6}} (7) </option>
                                <option value = "8"> {{nameReadField7}} (8) </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td><span class="opt_cn"> THING5 </span></td>
                            <td><select id = "statThing4">
                                <option value = "0"> Не использовать </option>
                                <option value = "1"> {{nameReadField0}} (1) </option>
                                <option value = "2"> {{nameReadField1}} (2) </option>
                                <option value = "3"> {{nameReadField2}} (3) </option>
                                <option value = "4"> {{nameReadField3}} (4) </option>
                                <option value = "5"> {{nameReadField4}} (5) </option>
                                <option value = "6"> {{nameReadField5}} (6) </option>
                                <option value = "7"> {{nameReadField6}} (7) </option>
                                <option value = "8"> {{nameReadField7}} (8) </option>
                            </select></td>
                        </tr>
                    </table>
                    <br>
                </center>
                <br>
                <span class="save_booton" onclick="set_thing_ust(this);">Сохранить настройки ThingSpeak</span>
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

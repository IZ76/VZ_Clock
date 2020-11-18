const char P_setup[] PROGMEM =  R"=====(
<html>
    <head>
        <!--meta name = "viewport" content = "width=device-width, initial-scale=1.0"-->
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер VZ_Clock_x2 (by IvanZah)</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function set_setup(submit) {
                server = "/setup?tbd=" + val('tbd') + "&vbd=" + val('vbd') + "&tbn=" + val('tbn') + "&vbn=" + val('vbn') + "&vba=" + val_sw('vba');
                server += "&rotate0=" + val('rotate0') + "&rotate1=" + val('rotate1') + "&kuOn=" + val('kuOn') + "&kuOff=" + val('kuOff') + "&secLine=" + val('secLine') + "&timeStopBigCklock=" + val('timeStopBigCklock');
                server += "&param0=" + val('param0') + "&param1=" + val('param1') + "&param2=" + val('param2') + "&param3=" + val('param3') + "&param4=" + val('param4');
                server += "&sensor00=" + val('sensor00') + "&sensor01=" + val('sensor01') + "&sensor02=" + val('sensor02') + "&sensor03=" + val('sensor03') + "&sensor04=" + val('sensor04') + "&pressSys=" + val('pressSys') + "&clock2line=" + val('clock2line') + "&buzzerSet=" + val_sw('buzzerSet') + "&timeScrollSpeed=" + val('timeScrollSpeed');
                server += "&fontCLOCK=" + val('fontCLOCK') + "&fontSizeCLOCK=" + val('fontSizeCLOCK') + "&fontSizeData=" + val('fontSizeData') + "&dataDown=" + val('dataDown') + "&animPoint=" + val('animPoint') + "&textDown=" + val('textDown') + "&displayData=" + val('displayData');
                server += "&corr00=" + val('corr00') + "&corr01=" + val('corr01') + "&corr02=" + val('corr02') + "&corr03=" + val('corr03') + "&corr04=" + val('corr04') + "&NUM_MAX0=" + val('NUM_MAX0') + "&NUM_MAX1=" + val('NUM_MAX1') + "&butStat=" + val('butStat');
                send_request(submit,server);
            }
        </script>
    </head>
    <body onload = "load('/configs_setup');">
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
                <span class="opt_cn">Настройки Часов</span>
            </titles>
            <div class="informer">
                <span class="opt_cn">Здесь можно настроить параметры экрана, отображение данных на экране, параметры и отображение датчиков.</a>
            </div> 
            <div id="main_unit">
                <center>
                    <paragraph><span class="opt_cn">Настройки экрана</span></paragraph>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Дневной режим экрана с:</span></td>
                            <td><select id = "tbd">
                                <option value = "0"> 0 </option>
                                <option value = "1"> 1 </option>
                                <option value = "2"> 2 </option>
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>
                                <option value = "8"> 8 </option>
                                <option value = "9"> 9 </option>
                                <option value = "10"> 10 </option>
                                <option value = "11"> 11 </option>
                                <option value = "12"> 12 </option>
                                <option value = "13"> 13 </option>
                                <option value = "14"> 14 </option>
                                <option value = "15"> 15 </option>
                                <option value = "16"> 16 </option>
                                <option value = "17"> 17 </option>
                                <option value = "18"> 18 </option>
                                <option value = "19"> 19 </option>
                                <option value = "20"> 20 </option>
                                <option value = "21"> 21 </option>
                                <option value = "22"> 22 </option>
                                <option value = "23"> 23 </option>
                            </select></td>
                            <td align = "right"><span class="opt_cn">до:</span></td>
                            <td><select id = "tbn">
                                <option value = "1"> 1 </option>
                                <option value = "2"> 2 </option>
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>
                                <option value = "8"> 8 </option>
                                <option value = "9"> 9 </option>
                                <option value = "10"> 10 </option>
                                <option value = "11"> 11 </option>
                                <option value = "12"> 12 </option>
                                <option value = "13"> 13 </option>
                                <option value = "14"> 14 </option>
                                <option value = "15"> 15 </option>
                                <option value = "16"> 16 </option>
                                <option value = "17"> 17 </option>
                                <option value = "18"> 18 </option>
                                <option value = "19"> 19 </option>
                                <option value = "20"> 20 </option>
                                <option value = "21"> 21 </option>
                                <option value = "22"> 22 </option>
                                <option value = "23"> 23 </option>
                                <option value = "24"> полуночи </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Уровень яркости днем:</span></td>
                            <td><select id = "vbd">
                                <option value = "0"> 0 </option>
                                <option value = "1"> 1 </option>
                                <option value = "2"> 2 </option>
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>
                                <option value = "8"> 8 </option>
                                <option value = "9"> 9 </option>
                                <option value = "10"> 10 </option>
                                <option value = "11"> 11 </option>
                                <option value = "12"> 12 </option>
                                <option value = "13"> 13 </option>
                                <option value = "14"> 14 </option>
                                <option value = "15"> 15 </option>
                            </select></td>
                            <td align = "right"><span class="opt_cn">ночью:</span></td>
                            <td><select id = "vbn">
                                <option value = "0"> 0 </option>
                                <option value = "1"> 1 </option>
                                <option value = "2"> 2 </option>
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>
                                <option value = "8"> 8 </option>
                                <option value = "9"> 9 </option>
                                <option value = "10"> 10 </option>
                                <option value = "11"> 11 </option>
                                <option value = "12"> 12 </option>
                                <option value = "13"> 13 </option>
                                <option value = "14"> 14 </option>
                                <option value = "15"> 15 </option>
                            </select></td>
                        </tr>
                        <tr>
                    </table>
                    <label class="switch"><span class="opt_cn">Автояркость (при наличии модуля с фоторезистором)</span>
                        <input type="checkbox" id="vba">
                        <span class="switch-btn"></span>
                    </label>
                    <br>
                    <table id="table__font" class = "table">
                        <tr>
                            <td></td>
                            <td><span class="opt_cn"><b> 1-я строка </b></span></td>
                            <td><span class="opt_cn"><b> 2-я строка </b></span></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Кол-во модулей</span></td>
                            <td><select id = "NUM_MAX0">
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>
                                <option value = "8"> 8 </option>
                            </select></td>
                            <td><select id = "NUM_MAX1">
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>
                                <option value = "8"> 8 </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Вращение</span></td>
                            <td><select id = "rotate0">
                                <option value = "0"> 0 </option>
                                <option value = "90"> 90 </option>
                                <option value = "180"> 180 </option>
                                <option value = "270"> 270 </option>
                            </select></td>
                            <td><select id = "rotate1">
                                <option value = "0"> 0 </option>
                                <option value = "90"> 90 </option>
                                <option value = "180"> 180 </option>
                                <option value = "270"> 270 </option>
                            </select></td>
                        </tr>
                    </table>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Шрифт часов</span></td>
                            <td><select id = "fontCLOCK">
                                <option value = "0"> Широкий </option>
                                <option value = "1"> Широкий цифровой </option>
                                <option value = "2"> Жирный </option>
                                <option value = "3"> Жирный цифровой</option>
                                <option value = "4"> Нормальный </option>
                                <option value = "5"> Нормальный цифровой</option>
                                <option value = "6"> Узкий </option>
                                <option value = "7"> Узкий цифровой</option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Высота часов</span></td>
                            <td><select id = "fontSizeCLOCK">
                                <option value = "0"> 8 линий </option>
                                <option value = "1"> 7 линий </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Высота данных</span></td>
                            <td><select id = "fontSizeData">
                                <option value = "0"> 8 линий </option>
                                <option value = "1"> 7 линий </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Смещение данных</span></td>
                            <td><select id = "dataDown">
                                <option value = "0"> Без смещения </option>
                                <option value = "1"> Смещение вниз </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Отображать данные</span></td>
                            <td><select id = "timeStopBigCklock">
                                <option value = "0"> Не отображать </option>
                                <option value = "1"> 1 сек. </option>
                                <option value = "2"> 2 сек. </option>
                                <option value = "3"> 3 сек. </option>
                                <option value = "4"> 4 сек. </option>
                                <option value = "5"> 5 сек. </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Смещение текста</span></td>
                            <td><select id = "textDown">
                                <option value = "0"> Без смещения </option>
                                <option value = "1"> Смещение вниз </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Отображать дату</span></td>
                            <td><select id = "displayData">
                                <option value = "0"> Откл. </option>
                                <option value = "1"> Статично </option>
                                <option value = "2"> Строкой </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Часы в две строки</span></td>
                            <td><select id = "clock2line">
                                <option value = "0"> Никогда </option>
                                <option value = "1"> Только ночью </option>
                                <option value = "2"> Всегда </option>
                                <option value = "3"> Всегда + сек.L</option>
                                <option value = "4"> Всегда + сек.B</option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Анимация точек</span></td>
                            <td><select id = "animPoint">
                                <option value = "0"> Нет </option>
                                <option value = "1"> Простая H </option>
                                <option value = "2"> Простая L </option>
                                <option value = "3"> MAX H</option>
                                <option value = "4"> MAX L</option>
                                <option value = "5"> MAX2 H</option>
                                <option value = "6"> MAX2 L</option>
                                <option value = "7"> Мерцание H</option>
                                <option value = "8"> Мерцание L</option>
                                <option value = "9"> Вращение H</option>
                                <option value = "10"> Вращение L</option>
                                
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Анимация секунд</span></td>
                            <td><select id = "secLine">
                                <option value = "0"> Нет </option>
                                <option value = "1"> Часы </option>
                                <option value = "2"> 2 линии </option>
                                <option value = "3"> 1 линия </option>
                            </select></td>
                        </tr>
                        <tr>
                          <td align = "right"><span class="opt_cn">Скорость бегущей строки</span></td> 
                          <td><select id = "timeScrollSpeed">
                              <option value = "1"> 1 </option>
                              <option value = "5"> 5 </option>
                              <option value = "10"> 10 </option>
                              <option value = "15"> 15 </option>
                              <option value = "20"> 20 </option>
                              <option value = "25"> 25 </option>
                              <option value = "30"> 30 </option>
                              <option value = "35"> 35 </option>
                              <option value = "40"> 40 </option>
                              <option value = "45"> 45 </option>
                              <option value = "50"> 50 </option>
                              <option value = "55"> 55 </option>
                              <option value = "60"> 60 </option>
                              <option value = "65"> 65 </option>
                              <option value = "70"> 70 </option>
                              <option value = "75"> 75 </option>
                              <option value = "80"> 80 </option>
                              <option value = "85"> 85 </option>
                              <option value = "90"> 90 </option>
                              <option value = "95"> 95 </option>
                              <option value = "99"> 99 </option>
                          </select></td>
                        </tr>
                    </table>
                    <br>
                    <paragraph><span class="opt_cn">Настройки датчиков</span></paragraph>
                    <div class="informer">
                        <span class="opt_cn">Уровень яркости фоторезистора (0-15): </span>{{lba}}
                    </div>
                    <label class="switch"><span class="opt_cn">Использовать активный базер</span>
                        <input type="checkbox" id="buzzerSet">
                        <span class="switch-btn"></span>
                    </label>
                    <br>
                    <table>
                        <tr>
                            <td align = "right"><span class="opt_cn">Сигнал в начале каждого часа с:</span></td>
                            <td><select id = "kuOn">
                                <option value = "0"> 0 </option>
                                <option value = "1"> 1 </option>
                                <option value = "2"> 2 </option>
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>    
                                <option value = "8"> 8 </option>
                                <option value = "9"> 9 </option>
                                <option value = "10"> 10 </option>
                                <option value = "11"> 11 </option>
                                <option value = "12"> 12 </option>
                                <option value = "13"> 13 </option>
                                <option value = "14"> 14 </option>
                                <option value = "15"> 15 </option>
                                <option value = "16"> 16 </option>
                                <option value = "17"> 17 </option>
                                <option value = "18"> 18 </option>
                                <option value = "19"> 19 </option>
                                <option value = "20"> 20 </option>
                                <option value = "21"> 21 </option>
                                <option value = "22"> 22 </option>
                                <option value = "23"> 23 </option>
                            </select></td>
                            <td align = "right"><span class="opt_cn">до:</span></td>
                            <td><select id = "kuOff">
                                <option value = "1"> 1 </option>
                                <option value = "2"> 2 </option>
                                <option value = "3"> 3 </option>
                                <option value = "4"> 4 </option>
                                <option value = "5"> 5 </option>
                                <option value = "6"> 6 </option>
                                <option value = "7"> 7 </option>
                                <option value = "8"> 8 </option>
                                <option value = "9"> 9 </option>
                                <option value = "10"> 10 </option>
                                <option value = "11"> 11 </option>
                                <option value = "12"> 12 </option>
                                <option value = "13"> 13 </option>
                                <option value = "14"> 14 </option>
                                <option value = "15"> 15 </option>
                                <option value = "16"> 16 </option>
                                <option value = "17"> 17 </option>
                                <option value = "18"> 18 </option>
                                <option value = "19"> 19 </option>
                                <option value = "20"> 20 </option>
                                <option value = "21"> 21 </option>
                                <option value = "22"> 22 </option>
                                <option value = "23"> 23 </option>
                                <option value = "24"> полуночи </option>
                            </select></td>
                        </tr>
                        <tr>
                            <td align = "right"><span class="opt_cn">Кнопка подключена к GPIO16 и к:</span></td>
                            <td><select id = "butStat">
                                <option value = "0"> - 5V </option>
                                <option value = "1"> + 5V </option>
                            </select></td>
                        </tr>
                    </table>
                    <div class="informer">
                        <span class="opt_cn">Подтягивающий резистор подключен соответственно к GPIO16 и к противоположному полюсу питания</span>
                    </div>
                    <table id="table__font" class = "table">
                        <tr>
                            <td></td>
                            <td><span class="opt_cn"><b> Датчик </b></span></td>
                            <td><span class="opt_cn"><b> Корр. </b></span></td>
                            <td><span class="opt_cn"><b> Данные </b></span></td>
                        </tr>
                        <tr>
                            <td><select id = "param0">
                              <option value = "0"> tD </option>
                              <option value = "1"> tU </option>
                              <option value = "2"> tH </option>
                              <option value = "3"> tT </option>
                              <option value = "4"> tL </option>
                              <option value = "10"> h </option>
                              <option value = "11"> h1 </option>
                              <option value = "12"> h2 </option>
                              <option value = "13"> h3 </option>
                              <option value = "14"> h4 </option>
                              <option value = "20"> P </option>
                              <option value = "21"> C </option>
                              <option value = "22"> V </option>
                              <option value = "23"> A </option>
                            </select></td>
                            <td><select id = "sensor00">
                              <option value = "0"> НЕТ </option>
                              <option value = "1"> DS18B20 </option>
                              <option value = "10"> AHTx0 </option>
                              <option value = "2"> SI7021 </option>
                              <option value = "3"> BMP180/280 </option>
                              <option value = "4"> BME280 </option>
                              <option value = "5"> DHT11/22 </option>
                              <option value = "9"> NMon </option>
                              <option value = "6"> MQTT1 </option>
                              <option value = "7"> MQTT2 </option>
                              <option value = "8"> MQTT3 </option>
                              <option value = "11"> THING1 </option>
                              <option value = "12"> THING2 </option>
                              <option value = "13"> THING3 </option>
                              <option value = "14"> THING4 </option>
                              <option value = "15"> THING5 </option>
                            </select></td>
                            <td><input type="number" class="field form-control" id="corr00" value="{{corr00}}" pattern="\- \d+(\.\d{2})?"min="-9999" max="99999" step="0.01"></td>
                            <td><span class="opt_cn"> {{Td}} </span></td>
                        </tr>
                        <tr>
                            <td><select id = "param1">
                              <option value = "0"> tD </option>
                              <option value = "1"> tU </option>
                              <option value = "2"> tH </option>
                              <option value = "3"> tT </option>
                              <option value = "4"> tL </option>
                              <option value = "10"> h </option>
                              <option value = "11"> h1 </option>
                              <option value = "12"> h2 </option>
                              <option value = "13"> h3 </option>
                              <option value = "14"> h4 </option>
                              <option value = "20"> P </option>
                              <option value = "21"> C </option>
                              <option value = "22"> V </option>
                              <option value = "23"> A </option>
                            </select></td>
                            <td><select id = "sensor01">
                              <option value = "0"> НЕТ </option>
                              <option value = "1"> DS18B20 </option>
                              <option value = "10"> AHTx0 </option>
                              <option value = "2"> SI7021 </option>
                              <option value = "3"> BMP180/280 </option>
                              <option value = "4"> BME280 </option>
                              <option value = "5"> DHT11/22 </option>
                              <option value = "9"> NMon </option>
                              <option value = "6"> MQTT1 </option>
                              <option value = "7"> MQTT2 </option>
                              <option value = "8"> MQTT3 </option>
                              <option value = "11"> THING1 </option>
                              <option value = "12"> THING2 </option>
                              <option value = "13"> THING3 </option>
                              <option value = "14"> THING4 </option>
                              <option value = "15"> THING5 </option>
                            </select></td>
                            <td><input type="number" class="field form-control" id="corr01" value="{{corr01}}" pattern="\- \d+(\.\d{2})?" min="-9999" max="99999" step="0.01"></td>
                            <td><span class="opt_cn"> {{Tu}} </span></td>
                        </tr>
                        <tr>
                            <td><select id = "param2">
                              <option value = "0"> tD </option>
                              <option value = "1"> tU </option>
                              <option value = "2"> tH </option>
                              <option value = "3"> tT </option>
                              <option value = "4"> tL </option>
                              <option value = "10"> h </option>
                              <option value = "11"> h1 </option>
                              <option value = "12"> h2 </option>
                              <option value = "13"> h3 </option>
                              <option value = "14"> h4 </option>
                              <option value = "20"> P </option>
                              <option value = "21"> C </option>
                              <option value = "22"> V </option>
                              <option value = "23"> A </option>
                            </select></td>
                            <td><select id = "sensor02">
                              <option value = "0"> НЕТ </option>
                              <option value = "1"> DS18B20 </option>
                              <option value = "10"> AHTx0 </option>
                              <option value = "2"> SI7021 </option>
                              <option value = "3"> BMP180/280 </option>
                              <option value = "4"> BME280 </option>
                              <option value = "5"> DHT11/22 </option>
                              <option value = "9"> NMon </option>
                              <option value = "6"> MQTT1 </option>
                              <option value = "7"> MQTT2 </option>
                              <option value = "8"> MQTT3 </option>
                              <option value = "11"> THING1 </option>
                              <option value = "12"> THING2 </option>
                              <option value = "13"> THING3 </option>
                              <option value = "14"> THING4 </option>
                              <option value = "15"> THING5 </option>
                            </select></td>
                            <td><input type="number" class="field form-control" id="corr02" value="{{corr02}}" pattern="\- \d+(\.\d{2})?"min="-9999" max="99999" step="0.01"></td>
                            <td><span class="opt_cn"> {{Th}} </span></td>
                        </tr>
                        <tr>
                            <td><select id = "param3">
                              <option value = "0"> tD </option>
                              <option value = "1"> tU </option>
                              <option value = "2"> tH </option>
                              <option value = "3"> tT </option>
                              <option value = "4"> tL </option>
                              <option value = "10"> h </option>
                              <option value = "11"> h1 </option>
                              <option value = "12"> h2 </option>
                              <option value = "13"> h3 </option>
                              <option value = "14"> h4 </option>
                              <option value = "20"> P </option>
                              <option value = "21"> C </option>
                              <option value = "22"> V </option>
                              <option value = "23"> A </option>
                            </select></td>
                            <td><select id = "sensor03">
                              <option value = "0"> НЕТ </option>
                              <option value = "1"> DS18B20 </option>
                              <option value = "10"> AHTx0 </option>
                              <option value = "2"> SI7021 </option>
                              <option value = "3"> BMP180/280 </option>
                              <option value = "4"> BME280 </option>
                              <option value = "5"> DHT11/22 </option>
                              <option value = "9"> NMon </option>
                              <option value = "6"> MQTT1 </option>
                              <option value = "7"> MQTT2 </option>
                              <option value = "8"> MQTT3 </option>
                              <option value = "11"> THING1 </option>
                              <option value = "12"> THING2 </option>
                              <option value = "13"> THING3 </option>
                              <option value = "14"> THING4 </option>
                              <option value = "15"> THING5 </option>
                              <option value = "99">  </option>
                            </select></td>
                            <td><input type="number" class="field form-control" id="corr03" value="{{corr03}}" pattern="\- \d+(\.\d{2})?"min="-9999" max="99999" step="0.01"></td>
                            <td><span class="opt_cn"> {{Hd}} </span></td>
                        </tr>
                        <tr>
                            <td><select id = "param4">
                              <option value = "0"> tD </option>
                              <option value = "1"> tU </option>
                              <option value = "2"> tH </option>
                              <option value = "3"> tT </option>
                              <option value = "4"> tL </option>
                              <option value = "10"> h </option>
                              <option value = "11"> h1 </option>
                              <option value = "12"> h2 </option>
                              <option value = "13"> h3 </option>
                              <option value = "14"> h4 </option>
                              <option value = "20"> P </option>
                              <option value = "21"> C </option>
                              <option value = "22"> V </option>
                              <option value = "23"> A </option>
                            </select></td>
                            <td><select id = "sensor04">
                              <option value = "0"> НЕТ </option>
                              <option value = "1"> DS18B20 </option>
                              <option value = "10"> AHTx0 </option>
                              <option value = "2"> SI7021 </option>
                              <option value = "3"> BMP180/280 </option>
                              <option value = "4"> BME280 </option>
                              <option value = "5"> DHT11/22 </option>
                              <option value = "9"> NMon </option>
                              <option value = "6"> MQTT1 </option>
                              <option value = "7"> MQTT2 </option>
                              <option value = "8"> MQTT3 </option>
                              <option value = "11"> THING1 </option>
                              <option value = "12"> THING2 </option>
                              <option value = "13"> THING3 </option>
                              <option value = "14"> THING4 </option>
                              <option value = "15"> THING5 </option>
                              <option value = "99">  </option>
                            </select></td>
                            <td><input type="number" class="field form-control" id="corr04" value="{{corr04}}" pattern="\- \d+(\.\d{2})?" min="-9999" max="99999" step="0.01"></td>
                            <td><span class="opt_cn"> {{Pu}} </span></td>
                        </tr>
                        <tr>
                            <td></td>
                            <td><select id = "pressSys">
                              <option value = "0"> hPa </option>
                              <option value = "1"> mmHg </option>
                            </select></td>
                            <td></td>
                            <td></td>
                        </tr>
                    </table>
                    <br>
                    <span class="link__text" onclick="get_html('sgp.vz');" id="sgp"><b>{{sgp}}</b></span>
                    <br>
                    <br>
                    <span class="save_booton" onclick="set_setup(this);">Сохранить настройки</span>
                </center>
            </div>
            <div>
                <a href="http://forum.amperka.ru/threads/%D0%A7%D0%B0%D1%81%D1%8B-%D0%BD%D0%B0-esp-01-nodemcu-max7219-web-%D0%B8%D0%BD%D1%82%D0%B5%D1%80%D1%84%D0%B5%D0%B9%D1%81.12767/">Forum</a>
                <a href="https://github.com/IZ76/ESP8266_Clock_Weather-station_Informer_with_WEB_interface" class = "right">GitHub</a>
            </div>
        </central-unit>
    </body>
</html>
)=====";

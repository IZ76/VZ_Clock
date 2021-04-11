function load(){
  load_ver();
  out_report();
  values();
  val2();
}
function values(){
    ajax("sgps.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('sgpCo2LivelAlarm').value=vals.sgpCo2LivelAlarm;
            document.getElementById('eCo2AlarmEsp').checked=vals.eCo2AlarmEsp;
            document.getElementById('eCo2AlarmMqtt').checked=vals.eCo2AlarmMqtt;
            document.getElementById('eCo2Led').checked=vals.eCo2Led;
            document.getElementById('sgpTvocLivelAlarm').value=vals.sgpTvocLivelAlarm;
            document.getElementById('tvocAlarmEsp').checked=vals.tvocAlarmEsp;
            document.getElementById('tvocAlarmMqtt').checked=vals.tvocAlarmMqtt;
            document.getElementById('tvocLed').checked=vals.tvocLed;
            document.getElementById('setSgpCorr').value=vals.setSgpCorr;
            document.getElementById('sgpCorrTemp').value=vals.sgpCorrTemp;
            document.getElementById('sgpCorrHumi').value=vals.sgpCorrHumi;
        },data:"VALUES="+Math.random()
    })
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    vals.sgpCo2LivelAlarm=enc(e.sgpCo2LivelAlarm);
    document.getElementById("eCo2AlarmEsp").checked?vals.eCo2AlarmEsp=1:vals.eCo2AlarmEsp=0;
    document.getElementById("eCo2AlarmMqtt").checked?vals.eCo2AlarmMqtt=1:vals.eCo2AlarmMqtt=0;
    document.getElementById("eCo2Led").checked?vals.eCo2Led=1:vals.eCo2Led=0;
    vals.sgpTvocLivelAlarm=enc(e.sgpTvocLivelAlarm);
    document.getElementById("tvocAlarmEsp").checked?vals.tvocAlarmEsp=1:vals.tvocAlarmEsp=0;
    document.getElementById("tvocAlarmMqtt").checked?vals.tvocAlarmMqtt=1:vals.tvocAlarmMqtt=0;
    document.getElementById("tvocLed").checked?vals.tvocLed=1:vals.tvocLed=0;
    vals.setSgpCorr=enc(e.setSgpCorr);
    vals.sgpCorrTemp=enc(e.sgpCorrTemp);
    vals.sgpCorrHumi=enc(e.sgpCorrHumi);
     t+="&JS="+JSON.stringify(vals);
    ajax("esp/sgps.php",{
        success:function(r){
            inner("mess",r.responseText);
            var sv=document.getElementById("mess");
            sv.style.display="block";
            setTimeout(function(){
                sv.style.display="none";
                inner("mess","отправка")
            },5000)
        },data:t
    })
}
function val2(){
    ajax("esp/sgps_var.json",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('sgpCo2',j.sgpCo2);
            inner('textCo2',j.textCo2);
            inner('sgpTvoc',j.sgpTvoc);
            inner('textTvoc',j.textTvoc);
            if(j.BME=="1") document.getElementById('BME').style.display='block';
            else document.getElementById('BME').style.display='none';
            if(j.SI=="1")  document.getElementById('SI').style.display='block';
            else document.getElementById('SI').style.display='none';
            if(j.AHT=="1") document.getElementById('AHT').style.display='block';
            else document.getElementById('AHT').style.display='none';
            //if(j.DHT=="1") document.getElementById('DHT').style.display='block';
            //else document.getElementById('DHT').style.display='none';                    
        },data:"VALUES="+Math.random()
    })
}
function load(){
  load_ver();
  out_report();
  values();
  setInterval("val3()",3000);
}
function values(){
    ajax("senss.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('mqttOn').checked=vals.mqttOn;
            document.getElementById('thingOn').checked=vals.thingOn;
            document.getElementById('nmonOn').checked=vals.nmonOn;
            document.getElementById('sgpOn').checked=vals.sgpOn;
            document.getElementById('params0').value=vals.params0;
            document.getElementById('params1').value=vals.params1;
            document.getElementById('params2').value=vals.params2;
            document.getElementById('params3').value=vals.params3;
            document.getElementById('params4').value=vals.params4;
            document.getElementById('params5').value=vals.params5;
            document.getElementById('params6').value=vals.params6;
            document.getElementById('params7').value=vals.params7;
            document.getElementById('sensors0').value=vals.sensors0;
            document.getElementById('sensors1').value=vals.sensors1;
            document.getElementById('sensors2').value=vals.sensors2;
            document.getElementById('sensors3').value=vals.sensors3;
            document.getElementById('sensors4').value=vals.sensors4;
            document.getElementById('sensors5').value=vals.sensors5;
            document.getElementById('sensors6').value=vals.sensors6;
            document.getElementById('sensors7').value=vals.sensors7;
            document.getElementById('corr0').value=vals.corr0;
            document.getElementById('corr1').value=vals.corr1;
            document.getElementById('corr2').value=vals.corr2;
            document.getElementById('corr3').value=vals.corr3;
            document.getElementById('corr4').value=vals.corr4;
            document.getElementById('corr5').value=vals.corr5;
            document.getElementById('corr6').value=vals.corr6;
            document.getElementById('corr7').value=vals.corr7;
            document.getElementById('buzzerSet').value=vals.buzzerSet;
            document.getElementById('kuOn').value=vals.kuOn;
            document.getElementById('kuOff').value=vals.kuOff;
            document.getElementById('butStat').value=vals.butStat;
            document.getElementById('pressSys').value=vals.pressSys;
            document.getElementById('printCom').checked=vals.printCom;
			document.getElementById('updateOTA').checked=vals.updateOTA;
            document.getElementById('messLang').value=vals.messLang;
            document.getElementById('beepSave').checked=vals.beepSave;
        },data:"VALUES="+Math.random()
    })
    val2();
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    document.getElementById("mqttOn").checked?vals.mqttOn=1:vals.mqttOn=0;
    document.getElementById("thingOn").checked?vals.thingOn=1:vals.thingOn=0;
    document.getElementById("nmonOn").checked?vals.nmonOn=1:vals.nmonOn=0;
    document.getElementById("sgpOn").checked?vals.sgpOn=1:vals.sgpOn=0;
    vals.params0=enc(e.params0);
    vals.params1=enc(e.params1);
    vals.params2=enc(e.params2);
    vals.params3=enc(e.params3);
    vals.params4=enc(e.params4);
    vals.params5=enc(e.params5);
    vals.params6=enc(e.params6);
    vals.params7=enc(e.params7);
    vals.sensors0=enc(e.sensors0);
    vals.sensors1=enc(e.sensors1);
    vals.sensors2=enc(e.sensors2);
    vals.sensors3=enc(e.sensors3);
    vals.sensors4=enc(e.sensors4);
    vals.sensors5=enc(e.sensors5);
    vals.sensors6=enc(e.sensors6);
    vals.sensors7=enc(e.sensors7);
    vals.corr0=enc(e.corr0);
    vals.corr1=enc(e.corr1);
    vals.corr2=enc(e.corr2);
    vals.corr3=enc(e.corr3);
    vals.corr4=enc(e.corr4);
    vals.corr5=enc(e.corr5);
    vals.corr6=enc(e.corr6);
    vals.corr7=enc(e.corr7);
    vals.buzzerSet=enc(e.buzzerSet);
    vals.kuOn=enc(e.kuOn);
    vals.kuOff=enc(e.kuOff);
    vals.butStat=enc(e.butStat);
    vals.pressSys=enc(e.pressSys);
    vals.messLang=enc(e.messLang);
    document.getElementById("printCom").checked?vals.printCom=1:vals.printCom=0;
	document.getElementById("updateOTA").checked?vals.updateOTA=1:vals.updateOTA=0;
    document.getElementById("beepSave").checked?vals.beepSave=1:vals.beepSave=0;
    t+="&JS="+JSON.stringify(vals);
    ajax("esp/senss.php",{
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
    values();
}
function val3(){
    ajax("esp/sens_val.json",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('vol00',j.vol00);
            inner('vol01',j.vol01);
            inner('vol02',j.vol02);
            inner('vol03',j.vol03);
            inner('vol04',j.vol04);
            inner('vol05',j.vol05);
            inner('vol06',j.vol06);
            inner('vol07',j.vol07);
        },data:"VALUES="+Math.random()
    })
}
function val2(){
    ajax("esp/sens_var.json",{
        success:function(r){
            var j=JSON.parse(r.responseText);
                for(var i=0;i<8;i++){
                    if(j.DS=="1")  document.getElementById('DS_'+String(i)).style.display='block';
                    if(j.BMP=="1") document.getElementById('BMP_'+String(i)).style.display='block';
                    if(j.BME=="1") document.getElementById('BME_'+String(i)).style.display='block';
                    if(j.SI=="1")  document.getElementById('SI_'+String(i)).style.display='block';
                    if(j.AHT=="1") document.getElementById('AHT_'+String(i)).style.display='block';
                    //if(j.DHT=="1") document.getElementById('DHT_'+String(i)).style.display='block';
                    if(j.MQ0=="1") document.getElementById('MQ_'+String(i)+'_0').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_0').style.display='none';
                    if(j.MQ1=="1") document.getElementById('MQ_'+String(i)+'_1').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_1').style.display='none';
                    if(j.MQ2=="1") document.getElementById('MQ_'+String(i)+'_2').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_2').style.display='none';
                    if(j.MQ3=="1") document.getElementById('MQ_'+String(i)+'_3').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_3').style.display='none';
                    if(j.MQ4=="1") document.getElementById('MQ_'+String(i)+'_4').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_4').style.display='none';
                    if(j.MQ5=="1") document.getElementById('MQ_'+String(i)+'_5').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_5').style.display='none';
                    if(j.MQ6=="1") document.getElementById('MQ_'+String(i)+'_6').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_6').style.display='none';
                    if(j.MQ7=="1") document.getElementById('MQ_'+String(i)+'_7').style.display='block';
                    else document.getElementById('MQ_'+String(i)+'_7').style.display='none';
                    if(j.TH0=="1") document.getElementById('TH_'+String(i)+'_0').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_0').style.display='none';
                    if(j.TH1=="1") document.getElementById('TH_'+String(i)+'_1').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_1').style.display='none';
                    if(j.TH2=="1") document.getElementById('TH_'+String(i)+'_2').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_2').style.display='none';
                    if(j.TH3=="1") document.getElementById('TH_'+String(i)+'_3').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_3').style.display='none';
                    if(j.TH4=="1") document.getElementById('TH_'+String(i)+'_4').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_4').style.display='none';
                    if(j.TH5=="1") document.getElementById('TH_'+String(i)+'_5').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_5').style.display='none';
                    if(j.TH6=="1") document.getElementById('TH_'+String(i)+'_6').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_6').style.display='none';
                    if(j.TH7=="1") document.getElementById('TH_'+String(i)+'_7').style.display='block';
                    else document.getElementById('TH_'+String(i)+'_7').style.display='none';
                    if(j.NM0=="1") document.getElementById('NM_'+String(i)+'_0').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_0').style.display='none';
                    if(j.NM1=="1") document.getElementById('NM_'+String(i)+'_1').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_1').style.display='none';
                    if(j.NM2=="1") document.getElementById('NM_'+String(i)+'_2').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_2').style.display='none';
                    if(j.NM3=="1") document.getElementById('NM_'+String(i)+'_3').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_3').style.display='none';
                    if(j.NM4=="1") document.getElementById('NM_'+String(i)+'_4').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_4').style.display='none';
                    if(j.NM5=="1") document.getElementById('NM_'+String(i)+'_5').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_5').style.display='none';
                    if(j.NM6=="1") document.getElementById('NM_'+String(i)+'_6').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_6').style.display='none';
                    if(j.NM7=="1") document.getElementById('NM_'+String(i)+'_7').style.display='block';
                    else document.getElementById('NM_'+String(i)+'_7').style.display='none';
                }
        },data:"VALUES="+Math.random()
    })
}
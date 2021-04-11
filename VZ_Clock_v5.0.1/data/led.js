function load(){
  load_ver();
  out_report();
  values();
  setInterval("vba()",3000);
}
function values(){
    ajax("leds.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('NUM_VER_X').value=vals.NUM_VER_X;
            document.getElementById('NUM_MAX0').value=vals.NUM_MAX0;
            document.getElementById('NUM_MAX1').value=vals.NUM_MAX1;
            document.getElementById('rotate0').value=vals.rotate0;
            document.getElementById('rotate1').value=vals.rotate1;
            document.getElementById('mirror0').checked=vals.mirror0;
            document.getElementById('mirror1').checked=vals.mirror1;
            document.getElementById('timeDay').value=vals.timeDay;
            document.getElementById('timeNight').value=vals.timeNight;
            document.getElementById('volBrightsMax').value=vals.volBrightsMax;
            document.getElementById('volBrightsMin').value=vals.volBrightsMin;
            document.getElementById('typeSwitchBrightness').value=vals.typeSwitchBrightness;
            document.getElementById('fontSizeData').value=vals.fontSizeData;
            document.getElementById('textDown').value=vals.textDown;
            document.getElementById('dataDown').value=vals.dataDown;
            document.getElementById('displayData').value=vals.displayData;
            document.getElementById('clockAndSec').value=vals.clockAndSec;
            document.getElementById('maxModeSec').value=vals.maxModeSec;
            document.getElementById('animPoint').value=vals.animPoint;
            document.getElementById('secLine').value=vals.secLine;
            document.getElementById('timeScrollSpeed').value=vals.timeScrollSpeed;
            document.getElementById('function00').value=vals.function00;
            document.getElementById('function01').value=vals.function01;
            document.getElementById('function02').value=vals.function02;
            document.getElementById('function03').value=vals.function03;
            document.getElementById('function04').value=vals.function04;
            document.getElementById('function05').value=vals.function05;
            document.getElementById('function06').value=vals.function06;
            document.getElementById('function07').value=vals.function07;
            document.getElementById('function08').value=vals.function08;
            document.getElementById('function09').value=vals.function09;
            document.getElementById('function10').value=vals.function10;
            document.getElementById('function11').value=vals.function11;
            document.getElementById('period00').value=vals.period00;
            document.getElementById('period01').value=vals.period01;
            document.getElementById('period02').value=vals.period02;
            document.getElementById('period03').value=vals.period03;
            document.getElementById('period04').value=vals.period04;
            document.getElementById('period05').value=vals.period05;
            document.getElementById('period06').value=vals.period06;
            document.getElementById('period07').value=vals.period07;
            document.getElementById('period08').value=vals.period08;
            document.getElementById('period09').value=vals.period09;
            document.getElementById('period10').value=vals.period10;
            document.getElementById('period11').value=vals.period11;
            document.getElementById('view00').value=vals.view00;
            document.getElementById('view01').value=vals.view01;
            document.getElementById('view02').value=vals.view02;
            document.getElementById('view03').value=vals.view03;
            document.getElementById('view04').value=vals.view04;
            document.getElementById('view05').value=vals.view05;
            document.getElementById('view06').value=vals.view06;
            document.getElementById('view07').value=vals.view07;
            document.getElementById('view08').value=vals.view08;
            document.getElementById('view09').value=vals.view09;
            document.getElementById('view10').value=vals.view10;
            document.getElementById('view11').value=vals.view11;
            document.getElementById('dataAlig').value=vals.dataAlig;
            document.getElementById('clockAlig').value=vals.clockAlig;
            document.getElementById('fontCLOCK').value=vals.fontCLOCK;
            if(vals.typeSwitchBrightness!="0") document.getElementById('lba').style.display='block';
            else document.getElementById('lba').style.display='none';
            if(vals.clockAndSec=="0") document.getElementById('111').style.display='table-row';
            else document.getElementById('111').style.display='none';
            if(vals.clockAndSec!="1"&&vals.NUM_VER_X=="1") document.getElementById('112').style.display='table-row';
            else document.getElementById('112').style.display='none';
            if(vals.NUM_VER_X=="1"){
              document.getElementById('000').style.display='table-row';
              document.getElementById('001').style.display='table-cell';
              document.getElementById('002').style.display='table-cell';
              document.getElementById('003').style.display='table-cell';
              document.getElementById('004').style.display='table-cell';
              document.getElementById('014').style.display='table-cell';
              document.getElementById('005').style.display='none';
              document.getElementById('006').style.display='none';
            }else{
              document.getElementById('000').style.display='none';
              document.getElementById('001').style.display='none';
              document.getElementById('002').style.display='none';
              document.getElementById('003').style.display='none';
              document.getElementById('004').style.display='none';
              document.getElementById('014').style.display='none';
              document.getElementById('005').style.display='table-cell';
              document.getElementById('006').style.display='table-cell';
            }
            if(vals.clockAndSec=="2")document.getElementById('MMS').style.display='table-row';
        },data:"VALUES="+Math.random()
    })
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    vals.NUM_VER_X=enc(e.NUM_VER_X);
    vals.NUM_MAX0=enc(e.NUM_MAX0);
    vals.NUM_MAX1=enc(e.NUM_MAX1);
    vals.rotate0=enc(e.rotate0);
    vals.rotate1=enc(e.rotate1);
    document.getElementById("mirror0").checked?vals.mirror0=1:vals.mirror0=0;
    document.getElementById("mirror1").checked?vals.mirror1=1:vals.mirror1=0;
    vals.timeDay=enc(e.timeDay);
    vals.timeNight=enc(e.timeNight);
    vals.volBrightsMax=enc(e.volBrightsMax);
    vals.volBrightsMin=enc(e.volBrightsMin);
    vals.typeSwitchBrightness=enc(e.typeSwitchBrightness);
    vals.fontSizeData=enc(e.fontSizeData);
    vals.textDown=enc(e.textDown);
    vals.dataDown=enc(e.dataDown);
    vals.displayData=enc(e.displayData);
    vals.clockAndSec=enc(e.clockAndSec);
    vals.maxModeSec=enc(e.maxModeSec);
    vals.animPoint=enc(e.animPoint);
    vals.secLine=enc(e.secLine);
    vals.timeScrollSpeed=enc(e.timeScrollSpeed);
    vals.function00=enc(e.function00);
    vals.function01=enc(e.function01);
    vals.function02=enc(e.function02);
    vals.function03=enc(e.function03);
    vals.function04=enc(e.function04);
    vals.function05=enc(e.function05);
    vals.function06=enc(e.function06);
    vals.function07=enc(e.function07);
    vals.function08=enc(e.function08);
    vals.function09=enc(e.function09);
    vals.function10=enc(e.function10);
    vals.function11=enc(e.function11);
    vals.period00=enc(e.period00);
    vals.period01=enc(e.period01);
    vals.period02=enc(e.period02);
    vals.period03=enc(e.period03);
    vals.period04=enc(e.period04);
    vals.period05=enc(e.period05);
    vals.period06=enc(e.period06);
    vals.period07=enc(e.period07);
    vals.period08=enc(e.period08);
    vals.period09=enc(e.period09);
    vals.period10=enc(e.period10);
    vals.period11=enc(e.period11);
    vals.view00=enc(e.view00);
    vals.view01=enc(e.view01);
    vals.view02=enc(e.view02);
    vals.view03=enc(e.view03);
    vals.view04=enc(e.view04);
    vals.view05=enc(e.view05);
    vals.view06=enc(e.view06);
    vals.view07=enc(e.view07);
    vals.view08=enc(e.view08);
    vals.view09=enc(e.view09);
    vals.view10=enc(e.view10);
    vals.view11=enc(e.view11);
    vals.dataAlig=enc(e.dataAlig);
    vals.clockAlig=enc(e.clockAlig);
    vals.fontCLOCK=enc(e.fontCLOCK);
    if(vals.clockAndSec=="2")document.getElementById('MMS').style.display='table-row';
    else document.getElementById('MMS').style.display='none';
    t+="&JS="+JSON.stringify(vals);
    ajax("esp/leds.php",{
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
function vba(){
  if(vals.typeSwitchBrightness=="0"){
    inner('lba','');
  }
  ajax("esp/leds_var.json",{
      success:function(r){
          var j=JSON.parse(r.responseText);
          inner('lba','Уровень с датчика: '+j.lba);
          if(j.BH=="1") document.getElementById('BH').style.display='block';
          if(j.MAX=="1") document.getElementById('MAX').style.display='block';
          if(vals.NUM_VER_X!="1"){
            if(j.clockAlig=="1"){
              document.getElementById('110').style.display='table-row';
              document.getElementById('109').style.display='table-row';
            }else{
              document.getElementById('110').style.display='none';
              document.getElementById('109').style.display='none';
            }
          }else{
            if(j.clockAlig=="1"){
              document.getElementById('109').style.display='table-row';
            }else{
              document.getElementById('109').style.display='none';
            }
            if(j.dataAlig=="1"){
              document.getElementById('110').style.display='table-row';
            }else{
              document.getElementById('110').style.display='none';
            }
          }
      },data:"VALUES="+Math.random()
  })
}
function send_test(){
    var text="&TXT="+document.getElementById('text').value;
    ajax("/mess.php" ,{success:function(r){},data:"?SETS="+Math.random()+text});
}
function send_rss(){
    var text="&TXT="+document.getElementById('text').value;
    ajax("/rss.php" ,{success:function(r){},data:"?SETS="+Math.random()+text});
}
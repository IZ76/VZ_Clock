function load(){
  load_ver();
  out_report();
  values();
  val2();
}
function values(){
    ajax("nmons.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('uuid').value=vals.uuid;
            document.getElementById('api_key').value=vals.api_key;
            document.getElementById('sensors_ID0').value=vals.sensors_ID0;
            document.getElementById('sensors_ID1').value=vals.sensors_ID1;
            document.getElementById('sensors_ID2').value=vals.sensors_ID2;
            document.getElementById('sensors_ID3').value=vals.sensors_ID3;
            document.getElementById('sensors_ID4').value=vals.sensors_ID4;
            document.getElementById('sensors_ID5').value=vals.sensors_ID5;
            document.getElementById('sensors_ID6').value=vals.sensors_ID6;
            document.getElementById('sensors_ID7').value=vals.sensors_ID7;
            document.getElementById('latNmon').value=vals.latNmon;
            document.getElementById('lonNmon').value=vals.lonNmon;
            document.getElementById('periodNmon').value=vals.periodNmon;
            document.getElementById('writeNmon0').value=vals.writeNmon0;
            document.getElementById('writeNmon1').value=vals.writeNmon1;
            document.getElementById('writeNmon2').value=vals.writeNmon2;
            document.getElementById('writeNmon3').value=vals.writeNmon3;
            document.getElementById('writeNmon4').value=vals.writeNmon4;
            document.getElementById('writeNmon5').value=vals.writeNmon5;
            document.getElementById('writeNmon6').value=vals.writeNmon6;
            document.getElementById('writeNmon7').value=vals.writeNmon7;
            document.getElementById('nameNmon').value=vals.nameNmon;
        },data:"VALUES="+Math.random()
    })
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    vals.uuid=enc(e.uuid);
    vals.api_key=enc(e.api_key);
    vals.sensors_ID0=enc(e.sensors_ID0);
    vals.sensors_ID1=enc(e.sensors_ID1);
    vals.sensors_ID2=enc(e.sensors_ID2);
    vals.sensors_ID3=enc(e.sensors_ID3);
    vals.sensors_ID4=enc(e.sensors_ID4);
    vals.sensors_ID5=enc(e.sensors_ID5);
    vals.sensors_ID6=enc(e.sensors_ID6);
    vals.sensors_ID7=enc(e.sensors_ID7);
    vals.latNmon=enc(e.latNmon);
    vals.lonNmon=enc(e.lonNmon);
    vals.periodNmon=enc(e.periodNmon);
    vals.writeNmon0=enc(e.writeNmon0);
    vals.writeNmon1=enc(e.writeNmon1);
    vals.writeNmon2=enc(e.writeNmon2);
    vals.writeNmon3=enc(e.writeNmon3);
    vals.writeNmon4=enc(e.writeNmon4);
    vals.writeNmon5=enc(e.writeNmon5);
    vals.writeNmon6=enc(e.writeNmon6);
    vals.writeNmon7=enc(e.writeNmon7);
    vals.nameNmon=enc(e.nameNmon);
    t+="&JS="+JSON.stringify(vals);
    ajax("esp/nmons.php",{
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
    ajax("esp/nmons_var.json",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('macNmon',j.macNmon);
        },data:"VALUES="+Math.random()
    })
}
function sh_nmonApi(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('api_key').setAttribute("type",t)
}
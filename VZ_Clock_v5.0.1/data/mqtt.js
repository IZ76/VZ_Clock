function load(){
  load_ver();
  out_report();
  values();
}
function values(){
    ajax("mqtts.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('mqtt_server').value=vals.mqtt_server;
            document.getElementById('mqtt_port').value=vals.mqtt_port;
            document.getElementById('mqtt_user').value=vals.mqtt_user;
            document.getElementById('mqtt_pass').value=vals.mqtt_pass;
            document.getElementById('mqtt_name').value=vals.mqtt_name;
            document.getElementById('mqtt_sub0').value=vals.mqtt_sub0;
            document.getElementById('mqtt_sub1').value=vals.mqtt_sub1;
            document.getElementById('mqtt_sub2').value=vals.mqtt_sub2;
            document.getElementById('mqtt_sub3').value=vals.mqtt_sub3;
            document.getElementById('mqtt_sub4').value=vals.mqtt_sub4;
            document.getElementById('mqtt_sub5').value=vals.mqtt_sub5;
            document.getElementById('mqtt_sub6').value=vals.mqtt_sub6;
            document.getElementById('mqtt_sub7').value=vals.mqtt_sub7;
            document.getElementById('mqtt_sub_inform').value=vals.mqtt_sub_inform;
            document.getElementById('mqtt_pub_sensors0').value=vals.mqtt_pub_sensors0;
            document.getElementById('mqtt_pub_sensors1').value=vals.mqtt_pub_sensors1;
            document.getElementById('mqtt_pub_sensors2').value=vals.mqtt_pub_sensors2;
            document.getElementById('mqtt_pub_sensors3').value=vals.mqtt_pub_sensors3;
            document.getElementById('mqtt_pub_sensors4').value=vals.mqtt_pub_sensors4;
            document.getElementById('mqtt_pub_sensors5').value=vals.mqtt_pub_sensors5;
            document.getElementById('mqtt_pub_sensors6').value=vals.mqtt_pub_sensors6;
            document.getElementById('mqtt_pub_sensors7').value=vals.mqtt_pub_sensors7;
            document.getElementById('timeOutMqtt').value=vals.timeOutMqtt;
        },data:"VALUES="+Math.random()
    })
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    vals.mqtt_server=enc(e.mqtt_server);
    vals.mqtt_port=enc(e.mqtt_port);
    vals.mqtt_user=enc(e.mqtt_user);
    vals.mqtt_pass=enc(e.mqtt_pass);
    vals.mqtt_name=enc(e.mqtt_name);
    vals.mqtt_sub0=enc(e.mqtt_sub0);
    vals.mqtt_sub1=enc(e.mqtt_sub1);
    vals.mqtt_sub2=enc(e.mqtt_sub2);
    vals.mqtt_sub3=enc(e.mqtt_sub3);
    vals.mqtt_sub4=enc(e.mqtt_sub4);
    vals.mqtt_sub5=enc(e.mqtt_sub5);
    vals.mqtt_sub6=enc(e.mqtt_sub6);
    vals.mqtt_sub7=enc(e.mqtt_sub7);
    vals.mqtt_sub_inform=enc(e.mqtt_sub_inform);
    vals.mqtt_pub_sensors0=enc(e.mqtt_pub_sensors0);
    vals.mqtt_pub_sensors1=enc(e.mqtt_pub_sensors1);
    vals.mqtt_pub_sensors2=enc(e.mqtt_pub_sensors2);
    vals.mqtt_pub_sensors3=enc(e.mqtt_pub_sensors3);
    vals.mqtt_pub_sensors4=enc(e.mqtt_pub_sensors4);
    vals.mqtt_pub_sensors5=enc(e.mqtt_pub_sensors5);
    vals.mqtt_pub_sensors6=enc(e.mqtt_pub_sensors6);
    vals.mqtt_pub_sensors7=enc(e.mqtt_pub_sensors7);
    vals.timeOutMqtt=enc(e.timeOutMqtt);
     t+="&JS="+JSON.stringify(vals);
    ajax("esp/mqtts.php",{
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
function sh_user(e){
    if(e.checked)var t="text";
    else t="password";
    document.getElementById('mqtt_user').setAttribute("type",t)
}
function sh_pass(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('mqtt_pass').setAttribute("type",t)
}
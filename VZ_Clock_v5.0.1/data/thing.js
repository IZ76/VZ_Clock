function load(){
  load_ver();
  out_report();
  val2();
  values();
}
function values(){
    ajax("things.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('channelIdWrite').value=vals.channelIdWrite;
            document.getElementById('writeApiKey').value=vals.writeApiKey;
            document.getElementById('wrApiKey').value=vals.wrApiKey;
            document.getElementById('writeFild0').value=vals.writeFild0;
            document.getElementById('writeFild1').value=vals.writeFild1;
            document.getElementById('writeFild2').value=vals.writeFild2;
            document.getElementById('writeFild3').value=vals.writeFild3;
            document.getElementById('writeFild4').value=vals.writeFild4;
            document.getElementById('writeFild5').value=vals.writeFild5;
            document.getElementById('writeFild6').value=vals.writeFild6;
            document.getElementById('writeFild7').value=vals.writeFild7;
            document.getElementById('channelIdRead').value=vals.channelIdRead;
            document.getElementById('readApiKey').value=vals.readApiKey;
            document.getElementById('statThing0').value=vals.statThing0;
            document.getElementById('statThing1').value=vals.statThing1;
            document.getElementById('statThing2').value=vals.statThing2;
            document.getElementById('statThing3').value=vals.statThing3;
            document.getElementById('statThing4').value=vals.statThing4;
            document.getElementById('statThing5').value=vals.statThing5;
            document.getElementById('statThing6').value=vals.statThing6;
            document.getElementById('statThing7').value=vals.statThing7;
        },data:"VALUES="+Math.random()
    })
}
function val2(){
    ajax("esp/things_var.json",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('nameWriteChannel',j.nameWriteChannel);
            inner('nameField0',j.nameField0);
            inner('nameField1',j.nameField1);
            inner('nameField2',j.nameField2);
            inner('nameField3',j.nameField3);
            inner('nameField4',j.nameField4);
            inner('nameField5',j.nameField5);
            inner('nameField6',j.nameField6);
            inner('nameField7',j.nameField7);
            inner('nameReadChannel',j.nameReadChannel);
            var data=document.getElementsByTagName('body')[0].innerHTML;
            var new_string;
            for (var key in j){
                new_string=data.replace(new RegExp('{{'+key+'}}', 'g'), j[key]);
                data=new_string;
            }
            document.getElementsByTagName('body')[0].innerHTML=new_string;
        },data:"VALUES="+Math.random()
    })
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    vals.channelIdWrite=enc(e.channelIdWrite);
    vals.writeApiKey=enc(e.writeApiKey);
    vals.wrApiKey=enc(e.wrApiKey);
    vals.writeFild0=enc(e.writeFild0);
    vals.writeFild1=enc(e.writeFild1);
    vals.writeFild2=enc(e.writeFild2);
    vals.writeFild3=enc(e.writeFild3);
    vals.writeFild4=enc(e.writeFild4);
    vals.writeFild5=enc(e.writeFild5);
    vals.writeFild6=enc(e.writeFild6);
    vals.writeFild7=enc(e.writeFild7);
    vals.channelIdRead=enc(e.channelIdRead);
    vals.readApiKey=enc(e.readApiKey);
    vals.statThing0=enc(e.statThing0);
    vals.statThing1=enc(e.statThing1);
    vals.statThing2=enc(e.statThing2);
    vals.statThing3=enc(e.statThing3);
    vals.statThing4=enc(e.statThing4);
    vals.statThing5=enc(e.statThing5);
    vals.statThing6=enc(e.statThing6);
    vals.statThing7=enc(e.statThing7);
    t+="&JS="+JSON.stringify(vals);
    ajax("esp/things.php",{
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
function sh_sendWrApi(e){
    if(e.checked)var t="text";
    else t="password";
    document.getElementById('writeApiKey').setAttribute("type",t)
}
function sh_sendRdApi(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('wrApiKey').setAttribute("type",t)
}
function sh_getRdApi(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('readApiKey').setAttribute("type",t)
}
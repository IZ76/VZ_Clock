var path=["","/wifis.json","/ssids.json","/times.json","/weaths.json","/leds.json","/senss.json","/mqtts.json","/things.json","/nmons.json","/sgps.json","/irs.json",""];
function load(){
  load_ver();
  values();
}
function values(){
    ajax("/status.php",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('type',j.type);
            inner('isOk',(j.isOk=="true"?"OK":"Ошибка"));
            inner('totalBytes',j.totalBytes);
            inner('usedBytes',j.usedBytes);
        },data:"VALUES="+Math.random()
    })
}
function restart(submit, texts){
  if(confirm(texts)){ 
    ajax("/send_restart.php",{
      success:function(r){
        },data:"?SETS="+Math.random()+"&restart=1"
    });
  }
}
function resetConfig(submit,  texts){
  if(confirm(texts)){
    var num_select=enc(document.getElementById("num_memory"));
	if(num_select==0){return alert("Выберите область настройки!")}
    var t="?SETS="+Math.random()+"&resetConfig="+num_select;
    ajax("/send_memory_res.php",{
      success:function(r){},data:t
    });
  }
}
function saveConfig(submit){
  var num_select=enc(document.getElementById("num_memory"));
  var url=path[num_select]+"?download=true";
  if(num_select==0){
	  alert("Выберите область настройки!");
  }else if(num_select==12){
	  for(var i=1;i<12;i++){
		  url=path[i]+"?download=true";
		  url+=convert("");
		  window.location.href=url;
		  if(!window.confirm('Скачать следующий?')){return}
	  }
  }else{
	  url+=convert("");
	  window.location.href=url;
  }
}
var h;
var select_file=document.getElementById("loadF");
function httpPostProcessRequest(){
  if(h.readyState==4){
    if(h.status!=200)alert("ERROR["+h.status+"]: "+h.responseText);
  }
}
function loadConfig(){//----------------------------------загрузка файла
  if(document.getElementById("loadF").value.length===0){
	  alert("Выбирите файл");
	  return;
  }
  var num_select=enc(document.getElementById("num_memory"));
  if(num_select==0){
	  alert("Выберите область настройки!"); 
  }else if(num_select>11){
	  alert("Возможна загрузка только одной настройки");
  }else{
	  h=new XMLHttpRequest();
	  h.onreadystatechange=httpPostProcessRequest;
	  var url="/loadConfig.php";
	  var auth=document.location.search;
	  if(auth.match(/auth=/)) url+=auth;
	  var a=new FormData();
	  a.append("data",document.getElementById("loadF").files[0],path[num_select])
	  h.open("POST",url);
	  h.send(a);
	  alert("Изменения вступят в силу, после перезагрузки устройства.")
  }
}
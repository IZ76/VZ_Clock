var tr={},vals={},ver={},hours,min,sec;
function ajax(url,opts){
    var req;
    if(window.XMLHttpRequest)req=new XMLHttpRequest();
    else req=new ActiveXObject("Microsoft.XMLHTTP");
    req.onreadystatechange=function(){
        if(req.readyState==4){
            if(/^2/.test(req.status)&&opts.success)opts.success(req);
            else if(/^5/.test(req.status)&&opts.fail)opts.fail(req);
            else if(opts.other)opts.other(req)
        }
    };
    var auth=document.location.search;
    if(auth.match(/auth=/)){
      url+=auth;
    }
    req.open(opts.method||'POST',url,true);
    req.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
    req.send(opts.data)
}
function enc(e){return encodeURIComponent(e.value)}
function inner(h,e){document.getElementById(h).innerHTML=e}
function get_html(submit) {
    var auth=document.location.search;
    if(auth.match(/auth=/)) submit += auth;
    <!--window.open(submit);-->
    window.location.href=submit;
}
function convert(submit){
    var auth=document.location.search;
    if(auth.match(/auth=/)) submit+=auth.replace("\?", "&");
    return (submit);
}
function real_time(){
    sec=Number(sec)+1;
    if (sec>=60){min=Number(min)+1;sec=0;}
    if (min>=60){hours=Number(hours)+1;min=0;}
    if (hours>=24){hours=0};
    document.getElementById("time").innerHTML=hours+":"+(min<10?"0":"")+min+":"+(sec<10?"0":"")+sec;
}
function load_ver(){
    ajax("esp/time_ver.php",{
        success:function(r){
            ver=JSON.parse(r.responseText);
            inner('ver',ver.ver);
            hours=Number(ver.hour);
            min=Number(ver.min);
            sec=Number(ver.sec);
        },data:"SSLIST="+Math.random()
    });
    setInterval("real_time()",1000)
}
function out_report(){
    setInterval(function(){
        var snd=document.getElementById('mess').innerHTML;
        if(snd.indexOf('.')+1)snd+='.';
        var n=0;
        for(var i=0;i<snd.length;i++){
          if(snd[i].indexOf('.')+1) n++;
          }
        if(n>5)snd=snd.replace(/\./g,"")+'.';
        document.getElementById('mess').innerHTML=snd
    },500)
}
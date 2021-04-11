var  ant={},nets={};
function load(){
  load_ver();
  values();
  load_network();
}
function values(){
    ajax("wifis.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            var s="selected";
            document.getElementById('idap').value=vals.APSSID;
            document.getElementById('pasap').value=vals['APPASS'];
            document.getElementById('auth').value=vals.AUTH;
            document.getElementById('authOn').checked=vals.AUTHON;
            document.getElementById('hiddenNetworks').checked=vals.HIDDENNETWORKS;
        },data:"VALUES="+Math.random()
    })
}
function load_network(){
    antena();     // рисуем антенки
    saved_ssids(); // создаем таблицу с сохраненными сетями
    ajax("esp/connect_wifi.php",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('ssid',j.ssid);
            inner('ch',j.ch);
            inner('sig',j.sig);
            inner('mac',j.mac);
            inner('ip',j.ip);
            inner('gw',j.gw);
            inner('mask',j.mask);
        },data:"SSLIST="+Math.random()
    });
    setInterval("ssid_update()",6e3);
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
function antena(){
    var an={},a={},c;
    for(c=1;c<8;c++){
        var i=document.getElementById("cnv");
        var cv=document.createElement("canvas");
        if(c<6)cv.setAttribute("id","ant"+c);
        if(c==6)cv.setAttribute("id","arr");
        if(c==7)cv.setAttribute("id","sav");
        i.appendChild(cv)
    }
    for(c=1;c<6;c++){
        an[c]=document.getElementById("ant"+c);
        a[c]=an[c].getContext("2d");
        a[c].fillStyle="#095";
        for(var x0=0,x1=0,y0=16,y1=2;x0<17;x0+=4,x1++,y0-=2,y1+=2){
            if(c==x1)a[c].fillStyle="#ccc";
            a[c].fillRect(x0,y0,3,y1)
         }
         ant[c]=an[c].toDataURL()
     }
}
function saved_ssids(){
    ajax("jssids.jsone",{
        success:function(r){
            nets=JSON.parse(r.responseText);
            for(s=document.getElementById("sSsid");s.lastChild;)s.removeChild(s.lastChild);
            var table=document.createElement('table');
            table.setAttribute("class","net");
            for(var n in nets){
                var row=table.insertRow(-1);
                var cell=row.insertCell(-1);
                cell.setAttribute("class","sSsid");
                cell.setAttribute("title","открыть");
                cell.innerHTML=n;
                cell.setAttribute("onclick",'ssid_select("'+n+'","'+nets[n]+'")');
                var pic=row.insertCell(-1);
                var del=document.createElement('b');
                del.innerHTML='×';
                del.setAttribute("class","dels");
                del.setAttribute("title","удалить");
                del.setAttribute("onclick",'del("'+n+'")');
                del=pic.appendChild(del)
            }s.appendChild(table)
        },data:"D="+Math.random()
    })
}
function ssid_update(){ // запрашиваем список доступных сетей
    ajax("esp/ssid.php",{
        success:function(r){
            var n,sss=JSON.parse(r.responseText);
            for(s=document.getElementById("ssids");s.lastChild;) s.removeChild(s.lastChild);
            for(var ss in sss){
                s=document.getElementById("ssids");
                n=document.createElement("input");
                n.setAttribute("onclick",'ssid_select("'+ss+'")');
                n.setAttribute("value",ss);
                n.setAttribute("class",'scanSids');
                n.setAttribute("readonly","readonly");
                s.appendChild(n);
                s=document.getElementById("ssids");
                n=document.createElement("input");
                n.setAttribute("onclick",'ssid_select("'+ss+'")');
                for(var net in nets){
                    if(net==ss)n.setAttribute("value",'❤')
                }
                n.setAttribute("class",'scanSecurity');
                n.setAttribute("readonly","readonly");
                s.appendChild(n);
                s=document.getElementById("ssids");
                n=document.createElement("input");
                n.setAttribute("onclick",'ssid_select("'+ss+'")');
                n.setAttribute("class",'scanAntena');
                n.setAttribute("readonly","readonly");
                if(sss[ss]>102)n.setAttribute("style","background-image:url("+ant[1]+")");
                if(sss[ss]>87&&sss[ss]<103)n.setAttribute("style","background-image:url("+ant[2]+")");
                if(sss[ss]>72&&sss[ss]<88)n.setAttribute("style","background-image:url("+ant[3]+")");
                if(sss[ss]>57&&sss[ss]<73)n.setAttribute("style","background-image:url("+ant[4]+")");
                if(sss[ss]<=57)n.setAttribute("style","background-image:url("+ant[5]+")");
                s.appendChild(n)
            }
        },data:"SSLIST="+Math.random()
    })
}
function togle(){
    if(window.getComputedStyle(document.getElementById('ssids')).display=='none'){
        document.getElementById('ssids').style.display='block';
        document.getElementById('toggle').value='▲'
    }else{
        document.getElementById('ssids').style.display='none';
        document.getElementById('toggle').value='▼'
    }
}
function sh_pass(e){
    if(e.checked)var t="text";
    else t="password";
    document.getElementById('pasw').setAttribute("type",t)
}
function sh_passap(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('pasap').setAttribute("type",t)
}
function sh_auth(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('auth').setAttribute("type",t)
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    vals.APSSID=enc(e.AP_SSID);
    vals.APPASS=enc(e.AP_PASS);
    vals.AUTH=enc(e.AUTH);
    document.getElementById("authOn").checked?vals.AUTHON=1:vals.AUTHON=0;
    document.getElementById("hiddenNetworks").checked?vals.HIDDENNETWORKS=1:vals.HIDDENNETWORKS=0;
    t+="&JS="+JSON.stringify(vals);
    t+="&JSSIDS="+JSON.stringify(nets);
    var num=0,ssids=[],save={};
    for(var net in nets){
        num++;
        ssids[ssids.length]=net;
        ssids[ssids.length]=nets[net]
    }
    save.num=num;
    save.nets=ssids;
    t+="&SSIDS="+JSON.stringify(save);
    ajax("esp/save.php",{
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
function ssid_select(e,p){
    if(p==undefined)p="";
    document.getElementById('idss').setAttribute("value",e);
    document.getElementById('pasw').setAttribute("value",p);
    document.getElementById('ssids').style.display='none';document.getElementById('toggle').value='▼'
}
function del(d){
    if(confirm(tr.n16+' "'+d+'"?')){
        delete nets[d];
        for(s=document.getElementById("sSsid");s.lastChild;)s.removeChild(s.lastChild);
        var table=document.createElement('table');
        table.setAttribute("class","net");
        for(var n in nets){
            var row=table.insertRow(-1);
            var cell=row.insertCell(-1);
            cell.setAttribute("class","sSsid");
            cell.innerHTML=n;
            cell.setAttribute("onclick",'ssid_select("'+n+'","'+nets[n]+'")');
            var pic=row.insertCell(-1);
            var del=document.createElement('b');
            del.innerHTML='×';del.setAttribute("class","dels");
            del.setAttribute("onclick",'del("'+n+'")');
            del=pic.appendChild(del)
        }s.appendChild(table)
    }
}
function add_net(){
    var ssid=document.getElementById('idss').value;
    if(ssid!="")nets[ssid]=document.getElementById('pasw').value;
    for(s=document.getElementById("sSsid");s.lastChild;)s.removeChild(s.lastChild);
    var table=document.createElement('table');
    table.setAttribute("class","net");
    for(var n in nets){
        var row=table.insertRow(-1);
        var cell=row.insertCell(-1);
        cell.setAttribute("class","sSsid");
        cell.innerHTML=n;
        cell.setAttribute("onclick",'ssid_select("'+n+'","'+nets[n]+'")');
        var pic=row.insertCell(-1);
        var del=document.createElement('b');
        del.innerHTML='×';
        del.setAttribute("class","dels");
        del.setAttribute("onclick",'del("'+n+'")');
        del=pic.appendChild(del)
    }s.appendChild(table)
}
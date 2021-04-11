function load(){
  load_ver();
  out_report();
  values();
  val2();
}
function values(){
    ajax("weaths.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('weathOn').checked=vals.weathOn;
            document.getElementById('weatherHost').value=vals.weatherHost;
            document.getElementById('weatherKey0').value=vals.weatherKey0;
            document.getElementById('weatherKey1').value=vals.weatherKey1;
            document.getElementById('cityID0').value=vals.cityID0;
            document.getElementById('cityID1').value=vals.cityID1;
            document.getElementById('personalCityName').value=vals.personalCityName;
            document.getElementById('displayCityName').checked=vals.displayCityName;
            document.getElementById('displayForecastNow').checked=vals.displayForecastNow;
            document.getElementById('displayForecastToday').checked=vals.displayForecastToday;
            document.getElementById('displayForecastTomorrow').checked=vals.displayForecastTomorrow;
            document.getElementById('displayForecastWind').value=vals.displayForecastWind;            
            document.getElementById('animNotWeather').checked=vals.animNotWeather;
            document.getElementById('timeStartViewWeather').value=vals.timeStartViewWeather;
            document.getElementById('timeEndViewWeather').value=vals.timeEndViewWeather;
        },data:"VALUES="+Math.random()
    })
}
function val2(){
    ajax("esp/weaths_var.json",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('location_name',j.location_name);
            inner('location_region',j.location_region);
            inner('location_country',j.location_country);
            inner('location_localtime',j.location_localtime);
            inner('location_sunrise',j.location_sunrise);
            inner('location_sunset',j.location_sunset);
            inner('location_temp',j.location_temp);
            inner('location_app_temp',j.location_app_temp);
            inner('location_rh',j.location_rh);
            inner('location_pres',j.location_pres);
            inner('location_wind_spd',j.location_wind_spd);
            inner('location_wind_cdir_full',j.location_wind_cdir_full);
            inner('location_clouds',j.location_clouds);
            inner('location_vis',j.location_vis);
            inner('location_uv',j.location_uv);
            inner('location_wds',j.location_wds);
        },data:"VALUES="+Math.random()
    })
}
function save(event){
    document.getElementById("mess").style.display='block';
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    document.getElementById("weathOn").checked?vals.weathOn=1:vals.weathOn=0;
    vals.weatherHost=enc(e.weatherHost);
    vals.weatherKey0=enc(e.weatherKey0);
    vals.weatherKey1=enc(e.weatherKey1);
    vals.cityID0=enc(e.cityID0);
    vals.cityID1=enc(e.cityID1);
    vals.personalCityName=enc(e.personalCityName);
    document.getElementById("displayCityName").checked?vals.displayCityName=1:vals.displayCityName=0;
    document.getElementById("displayForecastNow").checked?vals.displayForecastNow=1:vals.displayForecastNow=0;
    document.getElementById("displayForecastToday").checked?vals.displayForecastToday=1:vals.displayForecastToday=0;
    document.getElementById("displayForecastTomorrow").checked?vals.displayForecastTomorrow=1:vals.displayForecastTomorrow=0;
    vals.displayForecastWind=enc(e.displayForecastWind);
    document.getElementById("animNotWeather").checked?vals.animNotWeather=1:vals.animNotWeather=0;
    vals.timeStartViewWeather=enc(e.timeStartViewWeather);
    vals.timeEndViewWeather=enc(e.timeEndViewWeather);
    t+="&JS="+JSON.stringify(vals);
    ajax("esp/weaths.php",{
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
function sh_weathBitApi(e){
    if(e.checked)var t="text";
    else t="password";
    document.getElementById('weatherKey0').setAttribute("type",t)
}
function sh_weathOpenApi(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('weatherKey1').setAttribute("type",t)
}
function sh_nmonApi(e){
    if(e.checked) var t="text";
    else t="password";
    document.getElementById('api_key').setAttribute("type",t)
}
function send_onWeath(){
  var t;
  document.getElementById("weathOn").checked?t=1:t=0;
  t="?SETS="+Math.random()+"&SW="+t+"&SN=";
  document.getElementById("nmonOn").checked?t+=1:t+=0;
  ajax("/send_onWeath.php",{
    success:function(r){
      },data:t
  });
}
function load(){
  load_ver();
  values();
}
function values(){
    ajax("wsled.jsone",{
        success:function(r){
            vals=JSON.parse(r.responseText);
            document.getElementById('mode_WS').value=vals.mode_WS;
        },data:"VALUES="+Math.random()
    })
}
function save(event){
    var e=document.getElementById("form");
    event.preventDefault();
    var t="?SETS="+Math.random();
    vals.mode_WS=enc(e.mode_WS);
    t+="&mode_WS="+vals.mode_WS;
    ajax("wsled.php",{
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

function load(){
  load_ver();
  setInterval("values()",500);
}
function values(){
    ajax("esp/ir_val.json",{
        success:function(r){
            var j=JSON.parse(r.responseText);
            inner('ir_key0',j.ir_key0);
            inner('ir_key1',j.ir_key1);
            inner('ir_key2',j.ir_key2);
            inner('ir_key3',j.ir_key3);
			inner('ir_key4',j.ir_key4);
        },data:"VALUES="+Math.random()
    })
}
function start_scan(submit){
    ajax("/send_start_scan.php",{
        success:function(r){},data:"?SETS="+Math.random()+"&start="+enc(num_key)
    });
}
function stop_scan(submit){
    ajax("/send_stop_scan.php",{
        success:function(r){},data:"?SETS="+Math.random()+"&stop=1"
    });
}

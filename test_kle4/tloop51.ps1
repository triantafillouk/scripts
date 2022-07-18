# test loop with fori

cls

$p=0
$l=0
 
get-date -Format u
#show_time("Start fori test loop 1",0)

for($i=1;$i -le 100;$i++){ 
	for($j=1;$j -le 800000;$j++) { 
		$l = $i*$j+32.1; 
		$p = $p+$l
	} 
	$d=get-date -Format u
	Write-host "loop $i p=$p"
#	show_time("loop:"+i+" p="+p,1);
}
Write-host "i < $i and j< $j sets p=$p"
$t2=get-date -Format u
#print(_time)	: "Wed Feb 24 19:48:43 2016"
#tdif=show_time("End test",2)
#print("Test concluded in "+tdif+" seconds")
Write-host $t1
Write-host $t2

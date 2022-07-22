// test loop with fori

var p=0;
var l=0; 
var d1 = new Date();
var t1 = d1.getTime();
console.log(t1);

// show_time("Start fori test loop 1",0)

var i;
var j;
var l;
var p=0;

for(i=1;i<101;i++){ 
	for(j=1;j<800001;j++) { 
		l = i*j+32.1; 
		p = p+l
	}; 
//	show_time("loop:"+i+" p="+p,1);
	console.log(`loop ${i} p = ${p}`);
}

// _time	: "Wed Feb 24 19:48:43 2016"
// print(_time)	: "Wed Feb 24 19:48:43 2016"
//tdif=show_time("End test",2)
//print(_version+": p="+p+" Test concluded in "+tdif+" seconds")
console.log(` Test concluded p = ${p}`);

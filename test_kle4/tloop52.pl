#!/usr/bin/perl

use strict;

sub fact {
 my $num=shift;
 if($num<1) {
	return 1;
 } else {
	my $f=$num*fact($num-1);
	return $f;
 }
}

 print fact(6),"\n";

 my $p=0.0;
 my $l=0;

 my $i;
 my $j;
 
 for($i=1;$i<11;$i++) {
	for($j=1;$j<800001;$j++) {
		$l = $i*$j+32;
		$p = $p+$l+fact(2);
	}
	print "loop $i: p = $p\n";
 };
 print ("result is : $p\n");

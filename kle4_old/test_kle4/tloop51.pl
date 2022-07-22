#!/usr/bin/perl

use strict;

 my $p=0.0;
 my $l=0;

 my $i;
 my $j;
 print "Start for test loop\n";

 for($i=1;$i<101;$i++) {
	for($j=1;$j<800001;$j++) {
		$l = $i*$j+32.1;
		$p = $p+$l;
	}
	print "loop $i: p = $p\n";
 };
 print ("result is : $p\n");

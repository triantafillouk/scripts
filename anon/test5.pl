#!/usr/bin/perl

# We customarily omit quotes when the keys are identifiers. 
use strict;


sub fun1($) {
	my ($a) = @_;
	print "--1-$a-\n";
}

sub fun2($) {
	my ($a) = @_;
	print "--2-$a-\n";
}

my @farray = (
	\&fun1,
	\&fun2
);

my %field_func = (
	ts => \&fun1,
	ts2 => \&fun2
);

$farray[0]->("test");
$farray[1]->("demo");

$field_func{"ts"}->("test3");
$field_func{"ts2"}->("test3");

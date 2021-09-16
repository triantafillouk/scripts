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

my @num_digits = ('0','1','2','3','4','5','6','7','8','9');

my @args1 = ( \@num_digits,".@/",0,"",0 );

sub anon_generic1($$) {
 my $v = @_[0];
 my $ar = @_[1];
 my $t = @$ar[1];
 print "start $v = $t\n";
}

my $aref = \@args1;
my $ae = @$aref[2];
print "-- $ae\n";
anon_generic1("test",\@args1);


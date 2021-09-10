#!/usr/bin/perl
# anonymize logs for uat
# version 1.2.3

package Anonymizer;
use strict;
use utf8;
use integer;
#use 5.012;
# use Digest::MD4 qw(md4 md4_hex md4_base64);
binmode(STDOUT, "encoding(UTF-8)");

my $log_dir = "logs";
my $anon_log_dir = "anonymous_logs2";
my $category;# 
my $subtype;
my $subtype_dir;
my $archive_dir;
my $archive;
my $anon_archive_dir;

my %field_names = (
ts						=> "none",	# 20200103123507
src_filename			=> "none",	# R_ALEXADRAS01B.0476.2001031251
src_filedate			=> "none",	# 20191231005959
src_filesize			=> "none",	# 5839
duration				=> "none",	# 0001:02:16, 0000:33:45 or 33 (telecard)
calling					=> "phone",	# 2661047456, 2810242384
called					=> "phone",	# 0017183198803, 00972505228370
servicenumber			=> "none",	# 8071122
chronocard_id			=> "idnum",	# 0000000001673709
balanceafter			=> "none",	# 000002915048
balanceconsumed			=> "none",	# 000001391230
expirationoffset		=> "none",	# 00000183
recseparator			=> "none",	# 
parsed					=> "none",	# 8,9 or 2 or 11, 15 ...
service					=> "none",	# 0
prefix					=> "none",	# 16032
number					=> "phone",	# 00171########, 69########, 13888
logfile_line			=> "none",	# 1234
mailserver_name			=> "none",	# pop03, rigel
mailserver_type			=> "none",	# dovecot, amavis, postfix-smtpd ..
queue					=> "none",	# F40D81D0008C (hex number)
session					=> "none",	# d60EqHemurbDqgAq
message_id				=> "string",	# 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
resent_message_id		=> "string",	# 
sender					=> "email",	# notification@facebookmail.com
receiver				=> "email",	# vm1977@otenet.gr
user					=> "email",	# empty, xxxxx@xxx.com
status					=> "none",	# blank | sent | removed | bounced |defered |expired
domain_not_found		=> "none",	# Recipient address rejected | sender address rejected
helo					=> "domain",	# ip or domain name or blank
ctladdr					=> "email",	# username | email
orig_to					=> "email",	# 
sasl_sender				=> "email",	#
remote_ip				=> "ipv4",	#
content_ifnot_queue		=> "string", # lmtp(xxxx@otenet.gr) | (03908-22) Passed {CLEAN|BADHEADER} (it might have commas!)
subscriber_type			=> "none",	# 0
imei					=> "idnum",	# 353420100443370
imsi					=> "idnum",	# 208201402275281
calling_number			=> "phone",	# 0033663139639
ipaddress				=> "ipv4",	# 
called_number			=> "phone",	# 00302120001535
hc_network				=> "none",	# 274
ext_carrier				=> "none",	# 30971388006
serving_vplmn			=> "none",	# 0A180DE7
request_time			=> "none",	# 
start_time				=> "none",	# 20200103123507 (year,month,day,hour,min,sec)
end_time				=> "none",	# 20200103124052 (year,month,day,hour,min,sec)
call_duration			=> "none",	# 345 (seconds)
time_shift				=> "none",	# 10800
service_el_tp			=> "none",	# 3G
service_el_id			=> "none",	# ALEXADRAS01B
first_cell				=> "none",	# 202010300304553
dest_zone				=> "none",	# 597
call_flag				=> "none",	# O
service_qual			=> "none",	# NAP
record_id				=> "none",	# 75571013
session_scn				=> "none",	# 209734471
service_scn				=> "none",	# 04760000
service_id				=> "none",	# 11
service_mode			=> "none",	# Foreign_Relay
service_class			=> "none",	# 0 or wireless.twilio.com
service_type			=> "none",	# 0
service_prov_id			=> "none",	# Foreign_Relay
content_size			=> "none",	# 131233
content_size_upl		=> "none",	# 
content_size_dnl		=> "none",	#
service_status			=> "none",	# S
cdr_type				=> "none",	# 1,9
termination_ind			=> "none",	# C | 17
charge					=> "none",	# 117474
last_cell				=> "none",	# 202010300304553
master_msisdn			=> "none",	# 
incomingswitchid		=> "none",	# TKO,82
outgoingswitchid		=> "none",	# 82
recordcallid			=> "none",	# 011270006737657
linkfield				=> "none",	# 01
a_number				=> "phone",	# 69XXXXXXXX
b_number				=> "phone",	# 13888, 69XXXXXXXX
incomingtrunk			=> "none",	# COTKO1I
outgoingtrunk			=> "none",	# 1104_COSMOTE_ATH
incomingproduct			=> "none",	# HLP, IPTL
outgoingproduct			=> "none",	# IPTL
datavolume				=> "none",	# 
dataunit				=> "none",	# 
usersummarisation		=> "string",# 
udcsreserved			=> "none",	# 
product					=> "none",	# 00001 149-Αμεση συνδιάλεξη
username				=> "email",	# isgo56@otenet.gr
acct_status_type		=> "none",	# Interim-Update
acct_session_time		=> "none",	# 0
acct_unique_session_id	=> "idhex",	# 1cdf65221e6c8b1b
framed_ip_address		=> "ipv4",	# 100.68.219.0
nas_port_type			=> "none",	# Wireless-XGP
nas_port_id				=> "none",	# #KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35#
calling_station_id		=> "anum",	# 0 0/0/36.3223 # KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35
nas_ip_address			=> "ipv4",	# 79.128.219.74
port_parsed				=> "none",	# IGNORED
port_detail				=> "none",	#
delegated_ipv6_prefix	=> "ipv6",	# 2a02:587:b483:8d00::/56
haap_lte_ipv4			=> "ipv4",	#
haap_dsl_ipv4			=> "ipv4",	#
ishybrid				=> "none",	# false
validity_date_start		=> "none",	# 20171110000000
validity_date_end		=> "none",	# 20190930235959
calling_address			=> "string",# Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
card_balance_contained	=> "none",	# 3.36
card_balance_consumed	=> "none",	# 0.28
card_number				=> "idnum",	# 0507974295
version					=> "none",	# 10
seqnum					=> "none",	# 100000955753943
networkid				=> "none",	# 03
eventclass				=> "none",	# 001
networkprod				=> "none",	# 00001
action					=> "none",	# 01
eventsourceid			=> "none",	# INAYKTH,IMS
ndr						=> "phone",	# 6948457396
aaddrplan				=> "none",	# 1
nde						=> "phone",	# 2351028148
baddrplan				=> "none",	# 1
newdest					=> "phone",	# 2351028148
newdaddrplan			=> "none",	# 1
chargedparty			=> "none",	# U
date					=> "none",	# 20200101
starttime				=> "none",	# 063154
duratunit				=> "none",	# 1
secondunit				=> "none",	# 7
amountofsus				=> "none",	# 000000000000000
acategory				=> "none",	# 00
priority				=> "none",	# 00
numsuppserv				=> "none",	# 0
suppserv1				=> "none",	# 000
suppserv2				=> "none",	# 000
suppserv3				=> "none",	# 7
suppserv4				=> "none",	# 5900
suppserv5				=> "none",	# 01
suppserv6				=> "none",	# 000
suppserv7				=> "none",	# 000
suppserv8				=> "none",	# 000
numgroups				=> "none",	# 00
origcellid				=> "none",	# 000464
callid					=> "idnum",	# 0753665600
callid_hex				=> "idhex",	# 0753665600
inowner					=> "none",	# 2351028148
misc					=> "none",	# 00000000
reserved				=> "string",# 
);

# this is used for sample data
#my %fieldnames_read;

# my @categories = ['chronocard','email','gprs','h_msc','ict','ict_csv','itu_csv','mms','oss','radius','tap_in','telecard','msc_sups','udcs','udcslike','v_msc','w_sms',];
my %categories_config = (
	"email"			=> ["<*>",	[ "ts","src_filename","src_filedate","src_filesize","logfile_line","mailserver_name","mailserver_type","queue","session","message_id","resent_message_id","sender","receiver","user","status","domain_not_found","helo","ctladdr","orig_to","sasl_sender","remote_ip","content_ifnot_queue"]],
	"gprs"			=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"chronocard"	=> [','  ,	[ "ts","src_filename","src_filedate","src_filesize","duration","calling","called","servicenumber","chronocard_id","balanceafter","balanceconsumed","expirationoffset","recseparator","parsed","service","prefix","number"]],
	"ict"			=> ["<*>",	[ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","usersummarisation","udcsreserved","parsed","service","prefix","number" ]],
	"ict_csv"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number" ]],
	"itu"			=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number" ]],
	"itu_csv"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number" ]],
	"msc_sups"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"oss"			=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","a_number","b_number","duration","product","parsed","service","prefix","number" ]],
	"orig_mms"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"orig_msc"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"orig_pbx"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"orig_sms"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"radius"		=> ["<*>",	[ "ts","src_filename","src_filedate","src_filesize","username","acct_status_type","acct_session_time","acct_unique_session_id","framed_ip_address","nas_port_type","nas_port_id","calling_station_id","nas_ip_address","port_parsed","port_detail","delegated_ipv6_prefix","haap_lte_ipv4","haap_dsl_ipv4","ishybrid"]],
	"telecard"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","validity_date_start","validity_date_end","calling","calling_address","called","duration","card_balance_contained","card_balance_consumed","card_number" ]],
	"term_mms"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"term_msc"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"term_sms"		=> ['|'  ,	[ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
	"udcs"			=> ["<*>",	[ "ts","src_filename","src_filedate","src_filesize","version","seqnum","networkid","eventclass","networkprod","action","eventsourceid","ndr","aaddrplan","nde","baddrplan","newdest","newdaddrplan","chargedparty","date","starttime","duratunit","duration","secondunit","amountofsus","acategory","priority","numsuppserv","suppserv1","suppserv2","suppserv3","suppserv4","suppserv5","suppserv6","suppserv7","suppserv8","numgroups","origcellid","callid","inowner","misc","reserved","parsed","service","prefix","number" ]],
	"udcslike"		=> ["<*>",	[ "ts","src_filename","src_filedate","src_filesize","version","seqnum","networkid","eventclass","networkprod","action","eventsourceid","ndr","aaddrplan","nde","baddrplan","newdest","newdaddrplan","chargedparty","date","starttime","duratunit","duration","secondunit","amountofsus","acategory","priority","numsuppserv","suppserv1","suppserv2","suppserv3","suppserv4","suppserv5","suppserv6","suppserv7","suppserv8","numgroups","origcellid","callid_hex","inowner","misc","reserved","parsed","service","prefix","number" ]]
);

my $default_salt=23;	## a random value!
my @num_digits = ('0','1','2','3','4','5','6','7','8','9');
my @rand_digit = ('5','2','0','9','6','3','1','8','4','5');
my @hex_digits = ('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
my @unum_digits = ('0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'Α','Β','Γ','Δ','Ε','Ζ','Η','Θ','Ι','Κ','Λ','Μ','Ν','Ξ','Ο','Π','Ρ','Σ','Τ','Υ','Φ','Χ','Ψ','Ω'
);
my @anum_digits = ('0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
);

# encrypt number
sub anon_idnum($)
{
 my $v = $_[0];

 my $cc=0;
 my $salt = $default_salt;
 my @b = split('',$v);
 my $len = scalar @b;
 my $key  = int(@b[$len-1])+17;	
 my $val=0;
 my $i=0;
 my $l;
	foreach $l (@b) {
		$salt += ord($l);
	};
	# print " > salt=$salt k0=$key\n";
	my $start=1;
	foreach $l (@b) {	
		if($l eq '0' and $start==1) {
			@b[$i] = $l;
			# print "skip $l ";
		} else {
		my $ol=ord($l);
		my $ool = $cc ^ $ol;
		# print " -: v=$val,ol=$ol,k=$key,cc=$cc \n";
		$start=0;
		$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
		# print "  : k2=$key ";
		$val = ( ($key % 177)-$cc) % 177 ;
		# print " , v=$val cc0=$cc ";
		while($val<0) { $val += 10;};
		$cc = $val;
		
		if (++$salt >= 20857) {
			$salt = 0;
		}
		# print " val=$val cc=$cc salt=$salt";
		$key = $key + $key + ( $cc ^ $ol) + $salt;
		# print " k2=$key\n";
		@b[$i] = @num_digits[$cc % 10];
		};
		$i++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}

# encrypt hex
sub anon_idhex($)
{
 my $v = $_[0];

 my $cc=0;
 my $salt = $default_salt;
 my @b = split('',$v);
 my $len = scalar @b;
 my $key  = ord(@b[$len-1])+17-48;	
 my $val=0;
 my $i=0;
 my $l;
 # randomize salt
	foreach $l (@b) {
		$salt += ord($l);
	};
	# print " > salt=$salt k0=$key\n";
	my $start=1;
	foreach $l (@b) {	
		if($l eq '0' and $start==1) {
			# @b[$i] = $l;
		} else {
		my $ol=ord($l);
		my $ool = $cc ^ $ol;
		# print " -: v=$val,ol=$ol,k=$key,cc=$cc \n";
		$start=0;
		$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
		# print "  : k2=$key ";
		$val = ( ($key % 177)-$cc) % 177 ;
		# print " , v=$val ";
		while($val<0) { $val += 16;};
		$cc = $val;
		
		if (++$salt >= 20857) {
			$salt = 0;
		}
		# print " val=$val cc=$cc salt=$salt";
		$key = $key + $key + ( $cc ^ $ol) + $salt;
		# print " k2=$key\n";
		@b[$i] = @hex_digits[$cc % 16];
		};
		$i++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}

# encypt alphanumeric string
sub anon_anum($)
{
 my $v = $_[0];

 my $cc=0;
 my $salt = $default_salt;
 my @b = split('',$v);
 my $len = scalar @b;
 my $key  = ord(@b[$len-1])+17-48;	
 my $val=0;
 my $i=0;
 my $l;
 # randomize salt
	foreach $l (@b) {
		$salt += ord($l);
	};
	# print " > salt=$salt k0=$key\n";
	foreach $l (@b) {	
		if($l eq '.' or $l eq '@' or $l eq '?' or $l eq ':') {
			@b[$i] = $l;
		} else {
		my $ol=ord($l);
		my $ool = $cc ^ $ol;
		# print " -: v=$val,ol=$ol,k=$key,cc=$cc \n";
		$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
		# print "  : k2=$key ";
		$val = ( ($key % 177)-$cc) % 177 ;
		# print " , v=$val ";
		while($val<0) { $val += 36;};
		$cc = $val;
		
		if (++$salt >= 20857) {
			$salt = 0;
		}
		# print " val=$val cc=$cc salt=$salt";
		$key = $key + $key + ( $cc ^ $ol) + $salt;
		# print " k2=$key\n";
		@b[$i] = @anum_digits[$cc % 36];
		};
		$i++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}

sub anon_email($)
{
 my $v = $_[0];

 my $cc=0;
 my $salt = $default_salt;
 my @b = split('',$v);
 my $len = scalar @b;
 my $key  = ord(@b[$len-1])+17-48;	
 my $val=0;
 my $i=0;
 my $l;
 my $restart=1;
 # randomize salt
	foreach $l (@b) {
		$salt += ord($l);
	};
	
	# print " > salt=$salt k0=$key\n";
	foreach $l (@b) {
		if($l eq '.' or $l eq '@') {
			@b[$i] = $l;
		} else {
		my $ol=ord($l);
		my $ool = $cc ^ $ol;
		# print " -: v=$val,ol=$ol,k=$key,cc=$cc \n";
		$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
		# print "  : k2=$key ";
		$val = ( ($key % 177)-$cc) % 177 ;
		# print " , v=$val ";
		while($val<0) { $val += 36;};
		$cc = $val;
		
		if (++$salt >= 20857) {
			$salt = 0;
		}
		# print " val=$val cc=$cc salt=$salt";
		$key = $key + $key + ( $cc ^ $ol) + $salt;
		# print " k2=$key\n";
		@b[$i] = @anum_digits[$cc % 36];
		};
		$i++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}


sub anon_string($)
{
 my $v = $_[0];

 my $cc=0;
 my $salt = $default_salt;
 my @b = split('',$v);
 # my @b = split('', "123abcΑΒΓ");
 my $len = scalar @b;
 my $key  = int(@b[$len-1])+17;	
 my $val=0;
 my $i=0;
 my $l;
	foreach $l (@b) {
		my $ol=ord($l);
		print "$l -> $ol\n";
	};
	my @h = unpack('h*',$v);
	print "b len = $len\n";
	for ($i=0;$i<$len;$i++) {
		$b = @h[$i];
		print "$i $b\n";
	};
	exit;
	foreach $l (@b) {
		$salt += ord($l);
	};
	my $l1 = $len-1;
	my $vl = int(@b[$l1]);
	# print " > salt=$salt k0=$key len-1=$l1 v=$vl [$v]\n";
	my $is_utf=0;
	# print " $v\n";


	foreach $l (@b) {	
		my $ol=ord($l);
		# print " $l -> $ol\n";
		if($l<'0' or ($l>'9' and $l<'A') or ($l>'Z' and $l<'a') or ($l>'z' and $l<128)) {
			@b[$i] = $l;
			# print "skip $l ";
		} else {
			my $ool = $cc ^ $ol;
			# print " -: v=$val,ol=$ol,k=$key,cc=$cc \n";
			$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
			# print "  : k2=$key ";
			$val = ( ($key % 177)-$cc) % 177 ;
			# print " , v=$val ";
			while($val<0) { $val += 10;};
			$cc = $val;
			
			if (++$salt >= 20857) {
				$salt = 0;
			}
			# print " val=$val cc=$cc salt=$salt";
			$key = $key + $key + ( $cc ^ $ol) + $salt;
			# print " k2=$key\n";
			@b[$i] = @num_digits[$cc % 10];
		};
		$i++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}

# $v -> input string
# $chars -> array with output chars
# $exept -> string with chars to not convert
# $start -> start convert from this position
# $ignore_begin -> string with chars that do not convert at the beginning of the line
# $in_chars -> if not found in @chars then return original string
sub anon_generic($$$$$$) {
 my ($v, $chars, $exept, $start, $ignore_begin, $in_chars) = @_;
 my $max = scalar @$chars;
 my @b = split('',$v);
 my $flen = scalar @b;
 # initialize salt
 my $salt = $default_salt;
 # initialize key
 my $key = ord(@b[$flen-1])+17-48;	
 my $val=0;
 my $cc=0;
 my $l;
 my $pos=0;
 my $begin=1;
	foreach $l (@b) {
		$salt += ord($l);
		if($in_chars==1) {
			if( !grep( /^$l$/, @$chars)) { 
				# print "$l not in chars!\n";
				return $v;
			};
		};
	};
#  return $v;

	foreach $l (@b) {
		my $ol = ord($l);
		
		if(index($exept,$l)<0 and $pos>=$start and !($begin==1 and index($ignore_begin,$l)>=0)) {
			$begin=0;
			$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
			$val = ( ($key % 177)-$cc) % 177 ;
			while($val<0) { $val += $max;};
			$cc = $val;
			
			if (++$salt >= 20857) {
				$salt = 0;
			}
			$key = $key + $key + ( $cc ^ $ol) + $salt;
			@b[$pos] = @$chars[$cc % $max];
		};
		$pos++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}

sub anon_ustring($)
{
 my $v = $_[0];

 my $cc=0;
 my $salt = $default_salt;
 my @b = split('',$v);
 my $len = scalar @b;
 my $key = ord(@b[$len-1])+17-48;	
 my $val=0;
 my $i=0;
 my $l;
	foreach $l (@b) {
		$salt += ord($l);
	};
	# print " > [$v] len=$len,salt=$salt,k0=$key\n";
	foreach $l (@b) {	
		my $ol=ord($l);
		#print " $l -> $ol\n";
		if($l eq '.' or $l eq '@' or $l eq '/'){
			@b[$i] = $l;
			# print "skip $l \n";
		} else {
			# print " -: v=$val,ol=$ol,k=$key,cc=$cc \n";
			$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
			# print "  : k2=$key ";
			$val = ( ($key % 177)-$cc) % 177 ;
			# print " , v=$val ";
			while($val<0) { $val += 60;};
			$cc = $val;
			
			if (++$salt >= 20857) {
				$salt = 0;
			}
			# print " val=$val cc=$cc salt=$salt";
			$key = $key + $key + ( $cc ^ $ol) + $salt;
			# print " k2=$key\n";
			@b[$i] = @unum_digits[$cc % 60];
			# $l = @unum_digits[$cc % 60];
			# my $ind = $cc % 60;
			# print "$i $l $ind\n";
		};
		$i++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}

sub anon_phone($)
{
 my $v = $_[0];

 my $cc=0;
 my $salt = $default_salt;
 my @b = split('',$v);
 my $len = scalar @b;
 my $key  = int(@b[$len-1])+17;	
 my $val=0;
 my $i=0;
 my $l;
 if($v eq "DUMMY") {
	# print " - DUMMY:\n";
 	return("DUMMY");
 };
	foreach $l (@b) {
		$salt += ord($l);
	};
	my $l1 = $len-1;
	my $vl = int(@b[$l1]);
	# print " > salt=$salt k0=$key len-1=$l1 v=$vl [$v]\n";
	my $start=1;
	# print " $v\n";
	foreach $l (@b) {	
		
		my $ol=ord($l);
		# print " $l -> $ol\n";
		if(ord($l) > 57 or ord($l)<48) { return $v }
		if(($l eq '0' or $l eq '6') and $start==1) {
			@b[$i] = $l;
			# print "skip $l ";
		} else {
		my $ool = $cc ^ $ol;
		# print " -: v=$val,ol=$ol,k=$key,cc=$cc \n";
		$start=0;
		$key = $ol+($key & 0x1FFFFFFF) ^ (($key >> 29) & 0x3);
		# print "  : k2=$key ";
		$val = ( ($key % 177)-$cc) % 177 ;
		# print " , v=$val ";
		while($val<0) { $val += 10;};
		$cc = $val;
		
		if (++$salt >= 20857) {
			$salt = 0;
		}
		# print " val=$val cc=$cc salt=$salt";
		$key = $key + $key + ( $cc ^ $ol) + $salt;
		# print " k2=$key\n";
		@b[$i] = @num_digits[$cc % 10];
		};
		$i++;
	};
	my $anon_str = join('',@b);
	# print "> $v -> $anon_str\n";
	return $anon_str;
}


sub anon_ipv4($)
{
	my $v = $_[0];
	return $v;
}

sub anon_ipv6($)
{
	my $v = $_[0];
	return $v;
}

# anonymize_field(value,field name)
sub anonymize_field_old ($$)
{
	my ($f, $field_name) = @_;
	my $af;
	my $type = $field_names{$field_name};

#	create field sample data
#	if(!exists($fieldnames_read{$field_name})) {
#		if($f ne "") { $fieldnames_read{$field_name}=$f;};
#	};

	if($f eq "") { return "";};
	if($type eq "none") { return $f;};
	if($type eq "string") { return anon_ustring($f);};
	if($type eq "phone") { return anon_phone($f);};
	if($type eq "anum") { return anon_anum($f);};
	if($type eq "email") { return anon_email($f);};
	if($type eq "idnum") { return anon_idnum($f);};
	if($type eq "ipv4") { return anon_ipv4($f);};
	if($type eq "idhex") { return anon_idhex($f);};
	if($type eq "ipv6") { return anon_ipv6($f);};
	return $f;
}

sub anonymize_field ($$)
{
	my ($f, $field_name) = @_;
	my $af;
	my $type = $field_names{$field_name};

#	create field sample data
#	if(!exists($fieldnames_read{$field_name})) {
#		if($f ne "") { $fieldnames_read{$field_name}=$f;};
#	};

	if($f eq "") { return "";};
	if($type eq "none") { return $f;};
	if($type eq "string") { return anon_generic($f,\@unum_digits,".@/",0,"",0);};
	if($type eq "phone") { return anon_generic($f,\@num_digits,"",0,"06",1);};
	if($type eq "anum") { return anon_anum($f);};
	if($type eq "email") { return anon_generic($f,\@anum_digits,"@.",0,"",0);};
	if($type eq "idnum") { return anon_generic($f,\@num_digits,"",0,"0",1);};
	if($type eq "ipv4") { return anon_ipv4($f);};
	# if($type eq "ipv4") { return anon_generic($f,\@num_digits,".",6,"",0);};
	if($type eq "idhex") { return anon_generic($f,\@hex_digits,"",0,"0",0);};
	if($type eq "ipv6") { return anon_ipv6($f);};
	# if($type eq "ipv6") { return anon_generic($f,\@hex_digits,".:/",6,"",0);};
	return $f;
}

# anonymize_file(source_dir,file name,destination dir,category, subtype)
sub anonymize_file($$$$$)
{
 my ($from, $file, $out_dir,$category,$subtype) = @_;

 my $fname = "$from/$file";
 my $out_file = "$out_dir/$file";
 my $type;

 $type = ($subtype eq "default") ? $category : $subtype;

 if(exists($categories_config{$type})) {
	my @data = @{$categories_config{$type}};
	my $sep = @data[0];
	my @fields = @{@data[01]};
	my $field_count = scalar @fields;

	# print " > $category, $type, $subtype, separator is [$sep] field_count = $field_count\n";
	my $i=1;
	my $atype;
	foreach my $f (@fields) {
		if(!exists($field_names{$f})) {
			$field_names{$f}={$f};
			print "	insert	field = $f\n";
		};
	};

	my $oldsize = -s $fname;

	# open my $in, "<:encoding(utf8)", $fname or die "$fname: $!";
	open(FIN,  '<:encoding(utf8)', $fname) or die "$fname: $!";
# 	FIN->autoflush(1);
	# open(FIN,  '<:raw:perlio:utf8', $fname) or die "$fname: $!";
	open(FOUT, '>:encoding(UTF-8)', $out_file) or die $!;
	# FOUT->autoflush(1);
	# open(FOUT, '>:raw:perlio:utf8', $out_file) or die $!;
	# set the separator for the split command
	my $sep1;
	#	print " Separator is [$sep] ------------------------------------\n";

	if($sep eq "<*>") {
		$sep1 = '\<\*\>';
		# print "Sep is <*>\n";
	} elsif ($sep eq ',') {
		$sep1 = $sep;
		# print "separator is comma!\n";
	} else {
		$sep1 = '\|';
		# print "sep = [$sep1] [|]\n";
	};

	while (my $line = <FIN>) {
		my @line;
		my $line_fields;
		chomp $line;
		my $count = () = $line =~ /\Q$sep\E/gms;
		$line_fields = $count+1;

		@line = split($sep1,$line,100);

		if($line_fields ne $field_count) { print " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! @line\nvalidate number $line_fields <> $field_count  \n";};
		my $i=0;
		# show the data!
		my @line_out = [];
		foreach my $f (@fields) {
			# anonymize $f
			# print "$f:\n";
			my $anon_f = anonymize_field($line[$i],$f);	# field value,field name

			@line_out[$i] = $anon_f;
			$i++;
		};
		my $line_out = join ($sep,@line_out);
		# write the line to out file
		print FOUT "$line_out\n";
		# print "-- $line_out --\n";
	};
	close FIN;
	close FOUT;

	my $new_size = -s $out_file;
	print " > $file oldsize = $oldsize new size=$new_size\n";
 } else {
 	print "		skipping anonymize $type $subtype\n";
 };
#	print "anonymize file [$from/$file] to dir [$dir] type [$type] subtype [$subtype]\n";
#	exit;
}

sub anon_done($$)
{
 my ($source_dir, $fname) = @_;
#  my $source_dir = $_[0];
#  my $fname = $_[1];
 
 my @fa = split('\.',$fname,100);
 my $cat = @fa[1];
 my $type = @fa[2];
 # print "anon_done: cat=$cat, type=$type name=$fname\n";
 my $anon_archive_dir = "$anon_log_dir/$cat/queue/$type/archive";
 `mkdir -p $anon_archive_dir`;
 # print "  : -> $anon_archive_dir  $fname\n";
 # print "  : source=$source_dir -> outdor=$anon_archive_dir -----\n";
 
 anonymize_file($source_dir,$fname,$anon_archive_dir,$cat,$type);
}

sub get_dir($)
{
 my $dir = $_[0];
	opendir(DIR, $dir) or die "Could't open dir $dir";
	my @files = grep { !/^\.$|^\.\.$/ } readdir(DIR);
	closedir (DIR);
	return @files;
}

## ------ main -----------------------------------------------
# my $in="This is ’ελληνικά γράμματα’";
# my $out=anon_ustring($in);
# print "in [ $in ] -> [ $out ]\n";
# exit;
my $test=0;
if($test==1) {
	my $in;
	my $out;
	$in="This is ’ελληνικά γράμματα’";
	$out=anon_generic($in,\@unum_digits,".@/",0,"",0);
	print "anon_ustring    : in [ $in ] -> [ $out ]\n";
	$out=anon_ustring($in);
	print "anon_ustring new: in [ $in ] -> [ $out ]\n";
	$in="00abcde134";
	$out=anon_idhex($in);
	print "anon hex	: in [$in] -> [$out]\n";
	$out=anon_generic($in,\@hex_digits,"",0,"0",0);
	print "anon hex	: in [$in] -> [$out]\n";
	$in="dummy";
	$out=anon_generic($in,\@num_digits,"",0,"06",1);
	print "anon phone: in [$in] -> [$out]\n";
	$in="0006544";
	$out=anon_generic($in,\@num_digits,"",0,"06",1);
	print "anon phone: in [$in] -> [$out]\n";
	$out=anon_phone($in);
	print "anon phone: in [$in] -> [$out]\n";
	$in = "triantafillou kostas\@test.gr";
	$out=anon_generic($in,\@anum_digits,".@?:",0,"",0);
	print "anon_anum new : in [ $in ] -> [ $out ]\n";
	$out=anon_anum($in);
	print "anon_anum : in [ $in ] -> [ $out ]\n";

	exit;
};

# initialize anonymized directory
print "Initialize anonymized directory $anon_log_dir\n";
`rm -rf $anon_log_dir`;
# for each log category
my @categories = get_dir($log_dir);
foreach $category (@categories)
{
	if(1) {
	# print "# $category\n";
	# for each subcategory dir
	$subtype_dir = "$log_dir/$category/queue";
	my @subtypes = get_dir($subtype_dir);
	foreach $subtype (@subtypes)
	{
		$archive_dir = "$subtype_dir/$subtype/archive";
		print "# $archive_dir -> $anon_log_dir\n";
		# print "# $category  $subtype\n";
		# print "			archive_dir is [$archive_dir]\n";
		my @archives = get_dir ($archive_dir);
		# create anon corresponding dir
		$anon_archive_dir = "$anon_log_dir/$category/queue/$subtype/archive";
		# print "		create dir [$anon_archive_dir]\n";
		`mkdir -p $anon_archive_dir`;
		foreach $archive (@archives)
		{
			# print "      |-- $archive\n";
			# readfile and anonymize
			anonymize_file($archive_dir,$archive,$anon_archive_dir,$category,$subtype);
		};
	};
	};
# *********************** insert done ones **************
	my $category_dir = "$log_dir/$category";
	my @category_dirs = get_dir($category_dir);
	foreach my $done_dir (@category_dirs) {
		my $fdname = "$category_dir/$done_dir";
		if ( -d $fdname) {
			if ($done_dir =~ "done*") {
				print "done dir is $done_dir\n";
				my @done0_contents = get_dir($fdname);
				foreach my $done0_file (@done0_contents) {
					my $done0_name = "$fdname/$done0_file";
					if(-d $done0_name) {
						print "	  done subdir is $done0_name\n";
						my @done1_contents = get_dir($done0_name);
						foreach my $done1_file (@done1_contents) {
							anon_done($done0_name,$done1_file);
						};
					} else {
						# print " file$done0_name\n";
						anon_done($fdname,$done0_file);
					};
				};
			};
		};	
	};
};



#{
#	my $val = $fieldnames_read{$fn};
#	print "-- $fn -> $val\n";
#}

print "\nDONE!!\n";


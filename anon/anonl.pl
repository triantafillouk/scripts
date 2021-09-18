#!/usr/bin/perl
# anonymize logs for uat

package Anonymizer;
use strict;
use utf8;
use integer;
#use 5.012;
# use Digest::MD4 qw(md4 md4_hex md4_base64);
binmode(STDOUT, "encoding(UTF-8)");

my $app_version = "1.3.1";
my $log_dir = "logs";
my $anon_log_dir = "anonymous_logs2";
my $category;
my $subtype;
my $subtype_dir;
my $archive_dir;
my $archive;
my $anon_archive_dir;

my $default_salt=23;	## a random value!
my @num_digits = ('0','1','2','3','4','5','6','7','8','9');
my @rand_digits = ('5','8','6','4','7','9','3','2','0','1');
my @hex_digits = ('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
my @unum_digits = ('0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'Α','Β','Γ','Δ','Ε','Ζ','Η','Θ','Ι','Κ','Λ','Μ','Ν','Ξ','Ο','Π','Ρ','Σ','Τ','Υ','Φ','Χ','Ψ','Ω'
);
my @anum_digits = ('0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
);

sub anon_none($)
{
 my $v = $_[0];
 return $v;
}

sub anon_string1 ($)
{
 # my @args1 = ( \@unum_digits,".@/",0,"",0 );
 # return anon_generic1($_[0],\@args1);
	return anon_generic($_[0],\@unum_digits,".@/",0,"",0);
}

sub anon_phone1 ($)
{
	return rand_digits($_[0],4);
	# return anon_generic($_[0],\@num_digits,"",0,"06",1);
}

sub anon_idnum1($)
{
	return anon_generic($_[0],\@num_digits,"",0,"0",1);
}

sub anon_email1($)
{
	return anon_generic($_[0],\@anum_digits,"@.",0,"",0);
}

sub anon_domain($)
{
 my $v = $_[0];
 return $v;
}

sub anon_idhex1($)
{
	return anon_generic($_[0],\@hex_digits,"",0,"0",0);
}

sub anon_anum1($)
{
	return anon_anum($_[0]);
}


my %field_funcs = (
ts						=> \&anon_none,	# 20200103123507
src_filename			=> \&anon_none,	# R_ALEXADRAS01B.0476.2001031251
src_filedate			=> \&anon_none,	# 20191231005959
src_filesize			=> \&anon_none,	# 5839
duration				=> \&anon_none,	# 0001:02:16, 0000:33:45 or 33 (telecard)
calling					=> \&anon_phone,	# 2661047456, 2810242384
called					=> \&anon_phone,	# 0017183198803, 00972505228370
servicenumber			=> \&anon_none,	# 8071122
chronocard_id			=> \&anon_idnum,	# 0000000001673709
balanceafter			=> \&anon_none,	# 000002915048
balanceconsumed			=> \&anon_none,	# 000001391230
expirationoffset		=> \&anon_none,	# 00000183
recseparator			=> \&anon_none,	# 
parsed					=> \&anon_none,	# 8,9 or 2 or 11, 15 ...
service					=> \&anon_none,	# 0
prefix					=> \&anon_none,	# 16032
number					=> \&anon_phone,	# 00171########, 69########, 13888
logfile_line			=> \&anon_none,	# 1234
mailserver_name			=> \&anon_none,	# pop03, rigel
mailserver_type			=> \&anon_none,	# dovecot, amavis, postfix-smtpd ..
queue					=> \&anon_none,	# F40D81D0008C (hex number)
session					=> \&anon_none,	# d60EqHemurbDqgAq
message_id				=> \&anon_ustring,	# 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
resent_message_id		=> \&anon_ustring,	# 
sender					=> \&anon_email,	# notification@facebookmail.com
receiver				=> \&anon_email,	# vm1977@otenet.gr
user					=> \&anon_email,	# empty, xxxxx@xxx.com
status					=> \&anon_none,	# blank | sent | removed | bounced |defered |expired
domain_not_found		=> \&anon_none,	# Recipient address rejected | sender address rejected
helo					=> \&anon_domain,	# ip or domain name or blank
ctladdr					=> \&anon_email,	# username | email
orig_to					=> \&anon_email,	# 
sasl_sender				=> \&anon_email,	#
remote_ip				=> \&anon_ipv4,	#
content_ifnot_queue		=> \&anon_ustring, # lmtp(xxxx@otenet.gr) | (03908-22) Passed {CLEAN|BADHEADER} (it might have commas!)
subscriber_type			=> \&anon_none,	# 0
imei					=> \&anon_idnum,	# 353420100443370
imsi					=> \&anon_idnum,	# 208201402275281
calling_number			=> \&anon_phone,	# 0033663139639
ipaddress				=> \&anon_ipv4,	# 
called_number			=> \&anon_phone,	# 00302120001535
hc_network				=> \&anon_none,	# 274
ext_carrier				=> \&anon_none,	# 30971388006
serving_vplmn			=> \&anon_none,	# 0A180DE7
request_time			=> \&anon_none,	# 
start_time				=> \&anon_none,	# 20200103123507 (year,month,day,hour,min,sec)
end_time				=> \&anon_none,	# 20200103124052 (year,month,day,hour,min,sec)
call_duration			=> \&anon_none,	# 345 (seconds)
time_shift				=> \&anon_none,	# 10800
service_el_tp			=> \&anon_none,	# 3G
service_el_id			=> \&anon_none,	# ALEXADRAS01B
first_cell				=> \&anon_none,	# 202010300304553
dest_zone				=> \&anon_none,	# 597
call_flag				=> \&anon_none,	# O
service_qual			=> \&anon_none,	# NAP
record_id				=> \&anon_none,	# 75571013
session_scn				=> \&anon_none,	# 209734471
service_scn				=> \&anon_none,	# 04760000
service_id				=> \&anon_none,	# 11
service_mode			=> \&anon_none,	# Foreign_Relay
service_class			=> \&anon_none,	# 0 or wireless.twilio.com
service_type			=> \&anon_none,	# 0
service_prov_id			=> \&anon_none,	# Foreign_Relay
content_size			=> \&anon_none,	# 131233
content_size_upl		=> \&anon_none,	# 
content_size_dnl		=> \&anon_none,	#
service_status			=> \&anon_none,	# S
cdr_type				=> \&anon_none,	# 1,9
termination_ind			=> \&anon_none,	# C | 17
charge					=> \&anon_none,	# 117474
last_cell				=> \&anon_none,	# 202010300304553
master_msisdn			=> \&anon_none,	# 
incomingswitchid		=> \&anon_none,	# TKO,82
outgoingswitchid		=> \&anon_none,	# 82
recordcallid			=> \&anon_none,	# 011270006737657
linkfield				=> \&anon_none,	# 01
a_number				=> \&anon_phone,	# 69XXXXXXXX
b_number				=> \&anon_phone,	# 13888, 69XXXXXXXX
incomingtrunk			=> \&anon_none,	# COTKO1I
outgoingtrunk			=> \&anon_none,	# 1104_COSMOTE_ATH
incomingproduct			=> \&anon_none,	# HLP, IPTL
outgoingproduct			=> \&anon_none,	# IPTL
datavolume				=> \&anon_none,	# 
dataunit				=> \&anon_none,	# 
usersummarisation		=> \&anon_anum,# 
udcsreserved			=> \&anon_ustring,	# 
product					=> \&anon_none,	# 00001 149-Αμεση συνδιάλεξη
username				=> \&anon_email,	# isgo56@otenet.gr
acct_status_type		=> \&anon_none,	# Interim-Update
acct_session_time		=> \&anon_none,	# 0
acct_unique_session_id	=> \&anon_idhex,	# 1cdf65221e6c8b1b
framed_ip_address		=> \&anon_ipv4,	# 100.68.219.0
nas_port_type			=> \&anon_none,	# Wireless-XGP
nas_port_id				=> \&anon_none,	# #KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35#
calling_station_id		=> \&anon_anum,	# 0 0/0/36.3223 # KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35
nas_ip_address			=> \&anon_ipv4,	# 79.128.219.74
port_parsed				=> \&anon_none,	# IGNORED
port_detail				=> \&anon_none,	#
delegated_ipv6_prefix	=> \&anon_ipv6,	# 2a02:587:b483:8d00::/56
haap_lte_ipv4			=> \&anon_ipv4,	#
haap_dsl_ipv4			=> \&anon_ipv4,	#
ishybrid				=> \&anon_none,	# false
validity_date_start		=> \&anon_none,	# 20171110000000
validity_date_end		=> \&anon_none,	# 20190930235959
calling_address			=> \&anon_ustring,# Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
card_balance_contained	=> \&anon_none,	# 3.36
card_balance_consumed	=> \&anon_none,	# 0.28
card_number				=> \&anon_idnum,	# 0507974295
version					=> \&anon_none,	# 10
seqnum					=> \&anon_none,	# 100000955753943
networkid				=> \&anon_none,	# 03
eventclass				=> \&anon_none,	# 001
networkprod				=> \&anon_none,	# 00001
action					=> \&anon_none,	# 01
eventsourceid			=> \&anon_none,	# INAYKTH,IMS
ndr						=> \&anon_phone,	# 6948457396
aaddrplan				=> \&anon_none,	# 1
nde						=> \&anon_phone,	# 2351028148
baddrplan				=> \&anon_none,	# 1
newdest					=> \&anon_phone,	# 2351028148
newdaddrplan			=> \&anon_none,	# 1
chargedparty			=> \&anon_none,	# U
date					=> \&anon_none,	# 20200101
starttime				=> \&anon_none,	# 063154
duratunit				=> \&anon_none,	# 1
secondunit				=> \&anon_none,	# 7
amountofsus				=> \&anon_none,	# 000000000000000
acategory				=> \&anon_none,	# 00
priority				=> \&anon_none,	# 00
numsuppserv				=> \&anon_none,	# 0
suppserv1				=> \&anon_none,	# 000
suppserv2				=> \&anon_none,	# 000
suppserv3				=> \&anon_none,	# 7
suppserv4				=> \&anon_none,	# 5900
suppserv5				=> \&anon_none,	# 01
suppserv6				=> \&anon_none,	# 000
suppserv7				=> \&anon_none,	# 000
suppserv8				=> \&anon_none,	# 000
numgroups				=> \&anon_none,	# 00
origcellid				=> \&anon_none,	# 000464
callid					=> \&anon_idnum,	# 0753665600
callid_hex				=> \&anon_idhex,	# 0753665600
inowner					=> \&anon_none,	# 2351028148
misc					=> \&anon_none,	# 00000000
reserved				=> \&anon_ustring,# 
);

my %field_funcs_gen = (
ts						=> \&anon_none,	# 20200103123507
src_filename			=> \&anon_none,	# R_ALEXADRAS01B.0476.2001031251
src_filedate			=> \&anon_none,	# 20191231005959
src_filesize			=> \&anon_none,	# 5839
duration				=> \&anon_none,	# 0001:02:16, 0000:33:45 or 33 (telecard)
calling					=> \&anon_phone1,	# 2661047456, 2810242384
called					=> \&anon_phone1,	# 0017183198803, 00972505228370
servicenumber			=> \&anon_none,	# 8071122
chronocard_id			=> \&anon_idnum1,	# 0000000001673709
balanceafter			=> \&anon_none,	# 000002915048
balanceconsumed			=> \&anon_none,	# 000001391230
expirationoffset		=> \&anon_none,	# 00000183
recseparator			=> \&anon_none,	# 
parsed					=> \&anon_none,	# 8,9 or 2 or 11, 15 ...
service					=> \&anon_none,	# 0
prefix					=> \&anon_none,	# 16032
number					=> \&anon_phone1,	# 00171########, 69########, 13888
logfile_line			=> \&anon_none,	# 1234
mailserver_name			=> \&anon_none,	# pop03, rigel
mailserver_type			=> \&anon_none,	# dovecot, amavis, postfix-smtpd ..
queue					=> \&anon_none,	# F40D81D0008C (hex number)
session					=> \&anon_none,	# d60EqHemurbDqgAq
message_id				=> \&anon_string1,	# 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
resent_message_id		=> \&anon_string1,	# 
sender					=> \&anon_email1,	# notification@facebookmail.com
receiver				=> \&anon_email1,	# vm1977@otenet.gr
user					=> \&anon_email1,	# empty, xxxxx@xxx.com
status					=> \&anon_none,	# blank | sent | removed | bounced |defered |expired
domain_not_found		=> \&anon_none,	# Recipient address rejected | sender address rejected
helo					=> \&anon_domain,	# ip or domain name or blank
ctladdr					=> \&anon_email1,	# username | email
orig_to					=> \&anon_email1,	# 
sasl_sender				=> \&anon_email1,	#
remote_ip				=> \&anon_ipv4,	#
content_ifnot_queue		=> \&anon_string1, # lmtp(xxxx@otenet.gr) | (03908-22) Passed {CLEAN|BADHEADER} (it might have commas!)
subscriber_type			=> \&anon_none,	# 0
imei					=> \&anon_idnum1,	# 353420100443370
imsi					=> \&anon_idnum1,	# 208201402275281
calling_number			=> \&anon_phone1,	# 0033663139639
ipaddress				=> \&anon_ipv4,	# 
called_number			=> \&anon_phone1,	# 00302120001535
hc_network				=> \&anon_none,	# 274
ext_carrier				=> \&anon_none,	# 30971388006
serving_vplmn			=> \&anon_none,	# 0A180DE7
request_time			=> \&anon_none,	# 
start_time				=> \&anon_none,	# 20200103123507 (year,month,day,hour,min,sec)
end_time				=> \&anon_none,	# 20200103124052 (year,month,day,hour,min,sec)
call_duration			=> \&anon_none,	# 345 (seconds)
time_shift				=> \&anon_none,	# 10800
service_el_tp			=> \&anon_none,	# 3G
service_el_id			=> \&anon_none,	# ALEXADRAS01B
first_cell				=> \&anon_none,	# 202010300304553
dest_zone				=> \&anon_none,	# 597
call_flag				=> \&anon_none,	# O
service_qual			=> \&anon_none,	# NAP
record_id				=> \&anon_none,	# 75571013
session_scn				=> \&anon_none,	# 209734471
service_scn				=> \&anon_none,	# 04760000
service_id				=> \&anon_none,	# 11
service_mode			=> \&anon_none,	# Foreign_Relay
service_class			=> \&anon_none,	# 0 or wireless.twilio.com
service_type			=> \&anon_none,	# 0
service_prov_id			=> \&anon_none,	# Foreign_Relay
content_size			=> \&anon_none,	# 131233
content_size_upl		=> \&anon_none,	# 
content_size_dnl		=> \&anon_none,	#
service_status			=> \&anon_none,	# S
cdr_type				=> \&anon_none,	# 1,9
termination_ind			=> \&anon_none,	# C | 17
charge					=> \&anon_none,	# 117474
last_cell				=> \&anon_none,	# 202010300304553
master_msisdn			=> \&anon_none,	# 
incomingswitchid		=> \&anon_none,	# TKO,82
outgoingswitchid		=> \&anon_none,	# 82
recordcallid			=> \&anon_none,	# 011270006737657
linkfield				=> \&anon_none,	# 01
a_number				=> \&anon_phone1,	# 69XXXXXXXX
b_number				=> \&anon_phone1,	# 13888, 69XXXXXXXX
incomingtrunk			=> \&anon_none,	# COTKO1I
outgoingtrunk			=> \&anon_none,	# 1104_COSMOTE_ATH
incomingproduct			=> \&anon_none,	# HLP, IPTL
outgoingproduct			=> \&anon_none,	# IPTL
datavolume				=> \&anon_none,	# 
dataunit				=> \&anon_none,	# 
usersummarisation		=> \&anon_anum,# 
udcsreserved			=> \&anon_string1,	# 
product					=> \&anon_none,	# 00001 149-Αμεση συνδιάλεξη
username				=> \&anon_email1,	# isgo56@otenet.gr
acct_status_type		=> \&anon_none,	# Interim-Update
acct_session_time		=> \&anon_none,	# 0
acct_unique_session_id	=> \&anon_idhex1,	# 1cdf65221e6c8b1b
framed_ip_address		=> \&anon_ipv4,	# 100.68.219.0
nas_port_type			=> \&anon_none,	# Wireless-XGP
nas_port_id				=> \&anon_none,	# #KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35#
calling_station_id		=> \&anon_anum1,	# 0 0/0/36.3223 # KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35
nas_ip_address			=> \&anon_ipv4,	# 79.128.219.74
port_parsed				=> \&anon_none,	# IGNORED
port_detail				=> \&anon_none,	#
delegated_ipv6_prefix	=> \&anon_ipv6,	# 2a02:587:b483:8d00::/56
haap_lte_ipv4			=> \&anon_ipv4,	#
haap_dsl_ipv4			=> \&anon_ipv4,	#
ishybrid				=> \&anon_none,	# false
validity_date_start		=> \&anon_none,	# 20171110000000
validity_date_end		=> \&anon_none,	# 20190930235959
calling_address			=> \&anon_string1,# Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
card_balance_contained	=> \&anon_none,	# 3.36
card_balance_consumed	=> \&anon_none,	# 0.28
card_number				=> \&anon_idnum1,	# 0507974295
version					=> \&anon_none,	# 10
seqnum					=> \&anon_none,	# 100000955753943
networkid				=> \&anon_none,	# 03
eventclass				=> \&anon_none,	# 001
networkprod				=> \&anon_none,	# 00001
action					=> \&anon_none,	# 01
eventsourceid			=> \&anon_none,	# INAYKTH,IMS
ndr						=> \&anon_phone1,	# 6948457396
aaddrplan				=> \&anon_none,	# 1
nde						=> \&anon_phone1,	# 2351028148
baddrplan				=> \&anon_none,	# 1
newdest					=> \&anon_phone1,	# 2351028148
newdaddrplan			=> \&anon_none,	# 1
chargedparty			=> \&anon_none,	# U
date					=> \&anon_none,	# 20200101
starttime				=> \&anon_none,	# 063154
duratunit				=> \&anon_none,	# 1
secondunit				=> \&anon_none,	# 7
amountofsus				=> \&anon_none,	# 000000000000000
acategory				=> \&anon_none,	# 00
priority				=> \&anon_none,	# 00
numsuppserv				=> \&anon_none,	# 0
suppserv1				=> \&anon_none,	# 000
suppserv2				=> \&anon_none,	# 000
suppserv3				=> \&anon_none,	# 7
suppserv4				=> \&anon_none,	# 5900
suppserv5				=> \&anon_none,	# 01
suppserv6				=> \&anon_none,	# 000
suppserv7				=> \&anon_none,	# 000
suppserv8				=> \&anon_none,	# 000
numgroups				=> \&anon_none,	# 00
origcellid				=> \&anon_none,	# 000464
callid					=> \&anon_idnum1,	# 0753665600
callid_hex				=> \&anon_idhex1,	# 0753665600
inowner					=> \&anon_none,	# 2351028148
misc					=> \&anon_none,	# 00000000
reserved				=> \&anon_string1,# 
);


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


sub rand_digits($$)
{
 my ($v,$start) = @_;
 my @b = split('',$v);
 my $flen = scalar @b;
 my $pos=0;
 my $l;
 foreach $l (@b) {
	if($pos >= $start){
		my $ind = $l-'0';
		@b[$pos]=@rand_digits[$ind];
	};
	$pos++;
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


	if($in_chars==1) {
		foreach $l (@b) {
			if( !grep( /^$l$/, @$chars)) { 
				# print "$l not in chars!\n";
				return $v;
			};
			$salt += ord($l);
		};
	} else {
		foreach $l (@b) {
			$salt += ord($l);
		};
	};

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

sub anon_generic1($$) {
# my ($v, $chars, $exept, $start, $ignore_begin, $in_chars) = @_;
 my ($v, $ar) = @_;
 my $chars = @$ar[0];
 # my $exept = @$ar[1];
 # my $start = @$ar[2];
 # my $ignore_begin = @$ar[3];
 # my $in_chars = @$ar[4];

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


	if(@$ar[4]==1) {
		foreach $l (@b) {
			if( !grep( /^$l$/, @$chars)) { 
				# print "$l not in chars!\n";
				return $v;
			};
			$salt += ord($l);
		};
	} else {
		foreach $l (@b) {
			$salt += ord($l);
		};
	};

	foreach $l (@b) {
		my $ol = ord($l);
		
		if(index(@$ar[1],$l)<0 and $pos>=@$ar[2] and !($begin==1 and index(@$ar[3],$l)>=0)) {
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

my @anon_function = (
	\&anon_none,
	\&anon_anum1,
	\&anon_phone1,
	\&anon_string1,
	\&anon_email1,
	\&anon_idnum1,
	\&anon_ipv4,
	\&anpn_ipv6,
	\&anon_idhex1,
	\&anon_domain
);

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
# 	foreach my $f (@fields) {
# 		if(!exists($field_names{$f})) {
# 			$field_names{$f}={$f};
# 			print "	insert	field = $f\n";
# 		};
# 	};

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
			@line_out[$i] = %field_funcs_gen{$f}->($line[$i]);
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
	print " > $file oldsize = $oldsize new_size = $new_size\n";
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

# my $r0="91234567890";
# my $r=anon_phone1($r0);
# print "anon_phone1 [$r0] -> [$r]\n";
# sexit
# initialize anonymized directory
print "Version $app_version Initialize anonymized directory $anon_log_dir\n";
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
		print "# in $archive_dir\n";
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
				print " : convert files in $fdname\n";
				my @done0_contents = get_dir($fdname);
				foreach my $done0_file (@done0_contents) {
					my $done0_name = "$fdname/$done0_file";
					print "  - $done0_name\n";
					if(-d $done0_name) {
						my @done1_contents = get_dir($done0_name);
						my $done1_max = scalar @done1_contents;
						print "	   convert files in subdir $done0_name $done1_max files\n";
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



print "\nDONE $anon_log_dir created !!\n";


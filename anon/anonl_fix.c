/*
		Anonymize logs
*/
#define	version "c 1.4.6 fix"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
//#include <time.h>
#include <sys/time.h>

#include "alist.h"

#define	NUSE	0
#define MAX_CATEGORY	20
#define	ON_PROD	0

BTREE *bt_field_table;
int now_seconds=0;	// current time epoq seconds
double  last_days=1000.0;

#define TEST	1
#define FIX		1

#if	ON_PROD
char *log_dir = "/logs";
char *anon_log_dir = "/logs/anonymous_logs_c";
#else
#if	FIX
char *log_dir = "anonymous_logs_c";
char *anon_log_dir = "anonymous_logs_c_fixed";
#else
char *log_dir = "logs";
char *anon_log_dir = "anonymous_logs_c";
#endif
#endif


// T_PHONE_A
// a_number: calling: 00302 -> preserve 5 else none!
// T_PHONE_B
// b_number: called: 00306 -> preserve 6, 00302 ->preserve 5
// cronocard calling: a_number
// cronocard called: b_number
// ict a_number: same as cronocard calling
// ict b_number: same as cronocard called
// ict_csv a_number: a_number
// ict_csv b_numner: b_number

// gprs
// imei: preserve 8
// imsi: preserve 5
// T_PHONE_G
// calling number: 0030 -> preserve 6
// T_PHONE_B
// called number: b_number
// T_PHONE_G
// master imsdn: 0030 -> preserve 6
// msc_sups
// 




enum {
	T_NONE,
	T_ANUM,
	T_PHONE_A,	// a_number
	T_PHONE_B,	// b_number
	T_PHONE_G,	// g_number (generic)
	T_IDNUM_5,	// number_p5 preserve 5
	T_IDNUM_8,	// number_p8 preserve 8
	T_STRING,
	T_EMAIL,
	T_IDNUM,
	T_IPV4,
	T_IPV6,
	T_IDHEX,
	T_DOMAIN
};

#define DEFAULT_SALT	23
#define MAX_NUM_DIGITS	10
#define	MAX_HEX_DIGITS	16
#define MAX_ANUM_DIGITS	36
#define	MAX_UNUM_DIGITS 60
#define	true	1
#define	false	0

const unsigned char *anon_generic (const char *v, const char *chars[], int max, const char *except, int start,const char *ignore_begin,int in_char);
const unsigned char *arand_digits (const char *v, int start1, char *check1,int start2, char *check2,int start3,char *check3);
const unsigned char *mask_string (const char *v, int start, char mask_char);

typedef struct FIELD_TYPE
{
 char *name;
 int type;
} FIELD_TYPE;

typedef struct CAT_DEFINITION {
	char *name;
	char *sep;
	char **field;
} CAT_DEFINITION;

int num_digits[] =  {'0','1','2','3','4','5','6','7','8','9',0};
int rand_digits[] = {'5','8','6','4','7','9','3','2','0','1',0 };

int hex_digits[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',0};
int anum_digits[] = {
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',0
};

const char *cnum_digits[] = {"0","1","2","3","4","5","6","7","8","9",""};

const char *chex_digits[] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F",""};

const char *canum_digits[] = {
	"0","1","2","3","4","5","6","7","8","9",
	"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",""
};

const char *unum_digits[] = {"0","1","2","3","4","5","6","7","8","9",
	"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
	"Α","Β","Γ","Δ","Ε","Ζ","Η","Θ","Ι","Κ","Λ","Μ","Ν","Ξ","Ο","Π","Ρ","Σ","Τ","Υ","Φ","Χ","Ψ","Ω",""
};

const char *category_names[] = {
	"chronocard","gprs","udcs","udsclike","ict",
	// "itu_csv","email","chronocard","ict","ict_csv","oss","radius","w_sms","orig_sms","telecard",
	// "gprs","h_msc","mms","sms_gprs","tap_in","v_msc","w_sms","orig_sms",
	NULL
};

FIELD_TYPE field_types[] = {
{"ts"					, T_NONE  },	// 20200103123507
{"src_filename"			, T_NONE  },	// R_ALEXADRAS01B.0476.2001031251
{"src_filedate"			, T_NONE  },	// 20191231005959
{"src_filesize"			, T_NONE  },	// 5839
{"duration"				, T_NONE  },	// 0001:02:16, 0000:33:45 or 33 (telecard)
{"a_number"				, T_PHONE_A },	// 2661047456, 2810242384
{"b_number"				, T_PHONE_B },	// 0017183198803, 00972505228370
{"g_number"				, T_PHONE_G },
{"servicenumber"		, T_NONE  },	// 8071122
{"chronocard_id"		, T_NONE  },	// 0000000001673709
{"balanceafter"			, T_NONE  },	// 000002915048
{"balanceconsumed"		, T_NONE  },	// 000001391230
{"expirationoffset"		, T_NONE  },	// 00000183
{"recseparator"			, T_NONE  },	// 
{"parsed"				, T_NONE  },	// 8,9 or 2 or 11, 15 ...
{"service"				, T_NONE  },	// 0
{"prefix"				, T_NONE  },	// 16032
{"number"				, T_NONE  },	// 00171########, 69########, 13888
{"logfile_line"			, T_NONE  },	// 1234
{"mailserver_name"		, T_NONE  },	// pop03, rigel
{"mailserver_type"		, T_NONE  },	// dovecot, amavis, postfix-smtpd ..
{"queue"				, T_NONE  },	// F40D81D0008C (hex number)
{"session"				, T_NONE  },	// d60EqHemurbDqgAq
{"message_id"			, T_NONE  },	// 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
{"resent_message_id"	, T_NONE  },	// 
{"sender"				, T_EMAIL },	// notification@facebookmail.com
{"receiver"				, T_EMAIL },	// vm1977@otenet.gr
{"user"					, T_EMAIL },	// empty, xxxxx@xxx.com
{"status"				, T_NONE  },	// blank | sent | removed | bounced |defered |expired
{"domain_not_found"		, T_NONE  },	// Recipient address rejected | sender address rejected
{"helo"					, T_DOMAIN},	// ip or domain name or blank
{"ctladdr"				, T_EMAIL },	// username | email
{"orig_to"				, T_EMAIL },	// 
{"sasl_sender"			, T_EMAIL },	//
{"remote_ip"			, T_IPV4  },	//
{"content_ifnot_queue"	, T_STRING}, 	// lmtp(xxxx@otenet.gr) | (03908-22) Passed {CLEAN|BADHEADER} (it might have commas!)
{"subscriber_type"		, T_NONE  },	// 0
{"imei"					, T_IDNUM_5 },	// 353420100443370
{"imsi"					, T_IDNUM_8 },	// 208201402275281
{"calling_number"		, T_PHONE_A },	// 0033663139639
{"ipaddress"			, T_IPV4  },	// 
{"called_number"		, T_PHONE_B },	// 00302120001535
{"hc_network"			, T_NONE  },	// 274
{"ext_carrier"			, T_NONE  },	// 30971388006
{"serving_vplmn"		, T_NONE  },	// 0A180DE7
{"request_time"			, T_NONE  },	// 
{"start_time"			, T_NONE  },	// 20200103123507 (year,month,day,hour,min,sec)
{"end_time"				, T_NONE  },	// 20200103124052 (year,month,day,hour,min,sec)
{"call_duration"		, T_NONE  },	// 345 (seconds)
{"time_shift"			, T_NONE  },	// 10800
{"service_el_tp"		, T_NONE  },	// 3G
{"service_el_id"		, T_NONE  },	// ALEXADRAS01B
{"first_cell"			, T_NONE  },	// 202010300304553
{"dest_zone"			, T_NONE  },	// 597
{"call_flag"			, T_NONE  },	// O
{"service_qual"			, T_NONE  },	// NAP
{"record_id"			, T_NONE  },	// 75571013
{"session_scn"			, T_NONE  },	// 209734471
{"service_scn"			, T_NONE  },	// 04760000
{"service_id"			, T_NONE  },	// 11
{"service_mode"			, T_NONE  },	// Foreign_Relay
{"service_class"		, T_NONE  },	// 0 or wireless.twilio.com
{"service_type"			, T_NONE  },	// 0
{"service_prov_id"		, T_NONE  },	// Foreign_Relay
{"content_size"			, T_NONE  },	// 131233
{"content_size_upl"		, T_NONE  },	// 
{"content_size_dnl"		, T_NONE  },	//
{"service_status"		, T_NONE  },	// S
{"cdr_type"				, T_NONE  },	// 1,9
{"termination_ind"		, T_NONE  },	// C | 17
{"charge"				, T_NONE  },	// 117474
{"last_cell"			, T_NONE  },	// 202010300304553
{"master_msisdn"		, T_PHONE_G  },	// 
{"incomingswitchid"		, T_NONE  },	// TKO,82
{"outgoingswitchid"		, T_NONE  },	// 82
{"recordcallid"			, T_NONE  },	// 011270006737657
{"linkfield"			, T_NONE  },	// 01
{"incomingtrunk"		, T_NONE  },	// COTKO1I
{"outgoingtrunk"		, T_NONE  },	// 1104_COSMOTE_ATH
{"incomingproduct"		, T_NONE  },	// HLP, IPTL
{"outgoingproduct"		, T_NONE  },	// IPTL
{"datavolume"			, T_NONE  },	// 
{"dataunit"				, T_NONE  },	// 
{"usersummarisation"	, T_STRING},	// 
{"udcsreserved"			, T_NONE  },	// 
{"product"				, T_NONE  },	// 00001 149-Αμεση συνδιάλεξη
{"username"				, T_EMAIL },	// isgo56@otenet.gr
{"acct_status_type"		, T_NONE  },	// Interim-Update
{"acct_session_time"	, T_NONE  },	// 0
{"acct_unique_session_id",T_IDHEX },	// 1cdf65221e6c8b1b
{"framed_ip_address"	, T_IPV4  },	// 100.68.219.0
{"nas_port_type"		, T_NONE  },	// Wireless-XGP
{"nas_port_id"			, T_NONE  },	// #KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35#
{"calling_station_id"	, T_ANUM  },	// 0 0/0/36.3223 # KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35
{"nas_ip_address"		, T_IPV4  },	// 79.128.219.74
{"port_parsed"			, T_NONE  },	// IGNORED
{"port_detail"			, T_NONE  },	//
{"delegated_ipv6_prefix", T_IPV6  },	// 2a02:587:b483:8d00::/56
{"haap_lte_ipv4"		, T_IPV4  },	//
{"haap_dsl_ipv4"		, T_IPV4  },	//
{"ishybrid"				, T_NONE  },	// false
{"validity_date_start"	, T_NONE  },	// 20171110000000
{"validity_date_end"	, T_NONE  },	// 20190930235959
{"calling_address"		, T_NONE  },	// Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
{"card_balance_contained",T_NONE  },	// 3.36
{"card_balance_consumed", T_NONE  },	// 0.28
{"card_number"			, T_IDNUM },	// 0507974295
{"version"				, T_NONE  },	// 10
{"seqnum"				, T_NONE  },	// 100000955753943
{"networkid"			, T_NONE  },	// 03
{"eventclass"			, T_NONE  },	// 001
{"networkprod"			, T_NONE  },	// 00001
{"action"				, T_NONE  },	// 01
{"eventsourceid"		, T_NONE  },	// INAYKTH,IMS
{"ndr"					, T_PHONE_B },	// 6948457396
{"aaddrplan"			, T_NONE  },	// 1
{"nde"					, T_PHONE_B },	// 2351028148
{"baddrplan"			, T_NONE  },	// 1
{"newdest"				, T_PHONE_B },	// 2351028148
{"newdaddrplan"			, T_NONE  },	// 1
{"chargedparty"			, T_NONE  },	// U
{"date"					, T_NONE  },	// 20200101
{"starttime"			, T_NONE  },	// 063154
{"duratunit"			, T_NONE  },	// 1
{"secondunit"			, T_NONE  },	// 7
{"amountofsus"			, T_NONE  },	// 000000000000000
{"acategory"			, T_NONE  },	// 00
{"priority"				, T_NONE  },	// 00
{"numsuppserv"			, T_NONE  },	// 0
{"suppserv1"			, T_NONE  },	// 000
{"suppserv2"			, T_NONE  },	// 000
{"suppserv3"			, T_NONE  },	// 7
{"suppserv4"			, T_NONE  },	// 5900
{"suppserv5"			, T_NONE  },	// 01
{"suppserv6"			, T_NONE  },	// 000
{"suppserv7"			, T_NONE  },	// 000
{"suppserv8"			, T_NONE  },	// 000
{"numgroups"			, T_NONE  },	// 00
{"origcellid"			, T_NONE  },	// 000464
{"callid"				, T_NONE },	// 0753665600
{"callid_hex"			, T_NONE },	// ABCD665600
{"inowner"				, T_NONE  },	// 2351028148
{"misc"					, T_NONE  },	// 00000000
{"reserved"				, T_NONE},	// 
{NULL,0}
};

char *size2str(long int size)
{
 static char out[64];
	if(size<1000) {
		sprintf(out,"%ld bytes",size);
	} else if(size<1000000) {
		double s = size/1000.0;
		sprintf(out,"%.3f Kb",s);
	} else if(size<1000000000) {
		double s = size/1000000.0;
		sprintf(out,"%.3f Mb",s);
	} else {
		double s = size/1000000000.0;
		sprintf(out,"%.3f Gb",s);
	};
	return out;
}

int valid_log(char *lname)
{
 int i=0;
 while(category_names[i]!=NULL) {
		if(!strcmp(category_names[i],lname)) return 1;
		i++;
	};
	return 0;
}

unsigned const char *anon_none(const char *v)
{
	return (unsigned const char *)v;
}

unsigned const char *anon_anum1(const char *v)
{
	return anon_generic(v,canum_digits,MAX_ANUM_DIGITS,".@?:",0,"",0);
}

void set_gunzipped(char *fname)
{
 char unzipped_name[1024];
 int len=strlen(fname);
 if(strcmp(fname+len-3,".gz")==0) {
	char cmd[2048];
	int result;
 	// printf("%s is compressed!\n",fname);
	sprintf(cmd,"gunzip -c %s > /tmp/uncompressed",fname);
	result = system(cmd);
	sync();
	strcpy(fname,"/tmp/uncompressed");
 } else {
 	// printf("last 3 [%s]\n",fname+len-3);
 }
}

unsigned const char *anon_anum(const char *v)
{
 static unsigned char anon_str[1024];
 long int salt = DEFAULT_SALT;
 int len = strlen(v);
 anon_str[0]=0;
 long int cc=0;
 if(v==0) return anon_str;
 long int key = v[len-1]+17-48;
 long int val=0;
 int i=0;
 int l=0;
 
 for(i=0;i<len;i++) salt += v[i];
// printf(" > salt=%ld k0=%ld\n",salt,key);
 for(i=0;l=v[i];i++){
 	if(l=='.' || l=='@' || l=='?' || l==':') {
		anon_str[i]=l;
	} else {
		long int ool = cc ^ l;
//		printf( " -: v=%ld,ol=%d,k=%ld,cc=%ld \n",val,l,key,cc);
		key = l + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3);
//		printf("  : k2=%ld ",key);
		val = (( key % 177) - cc) % 177;
//		printf(" , v=%ld cc0=%ld ",val,cc);
		while(val<0) val += MAX_ANUM_DIGITS;
		cc=val;
		
		if( ++salt >= 20857) {
			salt=0;
		};
//		printf(" val=%ld cc=%ld salt=%ld",val,cc,salt);
		key = key + key + (cc ^ l) + salt;
//		printf(" k2=%ld\n",key);
		anon_str[i] = anum_digits[cc%MAX_ANUM_DIGITS];
	};
 };anon_str[i]=0;
// printf("> %s -> %s\n",v,anon_str);
 return anon_str;
}

unsigned const char *anon_phone1(const char *v)
{
	// return anon_generic(v,cnum_digits,MAX_NUM_DIGITS,"",0,"06",1);
	unsigned const char *v1 = arand_digits(v,4,NULL,0,NULL,0,NULL);
	// printf("anon_phone1:[%s]->[%s]\n",v,v1);
	return v1;
}

const unsigned char *anon_phone(const char *v)
{
 static unsigned char anon_str[256];
 long int salt = DEFAULT_SALT;
 int len = strlen(v);
 anon_str[0]=0;
 long int cc=0;
 if(v==0) return anon_str;
 if(!strcmp(v,"DUMMY")) {
// 	printf (" - DUMMY:\n");
	return (unsigned char *)"DUMMY";
 };

 long int key = v[len-1]+17-48;
 long int val=0;
 int i=0;
 int l=0;
 
 for(i=0;i<len;i++) salt += v[i];
// printf(" > salt=%ld k0=%ld\n",salt,key);
 int start=1;
 for(i=0;l=v[i];i++){
 	if(l>'9'||l<'0') return (unsigned char *)v;
 	if((l=='0' ||l=='6') && start==1) {
		anon_str[i]=l;
	} else {
		long int ool = cc ^ l;
//		printf( " -: v=%ld,ol=%d,k=%ld,cc=%ld \n",val,l,key,cc);
		start=0;
		key = l + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3);
//		printf("  : k2=%ld ",key);
		val = (( key % 177) - cc) % 177;
//		printf(" , v=%ld cc0=%ld ",val,cc);
		while(val<0) val += MAX_NUM_DIGITS;
		cc=val;
		
		if( ++salt >= 20857) {
			salt=0;
		};
//		printf(" val=%ld cc=%ld salt=%ld",val,cc,salt);
		key = key + key + (cc ^ l) + salt;
//		printf(" k2=%ld\n",key);
		anon_str[i] = num_digits[cc%MAX_NUM_DIGITS];
	};
 };anon_str[i]=0;
// printf("> %s -> %s\n",v,anon_str);
 return anon_str;
}

const unsigned char *anon_idnum1(const char *v)
{
	return anon_generic(v,cnum_digits,MAX_NUM_DIGITS,"",0,"0",1);
}

const unsigned char *anon_a_number(const char *v)
{
	unsigned const char *v1 = arand_digits(v,6,"00302",0,NULL,0,NULL);
	return v1;
}

const unsigned char *anon_b_number(const char *v)
{
	unsigned const char *v1 = arand_digits(v,5,"00302",6,"00306",0,NULL);
	return v1;
}

const unsigned char *anon_number_fix(const char *v)
{
	// unsigned const char *v1 = arand_digits(v,5,"00302",6,"00306",0,NULL);
	static char anon_str[1024];
	anon_str[0]=0;
	if(v==0) return (unsigned char *)anon_str;

	if(strncmp(v,"4531",4)==0) { 
		sprintf(anon_str,"3069%s",v+4);
		printf("	v [%s] -> [%s]\n",v,anon_str);
	} else if(strncmp(v,"31",2)==0) {
		sprintf(anon_str,"69%s",v+2);
		printf("	v [%s] -> [%s]\n",v,anon_str);
	} else strcpy(anon_str,v);
	return (unsigned char *)anon_str;
}

const unsigned char *anon_g_number(const char *v)
{
	unsigned const char *v1 = arand_digits(v,6,"0030",4,"30",2,"69");
	return v1;
}

const unsigned char *anon_preserve8(const char *v)
{
	unsigned const char *v1 = arand_digits(v,8,NULL,0,NULL,0,NULL);
	return v1;
}

const unsigned char *anon_preserve5(const char *v)
{
	unsigned const char *v1 = arand_digits(v,5,NULL,0,NULL,0,NULL);
	return v1;
}


const unsigned char *anon_idnum(const char *v)
{
 static char anon_str[1024];
 long int salt = DEFAULT_SALT;
 int len = strlen(v);
 anon_str[0]=0;
 long int cc=0;
 if(v==0) return (unsigned char *)anon_str;
 long int key = v[len-1]+17-48;
 long int val=0;
 int i=0;
 int l=0;
 
 for(i=0;i<len;i++) salt += v[i];
// printf(" > salt=%ld k0=%ld\n",salt,key);
 int start=1;
 for(i=0;l=v[i];i++){
 	if(l=='0' && start==1) {
		anon_str[i]=l;
	} else {
		long int ool = cc ^ l;
//		printf( " -: v=%ld,ol=%d,k=%ld,cc=%ld \n",val,l,key,cc);
		start=0;
		key = l + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3);
//		printf("  : k2=%ld ",key);
		val = (( key % 177) - cc) % 177;
//		printf(" , v=%ld cc0=%ld ",val,cc);
		while(val<0) val += MAX_NUM_DIGITS;
		cc=val;
		
		if( ++salt >= 20857) {
			salt=0;
		};
//		printf(" val=%ld cc=%ld salt=%ld",val,cc,salt);
		key = key + key + (cc ^ l) + salt;
//		printf(" k2=%ld\n",key);
		anon_str[i] = num_digits[cc%MAX_NUM_DIGITS];
	};
 };anon_str[i]=0;
//  printf("> %s -> %s\n",v,anon_str);
 return (unsigned char *)anon_str;
}

const unsigned char *anon_idhex1(const char *v)
{
	return anon_generic(v,chex_digits,MAX_HEX_DIGITS,"",0,"0",0);
}

#if	NUSE
const unsigned char *anon_idhex(const char *v)
{
 static char anon_str[1024];
 long int salt = DEFAULT_SALT;
 int len = strlen(v);
 anon_str[0]=0;
 long int cc=0;
 if(v==0) return (unsigned char *)anon_str;
 long int key = v[len-1]+17-48;
 long int val=0;
 int i=0;
 int l=0;
 
 for(i=0;i<len;i++) salt += v[i];
// printf(" > salt=%ld k0=%ld\n",salt,key);
 int start=1;
 for(i=0;l=v[i];i++){
 	if(l=='0' && start==1) {
		anon_str[i]=l;
	} else {
		long int ool = cc ^ l;
//		printf( " -: v=%ld,ol=%d,k=%ld,cc=%ld \n",val,l,key,cc);
		start=0;
		key = l + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3);
//		printf("  : k2=%ld ",key);
		val = (( key % 177) - cc) % 177;
//		printf(" , v=%ld cc0=%ld ",val,cc);
		while(val<0) val += MAX_HEX_DIGITS;
		cc=val;
		
		if( ++salt >= 20857) {
			salt=0;
		};
//		printf(" val=%ld cc=%ld salt=%ld",val,cc,salt);
		key = key + key + (cc ^ l) + salt;
//		printf(" k2=%ld\n",key);
		anon_str[i] = hex_digits[cc%MAX_HEX_DIGITS];
	};
 };anon_str[i]=0;
// printf("> %s -> %s\n",v,anon_str);
 return (unsigned char *)anon_str;
}
#endif

// no check previous utf8 char
unsigned char *prev_utf8_char(unsigned char *string,int pos)
{
	if(pos==0) return string;
	pos--;
	// printf("[%s] len=%d last = %X\n",string,pos+1,string[pos]);
	if(string[pos]<128) return string+pos;
	pos--;
// 	printf("%2X %2X\n",string[pos],string[pos+1]);
	if(string[pos]<0xE0 && string[pos]>=0xC0) return string+pos;
	pos--;
	if(string[pos]<0xF0 && string[pos]>=0xE0) return string+pos;
	pos--;
	return string+pos;
}

// utf8 character len no check!!
int utf8_len(unsigned char *str)
{
 unsigned char ch;
 if(str==NULL) return 0;
 ch=str[0];
 if(ch==0) return 0;
 if(ch<128) return 1;
 if(ch<0xE0) return 2;
 if(ch<0xF0) return 3;
 return 4;
}

int utf8_ord(unsigned char *str)
{
 unsigned char ch1,ch2,ch3,ch4;
 ch1=str[0];
 if(ch1<128) return (int)ch1;
 ch2=str[1];
 if(ch1<0xE0) return ((ch1-0xC0)*0x40+ch2%0x40);
 ch3=str[2];
 if(ch1<0xF0) return (ch1-0xE0)*0x1000+(ch2-0x80)*64+ch3%0x40;
 ch4=str[3];
 return (ch1-0xF0)*64*0x1000+(ch2-0x80)*0x1000+(ch3-0x80)*64+ch4%0x40;
}

const unsigned char *arand_digits (const char *v, int start1,char *check1,int start2, char *check2,int start3, char *check3)
{
	static char anon_str[1024];
	int from=0;
	// long int cc=0;
	// int slen = strlen(v);
	if(v==0) return (unsigned char *)anon_str;
	unsigned char *uv = (unsigned char *)v;
	if(check1!=NULL) {
		if(strncmp(v,check1,strlen(check1))==0) from=start1; 
	} else from=start1;
	if(check2!=NULL) {
		if(strncmp(v,check2,strlen(check2))==0) from=start2; 
	};
	int i=0;
	// printf("arand_digits: from=%d %d %s\n",from,start1,check1);
	int ulen=0;
	while(*uv) {
		uv += utf8_len(uv);
		ulen++;
	};

 anon_str[0]=0;
 int pos=0;
 int l;
 for(uv=(unsigned char*)v;(l=utf8_ord(uv));uv+=utf8_len(uv))
 {
	unsigned char c[2];
 	int asl=strlen((char *)anon_str);
	c[0]=uv[0];c[1]=0;
	// printf("[%s] [%s] l=%d\n",c,anon_str,l);
#if	1
	if(l<'0' || l>'9') {
		strcpy(anon_str,v);
		// printf("rand: start1=%d check1=%s [%s]\n",start1,check1,anon_str);
		return (unsigned char *)anon_str;
	};
#endif
	if(pos>=from) {
		anon_str[pos++]=rand_digits[l-'0'];
		// anon_str[pos++]='#';
	} else {
		anon_str[pos++]=l;
	};
 };
 anon_str[pos]=0;
 return (unsigned char *)anon_str;
}

// mask string with specific char. preserve first 'start', preserve length.
const unsigned char *mask_string(const char *v, int start, char mask_char)
{
 static char anon_str[1024];
 int len=strlen(v);
 if(len>1023) len=1023;
 int ind=0;
 int preserve_bytes=0;
 for(ind=0;ind<start;ind++) {
 	preserve_bytes += utf8_len((unsigned char *)(v+preserve_bytes));
	// printf("%d\n",preserve_bytes);
 };
 if(len>preserve_bytes) {
 	memcpy(anon_str,v,preserve_bytes);
	memset(anon_str+preserve_bytes,mask_char,len-preserve_bytes);
	anon_str[len]=0;
	// printf("%s -> [%s] start=%d len=%d preserve=%d\n",v,anon_str,start,len,preserve_bytes);
	return (unsigned char *)anon_str;
 } else return v;
}

// v -> input string
// chars -> array with output chars
// exept -> string with chars to not convert
// start -> start convert from this position
// ignore_begin -> string with chars that do not convert at the beginning of the line
// in_chars -> if not found in @chars then return original string
const unsigned char *anon_generic (const char *v, const char *chars[], int max,const char *except, int start,const char *ignore_begin,int in_char)
{
	static char anon_str[1024];
	// int max=0;
	int salt = DEFAULT_SALT ;
	anon_str[0]=0;
	long int cc=0;
	int slen = strlen(v);
	if(v==0) return (unsigned char *)anon_str;
	unsigned char *uv = (unsigned char *)v;
	unsigned char *last_char = prev_utf8_char(uv,slen);
	int last_ord = utf8_ord(last_char);
	long int key = last_ord+17-48;
	long int val = 0;

	int i=0;
	int l=0;
	int is_utf=0;
	int first_byte;
	int ulen=0;
	int begin=true;

	// for(;chars[max][0]!=0;max++);

	while(*uv) {
		salt += utf8_ord(uv);
		uv += utf8_len(uv);
		ulen++;
	};

	anon_str[0]=0;
// 	return (const unsigned char *)"";	

 for(uv=(unsigned char*)v;(l=utf8_ord(uv));uv+=utf8_len(uv))
 {
	unsigned char c[2];
 	int asl=strlen((char *)anon_str);
	c[0]=uv[0];c[1]=0;
	// printf("[%s] [%s] l=%d\n",c,anon_str,l);
	if(strstr(except,(const char *)c)!=NULL || i<start || 
		strstr(ignore_begin,(const char *)c)!=NULL && begin
		) {
		anon_str[asl]=l;anon_str[asl+1]=0;	// for not utf char!!!!!
	} else {
		if(in_char) {
		int in_chars=false;
		for(i=0;i<max;i++) {
			if(l==chars[i][0]) {in_chars=true;break;};
		};
		if(!in_chars) {
			// printf("return ori!\n");
			return (const unsigned char *)v;
		};
		};
		begin = false;
		key = l + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3);
		val = (( key % 177) - cc) % 177;
		while(val<0) val += max;
		cc=val;
		
		if( ++salt >= 20857) {
			salt=0;
		};
		key = key + key + (cc ^ l) + salt;
		strcat(anon_str,chars[cc%max]);
	};
 };
 return (unsigned char *)anon_str;
}


// Νομός Αττικής - ΦΥΛΑΚΕΣ ΚΟΡΥΔΑΛΛΟΥ ΑΝΔΡΩΝ (Γ)ΠΤΕΡΥΓΑ ΙΣΟΓΕΙΟ ΑΡΙΣΤΕΡΑ 
const unsigned char *anon_string1(const char *v)
{
	return mask_string(v,2,'*');
	// return anon_generic(v,unum_digits,MAX_UNUM_DIGITS,".@/",0,"",0);
}

#if	NUSE
const unsigned char *anon_string(const char *v)
{
 static char anon_str[1024];
 long int salt = DEFAULT_SALT;
 anon_str[0]=0;
 long int cc=0;
 int slen = strlen(v);
 if(v==0) return (unsigned char *)anon_str;
 unsigned char *uv = (unsigned char *)v;
 unsigned char *last_char = prev_utf8_char(uv,slen);
 int last_ord = utf8_ord(last_char);
 long int key = last_ord+17-48;

 long int val=0;
 int i=0;
 int l=0;
 int is_utf =0;
 int first_byte;
 int ulen=0;
 while(*uv) {
 	salt += utf8_ord(uv);
	uv += utf8_len(uv);
	ulen++;
 };
 // printf("last_char [%s] ord=%d\n",last_char,last_ord);
 // printf(" > [%s] len=%d salt=%ld k0=%ld\n",v,ulen,salt,key);

 anon_str[0]=0;
 for(uv=(unsigned char*)v;(l=utf8_ord(uv));uv+=utf8_len(uv))
 {
 	int asl=strlen((char *)anon_str);
	if(l=='.' || l=='@' || l=='/')
	{
		anon_str[asl]=l;anon_str[asl+1]=0;
	} else {
		// long int ool = cc ^ l;
		// printf(" -: v=%ld,ol=%d,k=%ld,cc=%ld\n",val,l,key,cc);
		key = l + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3);
		val = (( key % 177) - cc) % 177;
		while(val<0) val += MAX_UNUM_DIGITS;
		cc=val;
		
		if( ++salt >= 20857) {
			salt=0;
		};
		key = key + key + (cc ^ l) + salt;
		strcat(anon_str,unum_digits[cc%MAX_UNUM_DIGITS]);
	};
 };
// printf("\n::> %s -> %s\n",v,anon_str);
 return (unsigned char *)anon_str;
}
#endif

const unsigned char *anon_email1(const char *v)
{
	return anon_generic(v,canum_digits,MAX_ANUM_DIGITS,"@.",0,"",0);
}

#if	NUSE
const unsigned char *anon_email(const char *v)
{
 static char anon_str[1024];
 long int salt = DEFAULT_SALT;
 int len = strlen(v);
 anon_str[0]=0;
 long int cc=0;
 if(v==0) return (unsigned char *)anon_str;
 long int key = v[len-1]+17-48;
 long int val=0;
 int i=0;
 int l=0;

 for(i=0;i<len;i++) salt += v[i];
// printf(" > salt=%ld k0=%ld\n",salt,key);
 for(i=0;l=v[i];i++){
 	if(l=='.' || l=='@' ) {
		anon_str[i]=l;
	} else {
		long int ool = cc ^ l;
//		printf( " -: v=%ld,ol=%d,k=%ld,cc=%ld \n",val,l,key,cc);
		key = l + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3);
//		printf("  : k2=%ld ",key);
		val = (( key % 177) - cc) % 177;
//		printf(" , v=%ld cc0=%ld ",val,cc);
		while(val<0) val += MAX_ANUM_DIGITS;
		cc=val;
		
		if( ++salt >= 20857) {
			salt=0;
		};
//		printf(" val=%ld cc=%ld salt=%ld",val,cc,salt);
		key = key + key + (cc ^ l) + salt;
//		printf(" k2=%ld\n",key);
		anon_str[i] = anum_digits[cc%MAX_ANUM_DIGITS];
	};
 };anon_str[i]=0;
// printf("> %s -> %s\n",v,anon_str);
 return (unsigned char *)anon_str;
}
#endif

const unsigned char *anon_ipv4(const char *v)
{
	return (unsigned char *)v;
}

const unsigned char *anon_domain(const char *v)
{
	return (unsigned char *)v;
}

const unsigned char *anon_ipv6(const char *v)
{
	return (unsigned char *)v;
}

const unsigned char *(*anon_function1[])(const char *c) = {
	anon_none,
	anon_none,
	anon_number_fix,
	anon_number_fix,
	anon_number_fix,
	anon_none,
	anon_none,
	anon_none,
	anon_none,
	anon_none,
	anon_none,
	anon_none,
	anon_none,
	anon_none
};

const char *anonymize_field(char *f,char *field_name)
{
	if(f[0]==0) return "";
	int type = btival(bt_field_table,field_name);
//	printf("anonymize_field: %s -> %d\n",field_name,type);
	return anon_function1[type](f);
}

void free_anon_array(char **array)
{
 char **a=array;
	while(*a) {
		free(*a);
		*a++;
	};
}

void print_sarray(FILE *f,char **array,char *separator)
{
 char **a=array;
 	fprintf(f,"%s",*a);
// 	fprintf(stdout,"%s",*a++);
	*a++;
	while(*a) {
		fprintf(f,"%s%s",separator,*a);
//		fprintf(stdout,"%s%s",separator,*a);
		*a++;
	};
	fprintf(f,"\n");
}

char *check1 = "start field<*>field1<*>field3<*>last field";
char *check2 = "start field|field1|field3|last field";
char *check3 = "start field,field1,field3,last field";
char *check4 = "20200101020400|KT_Trs.2020-01-01.csv|20200102010001|4229827|20190904000000|20210701235959|2531073127|Νομός Ροδόπης - ΚΡΑΤΗΤΗΡΙΑ, ΑΣΤΥΝΟΜΙΑ, ΛΕΣΧΗ, ΚΟΜΟΤΗΝΗ|6975694249|140|3.6|0.2|0508943966|";

void free_sarray(char **sarray)
{
	char *buffer = sarray[0];
	free(buffer);	/* free contents  */
	// free(sarray);	/* free pointer array  */
}

// use the above free_sarray function to free the resulting array after use!
char **split_str_sarray(char *str,char *separator,int *count1)
{
// char **array=NULL;
   char *buffer = NULL;
   int string_size;
   int sep_len = strlen(separator);
   int count=0;
	static char *array[100];

   string_size=strlen(str);
   // Allocate a string that can hold it all
   buffer = strdup(str);
   if(buffer){
	int i;
	int l=0;
	char *start_substr=buffer;

	while(start_substr=strstr(start_substr,separator)) {
		start_substr[0]=0;
		start_substr+=sep_len;
//		printf("%s\n",start_substr);
		count++;
	};
//	printf("split: count=%d\n",count);
	start_substr=str;
	array[0]=buffer;
	l=1;
	while(start_substr=strstr(start_substr,separator)){
		start_substr+=sep_len;
//		printf("o=%ld\n",start_substr-buffer);
		array[l++]=buffer+(start_substr-str);
	};
	array[l]=NULL;
	*count1=count;
//	for(l=0;l<count;l++) printf("%d: %s\n",l,array[l]);
   } else {
    *count1=0;
   	return NULL;
   }
 return array;
}

void clear_snames(char **list,int size)
{
 int i;
 for(i=0;i<size;i++) {
 	if(list[i]==NULL) break;
	free(list[i]);
 };
 free(list);
}

char **get_dir(char *dirname,/*char *s_find, */int *num)
{
 int n; /* no of files in directory */
 char **namelist=NULL;
 DIR *d1;
 struct dirent *df1;
 int i;

 d1 = opendir((const char *)dirname);
// if(d1==NULL) { error_line("cant open dir");return(0);};
 if(d1==NULL) { printf ("No directory named %s",dirname);exit(1);};
 for(i=0;;i++) {
 	df1=readdir(d1);
	if(df1==NULL) { n=*num=i; closedir(d1);break;};
	if(strcmp(".",df1->d_name)==0) { i--;continue;};
	if(strcmp("..",df1->d_name)==0) { i--;continue;};
	/* filter it!  */
//	if(strncmp(s_find,df1->d_name,strlen(s_find))!=0) { i--; continue;};
	if(i%256 == 0) namelist = (char **)realloc((char *)namelist, (i+256)*sizeof(char *));
	namelist[i]=(char *)malloc(strlen(df1->d_name)+1);
	strcpy(namelist[i],df1->d_name);
 } ;
// qsort_str(namelist,n,stricmp1);

 return(namelist);
}

// category_fields
char *email_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","logfile_line","mailserver_name","mailserver_type","queue","session","message_id","resent_message_id","sender","receiver","user","status","domain_not_found","helo","ctladdr","orig_to","sasl_sender","remote_ip","content_ifnot_queue",NULL};
char *chronocard_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","duration","a_number","b_number","servicenumber","chronocard_id","balanceafter","balanceconsumed","expirationoffset","recseparator","parsed","service","prefix","number",NULL};
char *gprs_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","b_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *ict_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","usersummarisation","udcsreserved","parsed","service","prefix","number",NULL };
char *ict_csv_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number",NULL };
char *itu_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number",NULL };
char *itu_csv			[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number",NULL };
char *msc_sups_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","b_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *oss_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","a_number","b_number","duration","product","parsed","service","prefix","number",NULL };
char *orig_mms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *orig_msc_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *orig_pbx_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *orig_sms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *radious_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","username","acct_status_type","acct_session_time","acct_unique_session_id","framed_ip_address","nas_port_type","nas_port_id","calling_station_id","nas_ip_address","port_parsed","port_detail","delegated_ipv6_prefix","haap_lte_ipv4","haap_dsl_ipv4","ishybrid",NULL};
char *telecard_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","validity_date_start","validity_date_end","a_number","calling_address","b_number","duration","card_balance_contained","card_balance_consumed","card_number",NULL };
char *term_mms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","b_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *term_msc_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","b_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *term_sms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","g_number","ipaddress","b_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *udcs_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","version", "seqnum", "networkid", "eventclass", "networkprod", "action", "eventsourceid", "ndr", "aaddrplan", "nde", "baddrplan", "newdest", "newdaddrplan", "chargedparty", "date", "starttime", "duratunit", "duration", "secondunit", "amountofsus", "acategory", "priority", "numsuppserv", "suppserv1", "suppserv2", "suppserv3", "suppserv4", "suppserv5", "suppserv6", "suppserv7", "suppserv8", "numgroups", "origcellid", "callid", "inowner", "misc", "reserved", "parsed", "service", "prefix", "number",NULL };
char *udcslike_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","version", "seqnum", "networkid", "eventclass", "networkprod", "action", "eventsourceid", "ndr", "aaddrplan", "nde", "baddrplan", "newdest", "newdaddrplan", "chargedparty", "date", "starttime", "duratunit", "duration", "secondunit", "amountofsus", "acategory", "priority", "numsuppserv", "suppserv1", "suppserv2", "suppserv3", "suppserv4", "suppserv5", "suppserv6", "suppserv7", "suppserv8", "numgroups", "origcellid", "callid_hex", "inowner", "misc", "reserved", "parsed", "service", "prefix", "number",NULL };


CAT_DEFINITION categories[] = {
	{"email"      ,"<*>", email_fields       }, 
	{"gprs"       ,"|",   gprs_fields        }, 
	{"chronocard" ,",",   chronocard_fields  }, 
	{"ict"        ,"<*>", ict_fields         }, 
	{"ict_csv"    ,"|",   ict_csv_fields     }, 
	{"itu"        ,"|",   itu_fields         }, 
	{"itu_csv"    ,"|",   itu_csv            }, 
	{"msc_sups"   ,"|",   msc_sups_fields    }, 
	{"oss"        ,"|",   oss_fields         }, 
	{"orig_mms"   ,"|",   orig_mms_fields    }, 
	{"orig_msc"   ,"|",   orig_msc_fields    }, 
	{"orig_pbx"   ,"|",   orig_pbx_fields    }, 
	{"orig_sms"   ,"|",   orig_sms_fields    }, 
	{"radius"     ,"<*>", radious_fields     }, 
	{"telecard"   ,"|",   telecard_fields    }, 
	{"term_mms"   ,"|",   term_mms_fields    }, 
	{"term_msc"   ,"|",   term_msc_fields    }, 
	{"term_sms"   ,"|",   term_sms_fields    }, 
	{"udcs"       ,"<*>", udcs_fields        }, 
	{"udcslike"   ,"<*>", udcslike_fields    }, 
	{NULL,NULL,NULL          } 
};

long int anonymize_file(char *from,char *file,char *out_dir,char *category,char *subtype,int show_test)
{
 char fname[1024];
 char out_file[1024];

 char *anon_array[100];
 char anon_data[100][1024];

 sprintf(fname,"%s/%s",from,file);
 sprintf(out_file,"%s/%s",out_dir,file);
 if(strcmp(out_file+strlen(out_file)-3,".gz")==0) out_file[strlen(out_file)-3]=0;
 char *type;
 type = (strcmp(subtype,"default")==0) ? category : subtype;
// printf("\n-- anonymize: cat=%s subtype=%s type=%s [%s] ------\n",category,subtype,type,out_file);
 // find subtype fields
 int i=0;
 char **category_fields=NULL;
 char *category_sep=NULL;
 while(categories[i].name) {
 	if(!strcmp(categories[i].name,type)){
		category_fields = categories[i].field;
		category_sep = categories[i].sep;
//		printf("	field found!\n");
		break;
	};i++;
 };
 if(category_fields) {
 	char in_line[2048];
	FILE *in,*out;
	struct stat attr;
	stat(fname,&attr);
	int file_seconds = attr.st_mtime;
	double days_old=(now_seconds-file_seconds)/86400.0;
	// printf("Last modified time: %s days = %f \n", ctime(&attr.st_mtime),days_old);
	if(days_old > last_days) {
		printf("skip %s mod %f, access %f, status %f\n",fname,days_old, (now_seconds-attr.st_atime)/86400.0,(now_seconds-attr.st_ctime)/86400.0);
		return 0;
	}
	printf(" %s %f days old\n",fname,days_old);
	set_gunzipped(fname);	// gunzip to /tmp and return the temp file name
	// printf("anon file: %s\n",fname);
	// return;
	in = fopen(fname,"r");
	if(!show_test) out = fopen(out_file,"w+");
	int count1,i;
	for(i=0;category_fields[i];i++) count1=i;
	int line=0;
	while(fgets(in_line,2048,in)) {
		char last_char=in_line[strlen(in_line)-1];
		if(last_char<32)
		in_line[strlen(in_line)-1]=0; // remove newline

		char **out_array;
		int count=0;
		out_array = split_str_sarray(in_line,category_sep,&count);
//		print_sarray(stdout,out_array,category_sep);
//		printf("allocate %d\n",count);
		line++;
		if(count!=count1) {
			printf("line: %d count fields do not match !!!!!!!!!!!! error in %s count=%d count1=%d\n",line,fname,count,count1);
			printf("category %s subtype %s\n",category,subtype);
			printf("[%s]\n",in_line);
			return 0;
			// exit(1);
		};
		int i=0;
		for(i=0;category_fields[i];i++) {
//			printf("- %s\n",category_fields[i]);
			strcpy(anon_data[i],anonymize_field(out_array[i],category_fields[i]));;
			anon_array[i]=anon_data[i];
		};anon_array[i]=NULL;
		if(!show_test) print_sarray(out,anon_array,category_sep);
	};
	fclose(in);
	if(!show_test) {
	fclose(out);
	sync();
	stat(fname,&attr);
	long int old_size = attr.st_size;
	stat(out_file,&attr);
	long int new_size = attr.st_size;
	printf (" > %s oldsize = %ld new_size = %ld\n",file,old_size,new_size);
    char cmd[2048];
    int result;
    sprintf(cmd,"gzip %s 2> /dev/null",out_file);
    result = system(cmd);
    sync();
	sprintf(cmd,"%s.gz",out_file);
	stat(cmd,&attr);
	printf("  > gzipped size %ld\n",attr.st_size);
	return attr.st_size;
	} else {
		return 0;
	};
 } else {
 	printf("Skip category %s subtype %s\n",category,subtype);
 };
 return 0;
}

long int anon_done(char *source_dir,char *fname,int show_test)
{
 char anon_archive_dir[1024];
 int count;
 int result;
 char command[1024];
 long int file_size;
 char **fa=split_str_sarray(fname,".",&count);
 sprintf(anon_archive_dir,"%s/%s/queue/%s/archive",anon_log_dir,fa[1],fa[2]);
 sprintf(command,"mkdir -p %s",anon_archive_dir);
 result=system(command);
	// printf("	%d : anon_done: -> %s %s\n",result,anon_archive_dir,fname);
	file_size=anonymize_file(source_dir,fname,anon_archive_dir,fa[1],fa[2],show_test);
	// printf("	!!!ok\n");
	return file_size;
}

char subtype_dir[1024];
char archive_dir[1024];
char anon_archive_dir[1024];
char s[1024];

char *test1 = "20200101020400|KT_Trs.2020-01-01.csv|20200102010001|4229827|20190904000000|20210701235959|2531073127|Νομός Ροδόπης - ΚΡΑΤΗΤΗΡΙΑ, ΑΣΤΥΝΟΜΙΑ, ΛΕΣΧΗ, ΚΟΜΟΤΗΝΗ|6975694249|140|3.6|0.2|0508943966";


int main(int argc,char **argp)
{
	int result=0;
	struct stat t;
	char *single_category=NULL;
	long int max_limit=9000000000;	// 2GB
	int show_test = 0;
#if	TEST0
	while(int i=0;categories[i].name !=NULL;i++) {
		printf("%2d name=[%s] field [",i,categories[i].name);
		int j=0;
		while(categories[i].field[j]) {
			printf(" %s",categories[i].field[j++]);
		};
		printf("]\n");
	};
#endif
#if	TEST0
	char **a;
	a=split_str_sarray(check4,"|");
	printf("%s\n",check4);
	print_sarray(stdout,a,"|");
	free_sarray(a);
	return 0;
#endif
#if	TEST0
	char *in = "ΑΒΓδεζηθικλ 1213213";
	mask_string(in,3,'#');
	return 0;
#endif
	if(argc>1) {
		last_days=atof(argp[1]);
	};
	if(argc>2) {
		if(strcmp(argp[2],"all")) single_category=argp[2]; // if "all" leave it NULL
	};
	if(argc>3) {
		max_limit=atoi(argp[3])*1000;
	}
	printf("Convert the last %f days\n",last_days);
	bt_field_table=new_btree("table",0);
	int i;
	for(i=0;field_types[i].name;i++) {
		set_btival(bt_field_table,field_types[i].name,field_types[i].type);
	};

#if	TEST0
	char *anon;
	char *test="0030670790164198";
	anon = anon_g_number(test);
	printf("arand 0 [%s]-> [%s]\n",test,anon);
	return 1;
#endif
// get current time
	struct timeval      timev;
	gettimeofday(&timev, NULL);
	now_seconds = (int)timev.tv_sec;

// initialize anonymized directory
	int max_category=0;
	char **categories;
	if(single_category!=NULL) {
		categories = (char **)malloc(sizeof(char *)*2);
		categories[0]=single_category;
		max_category=1;
		printf("Version %s anonymize %s in %s result=%d\n",version,single_category,anon_log_dir,result);
	} else {
		categories = get_dir(log_dir,&max_category);
		sprintf(s,"rm -rf %s >/dev/null 2>/dev/null",anon_log_dir);
		result=system(s);
		printf("Version %s initialize dir %s result=%d\n",version,anon_log_dir,result);
	};
	// for each category!
	int category_i;
	for(category_i=0;category_i<max_category;category_i++) {
		int max_subtypes=0;
		long int category_size=0;
		if(!valid_log(categories[category_i])) continue;
		sprintf(subtype_dir,"%s/%s/queue",log_dir,categories[category_i]);
		char **subtypes = get_dir(subtype_dir,&max_subtypes);
//		printf("# %s -> %s %d subtypes\n",categories[i],subtype_dir,subtypes_num);
#if	1
		int subtype_i;
		for(subtype_i=0;subtype_i<max_subtypes;subtype_i++) {
			long int file_size=0;
			category_size=0;
//			printf("# %s %s\n",categories[category_i],subtypes[subtype_i]);
			sprintf(archive_dir,"%s/%s/archive",subtype_dir,subtypes[subtype_i]);
//			printf(" :  %s\n",archive_dir);
			sprintf(anon_archive_dir,"%s/%s/queue/%s/archive",anon_log_dir,categories[category_i],subtypes[subtype_i]);
			printf("# in %s\n",archive_dir);
			// create dir
			if(!show_test) {
				sprintf(s,"mkdir -p %s",anon_archive_dir);
				result=system(s);
			};
//			printf(" -> %s\n",anon_archive_dir);
			int max_archive=0;
			char **archives = get_dir(archive_dir,&max_archive);
			int l;
			for(l=0;l<max_archive;l++) {
				file_size=anonymize_file(archive_dir,archives[l],anon_archive_dir,categories[category_i],subtypes[subtype_i],show_test);
				category_size += file_size;
				if(category_size > max_limit) {
					printf("----->>> size limit break %s\n",size2str(category_size));
					break;
				};
			};
			clear_snames(archives,max_archive);
		};
		clear_snames(subtypes,max_subtypes);
#endif
#if	0
		char category_dir[1024];
		sprintf(category_dir,"%s/%s",log_dir,categories[category_i]);
		int max_cat_files=0;
		char **category_files = get_dir(category_dir,&max_cat_files);
		// printf("	check %s %d\n",category_dir,max_cat_files);
		int cat_file_i;
		long int file_size=0;
		for(cat_file_i=0;cat_file_i<max_cat_files;cat_file_i++) {
			char fdname[1024];
			long int file_size=0;
			category_size=0;
			sprintf(fdname,"%s/%s",category_dir,category_files[cat_file_i]);
			stat(fdname,&t);
			// printf("	file: %s\n",category_files[cat_file_i]);
			if((t.st_mode & S_IFMT) == S_IFDIR) {
			// printf("		%s\n",fdname);
				if(!strncmp(category_files[cat_file_i],"done",4)){
					printf(" : convert files in %s\n",fdname);

					int done0_max;
					char **done0_contents = get_dir(fdname,&done0_max);
					char done0_file[1024];
					int done0_i;
					for(done0_i=0;done0_i<done0_max;done0_i++){
						char done0_name[1024];
						sprintf(done0_name,"%s/%s",fdname,done0_contents[done0_i]);
						printf("  - %s\n",done0_name);

						stat(done0_name,&t);
						if((t.st_mode & S_IFMT) == S_IFDIR){
							int done1_max=0;
							char **done1_contents = get_dir(done0_name,&done1_max);
							printf("    convert files in subdir %s %d files\n",done0_name,done1_max);
							int done1_i;
							for(done1_i=0;done1_i<done1_max;done1_i++) {
								file_size = anon_done(done0_name,done1_contents[done1_i],show_test);
								category_size += file_size;
								if(category_size > max_limit) {
									printf("----->>> size limit break %s\n",size2str(category_size));
									break;
								};
							};
							// clear_snames(done1_contents,done1_max);
						} else {
							file_size = anon_done(fdname,done0_contents[done0_i],show_test);
							category_size += file_size;
							if(category_size > max_limit) {
								printf("----->>> size limit break %s\n",size2str(category_size));
								break;
							};
						};
					};
					// clear_snames(done0_contents,done0_max);

				};
			};
		};
		clear_snames(category_files,max_cat_files);
#endif
		printf("--- Category [%s] log files size is %s\n",categories[category_i],size2str(category_size));
	};
	printf("\nDONE %s created !!\n",anon_log_dir);
	return 0;
}

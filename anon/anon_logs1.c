#include <stdio.h>
#include <stdlib.h>
// #include <strings.h>
#include <string.h>
// #include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "alist.h"

#define MAX_CATEGORY	20
BTREE *bt_field_table;

#define TEST	1

#define BTABLE	1
#define NALLOC	1

enum {
	T_NONE,
	T_ANUM,
	T_PHONE,
	T_STRING,
	T_EMAIL,
	T_IDNUM,
	T_IPV4,
	T_IPV6,
	T_IDHEX,
	T_DOMAIN
};

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

char *crypt_idnum(char *v)
{
	return "crypt_idnum";
//	return v;
}

char *crypt_idhex(char *v)
{
	return "crypt_idhex";
//	return v;
}

char *crypt_anum(char *v)
{
	return "crypt_anum";
//	return v;
}

char *crypt_email(char *v)
{
	return "crypt_email";
//	return v;
}

char *anon_string(char *v)
{
	return "anon_string";
//	return v;
}

char *anon_phone(char *v)
{
	return "anon_phone";
//	return v;
}

char *anon_ipv4(char *v)
{
	return "192.168.555.666";
//	return v;
}

char *crypt_domain(char *v)
{
	return "domain.com";
//	return v;
}

char *anon_ipv6(char *v)
{
	return v;
}

char *anon_none(char *v)
{
	return v;
}

char *(*anon_function[])(char *c) = {
	anon_none,
	crypt_anum,
	anon_phone,
	anon_string,
	crypt_email,
	crypt_idnum,
	anon_ipv4,
	anon_ipv6,
	crypt_idhex,
	crypt_domain
};

#if	NUSE
char *field_names[][2] = {
{"ts"					, "none"  },	// 20200103123507
{"src_filename"			, "none"  },	// R_ALEXADRAS01B.0476.2001031251
{"src_filedate"			, "none"  },	// 20191231005959
{"src_filesize"			, "none"  },	// 5839
{"duration"				, "none"  },	// 0001:02:16, 0000:33:45 or 33 (telecard)
{"calling"				, "phone" },	// 2661047456, 2810242384
{"called"				, "phone" },	// 0017183198803, 00972505228370
{"servicenumber"		, "none"  },	// 8071122
{"chronocard_id"		, "idnum" },	// 0000000001673709
{"balanceafter"			, "none"  },	// 000002915048
{"balanceconsumed"		, "none"  },	// 000001391230
{"expirationoffset"		, "none"  },	// 00000183
{"recseparator"			, "none"  },	// 
{"parsed"				, "none"  },	// 8,9 or 2 or 11, 15 ...
{"service"				, "none"  },	// 0
{"prefix"				, "none"  },	// 16032
{"number"				, "phone" },	// 00171########, 69########, 13888
{"logfile_line"			, "none"  },	// 1234
{"mailserver_name"		, "none"  },	// pop03, rigel
{"mailserver_type"		, "none"  },	// dovecot, amavis, postfix-smtpd ..
{"queue"				, "none"  },	// F40D81D0008C (hex number)
{"session"				, "none"  },	// d60EqHemurbDqgAq
{"message_id"			, "string"},	// 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
{"resent_message_id"	, "string"},	// 
{"sender"				, "email" },	// notification@facebookmail.com
{"receiver"				, "email" },	// vm1977@otenet.gr
{"user"					, "email" },	// empty, xxxxx@xxx.com
{"status"				, "none"  },	// blank | sent | removed | bounced |defered |expired
{"domain_not_found"		, "none"  },	// Recipient address rejected | sender address rejected
{"helo"					, "domain"},	// ip or domain name or blank
{"ctladdr"				, "email" },	// username | email
{"orig_to"				, "email" },	// 
{"sasl_sender"			, "email" },	//
{"remote_ip"			, "ipv4"  },	//
{"content_ifnot_queue"	, "string"}, 	// lmtp(xxxx@otenet.gr) | (03908-22) Passed {CLEAN|BADHEADER} (it might have commas!)
{"subscriber_type"		, "none"  },	// 0
{"imei"					, "idnum" },	// 353420100443370
{"imsi"					, "idnum" },	// 208201402275281
{"calling_number"		, "phone" },	// 0033663139639
{"ipaddress"			, "ipv4"  },	// 
{"called_number"		, "phone" },	// 00302120001535
{"hc_network"			, "none"  },	// 274
{"ext_carrier"			, "none"  },	// 30971388006
{"serving_vplmn"		, "none"  },	// 0A180DE7
{"request_time"			, "none"  },	// 
{"start_time"			, "none"  },	// 20200103123507 (year,month,day,hour,min,sec)
{"end_time"				, "none"  },	// 20200103124052 (year,month,day,hour,min,sec)
{"call_duration"		, "none"  },	// 345 (seconds)
{"time_shift"			, "none"  },	// 10800
{"service_el_tp"		, "none"  },	// 3G
{"service_el_id"		, "none"  },	// ALEXADRAS01B
{"first_cell"			, "none"  },	// 202010300304553
{"dest_zone"			, "none"  },	// 597
{"call_flag"			, "none"  },	// O
{"service_qual"			, "none"  },	// NAP
{"record_id"			, "none"  },	// 75571013
{"session_scn"			, "none"  },	// 209734471
{"service_scn"			, "none"  },	// 04760000
{"service_id"			, "none"  },	// 11
{"service_mode"			, "none"  },	// Foreign_Relay
{"service_class"		, "none"  },	// 0 or wireless.twilio.com
{"service_type"			, "none"  },	// 0
{"service_prov_id"		, "none"  },	// Foreign_Relay
{"content_size"			, "none"  },	// 131233
{"content_size_upl"		, "none"  },	// 
{"content_size_dnl"		, "none"  },	//
{"service_status"		, "none"  },	// S
{"cdr_type"				, "none"  },	// 1,9
{"termination_ind"		, "none"  },	// C | 17
{"charge"				, "none"  },	// 117474
{"last_cell"			, "none"  },	// 202010300304553
{"master_msisdn"		, "none"  },	// 
{"incomingswitchid"		, "none"  },	// TKO,82
{"outgoingswitchid"		, "none"  },	// 82
{"recordcallid"			, "none"  },	// 011270006737657
{"linkfield"			, "none"  },	// 01
{"a_number"				, "phone" },	// 69XXXXXXXX
{"b_number"				, "phone" },	// 13888, 69XXXXXXXX
{"incomingtrunk"		, "none"  },	// COTKO1I
{"outgoingtrunk"		, "none"  },	// 1104_COSMOTE_ATH
{"incomingproduct"		, "none"  },	// HLP, IPTL
{"outgoingproduct"		, "none"  },	// IPTL
{"datavolume"			, "none"  },	// 
{"dataunit"				, "none"  },	// 
{"usersummarisation"	, "string"},	// 
{"udcsreserved"			, "none"  },	// 
{"product"				, "none"  },	// 00001 149-Αμεση συνδιάλεξη
{"username"				, "email" },	// isgo56@otenet.gr
{"acct_status_type"		, "none"  },	// Interim-Update
{"acct_session_time"	, "none"  },	// 0
{"acct_unique_session_id","idhex" },	// 1cdf65221e6c8b1b
{"framed_ip_address"	, "ipv4"  },	// 100.68.219.0
{"nas_port_type"		, "none"  },	// Wireless-XGP
{"nas_port_id"			, "none"  },	// #KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35#
{"calling_station_id"	, "string"},	// 0 0/0/36.3223 # KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35
{"nas_ip_address"		, "ipv4"  },	// 79.128.219.74
{"port_parsed"			, "none"  },	// IGNORED
{"port_detail"			, "none"  },	//
{"delegated_ipv6_prefix", "ipv6"  },	// 2a02:587:b483:8d00::/56
{"haap_lte_ipv4"		, "ipv4"  },	//
{"haap_dsl_ipv4"		, "ipv4"  },	//
{"ishybrid"				, "none"  },	// false
{"validity_date_start"	, "none"  },	// 20171110000000
{"validity_date_end"	, "none"  },	// 20190930235959
{"calling_address"		, "string"},	// Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
{"card_balance_contained","none"  },	// 3.36
{"card_balance_consumed", "none"  },	// 0.28
{"card_number"			, "idnum" },	// 0507974295
{"version"				, "none"  },	// 10
{"seqnum"				, "none"  },	// 100000955753943
{"networkid"			, "none"  },	// 03
{"eventclass"			, "none"  },	// 001
{"networkprod"			, "none"  },	// 00001
{"action"				, "none"  },	// 01
{"eventsourceid"		, "none"  },	// INAYKTH,IMS
{"ndr"					, "phone" },	// 6948457396
{"aaddrplan"			, "none"  },	// 1
{"nde"					, "phone" },	// 2351028148
{"baddrplan"			, "none"  },	// 1
{"newdest"				, "phone" },	// 2351028148
{"newdaddrplan"			, "none"  },	// 1
{"chargedparty"			, "none"  },	// U
{"date"					, "none"  },	// 20200101
{"starttime"			, "none"  },	// 063154
{"duratunit"			, "none"  },	// 1
{"secondunit"			, "none"  },	// 7
{"amountofsus"			, "none"  },	// 000000000000000
{"acategory"			, "none"  },	// 00
{"priority"				, "none"  },	// 00
{"numsuppserv"			, "none"  },	// 0
{"suppserv1"			, "none"  },	// 000
{"suppserv2"			, "none"  },	// 000
{"suppserv3"			, "none"  },	// 7
{"suppserv4"			, "none"  },	// 5900
{"suppserv5"			, "none"  },	// 01
{"suppserv6"			, "none"  },	// 000
{"suppserv7"			, "none"  },	// 000
{"suppserv8"			, "none"  },	// 000
{"numgroups"			, "none"  },	// 00
{"origcellid"			, "none"  },	// 000464
{"callid"				, "idnum" },	// 0753665600
{"inowner"				, "none"  },	// 2351028148
{"misc"					, "none"  },	// 00000000
{"reserved"				, "string"}	// 
};
#endif

FIELD_TYPE field_types[] = {
{"ts"					, T_NONE  },	// 20200103123507
{"src_filename"			, T_NONE  },	// R_ALEXADRAS01B.0476.2001031251
{"src_filedate"			, T_NONE  },	// 20191231005959
{"src_filesize"			, T_NONE  },	// 5839
{"duration"				, T_NONE  },	// 0001:02:16, 0000:33:45 or 33 (telecard)
{"calling"				, T_PHONE },	// 2661047456, 2810242384
{"called"				, T_PHONE },	// 0017183198803, 00972505228370
{"servicenumber"		, T_NONE  },	// 8071122
{"chronocard_id"		, T_IDNUM },	// 0000000001673709
{"balanceafter"			, T_NONE  },	// 000002915048
{"balanceconsumed"		, T_NONE  },	// 000001391230
{"expirationoffset"		, T_NONE  },	// 00000183
{"recseparator"			, T_NONE  },	// 
{"parsed"				, T_NONE  },	// 8,9 or 2 or 11, 15 ...
{"service"				, T_NONE  },	// 0
{"prefix"				, T_NONE  },	// 16032
{"number"				, T_PHONE },	// 00171########, 69########, 13888
{"logfile_line"			, T_NONE  },	// 1234
{"mailserver_name"		, T_NONE  },	// pop03, rigel
{"mailserver_type"		, T_NONE  },	// dovecot, amavis, postfix-smtpd ..
{"queue"				, T_NONE  },	// F40D81D0008C (hex number)
{"session"				, T_NONE  },	// d60EqHemurbDqgAq
{"message_id"			, T_STRING},	// 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
{"resent_message_id"	, T_STRING},	// 
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
{"imei"					, T_IDNUM },	// 353420100443370
{"imsi"					, T_IDNUM },	// 208201402275281
{"calling_number"		, T_PHONE },	// 0033663139639
{"ipaddress"			, T_IPV4  },	// 
{"called_number"		, T_PHONE },	// 00302120001535
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
{"master_msisdn"		, T_NONE  },	// 
{"incomingswitchid"		, T_NONE  },	// TKO,82
{"outgoingswitchid"		, T_NONE  },	// 82
{"recordcallid"			, T_NONE  },	// 011270006737657
{"linkfield"			, T_NONE  },	// 01
{"a_number"				, T_PHONE },	// 69XXXXXXXX
{"b_number"				, T_PHONE },	// 13888, 69XXXXXXXX
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
{"calling_station_id"	, T_STRING},	// 0 0/0/36.3223 # KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35
{"nas_ip_address"		, T_IPV4  },	// 79.128.219.74
{"port_parsed"			, T_NONE  },	// IGNORED
{"port_detail"			, T_NONE  },	//
{"delegated_ipv6_prefix", T_IPV6  },	// 2a02:587:b483:8d00::/56
{"haap_lte_ipv4"		, T_IPV4  },	//
{"haap_dsl_ipv4"		, T_IPV4  },	//
{"ishybrid"				, T_NONE  },	// false
{"validity_date_start"	, T_NONE  },	// 20171110000000
{"validity_date_end"	, T_NONE  },	// 20190930235959
{"calling_address"		, T_STRING},	// Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
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
{"ndr"					, T_PHONE },	// 6948457396
{"aaddrplan"			, T_NONE  },	// 1
{"nde"					, T_PHONE },	// 2351028148
{"baddrplan"			, T_NONE  },	// 1
{"newdest"				, T_PHONE },	// 2351028148
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
{"callid"				, T_IDNUM },	// 0753665600
{"inowner"				, T_NONE  },	// 2351028148
{"misc"					, T_NONE  },	// 00000000
{"reserved"				, T_STRING},	// 
{NULL,0}
};

char *anonymize_field(char *f,char *field_name)
{
	if(f[0]==0) return "";
	int type = btival(bt_field_table,field_name);
//	printf("anonymize_field: %s -> %d\n",field_name,type);
#if	1
//	char *(afun)(char *c) = anon_function[type];
//	return afun(f);
	return anon_function[type](f);
#else
	switch(type) {
		case T_NONE:	return anon_none(f);
		case T_STRING:	return anon_string(f);
		case T_PHONE:	return anon_phone(f);
		case T_EMAIL:	return crypt_email(f);
		case T_ANUM:	return crypt_anum(f);
		case T_IDHEX:	return crypt_idhex(f);
		case T_IDNUM:	return crypt_idnum(f);
		case T_IPV4:	return anon_ipv4(f);
		case T_IPV6:	return anon_ipv6(f);
	};
#endif
	return f;
}

void free_anon_array(char **array)
{
 char **a=array;
	while(*a) {
		free(*a);
		*a++;
	};
#if	!NALLOC
	free(array);
#endif
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
//	fprintf(stdout,"\n");
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
#if	NALLOC
	static char *array[100];
#endif
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
#if	!NALLOC
	array=(char **)malloc((count+2)*sizeof(char *));
#endif
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
char *gprs_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *chronocard_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","duration","calling","called","servicenumber","chronocard_id","balanceafter","balanceconsumed","expirationoffset","recseparator","parsed","service","prefix","number",NULL};
char *ict_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","usersummarisation","udcsreserved","parsed","service","prefix","number",NULL };
char *ict_csv_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number",NULL };
char *itu_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number",NULL };
char *itu_csv			[]=	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number",NULL };
char *msc_sups_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *oss_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","a_number","b_number","duration","product","parsed","service","prefix","number",NULL };
char *orig_mms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *orig_msc_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *orig_pbx_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *orig_sms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *radious_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","username","acct_status_type","acct_session_time","acct_unique_session_id","framed_ip_address","nas_port_type","nas_port_id","calling_station_id","nas_ip_address","port_parsed","port_detail","delegated_ipv6_prefix","haap_lte_ipv4","haap_dsl_ipv4","ishybrid",NULL};
char *telecard_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","validity_date_start","validity_date_end","calling","calling_address","called","duration","card_balance_contained","card_balance_consumed","card_number",NULL };
char *term_mms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *term_msc_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *term_sms_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn",NULL };
char *udsc_fields		[]=	{"ts","src_filename","src_filedate","src_filesize","version", "seqnum", "networkid", "eventclass", "networkprod", "action", "eventsourceid", "ndr", "aaddrplan", "nde", "baddrplan", "newdest", "newdaddrplan", "chargedparty", "date", "starttime", "duratunit", "duration", "secondunit", "amountofsus", "acategory", "priority", "numsuppserv", "suppserv1", "suppserv2", "suppserv3", "suppserv4", "suppserv5", "suppserv6", "suppserv7", "suppserv8", "numgroups", "origcellid", "callid", "inowner", "misc", "reserved", "parsed", "service", "prefix", "number",NULL };
char *udsclike_fields	[]=	{"ts","src_filename","src_filedate","src_filesize","version", "seqnum", "networkid", "eventclass", "networkprod", "action", "eventsourceid", "ndr", "aaddrplan", "nde", "baddrplan", "newdest", "newdaddrplan", "chargedparty", "date", "starttime", "duratunit", "duration", "secondunit", "amountofsus", "acategory", "priority", "numsuppserv", "suppserv1", "suppserv2", "suppserv3", "suppserv4", "suppserv5", "suppserv6", "suppserv7", "suppserv8", "numgroups", "origcellid", "callid", "inowner", "misc", "reserved", "parsed", "service", "prefix", "number",NULL };


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
	{"udcs"       ,"<*>", udsc_fields        }, 
	{"udcslike"   ,"<*>", udsclike_fields    }, 
	{NULL,NULL,NULL          } 
};

int anonymize_file(char *from,char *file,char *out_dir,char *category,char *subtype)
{
 char fname[1024];
 char out_file[1024];
#if	NALLOC
 char *anon_array[100];
#endif
 sprintf(fname,"%s/%s",from,file);
 sprintf(out_file,"%s/%s",out_dir,file);
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
//	printf("anon file: %s\n",fname);
	in = fopen(fname,"r");
	out = fopen(out_file,"w+");
	while(fgets(in_line,2048,in)) {
//		printf("%s\n",in_line);
		in_line[strlen(in_line)-1]=0; // remove newline
		char **out_array;
		int count=0;
		out_array = split_str_sarray(in_line,category_sep,&count);
//		print_sarray(stdout,out_array,category_sep);
//		printf("allocate %d\n",count);
#if	!NALLOC
		char **anon_array = (char **)malloc(sizeof(char *) * (count+2));
#endif
//		printf("allocated %d\n",count);
		int i=0;
		for(i=0;category_fields[i];i++) {
//			printf("- %s\n",category_fields[i]);
#if	NALLOC
			anon_array[i]=anonymize_field(out_array[i],category_fields[i]);
#else
			anon_array[i]=strdup(anonymize_field(out_array[i],category_fields[i]));
#endif
		};anon_array[i]=NULL;
		print_sarray(out,anon_array,category_sep);
#if	!NALLOC
		free_sarray(out_array);
#endif
//		free_anon_array(anon_array);
	};
	fclose(in);
	fclose(out);
	struct stat t;
	stat(fname,&t);
	long int old_size = t.st_size;
	stat(out_file,&t);
	long int new_size = t.st_size;
	printf (" > %s oldsize=%ld new_size=%ld\n",file,old_size,new_size);
	
 } else {
 	printf("Skip category %s subtype %s\n",category,subtype);
 };
}

char *log_dir = "logs";
char *anon_log_dir = "anonymous_logs_c";
char subtype_dir[1024];
char archive_dir[1024];
char anon_archive_dir[1024];
char s[1024];

char *test1 = "20200101020400|KT_Trs.2020-01-01.csv|20200102010001|4229827|20190904000000|20210701235959|2531073127|Νομός Ροδόπης - ΚΡΑΤΗΤΗΡΙΑ, ΑΣΤΥΝΟΜΙΑ, ΛΕΣΧΗ, ΚΟΜΟΤΗΝΗ|6975694249|140|3.6|0.2|0508943966";


int main(int argc,char **argp)
{
	int i=0;
	int result=0;
	struct stat t;
#if	TEST0
	while(categories[i].name !=NULL) {
		printf("%2d name=[%s] field [",i,categories[i].name);
		int j=0;
		while(categories[i].field[j]) {
			printf(" %s",categories[i].field[j++]);
		};
		printf("]\n");
		i++;
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
#if	BTABLE
	bt_field_table=new_btree("table",0);
	i=0;
	while(field_types[i].name) {
		set_btival(bt_field_table,field_types[i].name,field_types[i].type);
		i++;
	};
#endif
#if	TEST0
	char *anon;
	char *test="testtest";
	anon = anonymize_field(test,"newdest");
	printf("anon [%s]-> [%s]\n",test,anon);
	return 1;
#endif

// initialize anonymized directory
	sprintf(s,"rm -rf %s >/dev/null 2>/dev/null",anon_log_dir);
	result=system(s);
	printf("result=%d\n",result);
	int n=0;
	char **categories = get_dir(log_dir,&n);
	// for each category!
	for(i=0;i<n;i++) {
		int subtypes_num=0;
		sprintf(subtype_dir,"%s/%s/queue",log_dir,categories[i]);
		char **subtypes = get_dir(subtype_dir,&subtypes_num);
//		printf("# %s -> %s %d subtypes\n",categories[i],subtype_dir,subtypes_num);
		for(int j=0;j<subtypes_num;j++) {
//			printf("# %s %s\n",categories[i],subtypes[j]);
			sprintf(archive_dir,"%s/%s/archive",subtype_dir,subtypes[j]);
//			printf(" :  %s\n",archive_dir);
			sprintf(anon_archive_dir,"%s/%s/queue/%s/archive",anon_log_dir,categories[i],subtypes[j]);
			printf("# %s -> %s\n",archive_dir,anon_log_dir);
			// create dir			
			sprintf(s,"mkdir -p %s",anon_archive_dir);
			result=system(s);
//			printf(" -> %s\n",anon_archive_dir);
			int archives_num=0;
			char **archives = get_dir(archive_dir,&archives_num);
			int l=0;
			for(l=0;l<archives_num;l++) {
				anonymize_file(archive_dir,archives[l],anon_archive_dir,categories[i],subtypes[j]);
			};
		};
		clear_snames(subtypes,subtypes_num);
	};
	return 0;
}

// go version of anon_logs
//

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"bufio"
	"strings"
//	"path/filepath"
)

const (
	version = "1.2.3"
)

const (
	T_NONE=iota
	T_ANUM
	T_PHONE
	T_STRING
	T_EMAIL
	T_IDNUM
	T_IPV4
	T_IPV6
	T_IDHEX
	T_DOMAIN
)

const (
	DEFAULT_SALT=23
	MAX_NUM_DIGITS=10
	MAX_HEX_DIGITS=16
	MAX_ANUM_DIGITS=36
	MAX_UNUM_DIGITS=60
)

type FIELD_TYPE struct
{
 name string
 field_type int
}

type CAT_DEFINITION struct {
	name string
	sep string
	field []string
}


var num_digits = []int {'0','1','2','3','4','5','6','7','8','9'};
var hex_digits = []int {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
var anum_digits = []int { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
var unum_digits = []int { 
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'Α','Β','Γ','Δ','Ε','Ζ','Η','Θ','Ι','Κ','Λ','Μ','Ν','Ξ','Ο','Π','Ρ','Σ','Τ','Υ','Φ','Χ','Ψ','Ω' };


var field_types = []FIELD_TYPE {
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
{"message_id"			, T_STRING  },	// 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
{"resent_message_id"	, T_STRING  },	// 
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
{"content_ifnot_queue"	, T_STRING  }, 	// lmtp(xxxx@otenet.gr) | (03908-22) Passed {CLEAN|BADHEADER} (it might have commas!)
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
{"usersummarisation"	, T_ANUM  },	// 
{"udcsreserved"			, T_STRING  },	// 
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
{"calling_address"		, T_STRING  },	// Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
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
{"callid_hex"			, T_IDHEX },	// ABCD665600
{"inowner"				, T_NONE  },	// 2351028148
{"misc"					, T_NONE  },	// 00000000
{"reserved"				, T_STRING  },	// 
}


func anon_none (f string) string {
	return f
}

func anon_anum1 (f string) string {
	return anon_generic(f,anum_digits,".@?:",0,"",0)
}

func anon_anum (f string) string {
	var salt int = DEFAULT_SALT 
	var len = len(f)
	var sarray = []byte(f)
	var cc int = 0
	var key int = int(sarray[len-1]+17-48)
	for _,l := range sarray {
		salt += int(l)
	}
	// fmt.Println("anon_idnum: f=",f," salt0=",salt," k=",key);
	for i,l := range sarray {
		var ll int = int(l)
//		fmt.Println("	",i," l=",l)
	 	if(l=='.' || l=='@' || l=='?' || l==':') {
			
		} else {
			key = ll + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3)
			var val int = ((key % 177)-cc)%177
			for val<0 { val += MAX_ANUM_DIGITS }
			cc=val
			salt++
			if( salt >= 20857) {
				salt=0
			}
			key = key + key + (cc ^ ll) + salt
			sarray[i] = byte(anum_digits[ cc % MAX_ANUM_DIGITS])
		}
	};
	s:= string(sarray[:])
	// fmt.Println("	>",s)
	return s
}

func anon_phone1 (f string) string {
	return anon_generic(f,num_digits,"",0,"06",1)
}

func anon_phone (f string) string {
	var salt int = DEFAULT_SALT 
	var start int = 1
	var cc int = 0
	var val = 0
	if f == "DUMMY" { 
		return f
	}
	var sarray = []rune(f)
	var len = len(f)
	var key int = int(sarray[len-1]+17-48)

	for _,l := range sarray {
		salt += int(l)
	}
//	fmt.Println(" > salt=",salt,"k0=",key)
	for i,l := range sarray {
		var ll int = int(l)
//		fmt.Println("	",i," l=",l)
		if l>'9' || l<'0' {
//			fmt.Println(" > ",f)
			return f
		}
		if((l=='0' ||l=='6') && start==1) {
			sarray[i]=l
		} else {
//			fmt.Println(" -: v=",val,",ol=",l,",k=",key,",cc=",cc);
			start=0
			key = ll + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3)
			val = ((key % 177)-cc)%177
			for val<0 { val += MAX_NUM_DIGITS }
			cc=val
			salt++
			if( salt >= 20857) {
				salt=0
			}
			key = key + key + (cc ^ ll) + salt
			sarray[i] = rune(num_digits[ cc % MAX_NUM_DIGITS])
		}
	};
	s:= string(sarray[:])
//	fmt.Println(s)
	return s
}



func anon_ustring1 (f string) string {
	return anon_generic(f,unum_digits,".@/",0,"",0)
}

func anon_ustring (f string) string {
	var salt int = DEFAULT_SALT 
	var sarray = []rune(f)
	var flen = len(sarray)
	var cc int = 0
	var val int = 0
//	var l0 = sarray[flen-1]
//	fmt.Println("len=",flen," l0=",int(l0))
	var key int = int(sarray[flen-1])+17-48
	for _,l := range sarray {
		salt += int(l)
//		fmt.Println(" ",i,"v=",int(l),"salt=",salt)
	}
//	fmt.Println(" > len=",flen,",salt=",salt,"k0=",key)
	for i,l := range sarray {
		var ll int = int(l)
//		fmt.Println(" ",l,"->",ll)
	 	if( l == '.' || l=='@' || l=='/'){
//			fmt.Println("skip ",l);
			sarray[i]=l;
		} else {
			key = ll + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3)
			val = ((key % 177)-cc)%177
			for val<0 { val += MAX_UNUM_DIGITS }
			cc=val
			salt++
			if( salt >= 20857) {
				salt=0
			}
			key = key + key + (cc ^ ll) + salt
			// l = rune(unum_digits[ cc % MAX_UNUM_DIGITS])
			// fmt.Println(i,l,unum_digits[ cc % MAX_UNUM_DIGITS],unum_digits[ cc % MAX_UNUM_DIGITS])
			sarray[i] = rune(unum_digits[ cc % MAX_UNUM_DIGITS])
			
		}
	};
	s:= string(sarray[:])
//	fmt.Println(s)
	return s
}

func contains(l int,array []int) bool {
	for _,i := range array {
		if (i==l) {
			return true
		}
	}
	return false
}

// v -> input string
// chars -> array with output chars
// exept -> string with chars to not convert
// start -> start convert from this position
// ignore_begin -> string with chars that do not convert at the beginning of the line
// in_chars -> if not found in @chars then return original string

func anon_generic (v string, chars []int, except string,start int,ignore_begin string,in_char int) string {
	var max = len(chars)
	var salt int = DEFAULT_SALT 
	var sarray = []rune(v)
	var flen = len(sarray)
	var cc int = 0
	var val int = 0
//	fmt.Println("len=",flen," l0=",int(l0))
	var key int = int(sarray[flen-1])+17-48
	var begin = true
	for _,l := range sarray {
		salt += int(l)
//		fmt.Println(" ",i,"v=",int(l),"salt=",salt)
	}
//	fmt.Println(" > len=",flen,",salt=",salt,"k0=",key)
	for i,l := range sarray {
		var ll int = int(l)
//		fmt.Println(" ",l,"->",ll)
	 	if( strings.Index(except,string(l))>=0 || i<start || (strings.Index(ignore_begin,string(l))>-1 && begin) ){
//			fmt.Println("skip ",l);
			sarray[i]=l;
		} else {
			if(in_char==1) {
				if !contains(int(l),chars) {
					return v 
				}
			}
			begin = false
			key = ll + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3)
			val = ((key % 177)-cc)%177
			for val<0 { val += max }
			cc=val
			salt++
			if( salt >= 20857) {
				salt=0
			}
			key = key + key + (cc ^ ll) + salt
			// l = rune(unum_digits[ cc % MAX_UNUM_DIGITS])
			// fmt.Println(i,l,unum_digits[ cc % MAX_UNUM_DIGITS],unum_digits[ cc % MAX_UNUM_DIGITS])
			sarray[i] = rune(chars[cc % max])
			
		}
	};
	s:= string(sarray[:])
//	fmt.Println(s)
	return s
}


func test_string (f string) string {
	var sarray = []rune(f)
	for i,l := range sarray {
		fmt.Println(" ",i,"v=",int(l),"l=",l)
	}
	return f
}

func anon_email1 (f string) string {
	return anon_generic(f,anum_digits,"@.",0,"",0)
}

func anon_email (f string) string {
	var salt int = DEFAULT_SALT 
	var len = len(f)
	var sarray = []byte(f)
	var cc int = 0
	var key int = int(sarray[len-1]+17-48)
	for _,l := range sarray {
		salt += int(l)
	}
//	fmt.Println("anon_idnum: f=",f," salt0=",salt," k=",key);
	for i,l := range sarray {
		var ll int = int(l)
//		fmt.Println("	",i," l=",l)
	 	if(l=='.' || l=='@' ) {
			
		} else {
			key = ll + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3)
			var val int = ((key % 177)-cc)%177
			for val<0 { val += MAX_ANUM_DIGITS }
			cc=val
			salt++
			if( salt >= 20857) {
				salt=0
			}
			key = key + key + (cc ^ ll) + salt
			sarray[i] = byte(anum_digits[ cc % MAX_ANUM_DIGITS])
		}
	};
	s:= string(sarray[:])
//	fmt.Println(s)
	return s
}

func anon_idnum1 (f string) string {
	return anon_generic(f,num_digits,"",0,"0",1)
}

func anon_idnum (f string) string {
	var salt int = DEFAULT_SALT 
	var len = len(f)
	var sarray = []byte(f)
	var start int = 1
	var cc int = 0
	var key int = int(sarray[len-1]+17-48)
	for _,l := range sarray {
		salt += int(l)
	}
//	fmt.Println("anon_idnum: f=",f," salt0=",salt," k=",key);
	for i,l := range sarray {
		var ll int = int(l)
//		fmt.Println("	",i," l=",l)
		if(l=='0' && start==1) {
			
		} else {
			start=0
			key = ll + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3)
			var val int = ((key % 177)-cc)%177
			for val<0 { val += MAX_NUM_DIGITS }
			cc=val
			salt++
			if( salt >= 20857) {
				salt=0
			}
			key = key + key + (cc ^ ll) + salt
			sarray[i] = byte(num_digits[ cc % MAX_NUM_DIGITS])
		}
	};
	s:= string(sarray[:])
//	fmt.Println(s)
	return s
}

func anon_ipv4 (f string) string {
	return f
}

func anon_ipv6 (f string) string {
	return f
}

func anon_idhex1 (f string) string {
	return anon_generic(f,hex_digits,"",0,"0",0)
}

func anon_idhex (f string) string {
	var salt int = DEFAULT_SALT 
	var len = len(f)
	var sarray = []byte(f)
	var start int = 1
	var cc int = 0
	var key int = int(sarray[len-1]+17-48)
	for _,l := range sarray {
		salt += int(l)
	}
//	fmt.Println("anon_idnum: f=",f," salt0=",salt," k=",key);
	for i,l := range sarray {
		var ll int = int(l)
//		fmt.Println("	",i," l=",l)
		if(l=='0' && start==1) {
			
		} else {
			start=0
			key = ll + (key & 0x1FFFFFFF) ^ ((key >> 29) & 0x3)
			var val int = ((key % 177)-cc)%177
			for val<0 { val += MAX_HEX_DIGITS }
			cc=val
			salt++
			if( salt >= 20857) {
				salt=0
			}
			key = key + key + (cc ^ ll) + salt
			sarray[i] = byte(hex_digits[ cc % MAX_HEX_DIGITS])
		}
	};
	s:= string(sarray[:])
//	fmt.Println(s)
	return s
}

func crypt_domain (f string) string {
	return f
}

func substr(input string, start int, length int) string {
    asRunes := []rune(input)
    
    if start >= len(asRunes) {
        return ""
    }
    
    if start+length > len(asRunes) {
        length = len(asRunes) - start
    }
    
    return string(asRunes[start : start+length])
}

var anon_function = []func(string) string {
	anon_none,
	anon_anum,
	anon_phone1,
	anon_ustring1,
	anon_email1,
	anon_idnum1,
	anon_ipv4,
	anon_ipv6,
	anon_idhex1,
	crypt_domain,
}


var bt_field_table map[string]int

var log_dir = "logs"
var anon_log_dir = "anonymous_go"
var subtype_dir string
var archive_dir string
var anon_archive_dir string

var email_fields 		= []string  {"ts","src_filename","src_filedate","src_filesize","logfile_line","mailserver_name","mailserver_type","queue","session","message_id","resent_message_id","sender","receiver","user","status","domain_not_found","helo","ctladdr","orig_to","sasl_sender","remote_ip","content_ifnot_queue"}
var gprs_fields			= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" }
var chronocard_fields	= []string	{"ts","src_filename","src_filedate","src_filesize","duration","calling","called","servicenumber","chronocard_id","balanceafter","balanceconsumed","expirationoffset","recseparator","parsed","service","prefix","number"}
var ict_fields			= []string	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","usersummarisation","udcsreserved","parsed","service","prefix","number"}
var ict_csv_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number"}
var itu_fields			= []string	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number"}
var itu_csv				= []string	{"ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number"}
var msc_sups_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn"}
var oss_fields			= []string	{"ts","src_filename","src_filedate","src_filesize","a_number","b_number","duration","product","parsed","service","prefix","number"}
var orig_mms_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" }
var orig_msc_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" }
var orig_pbx_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" }
var orig_sms_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" }
var radious_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","username","acct_status_type","acct_session_time","acct_unique_session_id","framed_ip_address","nas_port_type","nas_port_id","calling_station_id","nas_ip_address","port_parsed","port_detail","delegated_ipv6_prefix","haap_lte_ipv4","haap_dsl_ipv4","ishybrid"}
var telecard_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","validity_date_start","validity_date_end","calling","calling_address","called","duration","card_balance_contained","card_balance_consumed","card_number"}
var term_mms_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn"}
var term_msc_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn"}
var term_sms_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn"}
var udsc_fields			= []string	{"ts","src_filename","src_filedate","src_filesize","version", "seqnum", "networkid", "eventclass", "networkprod", "action", "eventsourceid", "ndr", "aaddrplan", "nde", "baddrplan", "newdest", "newdaddrplan", "chargedparty", "date", "starttime", "duratunit", "duration", "secondunit", "amountofsus", "acategory", "priority", "numsuppserv", "suppserv1", "suppserv2", "suppserv3", "suppserv4", "suppserv5", "suppserv6", "suppserv7", "suppserv8", "numgroups", "origcellid", "callid", "inowner", "misc", "reserved", "parsed", "service", "prefix", "number"}
var udsclike_fields		= []string	{"ts","src_filename","src_filedate","src_filesize","version", "seqnum", "networkid", "eventclass", "networkprod", "action", "eventsourceid", "ndr", "aaddrplan", "nde", "baddrplan", "newdest", "newdaddrplan", "chargedparty", "date", "starttime", "duratunit", "duration", "secondunit", "amountofsus", "acategory", "priority", "numsuppserv", "suppserv1", "suppserv2", "suppserv3", "suppserv4", "suppserv5", "suppserv6", "suppserv7", "suppserv8", "numgroups", "origcellid", "callid_hex", "inowner", "misc", "reserved", "parsed", "service", "prefix", "number"}

var categories = []CAT_DEFINITION {
	{"email"      ,"<*>", email_fields       }, 
	{"gprs"       ,"|",   gprs_fields        }, 
	{"sms_gprs"   ,"|",   gprs_fields        }, 
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
}


//func split_str_sarray(str string,category_sep string){
//	array := strings.Split(str, category_sep)
//	count := len(array)
//	return array,count
//}

func anonymize_field(f string, field_name string) string {
	if len(f) == 0 {
		return f 
	}
	field_type := bt_field_table[field_name]
//	fmt.Println("af: ["+field_name,"] t=",field_type," [",f,"]")
	return anon_function[field_type](f)
}

func anonymize_file(from string,file string,out_dir string,category string,subtype string){
	fname := from+"/"+file
	out_file := out_dir+"/"+file
	// fmt.Println("	- ",file)
	var field_type string
	if subtype == "default" {
		field_type = category
	} else {
		field_type = subtype
	}

	var category_sep string
	var category_fields []string
	for _, cat := range categories {
		if cat.name == field_type {
			category_sep = cat.sep
			category_fields = cat.field
//			fmt.Println("- ",cat.name," separator ",category_sep,category_fields)
			break;
		}
	}
	count1 := len(category_fields)
//	fmt.Println("anon file: ",out_file," field count=",count1);

	fh, _ := os.Open(fname)


	out_fh, err := os.OpenFile(out_file, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		fmt.Println("Cannot create file ",out_file)
		return
	}
	datawriter := bufio.NewWriter(out_fh)
 
	scanner := bufio.NewScanner(fh)
	line := 0
    for scanner.Scan() {
	   	// fmt.Println("[",scanner.Text(),"]")
		// out_array,count := split_str_sarray(scanner.Text(),category_sep)
		out_array := strings.Split(scanner.Text(), category_sep)
		line++
		count := len(out_array)
		if count != count1 { 
			fmt.Println("line:",line," count fields do not match!!!!! ",count,count1,"in",fname )
			fmt.Println("category ",category,"subtype=",subtype);
			fmt.Println(line,scanner.Text(),"]")
			return
		}

		var anon_data string
		for i, field := range category_fields {
			anon_data = anonymize_field(out_array[i],field)
			out_array[i]=anon_data
		}

		out_line := strings.Join(out_array, category_sep)
		_,_ = datawriter.WriteString(out_line+"\n")
	}		
	fh.Close()
	datawriter.Flush()
	out_fh.Close()
	fi, _ := os.Stat(fname)
	old_size := fi.Size()
	fo, _ := os.Stat(out_file)
	new_size := fo.Size()

	fmt.Println(" >",file,"oldsize =",old_size,"new_size =",new_size)
}

var nuse = true

func anon_done(source_dir string,fname string) {
	fa := strings.Split(fname,".")
	// fmt.Println("		cat=",fa[1],"type=",fa[2])
	anon_archive_dir := anon_log_dir+"/"+fa[1]+"/queue/"+fa[2]+"/archive"
	err := os.MkdirAll(anon_archive_dir, 0755)
	if err != nil {
		fmt.Println("Cannot create directory ",anon_archive_dir)
		return;
	};
	// fmt.Println("	:","->",anon_archive_dir,fname)
	anonymize_file(source_dir,fname,anon_archive_dir,fa[1],fa[2])
}

func main() {
//	in := "This is ’ελληνικά γράμματα’"
//	l := anon_ustring(in)
//	fmt.Println("in [",in,"] -> [",l)
//	return
//	create the btree table
	bt_field_table = make(map[string]int)
	for _,f := range field_types {
		bt_field_table[f.name] = f.field_type
	}

	// initialize anonymized directory
	os.RemoveAll(anon_log_dir)
	fmt.Println("version",version,"initialize dir",anon_log_dir);

	category_files, err := ioutil.ReadDir(log_dir)
	if err!=nil {
		fmt.Println("Cannot read directory ",log_dir);
		return
	};

	// for each category
	for _, cat_dir := range category_files {
//		fmt.Println(" ", cat_dir.Name(), cat_dir.IsDir())
		subtype_dir = log_dir+"/"+cat_dir.Name()+"/queue"
//		fmt.Println("  subtype=",subtype_dir)
		subtypes, err := ioutil.ReadDir(subtype_dir);
		if err!=nil { 
			fmt.Println("Cannot read subdir ",subtype_dir)
			return
		}
		for _, subt := range subtypes {
//			fmt.Println("	- ",subt.Name())
			archive_dir = subtype_dir+"/"+subt.Name()+"/archive"
			anon_archive_dir = anon_log_dir+"/"+cat_dir.Name()+"/queue/"+subt.Name()+"/archive"
			fmt.Println("# in",archive_dir)
			err = os.MkdirAll(anon_archive_dir, 0755)
			if err != nil {
				fmt.Println("Cannot create directory ",anon_archive_dir)
				return;
			};			
			archives, _ := ioutil.ReadDir(archive_dir)
			for _, arch := range archives {
				anonymize_file(archive_dir,arch.Name(),anon_archive_dir,cat_dir.Name(),subt.Name())
			}
		}
// *******************************************************

		category_dir := log_dir+"/"+cat_dir.Name()
		category_dirs, err := ioutil.ReadDir(category_dir)
		if err!=nil {
			fmt.Println("Cannot read directory ",category_dir);
			return
		};

		for _, done_dir := range category_dirs {
			dname := done_dir.Name()
			fdname := category_dir+"/"+done_dir.Name()
			// fmt.Println(" ---: ",dname)
			// if is dir and start with done
			fileinfo, _ := os.Stat(fdname)
			if fileinfo.IsDir() {
				dname4 := substr(dname,0,4)
				// fmt.Println(" compare ",dname,"done")
				if(dname4 == "done") {
					fmt.Println(" : convert files in",fdname)
					done0_contents, _ := ioutil.ReadDir(fdname)
					for _, done0_file := range done0_contents {
						done0_name := fdname+"/"+done0_file.Name()
						fileinfo, _ := os.Stat(done0_name)
						fmt.Println("  -",done0_name)
						if fileinfo.IsDir() {
							// fmt.Println("	dir=",done0_name)
							done1_contents,_ := ioutil.ReadDir(done0_name)
							fmt.Println("    convert files in subdir",done0_name,len(done1_contents),"files")
							// last done subdir
							for _, done1_file := range done1_contents {
								done1_name := done1_file.Name()
								// fmt.Println("	-	log=",done1_name)
								anon_done(done0_name,done1_name)
								// anonymize_file(archive_dir,arch.Name(),anon_archive_dir,cat_dir.Name(),subt.Name())
							}
						} else { // split the name!
							// fmt.Println("	log=",done0_file.Name())
							anon_done(fdname,done0_file.Name())
						}
					}
				}
			}
		}
	}
	fmt.Println("\nDONE",anon_log_dir,"created !!")
}

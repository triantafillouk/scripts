#!/usr/local/bin/python3

# Anonymous logs

import os
import shutil
import pathlib
import sys

version="1.3.0"
log_dir = "logs"
anon_log_dir = 'anonymous_py'

categories_config = {
    "email"         : ["<*>",   [ "ts","src_filename","src_filedate","src_filesize","logfile_line","mailserver_name","mailserver_type","queue","session","message_id","resent_message_id","sender","receiver","user","status","domain_not_found","helo","ctladdr","orig_to","sasl_sender","remote_ip","content_ifnot_queue"]],
    "gprs"          : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "chronocard"    : [','  ,   [ "ts","src_filename","src_filedate","src_filesize","duration","calling","called","servicenumber","chronocard_id","balanceafter","balanceconsumed","expirationoffset","recseparator","parsed","service","prefix","number"]],
    "ict"           : ["<*>",   [ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","usersummarisation","udcsreserved","parsed","service","prefix","number" ]],
    "ict_csv"       : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number" ]],
    "itu"           : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number" ]],
    "itu_csv"       : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","incomingswitchid","outgoingswitchid","recordcallid","linkfield","duration","a_number","b_number","incomingtrunk","outgoingtrunk","incomingproduct","outgoingproduct","datavolume","dataunit","udcsreserved","parsed","service","prefix","number" ]],
    "msc_sups"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "oss"           : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","a_number","b_number","duration","product","parsed","service","prefix","number" ]],
    "orig_mms"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "orig_msc"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "orig_pbx"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "orig_sms"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "radius"        : ["<*>",   [ "ts","src_filename","src_filedate","src_filesize","username","acct_status_type","acct_session_time","acct_unique_session_id","framed_ip_address","nas_port_type","nas_port_id","calling_station_id","nas_ip_address","port_parsed","port_detail","delegated_ipv6_prefix","haap_lte_ipv4","haap_dsl_ipv4","ishybrid"]],
    "telecard"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","validity_date_start","validity_date_end","calling","calling_address","called","duration","card_balance_contained","card_balance_consumed","card_number" ]],
    "term_mms"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "term_msc"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "term_sms"      : ['|'  ,   [ "ts","src_filename","src_filedate","src_filesize","subscriber_type","imei","imsi","calling_number","ipaddress","called_number","hc_network","ext_carrier","serving_vplmn","request_time","start_time","end_time","call_duration","time_shift","service_el_tp","service_el_id","first_cell","dest_zone","call_flag","service_qual","record_id","session_scn","service_scn","service_id","service_mode","service_class","service_type","service_prov_id","content_size","content_size_upl","content_size_dnl","service_status","cdr_type","termination_ind","charge","last_cell","master_msisdn" ]],
    "udcs"          : ["<*>",   [ "ts","src_filename","src_filedate","src_filesize","version","seqnum","networkid","eventclass","networkprod","action","eventsourceid","ndr","aaddrplan","nde","baddrplan","newdest","newdaddrplan","chargedparty","date","starttime","duratunit","duration","secondunit","amountofsus","acategory","priority","numsuppserv","suppserv1","suppserv2","suppserv3","suppserv4","suppserv5","suppserv6","suppserv7","suppserv8","numgroups","origcellid","callid","inowner","misc","reserved","parsed","service","prefix","number" ]],
    "udcslike"      : ["<*>",   [ "ts","src_filename","src_filedate","src_filesize","version","seqnum","networkid","eventclass","networkprod","action","eventsourceid","ndr","aaddrplan","nde","baddrplan","newdest","newdaddrplan","chargedparty","date","starttime","duratunit","duration","secondunit","amountofsus","acategory","priority","numsuppserv","suppserv1","suppserv2","suppserv3","suppserv4","suppserv5","suppserv6","suppserv7","suppserv8","numgroups","origcellid","callid_hex","inowner","misc","reserved","parsed","service","prefix","number" ]]
}


def anon_none(f):
    return f

def anon_phone1(f):
    return f

def anon_anum1(f):
    return f

def anon_idnum1(f):
    return f

def anon_string1(f):
    return f

def anon_email1(f):
    return f

def anon_domain(f):
    return f

def anon_ipv4(f):
    return f

def anon_ipv6(f):
    return f

def anon_idhex1(f):
    return f

field_funcs_gen = {
    "ts"                        : anon_none,    # 20200103123507
    "src_filename"          : anon_none,    # R_ALEXADRAS01B.0476.2001031251
    "src_filedate"          : anon_none,    # 20191231005959
    "src_filesize"          : anon_none,    # 5839
    "duration"              : anon_none,    # 0001:02:16, 0000:33:45 or 33 (telecard)
    "calling"                   : anon_phone1,  # 2661047456, 2810242384
    "called"                    : anon_phone1,  # 0017183198803, 00972505228370
    "servicenumber"         : anon_none,    # 8071122
    "chronocard_id"         : anon_idnum1,  # 0000000001673709
    "balanceafter"          : anon_none,    # 000002915048
    "balanceconsumed"           : anon_none,    # 000001391230
    "expirationoffset"      : anon_none,    # 00000183
    "recseparator"          : anon_none,    # 
    "parsed"                    : anon_none,    # 8,9 or 2 or 11, 15 ...
    "service"                   : anon_none,    # 0
    "prefix"                    : anon_none,    # 16032
    "number"                    : anon_phone1,  # 00171########, 69########, 13888
    "logfile_line"          : anon_none,    # 1234
    "mailserver_name"           : anon_none,    # pop03, rigel
    "mailserver_type"           : anon_none,    # dovecot, amavis, postfix-smtpd ..
    "queue"                 : anon_none,    # F40D81D0008C (hex number)
    "session"                   : anon_none,    # d60EqHemurbDqgAq
    "message_id"                : anon_string1, # 3d9c2dc4-974a-4a9f-b6b1-22be8164481e_ebd4bd83-cd01-4832-bd87-5397606fcf28_20200525112519@brandsgalaxy.gr
    "resent_message_id"     : anon_string1, # 
    "sender"                    : anon_email1,  # notification@facebookmail.com
    "receiver"              : anon_email1,  # vm1977@otenet.gr
    "user"                  : anon_email1,  # empty, xxxxx@xxx.com
    "status"                    : anon_none,    # blank | sent | removed | bounced |defered |expired
    "domain_not_found"      : anon_none,    # Recipient address rejected | sender address rejected
    "helo"                  : anon_domain,  # ip or domain name or blank
    "ctladdr"                   : anon_email1,  # username | email
    "orig_to"                   : anon_email1,  # 
    "sasl_sender"               : anon_email1,  #
    "remote_ip"             : anon_ipv4,    #
    "content_ifnot_queue"       : anon_string1, # lmtp(xxxx@otenet.gr) | (03908-22) Passed {CLEAN|BADHEADER} (it might have commas!)
    "subscriber_type"           : anon_none,    # 0
    "imei"                  : anon_idnum1,  # 353420100443370
    "imsi"                  : anon_idnum1,  # 208201402275281
    "calling_number"            : anon_phone1,  # 0033663139639
    "ipaddress"             : anon_ipv4,    # 
    "called_number"         : anon_phone1,  # 00302120001535
    "hc_network"                : anon_none,    # 274
    "ext_carrier"               : anon_none,    # 30971388006
    "serving_vplmn"         : anon_none,    # 0A180DE7
    "request_time"          : anon_none,    # 
    "start_time"                : anon_none,    # 20200103123507 (year,month,day,hour,min,sec)
    "end_time"              : anon_none,    # 20200103124052 (year,month,day,hour,min,sec)
    "call_duration"         : anon_none,    # 345 (seconds)
    "time_shift"                : anon_none,    # 10800
    "service_el_tp"         : anon_none,    # 3G
    "service_el_id"         : anon_none,    # ALEXADRAS01B
    "first_cell"                : anon_none,    # 202010300304553
    "dest_zone"             : anon_none,    # 597
    "call_flag"             : anon_none,    # O
    "service_qual"          : anon_none,    # NAP
    "record_id"             : anon_none,    # 75571013
    "session_scn"               : anon_none,    # 209734471
    "service_scn"               : anon_none,    # 04760000
    "service_id"                : anon_none,    # 11
    "service_mode"          : anon_none,    # Foreign_Relay
    "service_class"         : anon_none,    # 0 or wireless.twilio.com
    "service_type"          : anon_none,    # 0
    "service_prov_id"           : anon_none,    # Foreign_Relay
    "content_size"          : anon_none,    # 131233
    "content_size_upl"      : anon_none,    # 
    "content_size_dnl"      : anon_none,    #
    "service_status"            : anon_none,    # S
    "cdr_type"              : anon_none,    # 1,9
    "termination_ind"           : anon_none,    # C | 17
    "charge"                    : anon_none,    # 117474
    "last_cell"             : anon_none,    # 202010300304553
    "master_msisdn"         : anon_none,    # 
    "incomingswitchid"      : anon_none,    # TKO,82
    "outgoingswitchid"      : anon_none,    # 82
    "recordcallid"          : anon_none,    # 011270006737657
    "linkfield"             : anon_none,    # 01
    "a_number"              : anon_phone1,  # 69XXXXXXXX
    "b_number"              : anon_phone1,  # 13888, 69XXXXXXXX
    "incomingtrunk"         : anon_none,    # COTKO1I
    "outgoingtrunk"         : anon_none,    # 1104_COSMOTE_ATH
    "incomingproduct"           : anon_none,    # HLP, IPTL
    "outgoingproduct"           : anon_none,    # IPTL
    "datavolume"                : anon_none,    # 
    "dataunit"              : anon_none,    # 
    "usersummarisation"     : anon_anum1,# 
    "udcsreserved"          : anon_string1, # 
    "product"                   : anon_none,    # 00001 149-Αμεση συνδιάλεξη
    "username"              : anon_email1,  # isgo56@otenet.gr
    "acct_status_type"      : anon_none,    # Interim-Update
    "acct_session_time"     : anon_none,    # 0
    "acct_unique_session_id"    : anon_idhex1,  # 1cdf65221e6c8b1b
    "framed_ip_address"     : anon_ipv4,    # 100.68.219.0
    "nas_port_type"         : anon_none,    # Wireless-XGP
    "nas_port_id"               : anon_none,    # #KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35#
    "calling_station_id"        : anon_anum1,   # 0 0/0/36.3223 # KERKY_SPARTYLA_D_HUA_11986 atm 0/2/0/27:8.35
    "nas_ip_address"            : anon_ipv4,    # 79.128.219.74
    "port_parsed"               : anon_none,    # IGNORED
    "port_detail"               : anon_none,    #
    "delegated_ipv6_prefix" : anon_ipv6,    # 2a02:587:b483:8d00::/56
    "haap_lte_ipv4"         : anon_ipv4,    #
    "haap_dsl_ipv4"         : anon_ipv4,    #
    "ishybrid"              : anon_none,    # false
    "validity_date_start"       : anon_none,    # 20171110000000
    "validity_date_end"     : anon_none,    # 20190930235959
    "calling_address"           : anon_string1,# Νομός Ηρακλείου - ΑΕΡΟΛΙΜΗΝ ΗΡΑΚΛΕΙΟΥ
    "card_balance_contained"    : anon_none,    # 3.36
    "card_balance_consumed" : anon_none,    # 0.28
    "card_number"               : anon_idnum1,  # 0507974295
    "version"                   : anon_none,    # 10
    "seqnum"                    : anon_none,    # 100000955753943
    "networkid"             : anon_none,    # 03
    "eventclass"                : anon_none,    # 001
    "networkprod"               : anon_none,    # 00001
    "action"                    : anon_none,    # 01
    "eventsourceid"         : anon_none,    # INAYKTH,IMS
    "ndr"                       : anon_phone1,  # 6948457396
    "aaddrplan"             : anon_none,    # 1
    "nde"                       : anon_phone1,  # 2351028148
    "baddrplan"             : anon_none,    # 1
    "newdest"                   : anon_phone1,  # 2351028148
    "newdaddrplan"          : anon_none,    # 1
    "chargedparty"          : anon_none,    # U
    "date"                  : anon_none,    # 20200101
    "starttime"             : anon_none,    # 063154
    "duratunit"             : anon_none,    # 1
    "secondunit"                : anon_none,    # 7
    "amountofsus"               : anon_none,    # 000000000000000
    "acategory"             : anon_none,    # 00
    "priority"              : anon_none,    # 00
    "numsuppserv"               : anon_none,    # 0
    "suppserv1"             : anon_none,    # 000
    "suppserv2"             : anon_none,    # 000
    "suppserv3"             : anon_none,    # 7
    "suppserv4"             : anon_none,    # 5900
    "suppserv5"             : anon_none,    # 01
    "suppserv6"             : anon_none,    # 000
    "suppserv7"             : anon_none,    # 000
    "suppserv8"             : anon_none,    # 000
    "numgroups"             : anon_none,    # 00
    "origcellid"                : anon_none,    # 000464
    "callid"                    : anon_idnum1,  # 0753665600
    "callid_hex"                : anon_idhex1,  # 0753665600
    "inowner"                   : anon_none,    # 2351028148
    "misc"                  : anon_none,    # 00000000
    "reserved"              : anon_string1,# 
}

def anonymize_file(from_dir,file,out_dir,category,subtype):
    fname = from_dir+"/"+file
    type = category if subtype=="default" else subtype

    print (" >> ",out_dir,category,subtype,">",type)
    if type in categories_config:
        sep = categories_config[type][0]
        #print (type,"is Found! separator is ",sep)
        fields = categories_config[type][1]
        field_count = len(fields)
        #for field in fields:
        #    print("         ",field)
        # a=txt.split(sep)
        filein = open(fname,'r')
        lines_in = filein.readlines()
        count=0
        for line in lines_in:
            count += 1
            aline = line.split(sep)
            count1 = len(aline)
            if count1!=field_count:
                print("field count ",field_count,"<>",count1)
                print(line)
                print(fields)
                return
            for f in fields:
                
        print (type,"is Found! separator is ",sep," fields ",field_count," lines ",count)
        
    else:
        print ("    skiping anonymize ",type," ",subtype)
        
        

# initialize anonymized directory
try:
    shutil.rmtree(anon_log_dir)
except:
    print("error while removing directory")

if not os.path.isdir(anon_log_dir):
    os.mkdir(anon_log_dir)
else:
    try:
        shutil.rmtree(anon_log_dir)
    except:
        print("error while removing directory")
    os.mkdir(anon_log_dir)

print("version ",version," initilize dir ",anon_log_dir)
i=0
for category_i in os.listdir(log_dir):
    subtype_dir = log_dir+"/"+category_i+"/queue"
    print(category_i)
    for subtype_i in os.listdir(subtype_dir):
        archives_dir=subtype_dir+"/"+subtype_i+"/archive"
        print ("  :",archives_dir)
        anon_archives_dir=anon_log_dir+"/"+category_i+"/queue/"+subtype_i+"/archive"
        # print ("  >",anon_archives_dir)
        os.makedirs(anon_archives_dir,exist_ok=True)
        for archive_i in os.listdir(archives_dir):
            print("     ",archive_i)
            anonymize_file(archives_dir,archive_i,anon_archives_dir,category_i,subtype_i)



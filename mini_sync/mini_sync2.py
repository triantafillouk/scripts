# mini_sync
# version 1.1

import subprocess
import json
import os.path
import time
from os import path
import s_time
import sys

def push_name(name):
    s.show_time(name,0)
    from_dir = local_root+"/"+name
    tmp1 = name.split("/",10)
    if len(tmp1) > 1:
        for i in range(0,len(tmp1)-1):
            to_dir = remote_root+"/"+tmp1[i]
    else:
        to_dir = remote_root
    print("--- Push "+name+" to "+to_dir+" host "+remote_host+" ----------------")

    try:
        cmd = 'rsync -avut '+from_dir+' '+remote_host+':'+to_dir
        if check_only == 0:
            subprocess.run(cmd,shell=True)
        else:
            print("cmd = [",cmd,"]")
    except:
        print ("cannot push ",from_dir)

def pull_name(name):
    s.show_time(name,0)
    from_dir = remote_host+":"+remote_root+"/"+name
    tmp1 = name.split("/",10)

    if len(tmp1) > 1:
        for i in range(0,len(tmp1)-1):
            to_dir = local_root+"/"+tmp1[i]
    else:
        to_dir = local_root
    print("--- Pull "+name+" to "+to_dir+" from host "+remote_host+" ----------------")

    try:
        cmd = "rsync -avut "+from_dir+" "+to_dir
        if check_only == 0:
            subprocess.run(cmd,shell=True)
        else:
            print("cmd = [",cmd,"]")
    except:
        print ("cannot pull ",from_dir)


# parameters
local_root=os.getenv("HOME")
list_file="dirs_list.json"
transfer_name=0

if len(sys.argv) < 2:
    print("Usage is :")
    print(" mini_sync pull")
    print(" mini_sync push")
    print(" mini_sync show")
    sys.exit(1)

if len(sys.argv) == 3:
    print("arg2 is "+sys.argv[2])
    transfer_name = sys.argv[2]
    # sys.exit(1)

command = sys.argv[1]

print("Initial home dir ",local_root)

# initialize timers
s = s_time.stime("Mini sync",command)

# check file "list_file"
s.show_time("Search for configuration file",1)
if not path.isfile(list_file):
    # print("no local file ",list_file)
    list_file = os.getenv("HOME")+"/.mini_sync/"+list_file
    if not path.isfile(list_file):
        print ("no common file named "+list_file)
        sys.exit(1)
    else:
        print("Found configuration in user config")
else:
    print("Found configuration file in local dir")

s.show_time("Read configuration file",0)
with open(list_file,'r') as f:
    conf_data = json.loads(f.read())
    conf_names = conf_data[0]
    local_root = conf_names["local_root"]
    remote_root = conf_names["remote_root"]
    remote_host = conf_names["remote_host"]
    check_only1 = 0
#    check_only = conf_names["check_only"]
    if "check_only" in conf_names.keys():
        check_only = conf_names["check_only"]

    dirs = conf_data[1]
    
    
if command == "show":
    print("Local root : ",local_root)
    print("remote root: ",remote_root)
    print("remote host: ",remote_host)
    print("check_only: ",check_only)
    print("check_only1: ",check_only1)
    for name in dirs:
        print(" - ",name)
    s.show_time("end",0)
    sys.exit(0)

if command == "pull":
    s.show_time("start pulling",0)
    print("Pulling")

    if transfer_name:
        pull_name(transfer_name)
    else:
        for name in dirs:
            pull_name(name)

if command == "push":
    s.show_time("start pushing",0)

    if transfer_name:
        push_name(transfer_name)
    else:
        for name in dirs:
            push_name(name)


s.show_time("end",0)



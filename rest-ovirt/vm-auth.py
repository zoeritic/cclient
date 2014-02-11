#!/usr/bin/python2

import sys
import os
import urllib
import ConfigParser
import getopt
import shutil
import subprocess
import time

import rest_helper.httplib2

from  rest_helper.rest_helper import *



class conf():
    M_PLAT=''
    M_IP=''
    MIPP=''
    DOMAIN='zjcloud.net'
    USER='admin@internal'
    PASSWD='zj@Cloud)(12'






def get_vm_id(Mipp,vmname):
    rt_xml=get_vms_by_name_xml(Mipp,vmname)
    doc=etree.fromstring(rt_xml)

    n_vm=len(doc.findall('vm'))
#    print "num of vm: ",n_vm
    if n_vm==0:
        print "WRONG VM"
        return None
    vmid=doc.find('vm').attrib['id']
    return vmid







def user_authorize(Mip,vmname,user=gc.USER,passwd=gc.PASSWD):
    Mipp=gc.URL_PROTO+Mip+gc.URL_PORT
    vmid=get_vm_id(Mipp,vmname)
#    print_ok("::"+user+" :: "+passwd+" VMID:"+vmid)
    #ticket_d=vm_set_ticket(Mipp,vmid)
    st,ticket_d=vm_set_ticket(Mipp,vmid,user,passwd)
#    print st,ticket_d
    sys.stdout.write(st+'\n')
    sys.stderr.write(st+'\n')
    if st=='401':
        print "AUTH-FAIL"
    else:
        print "AUTH-OK"
        sys.stderr.write("AUTH-OK")
    sys.stdout.flush()
    sys.stderr.flush()



mip=sys.argv[1]
vmname=sys.argv[2]
vmpasswd=sys.argv[3]


#sys.stdout.write("AUTH-OK")
#sys.stderr.write("AUTH-OK")
user_authorize(mip,vmname,vmname+"@"+conf.DOMAIN,vmpasswd)

#print"========="
#user_authorize("10.11.4.173","DBHZX503","DBHZX503@zjcloud.net","12345678")

sys.stdout.flush()
sys.stderr.flush()


sys.exit(0)

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
    DOMAIN=''
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



def get_vm_info(Mipp,vmname):
#    print_warn(Mipp+' '+vmname)
#    print_info("USER:"+conf.USER+"PASSWD:"+conf.PASSWD)
    rt_xml=get_vms_by_name_xml(Mipp,vmname)
    doc=etree.fromstring(rt_xml)

    n_vm=len(doc.findall('vm'))
#    print "num of vm: ",n_vm
    if n_vm==0:
        print "WRONG VM"
        return None
    vmid=doc.find('vm').attrib['id']

    vm_info_d={}
    res=Mipp+'/api/vms/'+vmid
    st,rt_xml=rest_request(res,'GET',user=conf.USER,passwd=conf.PASSWD)
#   print_info(rt_xml)
    vmnode=etree.fromstring(rt_xml)
#   vmnode=doc.find('vm')
#    print_info(vmnode)
    vmstate=vmnode.find('status').find('state').text

    display_nd=vmnode.find('display')
#---
    host_ip0=display_nd.find('address')
    if host_ip0 is None:
        host_ip=''
    else:
        host_ip=host_ip0.text
    #fi
    host_port0=display_nd.find('port')
    if host_port0 is None:
        host_port=''
    else:
        host_port=host_port0.text
    #fi
    host_sport0=display_nd.find('secure_port')
    if host_sport0 is None:
        host_sport=''
    else:
        host_sport=host_sport0.text
    #fi

    host_cert=display_nd.find('certificate')
    host_cert_subject=''
    if not host_cert is None:
        subject=host_cert.find('subject')
        if not subject is None:
            host_cert_subject=subject.text
#---
    domain=vmnode.find('domain').find('name').text
    vm_info_d['vm']=vmname
    vm_info_d['state']=vmstate
    vm_info_d['id']=vmid
    vm_info_d['host']=host_ip
    vm_info_d['port']=host_port
    vm_info_d['sport']=host_sport
    vm_info_d['domain']=domain
    vm_info_d['host_cert_subject']=host_cert_subject
    return vm_info_d




def get_state(Mip,vmname):
    Mipp=gc.URL_PROTO+Mip+gc.URL_PORT
    vminfo=get_vm_info(Mipp,vmname)
    stat=vminfo['state']
    if not stat is None:
        sys.stderr.write("STATE-OK"+"::::["+stat+"]::::\n")
        sys.stderr.write("OPER-OK"+"::::["+stat+"]::::\n")
        sys.stdout.write(vminfo['state'])
    else:
        sys.stdout.write("VMINFO is None")
        sys.stderr.write("VMINFO is None")
    sys.stdout.flush()
    sys.stderr.flush()


Mip=sys.argv[1]
vmname=sys.argv[2]

get_state(Mip,vmname)



sys.exit(0)



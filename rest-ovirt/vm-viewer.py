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

    FULL_SCREEN=True
#    CONFIG_FILE='conf_vm_console.ini'
    CERTS_DIR='rest-ovirt/.certs/'
    PERM_PATH='~/.spicec/spice_truststore.pem'

    CONSOLE='spicy'





def get_ca_crt(Mipp):
#	Mipp='https://'+ip+':8443'
    URL=Mipp+'/ca.crt'
    page=urllib.urlopen(URL)
    crt=page.read()
#    print crt
    ip=Mipp[8:-5]
    crt_file=conf.CERTS_DIR+ip+'_ca.crt'
    print "crt_file: "+crt_file
    f=open(crt_file,'w')
    f.write(crt)
    f.close()
#    shutil.copyfile(crt_file,conf.PERM_PATH)

    syscmd=' /usr/bin/cp '+crt_file+" "+conf.PERM_PATH
    print_info(syscmd)
    rt=os.system(syscmd)
    if rt!=0:
        print_err("Copy Cert_File Failed")
    print_ok("certification saved in [%s]"%crt_file)
    return crt_file



def get_vm_info(Mipp,vmname):
#    print_warn(Mipp+' '+vmname)
#    print_info("USER:"+conf.USER+"PASSWD:"+conf.PASSWD)
    rt_xml=get_vms_by_name_xml(Mipp,vmname)
#    print rt_xml
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
#    print st
#    print_info(rt_xml)
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
#    print vm_info_d
    return vm_info_d



def open_viewer(Mip,vmname,user=gc.USER,passwd=gc.PASSWD,C='spicy',options=None):
#    f4=Mhm2Mip(M)
#    Mipp=gen_ip(f4)
    Mipp=gc.URL_PROTO+Mip+gc.URL_PORT
    cert_file=get_ca_crt(Mipp)
    vm_info_d=get_vm_info(Mipp,vmname)
    vmid=vm_info_d['id']
    host=vm_info_d['host']
    port=vm_info_d['port']
    sport=vm_info_d['sport']
    host_cert_subject=vm_info_d['host_cert_subject']

    print_warn("::"+user+" :: "+passwd)
    #ticket_d=vm_set_ticket(Mipp,vmid)
    st,ticket_d=vm_set_ticket(Mipp,vmid,user,passwd)
    if st[0]!='2':
        sys.exit(-2)
    if ticket_d['state']!='complete':
        print_err("Set Ticket for VM: "+vmname)
        sys.exit(-1)
    #fi
    passwd=ticket_d['ticket_value']

    if C=='remote-viewer':
	CMD='''remote-viewer --spice-ca-file=%(cert_file)s --spice-host-subject '%(subject)s' spice://%(host)s/?port=%(port)s\&tls-port=%(sport)s\&password=%(passwd)s '''%{'cert_file':cert_file,'subject':host_cert_subject,'host':host,'port':port,'sport':sport,'passwd':passwd}
    elif C=='spicy':
	CMD='''spicy -h %(host)s -s %(sport)s  -w %(passwd)s '''%{'host':host,'sport':sport,'passwd':passwd}
    else:
        CMD='echo WRONG VIEWER!!'
    #fi
    if conf.FULL_SCREEN:
        CMD=CMD+" "+"-f "
    #fi
    if not options is None:
        CMD=CMD+" "+options
    #fi
    print CMD
    sys.stderr.write("\nOPER-OK::::["+C+"]::::\n")
    sys.stderr.flush()
    sys.stdout.flush()
    rv=os.system(CMD)


Mip=sys.argv[1]
vmname=sys.argv[2]
user=sys.argv[2]
passwd=sys.argv[3]

open_viewer(Mip,vmname,user+"@zjcloud.net",passwd,'remote-viewer')

sys.exit(0)

#!/usr/bin/python2

import sys
import os
import urllib
import ConfigParser
import getopt
import shutil
import subprocess
import time

from  rest_helper.rest_helper import *



class conf():
    M_PLAT=''
    M_IP=''
    MIPP=''
    DOMAIN=''
    USER='admin@internal'
    PASSWD='zj@Cloud)(12'





def get_ca_crt(Mipp):
#	Mipp='https://'+ip+':8443'
    URL=Mipp+'/ca.crt'
    page=urllib.urlopen(URL)
    crt=page.read()
    print crt
    ip=Mipp[8:-5]
    crt_file=conf.CERTS_DIR+ip+'_ca.crt'
    f=open(crt_file,'w')
    f.write(crt)
    f.close()
#    shutil.copyfile(crt_file,conf.PERM_PATH)

    syscmd=' /usr/bin/cp '+crt_file+conf.PERM_PATH
    print_info(syscmd)
    rt=os.system(syscmd)
    if rt!=0:
        print_err("Copy Cert_File Failed")
    print_ok("certification saved in [%s]"%crt_file)
    return crt_file


def do_SSH(HIP,cmd='hostname',user='root',password='redhat'):
#    CMD='ssh '+user+'@'+HIP+' '+cmd
    print_ok("H: "+HIP)
    print_ok('Executing: '+cmd)
    EXPECT_SSH_CMD='''
    expect  -c '
    set ip %(IP)s
    set password %(PASSWD)s
    set timeout 15
    spawn ssh root@$ip
    expect {
    "*yes/no" {send "yes\\r";exp_continue}
    "*password:" {send "$password\\r"}
    }
    expect "*]#"
    send "%(CMD)s\\r"
    expect "*]#"
    send "exit\\r"
    '

    '''%{'IP':HIP,'PASSWD':password,'CMD':cmd}

 #   print EXPECT_SSH_CMD
#    sys.exit(0)
    p=subprocess.Popen(EXPECT_SSH_CMD,shell=True,stdout=subprocess.PIPE,close_fds=True)
    SSH_OUT=p.stdout.read().strip()
#    print '======================='
#    print_info(SSH_OUT)
#    print "-----------------------"
#    vdsinfo=re.search(r"vdsClient.*\ni\(.*\)[",SSH_OUT).group(0)
    return SSH_OUT


def kill_vm_by_name(hip,vmname):
    CMD=r"vdsClient -s 0 list table |grep "+vmname+r' |awk \"{print \\\$2}\" |xargs kill -9 '
    rt=do_SSH(hip,CMD)
    return '222'
#    print rt





def get_vm_info(Mipp,vmname):
#    print_warn(Mipp+' '+vmname)
    rt_xml=get_vms_by_name_xml(Mipp,vmname)
#    print rt_xml
    if rt_xml is None:
        print "NoneXML"
        return None
    doc=etree.fromstring(rt_xml)
    vmid=doc.find('vm').attrib['id']

    vm_info_d={}
    res=Mipp+'/api/vms/'+vmid
    st,rt_xml=rest_request(res,'GET')
    sys.stderr.write(st)
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



def usage():
    message='''

Usage %s [OPTION.] VM
        -m | --m-plat M       \t: Set the Rhevm name.
        -l | --address ip     \t: ip of RhevM.

        --up    | -U              \t: power on VM.
        --down  | -D              \t: shutdown VM.
        --kill  | -K              \t: force kill VM.
        --state | -S

        -h | --help           \t: Print this Message.

for Rhevm 3.1 or Ovirt 3.1
3.2 not suitable[vm_set_ticket]
'''%sys.argv[0]
    print message




def main():
    try:
        opts,args=getopt.getopt(sys.argv[1:],'UDKm:l:hS',['m-plat=','address=','help','up','down','kill','state'])
    except getopt.GetoptError as err:
        print_err(err)
        usage()
    #yrt
    VMs=args[:]
    if len(VMs)==0:
        print_info("No VM was given..")
        sys.exit(-1)
    elif len(VMs)!=1:
        print_info("More than One VM was given,Only the First VM will Open..")
#        sys.exit(-2)
    #fi
    VM=VMs[0]
#    print_info(VM)


    for o,a in opts:
        if o in ('-m','--m-plat'):
            conf.M_PLAT=a
            conf.M_IP=gc.URL_PRE+Mhm2Mip(conf.M_PLAT)
            conf.MIPP=gen_ip(Mhm2Mip(conf.M_PLAT))
        elif o in ('-l','--address'):
            conf.M_IP=a
            conf.MIPP=gc.URL_PROTO+conf.M_IP+gc.URL_PORT
        elif o in ('-U','--up'):
            vmid=get_vm_info(conf.MIPP,VM)['id']
            st=vm_start(conf.MIPP,vmid)
            if st[0]=='2':
#                sys.stdout.write("::START")
                sys.stderr.write("OPER-OK"+"::::[START]::::")
#            sys.exit(0)
        elif o in ('-D','--down'):
            vmid=get_vm_info(conf.MIPP,VM)['id']
            st=vm_shutdown(conf.MIPP,vmid)
            if st[0]=='2':
#                sys.stdout.write("::SHUTDOWN")
                sys.stderr.write("OPER-OK"+"::::[SHUTDOWN]::::")
#            sys.exit(0)
        elif o in ('-K','--kill'):
            hip=get_vm_info(conf.MIPP,VM)['host']
            st=kill_vm_by_name(hip,VM)
            if st[0]=='2':
#                sys.stdout.write("::FORCE-KILL")
                sys.stderr.write("OPER-OK"+"::::[FORCE-KILL]::::")
#            sys.exit(0)
        elif o in ('-S','--state'):
            state=get_vm_info(conf.MIPP,VM)['state']
            if state:
                sys.stderr.write("OPER-OK STATE-OK"+"::::["+state+"]::::")

        elif o in ('-h','--help'):
            usage()
#            sys.exit(0)
        else:
            usage()
#            sys.exit(1)
        #fi
    #rof

    sys.stdout.flush()
    sys.stderr.flush()



if __name__ == '__main__':
    main()


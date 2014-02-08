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
    CONFIG_FILE='conf_vm_console.ini'
    CERTS_DIR='/root/.certs/'
    PERM_PATH=' ~/.spicec/spice_truststore.pem'

    CONSOLE='spicy'






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
    rt_xml=rest_request(res,'GET',user=conf.USER,passwd=conf.PASSWD)
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





def user_authorize(Mip,vmname,user=gc.USER,passwd=gc.PASSWD):
#    f4=Mhm2Mip(M)
#    Mipp=gen_ip(f4)
    Mipp=gc.URL_PROTO+Mip+gc.URL_PORT

    print_info(Mipp)
#    cert_file=get_ca_crt(Mipp)
#    vm_info_d=get_vm_info(Mipp,vmname)
    vmid=get_vm_id(Mipp,vmname)
#    vmid=vm_info_d['id']
#    host=vm_info_d['host']
#    port=vm_info_d['port']
#    sport=vm_info_d['sport']
#    host_cert_subject=vm_info_d['host_cert_subject']


    print_ok("::"+user+" :: "+passwd+" VMID:"+vmid)
    #ticket_d=vm_set_ticket(Mipp,vmid)
    ticket_d=vm_set_ticket(Mipp,vmid,user,passwd)





user_authorize("10.11.4.173","DBHZX501","DBHZX501@zjcloud.net","12345678")
print"========="
user_authorize("10.11.4.173","DBHZX502","DBHZX502@zjcloud.net","12345678")
print"========="
user_authorize("10.11.4.173","DBHZX503","DBHZX503@zjcloud.net","12345678")







def usage():
    message='''

Usage %s [OPTION.] VM
        -m | --m-plat M       \t: Set the Rhevm name.
        -l | --address ip     \t: ip of RhevM.
        -u | --usr loginname  \t: User name for Login.
        -p | --password PASSWD\t: Password with user.

        -d | --get-domain     \t: get domain.
        -s | --get-vm-state   \t: get vm's status.
        --up                  \t: power on VM.
        --down                \t: shutdown VM.
        --kill                \t: force kill VM.

        -f | --full           \t: Open Console full screen.
        -h | --help           \t: Print this Message.

for Rhevm 3.1 or Ovirt 3.1
3.2 not suitable[vm_set_ticket]
'''%argv[0]
    print message




def main():
    try:
        opts,args=getopt.getopt(sys.argv[1:],'AUDKsm:l:fhc:SRu:p:d',['get-vm-state','authorize','get-domain','m-plat=','address=','full','help','console=','spicy','remote-viewer','user=','password=','up','down','kill'])
    except getopt.GetoptError as err:
        print_err(err)
        usage()
    #yrt
#    if not os.path.exists(conf.CONFIG_FILE):
#        gen_conf(conf.CONFIG_FILE)
#        sys.exit(0)
#    else:
#        read_conf(conf.CONFIG_FILE,conf)
#    #fi
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
        elif o in ('-d','--get-domain'):
            print get_domain(conf.MIPP)
#            time.sleep(3)
#            print "ccloud.net"
#            sys.stderr.write("ERROR from vm-helper.py")
            sys.exit(0)
        elif o in ('-A','--authorize'):
            user_authorize(conf.MIPP,conf.VM,conf.USER,conf.PASSWD)
            sys.exit(0)
        elif o in ('-s','--get-vm-state'):
            info= get_vm_info(conf.MIPP,VM)
            if None is info:
                print "VM:",VM,"does not exists.."
            else:
                print info['state']
            sys.exit(0)
        elif o in ('-U','--up'):
#            sys.stdout.write("UUPP")
#            sys.stderr.write("{{ERROR::UP}}")
#            sys.exit(0)
            info= get_vm_info(conf.MIPP,VM)
            if None is info:
                print "VM:",VM,"does not exists.."
            else:
                vmid=info['id']
                vm_start(conf.MIPP,vmid)
            sys.exit(0)
        elif o in ('-D','--down'):
            info= get_vm_info(conf.MIPP,VM)
            if None is info:
                print "VM:",VM,"does not exists.."
                sys.stderr.write("Bye..")
            elif info['state']=='down':
                print "VM:",VM,"is down Already!!"
                sys.stderr.write("Bye..")
            else:
                vmid=info['id']
                vm_stop(conf.MIPP,vmid)
            sys.exit(0)
        elif o in ('-K','--kill'):
            info= get_vm_info(conf.MIPP,VM)
            if None is info:
                print "VM:",VM,"does not exists.."
                sys.stderr.write("Bye..")
            elif info['state'] == 'down':
                print "VM:",VM,"is down Already!!"
                sys.stderr.write("Bye..")
            else:
                hip=info['host']
                kill_vm_by_name(hip,VM)
            sys.exit(0)
        elif o in ('-u','--user'):
            conf.USER=a
        elif o in ('-p','--password'):
            conf.PASSWD=a
        elif o in ('-f','--full'):
            conf.FULL_SCREEN=True
        elif o in ('-c','--console'):
            conf.CONSOLE=a
        elif o in ('-S','--spicy'):
            conf.CONSOLE='spicy'
        elif o in ('-R','--remote-viewer'):
            conf.CONSOLE='remote-viewer'
        elif o in ('-h','--help'):
            usage()
            sys.exit(0)
        else:
            usage()
            sys.exit(1)
	#fi
    #rof


    viewer_open(conf.M_IP,VM,user=conf.USER,passwd=conf.PASSWD,C=conf.CONSOLE)



#if __name__ == '__main__':
#    main()


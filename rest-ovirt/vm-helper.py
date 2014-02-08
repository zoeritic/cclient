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
    USER=''
    PASSWD=''

    FULL_SCREEN=True
    CONFIG_FILE='conf_vm_console.ini'
    CERTS_DIR='/root/.certs/'
    PERM_PATH=' ~/.spicec/spice_truststore.pem'

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

    syscmd=' /usr/bin/cp '+crt_file+conf.PERM_PATH
    print_info(syscmd)
    rt=os.system(syscmd)
    if rt!=0:
        print_err("Copy Cert_File Failed")
    print_ok("certification saved in [%s]"%crt_file)
    return crt_file


def do_SSH(HIP,cmd='hostname',user='root',password='redhat'):
#    CMD='ssh '+user+'@'+HIP+' '+cmd
#    print_ok("H: "+HIP)
#    print_ok('Executing: '+cmd)
    print "Object Host:",HIP
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
#    print rt






def get_domain(Mipp):
#    pass
    uri=Mipp+r'/api/domains'
    st,rt_xml=rest_request(uri,'GET')
    doc=etree.fromstring(rt_xml)
    domains=doc.findall('domain')
    for domain in domains:
        dn=domain.find('name').text
        if dn=='internal':
            continue
        return dn



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
    print_info(rt_xml)
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



def viewer_open(Mip,vmname,C,user=gc.USER,passwd=gc.PASSWD,options=None):
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
    ticket_d=vm_set_ticket(Mipp,vmid,user,passwd)

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
    print_ok(CMD)
    rv=os.system(CMD)


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
            user_auth(conf.MIPP,conf.USER,conf.PASSWD)
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



if __name__ == '__main__':
    main()


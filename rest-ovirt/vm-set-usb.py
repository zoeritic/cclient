#!/usr/bin/python2
import os
import sys
import re
import ConfigParser
from rest_helper.rest_helper import *
import getopt



class conf():
    M_PLAT=''
    SET_ALL=False
    #ENABLED=False
    TYPE='Native'
    CONFIG_FILE='conf_usb_vm.ini'





def gen_conf_file(filename):

    config=ConfigParser.RawConfigParser()
    config.add_section('CONFIG')
    config.set('CONFIG','M_PLAT',conf.M_PLAT)
    config.set('CONFIG','SET_ALL',conf.SET_ALL)
    #config.set('CONFIG','ENABLED',conf.ENABLED)
    config.set('CONFIG','TYPE',conf.TYPE)
    
    with open(filename,'wb') as configfile:
        config.write(configfile)

    print_info( "----------------------------------")
    print_info( "[CONFIG]")
    print_info( "M_PLAT    =\t"+conf.M_PLAT)
    print_info( "SET_ALL   =\t"+str(conf.SET_ALL))
    #print "ENABLED   =\t",conf.ENABLED
    print_info( "TYPE      =\t"+conf.TYPE)
    print_info( "----------------------------------")



def read_conf_file(filename,conf):
    config=ConfigParser.RawConfigParser()

    config.read(filename)
    configc=config


    conf.M_PLAT=configc.get('CONFIG','M_PLAT')
    conf.SET_ALL=configc.getboolean('CONFIG','SET_ALL')
    #conf.ENABLED=configc.get('CONFIG','ENABLED')
    conf.TYPE=configc.get('CONFIG','TYPE')
    print_info( "----------------------------------")
    print_info( "[CONFIG]")
    print_info( "M_PLAT    =\t"+conf.M_PLAT)
    print_info( "SET_ALL   =\t"+str(conf.SET_ALL))
    #print "ENABLED   =\t",conf.ENABLED
    print_info( "TYPE      =\t"+conf.TYPE)
    print_info( "----------------------------------")






def usage():
    message='''
Usage: %s [OPTIONS..] VMs..
        -m|--m-plat Mhostname.  \t: Hostname of Rhev-M ,such as s-m04.
        -s|--enable             \t: enable USB support or not,default is not.
        -t|--type USB-TYPE      \t: TYPE of USB Legacy or Native(default if enable)
        -D|--disale             \t: equivalent to `-t disable'.
        -N|--native             \t: equivalent to `-t native'.
        -L|--legacy             \t: equivalent to `-t legacy'.
        -i|--ini conf_usb.ini   \t: from which file to get configuration,if not specified ,the `conf_usb.ini' will be read, or if not exist the file within the directory, the program will generate one.
        -E|--ere ere-pattern    \t: the USB of which the VM's name matched by the RE will be set.
        -g|--glob glob-pattern  \t: such `ABC*' matchs `ABC01',`ABCA'.. 
        -a|--all                \t: set USB for all the VMs in current Rhev-M (if option `-E|-g' was set,this option will be ignored).
        -h|--help               \t: print this message.
    

'''%sys.argv[0]
    print message







def main():
    
    try:
        opts,args=getopt.getopt(sys.argv[1:],'m:hst:i:E:g:DNLa',['m-plat=','help','enable','type=','ini=','all','extend=','glob=','disable','native','legacy'])
    
    except getopt.GetoptError as err:
        print str(err)
        usage()
        sys.exit(1)
    #yrt
    #print_err(args)
    need_help=False    
    have_conf_file=False
    if not os.path.exists(conf.CONFIG_FILE):
        gen_conf_file(conf.CONFIG_FILE)
        print_info('Config-file: '+conf.CONFIG_FILE+' generated!!')
        sys.exit(0)
    else:
        read_conf_file(conf.CONFIG_FILE,conf)
    #fi

    ere_pattern=''
    glob_pattern=''
    vmlist=args[:]
    

    for o,a in opts:
        if o in ('-i','--ini'):
            have_conf_file=True
            conf.CONFIG_FILE=a
            read_conf_file(conf.CONFIG_FILE,conf)
        elif o in ('-m','--m-plat'):
            conf.M_PLAT=a
        elif o in ('-t','--type'):
            conf.TYPE=a
        elif o in ('-E','--extend'):
            conf.SET_ALL=False
            ere_pattern=a
        elif o in ('-g','--glob'):
            conf.SET_ALL=False
            glob_pattern=a
        elif o in ('-a','--all'):
            conf.SET_ALL=True
        elif o in ('-D','--disable'):
            conf.TYPE='disable'
        elif o in ('-N','--native'):
            conf.TYPE='native'
        elif o in ('-L','--legacy'):
            conf.TYPE='legacy'

        elif o in('-h','--help'):
            need_help=True
            usage()
            sys.exit(0)
        else:
            usage()
            sys.exit(2)
        
    #rof

    print_info("conf.M_PLAT  : "+str(conf.M_PLAT))
    print_info("conf.SET_ALL : " +str(conf.SET_ALL))
    print_info("conf.TYPE    : " +str(conf.TYPE))


    

    ip=Mhm2Mip(conf.M_PLAT)
    Mipp=gen_ip(ip)
    print_ok(Mipp)

    vm2id_d={}
    
    if conf.SET_ALL:
        vms_xml=list_all_vms_xml(Mipp)
        vm2id_d=get_all_id(vms_xml)
    else:
        if ere_pattern!='':
#TODO
            vms_xml=list_all_vms_xml(Mipp)
            vm2id_d=get_all_id(vms_xml)
            for vmn in vm2id_d.keys():
                if not re.match(ere_pattern,vmn):
                    vm2id_d.pop(vmn)

        elif glob_pattern!='':
            #print_err(glob_pattern)
            vms_xml=get_vms_by_name_xml(Mipp,glob_pattern)
            vm2id_d=get_all_id(vms_xml)
    #fi
#    vm2id_d=get_all_id(vms_xml)
    if len(vm2id_d)==0:
        usage()
        sys.exit(-3)   
 
    keys=vm2id_d.keys()
    keys.sort()

    for vmn in keys:
        print_info('Set USB ['+conf.TYPE+'] for VM:'+vmn)
        set_usb(Mipp,vm2id_d[vmn],type=conf.TYPE)

    if need_help:
        usage()


if __name__ == '__main__':
    main()

#!/usr/bin/python2



import os
import sys
import re

sys.path.append('..')
from printc.printc import *
#from lxml import etree
import xml.etree.ElementTree as etree

import httplib2




class gc():
    URL_PROTO='https://'
    URL_PRE='10.11.4.'
    URL_PORT=':8443'
    USER='admin@internal'
    PASSWD='zj@Cloud)(12'

    M_LIST=['s-m01','s-m02','s-m03','s-m04','s-m05','s-m06','s-m07','s-m08','s-m09','s-m10','s-m11','s-m12','s-m13','s-m14','t-m01','t-m02','t-m03','t-m04','t-m05','t-m06','t-m07']
    M_HNUM=['170','171','172','173','174','175','176','177','178','179','181','182','183','184','160','161','162','163','164','165','166']
    MS_LIST=['s-m01','s-m02','s-m03','s-m04','s-m05','s-m06','s-m07','s-m08','s-m09','s-m10','s-m11','s-m12','s-m13','s-m14']
    MS_HNUM=['170','171','172','173','174','175','176','177','178','179','181','182','183','184']
    MH_LIST=['t-m01','t-m02','t-m03','t-m04','t-m05','t-m06','t-m07']
    MH_HNUM=['160','161','162','163','164','165','166']

    HS_LIST=['h-p-13','h-p-14','h-p-15','h-p-16','h-p-17','h-p-18','h-p-19','h-p-20','h-p-21','h-p-22','h-p-23','h-p-24','h-p-29','h-p-30','h-p-31','h-p-32','h-p-33','h-p-34','h-p-35','h-p-36','h-p-37','h-p-38','h-p-39','h-p-40','h-p-41','h-p-42','h-p-43','h-p-44','h-p-45','h-p-46','h-p-47','h-p-48','h-p-49','h-p-50','h-p-51','h-p-52','h-p-01']
    HS_HNUM=['103','104','105','106','107','108','109','110','111','112','113','114','119','120','121','122','123','124','125','126','127','128','129','130','131','132','133','134','135','136','137','138','139','140','141','142','91']
    HT_LIST=['h-p-02','h-p-03','h-p-04','h-p-05','h-p-06','h-p-07','h-p-08','h-p-09','h-p-10','h-p-11','h-p-12','h-p-25','h-p-26','h-p-27','h-p-28']
    HT_HNUM=['92','93','94','95','96','97','98','99','100','101','102','115','116','117','118']


    MH_MAP={'s-m01':['h-p-13','h-p-14'],\
            's-m02':['h-p-15','h-p-16'],\
            's-m03':['h-p-17','h-p-18'],\
            's-m04':['h-p-19','h-p-20'],\
            's-m05':['h-p-21','h-p-22'],\
            's-m06':['h-p-23','h-p-24'],\
            's-m07':['h-p-35','h-p-36'],\
            's-m08':['h-p-33','h-p-34'],\
            's-m09':['h-p-37','h-p-38','h-p-39','h-p-40'],\
            's-m10':['h-p-51','h-p-52','h-p-41','h-p-42'],\
            's-m11':['h-p-49','h-p-50','h-p-43','h-p-44'],\
            's-m12':['h-p-47','h-p-48','h-p-29','h-p-30'],\
            's-m13':['h-p-45','h-p-46','h-p-31','h-p-32'],\
            't-m01':['h-p-07','h-p-08'],\
            't-m02':['h-p-09','h-p-10'],\
            't-m03':['h-p-11','h-p-12'],\
            't-m04':['h-p-05','h-p-06'],\
            't-m05':['h-p-02','h-p-03','h-p-04'],\
            't-m06':['h-p-27','h-p-28'],\
            't-m07':['h-p-25','h-p-26'],\
            }


    CURRENT_M=''
    CURRENT_H=[]


    CURL_CMD=r'/usr/bin/curl -v '
    CURL_GET=r'-X GET -H "Accept:application/xml" '
    CURL_POST=r'-X POST -H "Accept:application/xml" -H "Content-Type:application/xml" '
    CURL_PUT=r'-X PUT -H "Accept:application/xml" -H "Content-Type:application/xml" '
    CURL_DEL=r'-X DELETE -H "Accept:application/xml" -H "Content-Type:application/xml" '
#    CURL_U='-u "admin@internal":"zj@Cloud)(12" '
    CURL_U=r'-u "'+USER+r'":"'+PASSWD+r'"'
    CURL_C=r' --insecure -s '
    CURL_O=r' -o  '
#    CURL_V=' -v '
    CURL_POST_D0=r" -d '"
    CURL_POST_D1=r"' "
    CURL_PUT_D0=r" -d '"
    CURL_PUT_D1=r"' "
    CURL_DEL_D0=r" -d '"
    CURL_DEL_D1=r"' "
    REQ_ARGS_MAX=r';max=2000'
    REQ_ARGS_CASE_F=r';case-sensitive=false'
    REQ_ARGS_CASE_T=r';case-sensitive=true'
    REQ_SEARCH=r'?search=name%3Dvm*'# Wildcard search query for name=vm*


class ac():
    START='start'
    SHUTDOWN='shutdown'
    STOP='stop'
    SUSPEND='suupend'
    MIGRATE='migrate'
    MOVE='move'
    DETACH='detach'
    EXPORT='export'



class lc():
    STATIS='statistics'
    PERMS='permissions'
    NICS='nics'
    DISKS='disks'


def Mhm2Mip(hostname):
    hm=hostname.lower()
    print_info(hm)
    if not hm in gc.M_LIST:
        print_err("Checking Hostname Please!")
        sys.exit(-2)
    return dict(zip(gc.M_LIST,gc.M_HNUM))[hm]


def Hhm2Hip(hostname):
    hm=hostname.lower()
    print_info(hm)
    if hm in gc.HS_LIST:
        return dict(zip(gc.HS_LIST,gc.HS_HNUM))[hm]
    elif  hm in gc.HT_LIST:
        return dict(zip(gc.HT_LIST,gc.HT_HNUM))[hm]
    else:
        print_err("Checking Hostname Please!")
        sys.exit(-2)


def Mhm2Hhm(hostname):
    hm=hostname.lower()
    print_info(hm)

    if hm in gc.MH_MAP.keys():
        return gc.MH_MAP[hm]#type:list
    else:
        return None

#==============================================================


def rest_request(link,method='GET',body=None,user=gc.USER,passwd=gc.PASSWD):
#    print_ok(link)
#    print_ok(body)
#    h=httplib2.Http(".cache",disable_ssl_certificate_validation=True)
    h=httplib2.Http(disable_ssl_certificate_validation=True)
#    print_info("REQUEST::USER: "+user+"  PASSWD:"+passwd)
    h.add_credentials(user,passwd)
    Headers={'content-type':'application/xml','accept':'application/xml'}
    Method=method.upper()
    resp,content=h.request(link,method=Method,headers=Headers,body=body)
#    print resp['status']
    if resp['status'][0]=='2':
#        print_ok(resp)
        return content
    else:
        ss=resp['status']
        print "STATUS:%s"%ss
#        if ss=='400':
#            sys.stdout.write("400")
        sys.stderr.write("ERROR:STATUS")
#        print_info(content)
        return None

def gen_link(m,sources):
    link=gc.URL_PROTO+gc.URL_PRE+m+gc.URL_PORT+source
    return link
#==============================================================


def check_head(head):
    print_err(head)
    state=re.search(r"< HTTP/1.1 (\d+) (\w+)",head)
    stat=state.groups()
    return stat


def exec_curl(curl_cmd,save=None):
    cmd=curl_cmd
    if not save is None and type(save)==type(''):
        cmd=curl_cmd+gc.CURL_O+save
    pf=os.popen3(cmd)
    xml=pf[1].read()
    head=pf[2].read()
    head_code=check_head(head)[0]
    if head_code=='200':
        return xml
    else:
        print_err("Error::exec_curl: \n=="+curl_cmd)
        print_err(head_code)
        print_info(head)
        return None



def gen_ip(m):
    IP=gc.URL_PROTO+gc.URL_PRE+m+gc.URL_PORT
    return IP


def gen_curl_get(IP,resource):
    cmd=gc.CURL_CMD+gc.CURL_C+gc.CURL_GET+gc.CURL_U+r'"'+IP+resource+r'"'
    return cmd


def gen_curl_post(IP,resource,desc):
    cmd=gc.CURL_CMD+gc.CURL_C+gc.CURL_POST+gc.CURL_U+gc.CURL_POST_D0+desc+gc.CURL_POST_D1+r'"'+IP+resource+r'"'
    return cmd

def gen_curl_put(IP,resource,desc):
    cmd=gc.CURL_CMD+gc.CURL_C+gc.CURL_PUT+gc.CURL_U+gc.CURL_PUT_D0+desc+gc.CURL_PUT_D1+r'"'+IP+resource+r'"'
    return cmd

def gen_curl_delete(IP,resource,desc):
    cmd=gc.CURL_CMD+gc.CURL_C+gc.CURL_DEL+gc.CURL_U+gc.CURL_DEL_D0+desc+gc.CURL_DEL_D1+r'"'+IP+resource+r'"'
    return cmd


#==============================================================


def list_all_vms_xml(Mipp,max=1000):
    link=Mipp+r'/api/vms;max='+str(max)
    cnt_xml=rest_request(link,'GET')
    return cnt_xml

def list_all_hosts_xml(Mipp,max=100):
    link=Mipp+r'/api/hosts;max='+str(max)
    cnt_xml=rest_request(link,'GET')
    return cnt_xml
#-------------------


def list_all_storagedomains_xml(Mipp,max=100):
    link=Mipp+r'/api/storagedomains;max='+str(max)
    cnt_xml=rest_request(link,'GET')
#    print cnt_xml
    return cnt_xml



def list_all_users_xml(Mipp,max=1000):
    link=Mipp+r'/api/users;max='+str(max)
    cnt_xml=rest_request(link,'GET')
    return cnt_xml

def get_vms_by_name_xml(Mipp,vmname):
    link=Mipp+r'/api/vms?search=name%3D'+vmname
#    print_err(link)
    cnt_xml=rest_request(link,'GET')
    return cnt_xml

def get_host_of_vms(xml):
    doc=etree.fromstring(xml)
    vms=doc.findall('vm')
    vm2hip={}
    for vm in vms:
        vmname=vm.find('name').text
        vmstat=vm.find('status').find("state").text
        if vmstat=='down':
            continue
        #fi
        vmhost=vm.find('display').find('address').text
        vm2hip[vmname]=[vmstat,vmhost]
    return vm2hip



def get_hosts_by_name_xml(Mipp,hostname):
    link=Mipp+r'/api/hosts?search=name%3D'+hostname
    cnt_xml=rest_request(link,'GET')
    return cnt_xml

def get_users_by_name_xml(Mipp,name):#not username:user1@domain ;
    link=Mipp+r'/api/users?search=name%3D'+name
    cnt_xml=rest_request(link,'GET')
    return cnt_xml

#-----------------------------------------------------------


def list_all_vms_curl(Mipp,max=1000):
    cmd=gen_curl_get(Mipp,'/api/vms;max='+str(max))
    return cmd

def list_all_hosts_curl(Mipp,max=100):
    cmd=gen_curl_get(Mipp,'/api/hosts;max='+str(max))
    return cmd

def list_all_users_curl(Mipp,max=1000):
    cmd=gen_curl_get(Mipp,'/api/users;max='+str(max))
    return cmd

def get_vm_by_name_curl(Mipp,vmname):
    cmd=gen_curl_get(Mipp,r'/api/vms?search=name%3D'+vmname)
    return cmd

def get_host_by_name_curl(Mipp,hostname):
    cmd=gen_curl_get(Mipp,r'/api/hosts?search=name%3D'+hostname)
    return cmd

def get_user_by_name_curl(Mipp,name):
    cmd=gen_curl_get(Mipp,r'/api/users?search=name%3D'+name)
    return cmd

#==============================================================
def get_all_href(xml,node='vm'): #xml  /api/vms or /api/hosts
    doc=etree.fromstring(xml)
    xmllists=doc.findall(node)
    nd2href_d={}
#    nd2id_d={}
    for nd in xmllists:
        href=nd.attrib['href']
        vmnm=nd.find('name').text
#        id_=nd.attrib['id']
        nd2href_d[vmnm]=href
#        nd2id_d[vmnm]=id_
    #rof
    return nd2href_d

def get_href_by_state(xml,node='vm',state='UP'): #xml  /api/vms or /api/hosts
    doc=etree.fromstring(xml)
    xmllists=doc.findall(node)

    nd2href_d={}
    for nd in xmllists:
        stat=nd.find('status').find('state').text.upper()
        if state.upper() == stat.upper():
            href=nd.attrib['href']
            vmnm=nd.find('name').text
            nd2href_d[vmnm]=href
    #rof
    return nd2href_d


def get_name_from_id(Mipp,id_,nodes='vms'):
    resource=r'/api/'+nodes+r'/'+id_
    rt_xml=rest_request(Mipp+resource,'GET')
    if rt_xml is None:
        print_err('Can not GET name from id:['+id_+']')
        return ''
    else:
        doc=etree.fromstring(rt_xml)
        name=doc.find('name').text
        if not name is '':
            return name
        else:
            return 'UnknownVM'
        #fi
    #fi

def get_all_id(xml,node='vm'): #xml  /api/vms or /api/hosts
    doc=etree.fromstring(xml)
    xmllists=doc.findall(node)
    nd2id_d={}
#    nd2id_d={}
    for nd in xmllists:
        id_=nd.attrib['id']
        vmnm=nd.find('name').text
#        id_=nd.attrib['id']
        nd2id_d[vmnm]=id_
    #rof
    #print nd2id_d
    return nd2id_d


def get_vms_by_host(Mipp,host=''):
    rt_xml=list_all_vms_xml(Mipp)
    doc=etree.fromstring(rt_xml)
    vmlist=doc.findall('vm')
    hid2nm_d={}
    vm2id_d={}
    for vm in vmlist:
        host=vm.find('host')
        if host is None:##the vm was down,not on any host
            continue
        #fi
        hid=host.attrib['id']
        hname=hid2nm_d.get(hid,None)

        if None is hname:
            resource=r'/api/hosts/'+hid
            host_xml=rest_request(Mipp+resource,'GET')
            hdoc=etree.fromstring(host_xml)
            hostname=hdoc.find('host').find('name').text
            hid2nm_d[hid]=hostname
        #fi
        hname=hid2nm_d[hid]
        if hname==host:
            vm2id_d[vm]=vm.attrib['id']
        #fi

    return vm2id_d


def get_id_by_state(xml,node='vm',state='UP'): #xml  /api/vms or /api/hosts
    doc=etree.fromstring(xml)
    xmllists=doc.findall(node)

    nd2id_d={}
    for nd in xmllists:
        state=nd.find('status').find('state').text.upper()
        if state.upper() == stat.upper():
            id_=nd.attrib['id']
            vmnm=nd.find('name').text
            nd2id_d[vmnm]=id_
    #rof
    return nd2id_d

def get_id_by_state_not(xml,node='vm',state='DOWN'): #xml  /api/vms or /api/hosts
    doc=etree.fromstring(xml)
    xmllists=doc.findall(node)

    nd2id_d={}
    for nd in xmllists:
        stat=nd.find('status').find('state').text.upper()
        if state.upper() != stat.upper():
            id_=nd.attrib['id']
            vmnm=nd.find('name').text
            nd2id_d[vmnm]=id_
    #rof
    return nd2id_d


def check_rt_xml(rt_xml,statements,Mipp,vmid,type='vms'):
    vmname=get_name_from_id(Mipp,vmid,type)
    if rt_xml is None:
        print_warn(statements+' :'+vmname+':['+vmid+'] failed')
        return False
    else:
        print_ok(statements+' :'+vmname+':['+vmid+'] Successfully')
        return True




#=============================================================================

def set_usb(Mipp,vmid,type='native'):
    if type.lower()=='disable':
        put_desc=r'<vm><usb><enabled>false</enabled></usb></vm>'
    else:
        put_desc=r'<vm><usb><enabled>true</enabled><type>'+type+r'</type></usb></vm>'
    #fi
    resource='/api/vms/'+vmid
    rt_xml=rest_request(Mipp+resource,'PUT',put_desc)
    return check_rt_xml(rt_xml,'Set USB of VM',Mipp,vmid)


def allow_override(Mipp,vmid,disable='true',monitors=1,dtype='spice'):
    put_desc=r'<vm><display><type>'+dtype+r'</type><monitors>'+str(monitors)+r'</monitors><allow_override>'+disable+r'</allow_override></display></vm>'
    resource='/api/vms/'+vmid
    rt_xml=rest_request(Mipp+resource,'PUT',put_desc)
    if disable=='true':
        prestat='Able'
    else:
        prestat='Disable'
    return check_rt_xml(rt_xml,prestat+' Allow Override',Mipp,vmid)


def add_user(Mipp,name):
    user_name=name.upper()+'@zjcloud.net'
    post_desc=r'<user><user_name>'+user_name+r'</user_name><roles><role><name>UserRole</name></role></roles></user>'
#----
#    curl_cmd=gen_curl_post(Mipp,'/api/users',post_desc)
#    rt_xml=exec_curl(curl_cmd)
#----
    rt_xml=rest_request(Mipp+'/api/users','POST',post_desc)
#----
    if rt_xml is None:
        print_err("Add_User["+name+"] --> Skipped")
        return ''
#        sys.exit(-2)
    #print rt_xml
    else:
        print_info('Add_user['+name+'] Successfully')
        doc=etree.fromstring(rt_xml)
        user_id=doc.attrib['id']
        return user_id



def vm_set_ticket(Mipp,vmid,user,passwd,expiry='120'):
    res=Mipp+r'/api/vms/'+vmid+r'/ticket'
    post_desc='<action><ticket><expiry>'+expiry+r'</expiry></ticket></action>'
    rt_xml=rest_request(res,'POST',post_desc,user,passwd)
    rdoc=etree.fromstring(rt_xml)
    state=rdoc.find('status').find('state').text
    ticket_value=rdoc.find('ticket').find('value').text
    ticket_expiry=rdoc.find('ticket').find('expiry').text
    ticket_d={}
    ticket_d['state']=state
    ticket_d['ticket_value']=ticket_value
    ticket_d['ticket_expiry']=ticket_expiry
    return ticket_d


def get_vm_permissions(Mipp,vmid):
    res=Mipp+r'/api/vms/'+vmid+r'/permissions'
    rt_xml=rest_request(res,'GET')
#    print rt_xml
    doc=etree.fromstring(rt_xml)
    permlist=doc.findall('permission')
    id2perm_d={}
#    print_err(str(permlist))
#    permvm2idr_d={}
    pid=0
    for p in permlist:
        perm_id=p.attrib['id']
#        print "P ",perm_id
        role_id=p.find('role').attrib['id']
#        print "R ",role_id
        user_id=p.find('user').attrib['id']
#        print "U ",user_id
#        vm_id=p.find('vm').attrib['id']
#        permvm2idr_d[vm_id]=(perm_id,user_id,role_id)
        id2perm_d[pid]=(perm_id,user_id,role_id)
        pid+=1
    return id2perm_d


def vm_give_permission(Mipp,vmid,name):
    user_id=add_user(Mipp,name)

    #default UserRole id:
    role_id='00000000-0000-0000-0001-000000000001'
    post_desc=r'<permission><role id="'+role_id+r'"/><user id="'+user_id+r'"/></permission>'
    resource='/api/vms/'+vmid+'/permissions'
#----
#    curl_cmd=gen_curl_post(Mipp,resource,post_desc)
#    rt_xml=exec_curl(curl_cmd)
#----
    rt_xml=rest_request(Mipp+resource,'POST',post_desc)
#----
    if rt_xml is None:
        vmnm=get_name_from_id(Mipp,vmid,'vms')
        print_err("give_permission_to_vm: "+name+":")
        #sys.exit(-2)
        return False
    else:
        print_ok("Give Permission to vm: "+name+" Successfully.")
        return True


def vm_remove_permissions(Mipp,vmid):
    perm_d=get_vm_permissions(Mipp,vmid)
    print perm_d
#    print_ok(perm_d.keys())
#    r=range(1,len(perm_d.keys()))

    for id in perm_d.keys():
#        print id
#        print perm_d[id]
        resource='/api/vms/'+vmid+'/permissions/'+perm_d[id][0]
        rt_xml=rest_request(Mipp+resource,'DELETE')
    vmnm=get_name_from_id(Mipp,vmid,'vms')
#    nperm_d=get_vm_permissions(Mipp,vmid)
#    print_info(nperm_d)
    print_ok("Remove all permissions of "+vmnm)
    return True




def get_vm_nics(Mipp,vmid):
    res=Mipp+r'/api/vms/'+vmid+'/nics'
    rt_xml=rest_request(res,'GET')
#    print rt_xml
    doc=etree.fromstring(rt_xml)
    niclist=doc.findall('nic')
#    nicid2nm_d={}
    nic2id_d={}
    for n in niclist:
        nic_name=n.find('name').text
        nic_id=n.attrib['id']
        nic2id_d[nic_name]=nic_id
#       nicid2nm_d[nic_id]=nic_name
#    print_warn(nicid2nm_d)
    return nic2id_d




def vm_set_memory(Mipp,vmid,memsize='1610612736'):
    put_desc=r'<vm><memory>'+memsize+r'</memory></vm>'
    resource='/api/vms/'+vmid

    rt_xml=rest_request(Mipp+resource,'PUT',put_desc)
    return check_rt_xml(rt_xml,'Modify Memory for VM',Mipp,vmid)



def vm_add_nic(Mipp,vmid,vlan_name,nic_name='nic1',nic_type='rtl8139'):
    post_desc=r'<nic><name>'+nic_name+r'</name><network><name>'+vlan_name+r'</name></network></nic>'
    resource='/api/vms/'+vmid+'/nics'
#----
#    curl_cmd=gen_curl_post(Mipp,resource,post_desc)
#    rt_xml=exec_curl(curl_cmd)
#----
    print_warn(Mipp+resource)
    rt_xml=rest_request(Mipp+resource,'POST',post_desc)
#----
    return check_rt_xml(rt_xml,'Add Nic to VM',Mipp,vmid)


def vm_modify_nic(Mipp,vmid,nicid,vlan_name,nic_name='nic1',nic_type='rtl8139'):
    put_desc=r'<nic><name>'+nic_name+r'</name><network><name>'+vlan_name+r'</name></network></nic>'
    resource='/api/vms/'+vmid+'/nics/'+nicid
#----
#    curl_cmd=gen_curl_put(Mipp,resource,put_desc)
#    rt_xml=exec_curl(curl_cmd)
#----
    rt_xml=rest_request(Mipp+resource,'PUT',put_desc)
    return check_rt_xml(rt_xml,'Modify Nic of VM',Mipp,vmid)



def vm_delete_nic(Mipp,vmid,nicid=None,all=False):
    if True==all:
        nic_d=get_vm_nics(Mipp,vmid)
        for nic in nic_d.keys():
            resource=r'/api/vms/'+vmid+r'/nics/'+nic
            print_info('--'+resource)
            rest_request(Mipp+resource,'DELETE')
        #rof
    else:
        resource=r'/api/vms/'+vmid+r'/nics/'+nicid
        print_info('-'+resource)
        rest_request(Mipp+resource,'DELETE')



def vm_start(Mipp,vmid,boot='hd',cdrom='',user='admin@internal',passwd='zj@Cloud)(12'):
    if cdrom=='':
        post_desc=r'<action><vm><os><boot dev="'+boot+r'"/></os></vm></action>'
    else:
        post_desc=r'<action><vm><os><boot dev="'+boot+r'"/></os><cdroms><cdrom><file id="'+cdrom+r'"/></cdrom></cdroms></vm></action>'

    resource=r'/api/vms/'+vmid+r'/start'
    rt_xml=rest_request(Mipp+resource,'POST',post_desc,user=user,passwd=passwd)
##    return check_rt_xml(rt_xml,'Start VM',Mipp,vmid)



def vm_stop(Mipp,vmid,user='admin@internal',passwd='zj@Cloud)(12'):
    post_desc='<action/>'
    resource=r'/api/vms/'+vmid+r'/stop'
    rt_xml=rest_request(Mipp+resource,'POST',post_desc,user=user,passwd=passwd)
##    return check_rt_xml(rt_xml,'Stop VM',Mipp,vmid)


def vm_shutdown(Mipp,vmid):
    post_desc='<action/>'
    resource=r'/api/vms/'+vmid+r'/shutdown'
    rt_xml=rest_request(Mipp+resource,'POST',post_desc)
##    return check_rt_xml(rt_xml,'Shutdown VM',Mipp,vmid)





def vm_migrate(Mipp,vmid,host=None,force=False):
    if host is None:
        post_desc='<action/>'
    else:
#        post_desc='<action><host name="'+hostname+r'"></action>'
        post_desc='<action><host><name>'+hostname+r'</name></host></action>'

    resource=r'/api/vms/'+vmid+r'/migrate'
    rt_xml=rest_request(Mipp+resource,'POST',post_desc)
    return check_rt_xml(rt_xml,'Migrate VM Operation',Mipp,vmid)

def vm_cancel_migrate(Mipp,vmid):
    post_desc='<action/>'
    resource=r'/api/vms/'+vmid+r'/cancelmigration'
    rt_xml=rest_request(Mipp+resource,'POST',post_desc)
    return check_rt_xml(rt_xml,'Cancel Migration VM Operation',Mipp,vmid)




def export_vm(Mipp,vmid,export='EXPORT',exclusive=True,discard_snapshots=True):

    post_desc='<action><storage_domain><name>'+export+'</name></storage_domain><exclusive>true</exclusive><discard_snapshots>true</discard_snapshots></action>'
    resource=r'/api/vms/'+vmid+r'/export'
    rt_xml=rest_request(Mipp+resource,'POST',post_desc)
    return check_rt_xml(rt_xml,'Export VM',Mipp,vmid)



def vm_create(Mipp,vmname,template='Blank',cluster='Default',vlan='',clone='false',usb='false',usbtype='Native'):
    post_desc_nousb='''<vm>
<name>%(vmname)s</name>
<template>
<name>%(templatename)s</name>
</template>
<cluster><name>%(clustername)s</name></cluster>
<usb>
<enabled>false</enabled>
</usb>
<timezone>Asia/Shanghai</timezone>
<display>
<allow_override>true</allow_override>
</display>
<disks>
<clone>%(clone)s</clone>
</disks>
</vm>'''%{'vmname':vmname,'templatename':template,'clustername':cluster,'clone':clone}

    post_desc_usb='''<vm>
<name>%(vmname)s</name>
<template>
<name>%(templatename)s</name>
</template>
<cluster><name>%(clustername)s</name></cluster>
<usb>
<enabled>true</enabled>
<type>%(usbtype)s</type>
</usb>
<timezone>Asia/Shanghai</timezone>
<display>
<allow_override>true</allow_override>
</display>
<disks>
<clone>%(clone)s</clone>
</disks>
</vm>'''%{'vmname':vmname,'templatename':template,'clustername':cluster,'usbtype':usbtype,'clone':clone}

    if usb=='false':
        post_desc=post_desc_nousb
    else:
        post_desc=post_desc_usb
    #fi


    rt_xml=rest_request(Mipp+r'/api/vms',"POST",post_desc)

#    rt_create=check_rt_xml(rt_xml,'Create VM',Mipp)
    if not rt_xml:
        print_err("Create VM["+vmname+"]")
        return None
    else:
        print_ok("VM :"+vmname+" Created!")
    #fi
    vmdoc=etree.fromstring(rt_xml)
    vmid=vmdoc.attrib['id']

    if vlan!='':#FIXME
#-----------------------
#        vm_delete_nic(Mipp,vmid,all=True)
#        vm_add_nic(Mipp,vmid,vlan_name=vlan,nic_name='nic1',nic_type='rtl8139')
#----------------------
        pass
    #fi
    return vmid



def vm_remove(Mipp,vmid):
    del_desc='<action><force>true</force></action>'
    resource=r'/api/vms/'+vmid
    rt_xml=rest_request(Mipp+resource,'DELETE',del_desc)
    return check_rt_xml(rt_xml,'Remove VM',Mipp,vmid)



def template_get_disks(Mipp,tmpid):
    rt_xml=rest_request(Mipp+r'/api/templates/'+tmpid+r'/disks','GET')
    check_rt_xml(rt_xml,'Get Disks of TMP',Mipp,tmpid,'templates')
    doc=etree.fromstring(rt_xml)
    disk_list=doc.findall('disk')
    diskid_l=[]
    for disk in disk_list:
        diskid.append(disk.attrib['id'])

    return diskid_l


def template_cp_disk(Mipp,tmpid,storagedomainname):
    diskids=template_get_disks(Mipp,tmpid)
    for did in diskids:
        post_desc=r'<action><storage_domain><name>'+storagedomainname+r'</name></storage_domain></action>'
        rt_xml=rest_request(Mipp+r'/api/templates/'+tmpid+r'/disks/'+did+r'/copy','POST',post_desc)
        if rt_xml is None:
            print_err('Copying Disk')






def get_template_info(Mipp,template):
    rt_xml=rest_request(Mipp+r'/api/templates','GET')
    tdoc=etree.fromstring(rt_xml)
    temp_list=tdoc.findall('template')
    tmpinfo_d={}
    for tmp in temp_list:
        name=tmp.find('name').text
        if name != template:
            continue
        else:
            tstate=tmp.find('status').find('state').text
            tname=name
            tid=tmp.attrib['id']
            ttype=tmp.find('type').text
            tmemory=tmp.find('memory').text
            tcpu_sockets=tmp.find('topology').attrib['sockets']
            tcpu_cores=tmp.find('topology').attrib['cores']
            tos_type=tmp.attrib['type']
            tos_boot_dev=tmp.find('boot').attrib['dev']
            tcluster_id=tmp.find('cluster').attrib['id']
            torigin=tmp.find('origin').text
            thigh_availablity_enabled=tmp.find('high_availability').find('enabled').text
            thigh_availablity_priority=tmp.find('high_availability').find('priority').text
            tdisplay_type=tmp.find('display').find('type').text
            tdisplay_monitors=tmp.find('display').find('monitors').text
            tdisplay_allow_override=tmp.find('display').find('allow_override').text
            tstateless=tmp.find('stateless').text
            ttimezone=tmp.find('timezone').text
            tdomain=tmp.find('domain').find('name').text
            tusb_enable=tmp.find('usb').find('enabled').text
            tmpinfo_d['state']=tstatus
            tmpinfo_d['name']=tname
            tmpinfo_d['id']=tid
            tmpinfo_d['type']=ttype
            tmpinfo_d['memory']=tmemory
            tmpinfo_d['topology_sockets']=tcpu_sockets
            tmpinfo_d['topology_cores']=tcpu_cores
            tmpinfo_d['os_type']=tos_type
            tmpinfo_d['boot_dev']=tos_boot_dev
            tmpinfo_d['cluster_id']=tcluster_id
            tmpinfo_d['origin']=torigin
            tmpinfo_d['high_availablility_enabled']=high_availablility_enabled
            tmpinfo_d['high_availablility_priority']=high_availablility_priority
            tmpinfo_d['display_type']=tdisplay_type
            tmpinfo_d['display_monitors']=tdisplay_monitors
            tmpinfo_d['display_allow_override']=tdisplay_allow_override

            tmpinfo_d['stateless']=tstateless
            tmpinfo_d['timezone']=ttimezone
            tmpinfo_d['domain']=tdomain
            tmpinfo_d['usb_enables']=tusb_enable
            return tmpinfo_d
        #fi
    #rof













#======================================================================



def get_vm_link_of(Mipp,vmid,link='statistics'):# /api/vms/VMID
    resource=r'/api/vms/'+vmid+'/'+link
#    print_info("Resource: "+resource)
    rt_xml=rest_request(Mipp+resource,'GET')
    if rt_xml is None:
        vmname=get_name_from_id(Mipp,vmid)
        print_err('Get '+link+' of VM: '+vmname+' Failed')
        return None
    else:
        return rt_xml



def statistics_of_vm(xml):#xml get from /api/vms/VMID/statistics
    doc=etree.fromstring(xml)
    statistic_lists=doc.findall('statistic')
    statis_d={}
    for stat in statistic_lists:
        desc=stat.find('name').text
        val=stat.find('values').find('value').find('datum').text
        statis_d[desc]=val
    #rof
    return statis_d

#----------------
#def disks_of_vm(xml):
#    doc=etree.fromstring(xml)
#    disk_lists=doc.findall('disk')
#    disks_d={}
#    for disk in disk_lists:



def is_nr_vms(stats):
    vh=stats["cpu.current.hypervisor"]
    vt=stats["cpu.current.total"]
    print_info("Hypervisor\t Total")
    print_info(str(vh)+'\t\t'+str(vt))
    if (vt=='110' or vt=='100') and (vh=='100' or vh=='110'):
        return True
    else:
        return False



def get_nr_vms(Mipp,xml):
    vmid_d=get_id_by_state_not(xml)

    keys=vmid_d.keys()
    keys.sort()
    for vm in keys:
        vmid=vmid_d[vm]
        rt_xml=get_vm_link_of(Mipp,vmid)
        print_ok(" VM : "+vm)
        if not is_nr_vms(statistics_of_vm(rt_xml)):
            vmid_d.pop(vm)
        #fi

    #rof
    return vmid_d





def get_name_from_href(Mipp,href): #vmname or hostname...
    cmd=gen_curl_get(Mipp,href)
    xml=exec_curl(cmd)
    if xml is None:
        print_err("Error::get_name_from_href ["+href+"]")
        sys.exit(-3)
    doc=etree.fromstring(xml)
    nm=doc.find('name')
    return nm.text








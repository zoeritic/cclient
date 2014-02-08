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







def get_domain(Mip):
#    pass
    Mipp=gc.URL_PROTO+Mip+gc.URL_PORT
    uri=Mipp+r'/api/domains'
    st,rt_xml=rest_request(uri,'GET')
    doc=etree.fromstring(rt_xml)
    domains=doc.findall('domain')
    for domain in domains:
        dn=domain.find('name').text
        if dn=='internal':
            continue
        else:
            sys.stderr.write("DOMAIN-OK")
#            sys.stdout.write("DOMAIN-OK")
        return dn


mip=sys.argv[1]

domain=get_domain(mip)

print domain




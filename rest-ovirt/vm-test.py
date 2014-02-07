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
    USER=''
    PASSWD=''

    FULL_SCREEN=True
    CONFIG_FILE='conf_vm_console.ini'
    CERTS_DIR='/root/.certs/'
    PERM_PATH=' ~/.spicec/spice_truststore.pem'

    CONSOLE='spicy'



rtxml=rest_request('https://10.11.4.176:8443/api/vms?search=name%3DSYQT501','GET',user='admin@internal',passwd='zj@Cloud)(12')

print rtxml

rtxml=rest_request('https://10.11.4.176:8443/api/vms/5eff02ea-a63c-4952-80bd-99cdb71118a7/','GET',user='syqt501@zjcloud.net',passwd='12345678')

print rtxml



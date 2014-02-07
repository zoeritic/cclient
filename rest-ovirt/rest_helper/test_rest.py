#!/usr/bin/python2




from rest_helper import *
from printc import *


print_err("WARN..")






ip=gen_ip('172')

cmd=list_all_hosts_curl(ip)
print_info(cmd)
xml=exec_curl(cmd)

#print xml



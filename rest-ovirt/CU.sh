#!/usr/bin/env bash

curl -X POST -H "Accept:application/xml" \
    -H "Content-Type:application/xml" -k\
    -u 't11@zjcloud.net':'12345678'\
    -d "<action><vm><os><boot dev=\"cdrom\"/></os></vm></action>"\
    https://10.11.4.184:8443/api/vms/94c4f1c0-180d-4261-8b85-e2b4498acbf0/start

#    -u 'admin@internal':'zj@Cloud)(12'\

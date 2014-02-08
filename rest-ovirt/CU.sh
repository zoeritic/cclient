#!/usr/bin/env bash

curl -X POST -H "Accept:application/xml" \
    -H "Content-Type:application/xml" -k\
    -u 'DBHZX501@zjcloud.net':'12345678'\
    -d "<action><ticket><expiry>120</expiry></ticket></action>"\
    https://10.11.4.173:8443/api/vms/e758eb61-fda3-40fb-9599-9f991601838c/ticket

#    -u 'admin@internal':'zj@Cloud)(12'\

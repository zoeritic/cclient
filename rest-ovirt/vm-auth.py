#!/usr/bin/python2

from ovirtsdk.api import API
from ovirtsdk.infrastructure.errors import RequestError, ConnectionError



try:
    api=API(url='https://10.11.4.184',username='T11@zjcloud.net',password='12345678',insecure=True)
except RequestError as reqerr:
    print "Login error"
except ConnectionError as connerr:
    print "Bad URL"


#!/bin/bash


n_cclient=$(ps aux|grep /build/cclient|grep -v grep|wc -l)

if [ "$n_cclient" -eq "0" ];then

/home/cclient/App/cclient/build/cclient

fi


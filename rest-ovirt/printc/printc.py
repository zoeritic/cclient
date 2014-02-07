#!/usr/bin/python2

import sys
import time
import logging
#timesuffix=time.strftime('%H%M%S')
#nm=__name__

#flog=open(nm+timesuffix+'.log','w')


def new_logging(fn,level=logging.INFO):
    logger=logging.getLogger(fn)
    logger.setLevel(level)
    formatter=logging.Formatter('%(asctime)s | %(name)s | %(levelname)s :: %(message)s')
    
    return logger






class msty():
    
    BLACK     ='\033[30m'    
    RED       ='\033[31m'
    GREEN     ='\033[32m'
    BROWN     ='\033[33m'
    BLUE      ='\033[34m'
    MAGENTA   ='\033[35m'
    CYAN      ='\033[36m'
    LIGHTCYAN ='\033[37m'

    BLACK_    ='\033[40m'    
    RED_      ='\033[41m'
    GREEN_    ='\033[42m'
    BROWN_    ='\033[43m'
    BLUE_     ='\033[44m'
    MAGENTA_  ='\033[45m'
    CYAN_     ='\033[46m'
    LIGHTCYAN_='\033[47m'

    _BOLD_   ='\033[1m'
    _DARK_   ='\033[2m'
    _UL_     ='\033[4m'
    _BLINK_  ='\033[5m'
    _RB_     ='\033[7m'
    _STRIKE_ ='\033[9m'
    END_     ='\033[0m'



class sty():

    S_OK   =msty.GREEN+msty._DARK_
    S_WARN =msty.BROWN+msty._BOLD_+msty._BLINK_
    S_ERR  =msty.RED+msty._RB_+msty._BOLD_+msty._BLINK_
    S_INFO =msty.BROWN
    S_END  =msty.END_


def printc(strs,style,fg='',bg=''):
    SS=fg+bg+style
#    print 'SS=:',SS,
#    print >>sys.stderr,style,
#    print strs,
#    print >>sys.stderr,sty.S_END
    strs=str(strs)
    sys.stderr.write(SS)
    sys.stdout.write(strs+'\n')
    sys.stderr.write(sty.S_END)




def print_err(strs):
    istrs='ERROR:: '+str(strs)
    printc(istrs,sty.S_ERR)
#    flog.write(istrs+'\n')

def print_info(strs):
    istrs='INFO:: '+str(strs)
    printc(istrs,sty.S_INFO)
#    flog.write(istrs+'\n')

def print_ok(strs):
    istrs='== '+str(strs)
    printc(istrs,sty.S_OK)
#    flog.write(istrs+'\n')

def print_warn(strs):
    istrs='WARN:: '+str(strs)
    printc(istrs,sty.S_WARN)
#    flog.write(istrs+'\n')













#print 'S_OK: ',sty.S_OK

#printc("HelloWorld",style=sty.S_OK)

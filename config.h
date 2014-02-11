#ifndef __CC_CONFIG_
#define __CC_CONFIG_

#define STYLE_PATH "./style/"
#define STATUS_ICON "./pics/cclient.png"

#define LINK_NAME_SIZE 32
#define RESTOVIRT_DIR "./rest-ovirt/"
#define UI_LOGING_WIN "cc-loging-win.ui"
#define UI_LOGING_WINER "cc-loging-winer.ui"
#define PASSWD "123"
#define UI_MPANEL "./cc-manager-panel.ui"
#define OVIRT_VMOPER "./rest-ovirt/vm-helper.py"
#define STATING_TIMEOUT_MAX 15
#define TIMEOUT_INTERVAL 1
#define REQUEST_INTERVAL 5
#define GETDOMAIN_INTERVAL 5
#define VMSTAT_DOWN 0
#define VMSTAT_UP 1
#define VMSTAT_POWERING_UP 2
#define VMSTAT_POWERING_DOWN 3
#define VMSTAT_NON_RESPONDING 5


#ifdef PATH
#define INI_LINK_CONFIG PATH"/config/LINK.ini"
#else
#define INI_LINK_CONFIG "./cc-link.ini"
#endif

#define INI_LOGING_CONFIG "./cc-loging-config.ini"




#endif

#ifndef __H_LOGING_OVIRT_CC_
#define __H_LOGING_OVIRT_CC_

#include<gtk/gtk.h>
#include"cc-link-info.h"
#include"cc-loging-info.h"

typedef struct _CCOvirtInfo {

    gchar *vmname;		//in
    gchar *user;		//in [vmname@domain]
    gchar *passwd;		//in
    gchar *maddr;		//in

} CCOvirtInfo;



CCOvirtInfo *cc_ovirt_info_new(void);
void cc_ovirt_info_destroy(CCOvirtInfo * oinfo);
void cc_ovirt_info_update(CCOvirtInfo * ovirt, CCLogingInfo * info,
			  CCLinkInfo * link);
void cc_ovirt_info_print(CCOvirtInfo * oinfo);






#endif

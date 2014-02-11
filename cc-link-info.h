#ifndef __H_LINK_LINK_CC_
#define __H_LINK_LINK_CC_

#include<glib.h>
#include<stdarg.h>
#include<stdio.h>
#include<glib/gprintf.h>
#include"cc-utils.h"

//#define INI_LINK_CONFIG "./cc-link-config.ini"

typedef struct _CCLinkInfo {

    gchar *name;
    gchar *url;
    gchar *addr;
    gchar *port;
    
} CCLinkInfo;


typedef enum _CCLinkInfoProp {
    PROP_LINK_0, PROP_LINK_NAME, PROP_LINK_URL
} CCLinkInfoProp;


CCLinkInfo *cc_link_info_new(void);
void cc_link_info_destroy(CCLinkInfo * link);
void cc_link_info_clean(CCLinkInfo * link);
void cc_link_info_set(CCLinkInfo * link, ...);
void cc_link_info_to_file(CCLinkInfo * link, gchar * filename);
void cc_link_info_read_file(CCLinkInfo * link, gchar * filename);
void cc_link_info_print(CCLinkInfo * link);






#endif

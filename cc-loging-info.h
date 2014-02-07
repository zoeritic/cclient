#ifndef __H_LOGING_INFO_CC_
#define __H_LOGING_INFO_CC_

#include<glib.h>
#include<glib/gprintf.h>
#include<stdarg.h>
#include<stdio.h>

typedef struct __CCLogingInfo {

    gchar *user;
    gchar *passwd;
    gchar *domain;
    gboolean sav_passwd;

} CCLogingInfo;


typedef enum _CCLogingInfoProp {
    PROP_INFO_0, PROP_INFO_USER, PROP_INFO_PASSWD, PROP_INFO_DOMAIN,
	PROP_INFO_SAVPASSWD
} CCLogingInfoProp;


CCLogingInfo *cc_loging_info_new(void);
void cc_loging_info_destroy(CCLogingInfo * info);
void cc_loging_info_clean(CCLogingInfo * info);
void cc_loging_info_set(CCLogingInfo * info, ...);
void cc_loging_info_to_file(CCLogingInfo * info, gchar * filename);
void cc_loging_info_read_file(CCLogingInfo * info, gchar * filename);
void cc_loging_info_print(CCLogingInfo * info);






#endif

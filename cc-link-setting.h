#ifndef __H_LINK_SETTING_CC
#define __H_LINK_SETTING_CC

#include<glib.h>
#include<stdio.h>
#include<string.h>


//#define LINK_NAME_SIZE 32

typedef struct _link {
//    int LINK_ID;
    char *LINK_NAME;
    char *LINK_URL;

} linker;



void cc_link_setting_new(const char *file);

void cc_link_setting_get_default(const char *file, linker * linkfile);

//TODO
//guint cc_link_setting_get_all(const char*file,GArray**array);

void cc_link_setting_print(const char *file);

void cc_link_setting_add_link(const gchar * file, linker * linkfile);

void cc_link_setting_remove_link(const gchar * file, const gchar * link);









#endif

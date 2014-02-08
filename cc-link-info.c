#include"cc-link-info.h"


void cc_link_info_print(CCLinkInfo * info)
{
    g_print("= CCLinkInfo =\n");
    g_print(" LNAME :\t [%s]\n", info->name);
    g_print(" URL   :\t [%s]\n", info->url);
    g_print(" ADDR  :\t [%s]\n", info->addr);
    g_print(" PORT  :\t [%s]\n", info->port);

}



CCLinkInfo *cc_link_info_new(void)
{
    CCLinkInfo *info = g_new0(CCLinkInfo, 1);
    return info;
}

void cc_link_info_set(CCLinkInfo * info, ...)
{

    va_list ap;
    va_start(ap, info);

    for (CCLinkInfoProp propid = va_arg(ap, CCLinkInfoProp);
	 propid; propid = va_arg(ap, CCLinkInfoProp)) {
	if (propid == PROP_LINK_NAME) {
        if(info->name){g_free(info->name);info->name=NULL;}
	    info->name = g_strdup(va_arg(ap, gchar *));
	} else if (propid == PROP_LINK_URL) {
        if(info->url){g_free(info->url);info->url=NULL;}
	    info->url = g_strdup(va_arg(ap, gchar *));
	} else {
	    g_warning("Prop Wrong!!");
	}
    }
    va_end(ap);

    info->addr=cc_get_addr_from_fullurl(info->url);
    info->port=cc_get_port_from_fullurl(info->url);

}

void cc_link_info_to_file(CCLinkInfo * info, gchar * filename)
{

    GKeyFile *confile = g_key_file_new();
    g_key_file_set_string(confile, "LINK", "NAME",
			  info->name ? info->name : "");
    g_key_file_set_string(confile, "LINK", "URL",
			  info->url ? info->url : "");

    FILE *fd = fopen(filename, "wb");
    gchar *data = g_key_file_to_data(confile, NULL, NULL);

    g_fprintf(fd, "%s", data);
    g_free(data);
    fclose(fd);
    g_key_file_unref(confile);

}

void cc_link_info_read_file(CCLinkInfo * info, gchar * filename)
{
    GKeyFile *confile = g_key_file_new();

    if (!g_key_file_load_from_file
	(confile, filename, G_KEY_FILE_NONE, NULL)) {
	g_critical("Load config file Error!!");
	return;
    }
//    if(g_key_file_has_key(confile,"LOGING","USER",NULL)){
//    if(info->name){g_free(info->name);info->name=NULL;}
    info->name = g_key_file_get_string(confile, "LINK", "NAME", NULL);
//    }
//    if(info->url){g_free(info->url);info->url=NULL;}
    info->url = g_key_file_get_string(confile, "LINK", "URL", NULL);
//        if(info->addr){g_free(info->addr);info->addr=NULL;}
    info->addr=cc_get_addr_from_fullurl(info->url);
//        if(info->port){g_free(info->port);info->port=NULL;}
    info->port=cc_get_port_from_fullurl(info->url);

    g_key_file_unref(confile);
}

void cc_link_info_destroy(CCLinkInfo * info)
{
    g_return_if_fail(info!=NULL);
    g_free(info->name);
    g_free(info->url);
    g_free(info->addr);
    g_free(info->port);

    g_free(info);
}

void cc_link_info_clean(CCLinkInfo * info)
{
    if(info->name){
    g_free(info->name);
    info->name=NULL;
    }
    if(info->url){
    g_free(info->url);
    info->url=NULL;
    }
    if(info->addr){
    g_free(info->addr);
    info->addr=NULL;
    }
    if(info->port){
    g_free(info->port);
    info->port=NULL;
    }

}

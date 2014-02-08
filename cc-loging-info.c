
#include"cc-loging-info.h"


void cc_loging_info_print(CCLogingInfo * info)
{
    g_print("= CCLogingInfo[%x] =\n",(guint)info);
    g_print(" USER  :\t [%s]\n", info->user);
    g_print(" PASSWD:\t [%s]\n", info->passwd);
    g_print(" DOMAIN:\t [%s]\n", info->domain);
    g_print(" SAV_PW:\t [%d]\n", info->sav_passwd);

}



CCLogingInfo *cc_loging_info_new(void)
{
    CCLogingInfo *info = g_new0(CCLogingInfo, 1);
    return info;
}

void cc_loging_info_set(CCLogingInfo * info, ...)
{
    va_list ap;
    va_start(ap, info);

    for (CCLogingInfoProp propid = va_arg(ap, CCLogingInfoProp);
	 propid; propid = va_arg(ap, CCLogingInfoProp)) {
	if (propid == PROP_INFO_USER) {
        if(info->user){
            g_free(info->user);
        info->user=NULL;}
	    info->user = g_ascii_strup(va_arg(ap, gchar *),-1);
	} else if (propid == PROP_INFO_PASSWD) {
        if(info->passwd){
            g_free(info->passwd);info->passwd=NULL;}
	    info->passwd = g_strdup(va_arg(ap, gchar *));
	} else if (propid == PROP_INFO_DOMAIN) {
        if(info->domain){
            g_free(info->domain);info->domain=NULL;}
	    info->domain = g_strdup(va_arg(ap, gchar *));
	} else if (propid == PROP_INFO_SAVPASSWD) {
	    info->sav_passwd = va_arg(ap, gboolean);
	} else {
	    g_warning("Prop Wrong!!");
	}
    }
    va_end(ap);
    g_message("cc_loging_info_set...");
    cc_loging_info_print(info);
}

void cc_loging_info_to_file(CCLogingInfo * info, gchar * filename)
{

    GKeyFile *confile = g_key_file_new();
    g_key_file_set_string(confile, "LOGING", "USER",
			  info->user?info->user:"" );
    g_key_file_set_string(confile, "LOGING", "PASSWD",
			  info->passwd?info->passwd:"" );
    g_key_file_set_string(confile, "LOGING", "DOMAIN",
			  info->domain?info->domain:"" );
    g_key_file_set_boolean(confile, "LOGING", "SAV_PASSWD",
			   info->sav_passwd);

    FILE *fd = fopen(filename, "wb");
    gchar *data = g_key_file_to_data(confile, NULL, NULL);

    g_fprintf(fd, "%s", data);
    g_free(data);
    fclose(fd);


}

void cc_loging_info_read_file(CCLogingInfo * info, gchar * filename)
{
    GKeyFile *confile = g_key_file_new();

    if (!g_key_file_load_from_file
	(confile, filename, G_KEY_FILE_NONE, NULL)) {
	g_critical("Load config file Error!!");
	return;
    }
//    if(g_key_file_has_key(confile,"LOGING","USER",NULL)){
    if(info->user){
        g_free(info->user);info->user=NULL;}
    info->user = g_key_file_get_string(confile, "LOGING", "USER", NULL);
//    }
    if(info->passwd){
        g_free(info->passwd);info->passwd=NULL;}
    info->passwd =g_key_file_get_string(confile, "LOGING", "PASSWD", NULL);
    if(info->domain){
        g_free(info->domain);info->domain=NULL;}
    info->domain =	g_key_file_get_string(confile, "LOGING", "DOMAIN", NULL);
    info->sav_passwd =g_key_file_get_boolean(confile, "LOGING", "SAV_PASSWD", NULL);

    g_message("cc_loging_info_read_from_file...");
    g_key_file_unref(confile);
    cc_loging_info_print(info);

}

void cc_loging_info_destroy(CCLogingInfo * info)
{
    g_return_if_fail(info!=NULL);
    if(info->user){
    g_free(info->user);
    info->user=NULL;}
    if(info->passwd){
    g_free(info->passwd);
    info->passwd=NULL;}
    if(info->domain){
    g_free(info->domain);
    info->domain=NULL;}
    g_free(info);
    info=NULL;
}

void cc_loging_info_clean(CCLogingInfo * info)
{
    g_free(info->user);
    g_free(info->passwd);
    g_free(info->domain);

}

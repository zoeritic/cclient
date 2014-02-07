
#include"cc-link-setting.h"






void cc_link_setting_new(const char *file)
{
    g_return_if_fail(file != NULL);

    GKeyFile *confile = g_key_file_new();


    gchar default_name[LINK_NAME_SIZE];
    g_sprintf(default_name, "LINK0");
    g_key_file_set_integer(confile, "DEFAULT", "ID", 0);

    g_key_file_set_string(confile, default_name, "NAME", "Default");
    g_key_file_set_string(confile, default_name, "URL",
			  "http://example.net/URI");

    gchar *fcnt = g_key_file_to_data(confile, NULL, NULL);

    FILE *fp = fopen(file, "w");
    fprintf(fp, "%s", fcnt);
    fclose(fp);
    g_free(fcnt);
    g_key_file_free(confile);


}


void cc_link_setting_get_default(const char *file, linker * linkfile)
{
    g_return_if_fail(linkfile != NULL);
    g_return_if_fail(file != NULL);

    GKeyFile *confile = g_key_file_new();
    g_key_file_load_from_file(confile, file, G_KEY_FILE_NONE, NULL);

    int default_id;

    if (g_key_file_has_group(confile, "DEFAULT")) {
	if (g_key_file_has_key(confile, "DEFAULT", "ID", NULL)) {
	    default_id =
		g_key_file_get_integer(confile, "DEFAULT", "ID", NULL);
	}
    }

    gchar *link_name;
    gchar *link_url;
    gchar default_name[20];
    g_sprintf(default_name, "LINK%d", default_id);
    if (g_key_file_has_group(confile, default_name)) {

	link_name =
	    g_key_file_get_string(confile, default_name, "NAME", NULL);
	link_url =
	    g_key_file_get_string(confile, default_name, "URL", NULL);
    }


    linkfile->LINK_NAME = link_name;
    linkfile->LINK_URL = link_url;

    g_key_file_free(confile);

}


/**
 *
 *TODO considering which return type play well;
 * */
guint cc_link_setting_get_all(const char *file, GArray ** array)
{
    guint rtval = 0;
    GKeyFile *confile = g_key_file_new();
    g_key_file_load_from_file(confile, file, G_KEY_FILE_NONE, NULL);

    gchar **groups = g_key_file_get_groups(confile, NULL);
    const int default_id =
	g_key_file_get_integer(confile, "DEFAULT", "ID", NULL);
    char gpnm[20];
    g_sprintf(gpnm, "LINK%d", default_id);
    char *link_name, *link_url;


    for (int i = 0; groups[i] != NULL; i++) {
	if (strcmp(groups[i], "DEFAULT") == 0)
	    continue;
	link_name =
	    g_key_file_get_string(confile, groups[i], "NAME", NULL);
	link_url = g_key_file_get_string(confile, groups[i], "URL", NULL);
	g_print(":%s :: ", link_name);
	if (strcmp(gpnm, groups[i]) == 0) {
	    g_print("%s : [Default]\n", link_url);
	} else {
	    g_print("%s :\n", link_url);
	}
	g_free(link_name);
	g_free(link_url);
    }
    g_strfreev(groups);
    return rtval;

}

void cc_link_setting_print(const char *file)
{
    guint rtval = 0;
    GKeyFile *confile = g_key_file_new();
    g_key_file_load_from_file(confile, file, G_KEY_FILE_NONE, NULL);

    gchar **groups = g_key_file_get_groups(confile, NULL);
    const int default_id =
	g_key_file_get_integer(confile, "DEFAULT", "ID", NULL);
    char gpnm[20];
    g_sprintf(gpnm, "LINK%d", default_id);
    char *link_name, *link_url;


    for (int i = 0; groups[i] != NULL; i++) {
	if (strcmp(groups[i], "DEFAULT") == 0)
	    continue;
	link_name =
	    g_key_file_get_string(confile, groups[i], "NAME", NULL);
	link_url = g_key_file_get_string(confile, groups[i], "URL", NULL);
	g_print(":%s :: ", link_name);
	if (strcmp(gpnm, groups[i]) == 0) {
	    g_print("%s : [Default]\n", link_url);
	} else {
	    g_print("%s :\n", link_url);
	}
	g_free(link_name);
	g_free(link_url);
    }
    g_strfreev(groups);

}





void cc_link_setting_add_link(const gchar * file, linker * linkfile)
{

    GKeyFile *confile = g_key_file_new();
    g_key_file_load_from_file(confile, file, G_KEY_FILE_NONE, NULL);

    int i;
    gchar gpnm[20];
    for (i = 0;; i++) {
	g_sprintf(gpnm, "LINK%d", i);
	if (!g_key_file_has_group(confile, gpnm)) {
	    g_print("does not has group [%s] \n", gpnm);
	    break;
	}
    }
//    g_print("i==%d.\n[%s]",i,gpnm);

    if (!g_key_file_has_group(confile, "DEFAULT")) {
	g_key_file_set_integer(confile, "DEFAULT", "ID", i);
    } else {
	if (g_key_file_has_key(confile, "DEFAULT", "ID", NULL)) {
	    g_key_file_remove_key(confile, "DEFAULT", "ID", NULL);
	}
	g_key_file_set_integer(confile, "DEFAULT", "ID", i);
    }


    g_key_file_set_string(confile, gpnm, "NAME", linkfile->LINK_NAME);
    g_key_file_set_string(confile, gpnm, "URL", linkfile->LINK_URL);

    gchar *fcnt = g_key_file_to_data(confile, NULL, NULL);

    FILE *fp = fopen(file, "w");
    fprintf(fp, "%s", fcnt);
    fclose(fp);
    g_free(fcnt);
    g_key_file_free(confile);
}

void cc_link_setting_remove_link(const gchar * file, const gchar * link)
{
    GKeyFile *confile = g_key_file_new();
    g_key_file_load_from_file(confile, file, G_KEY_FILE_NONE, NULL);

    g_key_file_remove_group(confile, link, NULL);
    gchar *fcnt = g_key_file_to_data(confile, NULL, NULL);

    FILE *fp = fopen(file, "w");
    fprintf(fp, "%s", fcnt);
    g_free(fcnt);
    g_key_file_free(confile);
    fclose(fp);

}

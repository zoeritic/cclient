#include "cc-utils.h"
#include"config.h"
//#include <glib.h>







void cc_run_external_async(gchar*cmd)
{
    GError*err;
gboolean prt=g_spawn_command_line_async(cmd,&err);

if(!prt){
    g_error("Execution [%s] failed!!::%s::",cmd,err->message);
    g_error_free(err);
}
}

void cc_run_external_sync(gchar*cmd)
{
    gchar* pout,*perr;
    gint srt;
    GError*err;
    gboolean prt=g_spawn_command_line_sync(cmd,&pout,&perr,&srt,&err);

if(!prt){
    g_error("Executing [%s] failed!!::%s::",cmd,err->message);
    g_error_free(err);
}
    g_message("Executing [%s]with[%d] :\nSTDOUT::%s \nSTDERR::%s",cmd,srt,pout,perr);
}















gchar *cc_get_addr_from_fullurl(gchar * url)
/**
* Translate fullurl like: `https://host.domain.net:port/ident` to `host.domain.net'
*/
{
    gchar **strs = g_strsplit(url, "/", 0);
    gchar *addr = NULL;
    gchar *port = NULL;
    gchar *pi = 0;
    int i;
    for (i = 0; strs[i] != NULL; i++) {
	if (strchr(strs[i], '.') != NULL) {
	    pi = strchr(strs[i], ':');
        if(pi!=NULL)
    	    *pi = 0;
	    addr = g_strdup(strs[i]);
        
        if(pi!=NULL)
            *pi = ':';
	}
    }

    g_strfreev(strs);

    return (gchar*)addr;

}

gchar *cc_get_port_from_fullurl(gchar * url)
/**
* Translate fullurl like: `https://host.domain.net:port/ident` to `port'
*/
{
    gchar **strs = g_strsplit(url, "/", 0);
//gchar*addr=NULL;
    gchar *port = NULL;
    gchar *pi = NULL;
    int i;
    for (i = 0; strs[i] != NULL; i++) {
	if (strchr(strs[i], '.') != NULL) {
	    pi = strchr(strs[i], ':');
        if(pi!=NULL)
    	    port = g_strdup(pi + 1);
        else
            port=g_strdup("");
	}
    }
    g_strfreev(strs);

    return (gchar*)port;

}



/*get status from stderr output of subprocess*/
gchar* cc_get_state(gchar* ss)
{

    gchar*buf=g_strdup(ss);

    gchar* l=strstr(buf,"::::[");
    gchar*r;
    if(l){
    
    r=strstr(buf,"]::::");
    
    l=l+5;
    *r='\0';
    
    gchar*nbuf=g_strdup_printf("%s",l);
    g_free(buf);
//    g_print("]%s[",nbuf);
    return nbuf;
    }

    return g_strdup("None");

}








/*
int main(int argc, char **argv)
{


    gchar *addr = cc_get_addr_from_fullurl(argv[1]);
    gchar *port = cc_get_port_from_fullurl(argv[1]);

    g_print("Addr :%s \n", addr);
    g_print("Port :%s \n", port);
    g_free(addr);
    g_free(port);

    return 0;
}*/

#ifndef __H_UTILS_CC_
#define __H_UTILS_CC_


#include<glib.h>
#include<string.h>

gchar *cc_get_addr_from_fullurl(gchar * url);
gchar *cc_get_port_from_fullurl(gchar * url);

gchar* cc_get_state(gchar* ss);


#endif

#include"cc-utils.h"
#include<glib.h>

int main(int argc,char**argv)
{


    gchar*b="sdsdsdsd::::[HELLO]::::";

    b="   ";

    gchar*n=cc_get_state(b);


    g_print("---%s---\n",n);


    g_free(n);
    return 0;


}

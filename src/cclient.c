#include<gtk/gtk.h>
#include"cc-manager-applet.h"
#include"config.h"


static GMainLoop *loop = NULL;


int main(int argc, char **argv)
{

int d=g_chdir(RUN_PATH);
if(d!=0){
	g_critical("Change director to [%s] !",RUN_PATH);
}

    gtk_init(&argc, &argv);


//    g_print("1111111111111\n");
    loop = g_main_loop_new(NULL, FALSE);
    g_print("main loop newed\n");

    CCManagerApplet *applet = cc_manager_applet_new(loop);
    g_print("ccmapplet  newed\n");
    gboolean rt = cc_manager_applet_setup_widget(applet);
    if (!rt) {
	g_error("cc_manager_applet_setup()\n");
    }
//   g_print("0000000000000000");

    g_main_loop_run(loop);
    g_object_unref(G_OBJECT(applet));






    return 0;

}

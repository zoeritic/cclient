#ifndef __H_MANAGER_APPLET_CC_

#define __H_MANAGER_APPLET_CC_

#include<gtk/gtk.h>
#include<string.h>
#include"cc-loging-win.h"
#include"cc-manager-panel.h"

#define PASSWD "123"
#define ICONPATH "cclient.png"


#define CC_TYPE_MANAGERAPPLET          (cc_manager_applet_get_type())
#define CC_MANAGERAPPLET(object)       (G_TYPE_CHECK_INSTANCE_CAST((object), CC_TYPE_MANAGERAPPLET, CCManagerApplet))
#define CC_MANAGERAPPLET_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST((klass), CC_TYPE_MANAGERAPPLET, CCManagerAppletClass))
#define CC_IS_MANAGERAPPLET(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), CC_TYPE_MANAGERAPPLET))
#define CC_IS_MANAGERAPPLET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), CC_TYPE_MANAGERAPPLET))
#define CC_MANAGERAPPLET_GET_CLASS(object)(G_TYPE_INSTANCE_GET_CLASS((object), CC_TYPE_MANAGERAPPLET, CCManagerAppletClass))


typedef struct {
    GObjectClass parent_class;

} CCManagerAppletClass;




typedef struct _CCManagerApplet {

    GObject parent_instance;
    GMainLoop *loop;

    GtkStatusIcon *status_icon;
    CCLogingWin *loging_win;

    GtkWidget *menu;
    GtkWidget *confirm_dialog;
    CCMPanel *manager_panel;	//pop up till authentication successfully.
//    CCLogingWin *cclient_win;	//background window[cc-loging].


    gboolean is_info_showed;
    gboolean is_confirm_showed;


} CCManagerApplet;




GType cc_manager_applet_get_type(void);

CCManagerApplet *cc_manager_applet_new(GMainLoop * loop);
gboolean cc_manager_applet_setup_widget(CCManagerApplet * applet);
void cc_manager_applet_update_icon(CCManagerApplet * applet);



#endif

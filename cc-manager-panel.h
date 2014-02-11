#ifndef __H_MANAGER_PANEL_CC_
#define __H_MANAGER_PANEL_CC_

#include<gtk/gtk.h>
#include"cc-link-info.h"
#include"cc-loging-win.h"

//#define UI_MPANEL "./cc-manager-panel.ui"
//#define URL_SCHEME "https://%s.zjcloud.net:8443"


#define CC_TYPE_MPANEL (cc_mpanel_get_type())
#define CC_MPANEL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),CC_TYPE_MPANEL,CCMPanel))
#define CC_IS_MPANEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),CC_TYPE_MPANEL))
#define CC_IS_MPANEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),CC_TYPE_MPANEL))
#define CC_MPANEL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),CC_TYPE_MPANEL,CCMPanelClass))
#define CC_MPANEL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj),CC_TYPE_MPANEL,CCMPanel))


extern CCLinkInfo* glink;

typedef struct _CCMPanel CCMPanel;

struct _CCMPanel{

    GObject parent_instance;
    GtkBuilder*builder;
    CCLinkInfo *link;
    GtkWidget*topwin;

};



typedef struct _CCMPanelClass CCMPanelClass;

struct _CCMPanelClass{
GObjectClass parent_class;


};



GType cc_mpanel_get_type(void);
CCMPanel* cc_mpanel_new(GtkWindow*);
void cc_mpanel_destroy(CCMPanel*);
void cc_mpanel_setup(CCMPanel*panel);





#endif


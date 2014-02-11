#ifndef __LOGING_API_CC_
#define __LOGING_API_CC_

#include<gtk/gtk.h>
#include"cc-loging-info.h"
#include"cc-link-info.h"
#include"cc-loging-ovirt.h"


#define GET_DOMAIN_ONCE 1


#define RESTOVIRT_DIR "./rest-ovirt/"

#define UI_LOGING_WIN "cc-loging-win.ui"
#define UI_LOGING_WINER "cc-loging-winer.ui"

#define INI_LOGING_CONFIG "./cc-loging-config.ini"
#define INI_LINK_CONFIG "./cc-link-config.ini"


#define CC_TYPE_LOGINGWIN (cc_loging_win_get_type())
#define CC_LOGINGWIN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),CC_TYPE_LOGINGWIN,CCLogingWin))
#define CC_LOGINGWIN_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),CC_TYPE_LOGINGWIN,CCLogingWinClass))
#define CC_IS_LOGINGWIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),CC_TYPE_LOGINGWIN))
#define CC_IS_LOGINGWIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),CC_TYPE_LOGINGWIN))

#define CC_LOGINGWIN_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj),CC_TYPE_LOGINGWIN,CCLogingWinClass))


extern CCLinkInfo*glink;


typedef struct _WinWidgets{
    GtkWidget*entry_user;
    GtkWidget*entry_passwd;
    GtkWidget*sav_passwd;
    GtkWidget*combo_domain;
    GtkWidget*login;

}WinWidgets;
typedef struct _WinerWidgets{
    GtkWidget*but_back;
//    GtkWidget*but_view;
    GtkWidget*but_start;
    GtkWidget*but_shutdown;
    GtkWidget*but_kill;
    GtkWidget*but_refresh;

    GtkWidget*but_view;

}WinerWidgets;



typedef struct _CCLogingWin CCLogingWin;
struct _CCLogingWin {

//    GtkWindow parent;
    GObject parent_instance;
    CCLogingInfo *info;
    CCOvirtInfo *ovirt;
//    CCLinkInfo *link;  move to global
    GtkBuilder *builder;
//
    GtkWidget*win;
    GtkWidget*winer;

    WinWidgets*wins;
    WinerWidgets*winers;

};
typedef struct _CCLogingWinClass CCLogingWinClass;

struct _CCLogingWinClass {
    GObjectClass parent_class;

};







GType cc_loging_win_get_type(void);

CCLogingWin *cc_loging_win_new(void);
void cc_loging_win_destroy(CCLogingWin * win);
//CCLogingInfo *cc_loging_win_get(CCLogingWin*self);

void cc_loging_winer_clean(CCLogingWin*self);
void cc_loging_winer_setup(CCLogingWin * self);
void cc_loging_win_clean(CCLogingWin*self);
void cc_loging_win_setup(CCLogingWin * self);
void cc_loging_win_initialize(CCLogingWin*self);






#endif

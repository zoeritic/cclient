#include"cc-loging-win.h"
#include"cc-ovirt-vm.h"
//#include"cc-ovirt-vm.h"
//#include"cc-loging-ovirt.h"

G_DEFINE_TYPE(CCLogingWin, cc_loging_win, G_TYPE_OBJECT);

#ifdef GET_DOMAIN_ONCE
extern gboolean domain_getten;
#endif

extern gchar*vm_stat_expect;

WinWidgets *win_widgets_new()
{
    return (WinWidgets *) g_new0(WinWidgets, 1);

}


WinerWidgets *winer_widgets_new()
{
    return (WinerWidgets *) g_new0(WinerWidgets, 1);

}




typedef enum _CCLogingProp {
    PROP_0,
    PROP_USER,
    PROP_PASSWD,
    PROP_SAV_PASSWD,
    PROP_DOMAIN,
    LAST_PROP
} CCLogingProp;

static void cc_loging_win_get_property(GObject * object,
				       guint prop_id, GValue * value,
				       GParamSpec * nspec);

static void cc_loging_win_set_property(GObject * object,
				       guint prop_id, const GValue * value,
				       GParamSpec * nspec);

void cc_loging_switch2winer(CCLogingWin * self);
void cc_loging_switch2win(CCLogingWin * self);

void cc_loging_win_setup(CCLogingWin * self);
void cc_loging_winer_setup(CCLogingWin * self);
void cc_loging_win_clean(CCLogingWin * self);
void cc_loging_winer_clean(CCLogingWin * self);





static void cc_loging_win_get_property(GObject * object,
				       guint prop_id, GValue * value,
				       GParamSpec * nspec)
{
    CCLogingWin *self = CC_LOGINGWIN(object);

    switch (prop_id) {

    case PROP_USER:
	g_value_set_string(value, self->info->user);
	break;
    case PROP_PASSWD:
	g_value_set_string(value, self->info->passwd);
	break;
    case PROP_DOMAIN:
	g_value_set_string(value, self->info->domain);
	break;
    case PROP_SAV_PASSWD:
	g_value_set_boolean(value, self->info->sav_passwd);
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, nspec);
	break;
    }

}



static void cc_loging_win_set_property(GObject * object,
				       guint prop_id, const GValue * value,
				       GParamSpec * nspec)
{
    CCLogingWin *self = CC_LOGINGWIN(object);

    switch (prop_id) {

    case PROP_USER:
	self->info->user = (gchar *) g_value_dup_string(value);
//    gtk_entry_set_text()
//      g_print("USER:\t\t[%s]\n", (self->info).user);
	break;

    case PROP_PASSWD:
	self->info->passwd = (gchar *) g_value_dup_string(value);
//      g_print("PASSWD:\t\t[%s]\n", self->info.passwd);
	break;

    case PROP_DOMAIN:
	self->info->domain = (gchar *) g_value_dup_string(value);
//      g_print("DOMAIN:\t\t[%s]\n", self->info.domain);
	break;

    case PROP_SAV_PASSWD:
	self->info->sav_passwd = g_value_get_boolean(value);
//      g_print("[%d]\n", self->info.sav_passwd);
	break;

    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, nspec);
	g_print("Invalid..\n");
	break;

    }

}



static gboolean cc_loging_authorize(CCLogingWin *win)
{

    gboolean rtval=FALSE;
    gchar* cmd=g_strdup_printf("./rest-ovirt/vm-auth.py %s %s %s",
            glink->addr,
            win->info->user,
            win->info->passwd);
    gchar*out,*err;
//    gint status;
    GError*e=NULL;//g_error_new();
    g_message("==-=-=-=::%s:: ",cmd);

    if (!g_spawn_command_line_sync(cmd,&out,&err,NULL,&e))
    {
        g_critical("SPAWN ERROR :USER Authorized!!-%s-",e->message);
    g_error_free(e);
    g_free(cmd);
        return FALSE;

    }else{
    
    g_print("OUT::%s:: \tERR::%s::\n",out,err);
//    g_free(out);
//    g_free(err);
    if(NULL!=strstr(out,"AUTH-OK"))
        rtval=TRUE;
    g_error_free(e);
    g_free(cmd);
    return rtval;
    } 
}



static void cc_loging_winer_goback_cb(GtkWidget * widget,
				      CCLogingWin * logingwin)
{
    g_message("switch to win\n");

//    cc_loging_switch2win(logingwin);
    cc_loging_win_setup(logingwin);
    cc_loging_winer_clean(logingwin);

}

static gboolean cc_loging_winer_console_cb(GtkWidget * widget,
					   GdkEventButton * event,
					   CCLogingWin * win)
{
    if (event->type == GDK_2BUTTON_PRESS) {
	g_message("open console\n");
  // 

    CCOvirtVM *ovm = cc_ovirt_vm_new();
    ovm->win = win;
    cc_ovirt_vm_console( ovm);
    }
    return FALSE;
}

static void cc_loging_winer_vm_up_cb(GtkWidget * widget, CCLogingWin * win)
{    
    CCOvirtVM *ovm = cc_ovirt_vm_new();
    ovm->win = win;
    cc_ovirt_vm_start_r(ovm);
//    CCOvirtVM *svm = cc_ovirt_vm_new();
//    svm->win = win;
    vm_stat_expect=g_strdup_printf("%s","up");
//    cc_ovirt_stating_vm_r(svm);//move into spawn_async_vm()
    g_message("boot VM\n");
}

static void cc_loging_winer_vm_down_cb(GtkWidget * widget,
				       CCLogingWin * win)
{
    CCOvirtVM *ovm = cc_ovirt_vm_new();
    ovm->win = win;
    cc_ovirt_vm_shutdown_r(ovm);
//    CCOvirtVM *svm = cc_ovirt_vm_new();
//    svm->win = win;
    vm_stat_expect=g_strdup_printf("%s","down");
//    cc_ovirt_stating_vm_r(svm);
    g_message("halt VM\n");
}

static void cc_loging_winer_vm_kill_cb(GtkWidget * widget,
				       CCLogingWin * win)
{
    CCOvirtVM *ovm = cc_ovirt_vm_new();
    ovm->win = win;
    cc_ovirt_vm_kill_r(ovm);
//    CCOvirtVM *svm = cc_ovirt_vm_new();
//    svm->win = win;
    vm_stat_expect=g_strdup_printf("%s","down");
//    cc_ovirt_stating_vm_r(svm);
    g_message("force kill VM\n");
}

static void cc_loging_winer_vm_fresh_cb(GtkWidget * widget,
					CCLogingWin * win)
{
    CCOvirtVM *ovm = cc_ovirt_vm_new();
    ovm->win = win;
    cc_ovirt_vm_stat_r(ovm);
    g_message("fresh VM\n");
}

static void cc_loging_winer_vm_stat(CCLogingWin*win)
{

    CCOvirtVM *ovm = cc_ovirt_vm_new();
    ovm->win = win;
    cc_ovirt_vm_stat_r(ovm);
    g_message("STAT VM\n");

}



void cc_loging_winer_clean(CCLogingWin * self)
/**
 * clean *builder ,*winer
 */
{
/*    if(self->builder)
g_object_unref(self->builder);
self->builder=NULL;

*/
    if (self->winer) {
	gtk_widget_destroy(self->winer);
//g_object_unref(self->winer);remove or double free
	self->winer = NULL;
    }


    g_free(self->winers);

    g_message("Winer Clean completed..");

}

void cc_loging_winer_setup(CCLogingWin * self)
{
    if (self->builder) {
//    GtkBuilder * builder = self->builder;
//    g_message("Winer Setup:Builder=%x[old] ",builder);
	g_object_unref(self->builder);
	self->builder = NULL;
    }

    self->builder = gtk_builder_new_from_file(UI_LOGING_WINER);
    GtkBuilder *builder = self->builder;
    g_message("Winer Setup:Builder=%x ", builder);

    self->winer = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *winer = self->winer;

    GtkWidget *but_console =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_console"));
    g_critical("but_console:%x", but_console);
    GtkWidget *but_back =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_back"));

    GtkWidget *but_up =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_up"));
    GtkWidget *but_down =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_down"));
    GtkWidget *but_kill =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_kill"));
    GtkWidget *but_fresh =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_fresh"));


    self->winers = winer_widgets_new();

    self->winers->but_back = but_back;
    self->winers->but_console = but_console;
    self->winers->but_start = but_up;
    self->winers->but_shutdown = but_down;
    self->winers->but_kill = but_kill;
    self->winers->but_fresh = but_fresh;



    g_signal_connect(G_OBJECT(but_console), "button_press_event",
		     G_CALLBACK(cc_loging_winer_console_cb), self);

    g_signal_connect(G_OBJECT(but_back), "clicked",
		     G_CALLBACK(cc_loging_winer_goback_cb), self);

    g_signal_connect(G_OBJECT(but_up), "clicked",
		     G_CALLBACK(cc_loging_winer_vm_up_cb), self);


    g_signal_connect(G_OBJECT(but_down), "clicked",
		     G_CALLBACK(cc_loging_winer_vm_down_cb), self);
    g_signal_connect(G_OBJECT(but_kill), "clicked",
		     G_CALLBACK(cc_loging_winer_vm_kill_cb), self);
    g_signal_connect(G_OBJECT(but_fresh), "clicked",
		     G_CALLBACK(cc_loging_winer_vm_fresh_cb), self);

    g_print("====in Winer Setup===\n");
    cc_link_info_print(glink);
    cc_loging_info_print(self->info);
    cc_ovirt_info_print(self->ovirt);
/*
    gtk_window_maximize(GTK_WINDOW(winer));
*/
    gtk_widget_grab_focus(winer);
    gtk_widget_show_all(winer);
//get_vm_stat
    cc_loging_winer_vm_stat(self);
    g_message("Winer Setup completed..");


}

static gboolean cc_entry_user_invalid_cb(GtkWidget * widget,
					 GdkEvent * event, gpointer data)
{

    const gchar *cnt = gtk_entry_get_text(GTK_ENTRY(widget));
    if (!strcmp(cnt, "用户名不能为空"))
	gtk_entry_set_text(GTK_ENTRY(widget), "");
    return FALSE;
}

static gboolean cc_entry_passwd_invalid_cb(GtkWidget * widget,
					   GdkEvent * event, gpointer data)
{

    const gchar *cnt = gtk_entry_get_text(GTK_ENTRY(widget));
    if (!strcmp(cnt, "密码不能为空"))
	gtk_entry_set_text(GTK_ENTRY(widget), "");
    gtk_entry_set_visibility(GTK_ENTRY(widget), FALSE);
    return FALSE;
}



static gboolean cc_loging_win_login_check_valid(CCLogingInfo * info,
						CCLogingWin * win)
{

    g_print("login valid checking!!\n");
    cc_loging_info_print(info);
    gboolean rtval = TRUE;

    if (strlen(info->user) == 0
	|| strcmp(info->user, "用户名不能为空") == 0) {
	GtkWidget *w = win->wins->entry_user;
	//GTK_WIDGET(gtk_builder_get_object(win->builder, "entry_user"));
	gtk_entry_set_text(GTK_ENTRY(w), "用户名不能为空");
	rtval = FALSE;
	g_critical("Username is required!!");
	g_signal_connect(G_OBJECT(w), "focus-in-event",
			 G_CALLBACK(cc_entry_user_invalid_cb), w);
    }

    if (strlen(info->passwd) == 0
	|| strcmp(info->passwd, "密码不能为空") == 0) {
	GtkWidget *w = win->wins->entry_passwd;
	//GTK_WIDGET(gtk_builder_get_object(win->builder, "entry_passwd"));
	gtk_entry_set_text(GTK_ENTRY(w), "密码不能为空");
	gtk_entry_set_visibility(GTK_ENTRY(w), TRUE);
	rtval = FALSE;
	g_critical("Password is required!!");
	g_signal_connect(G_OBJECT(w), "focus-in-event",
			 G_CALLBACK(cc_entry_passwd_invalid_cb), w);
    }

    return rtval;

}



static void cc_loging_win_button_login_cb(GtkWidget * widget,
					  CCLogingWin * win)
    /*realize *ovirt
     * */
{
//    cc_loging_switch2winer(win);
    CCLogingInfo *info = win->info;

    const gchar *user =
	gtk_entry_get_text(GTK_ENTRY(win->wins->entry_user));

    g_critical("user=%s", user);


    gchar *domain =
	gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT
					   (win->wins->combo_domain));

    g_critical("doamin=%s", domain);
    gboolean sav_passwd =
	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON
				     (win->wins->sav_passwd));


    const gchar *passwd =
	gtk_entry_get_text(GTK_ENTRY(win->wins->entry_passwd));


    g_critical("passwd=%s", passwd);




    cc_loging_info_set(info, PROP_INFO_USER, user,
		       PROP_INFO_PASSWD, passwd,
		       PROP_INFO_DOMAIN, domain,
		       PROP_INFO_SAVPASSWD, sav_passwd, NULL);

    //checkout login infomation
    gboolean valid = cc_loging_win_login_check_valid(info, win);
    if (!valid) {
	g_critical("login clicked  do Nothing!!");
	return;
    }

    if (!sav_passwd) {
	cc_loging_info_set(info, PROP_INFO_PASSWD, "", NULL);
	cc_loging_info_to_file(info, INI_LOGING_CONFIG);
	cc_loging_info_set(info, PROP_INFO_PASSWD, passwd, NULL);
    } else {
	cc_loging_info_to_file(info, INI_LOGING_CONFIG);
    }

    g_print("IN Login Callback\n");
    cc_loging_info_print(info);
    /*read new link settings..
     * */
//      cc_link_info_read_file(glink, INI_LINK_CONFIG);
//    g_message("switch to winer\n");

    cc_ovirt_info_update(win->ovirt, info, glink);
    g_print("IN Login Callback\n");


    cc_ovirt_info_print(win->ovirt);
    g_free(domain);
    gboolean au=cc_loging_authorize(win);

    if(!au)
        return;

    cc_loging_winer_setup(win);
    cc_loging_win_clean(win);

}

static void cc_loging_win_user_act_cb(GtkWidget * widget,
				      CCLogingWin * win)
{
    GtkWidget *entry_passwd =
//      GTK_WIDGET(gtk_builder_get_object(win->builder, "entry_passwd"));
	win->wins->entry_passwd;
    gtk_widget_grab_focus(entry_passwd);
}

static void cc_loging_win_passwd_act_cb(GtkWidget * widget,
					CCLogingWin * win)
{
    GtkWidget *but_login = win->wins->login;
//      GTK_WIDGET(gtk_builder_get_object(win->builder, "but_login"));
    gtk_widget_grab_focus(but_login);
}

void cc_loging_win_clean(CCLogingWin * self)
/**
 *clean *builder ,*win
 */
{
/*    if(self->builder){
        g_object_unref(self->builder);
        self->builder=NULL;
    }

*/ if (self->win) {
	gtk_widget_destroy(self->win);
//g_object_unref(self->win);
	self->win = NULL;
    }
//FIXME free widgets that gain from builder.
    g_free(self->wins);

    g_message("Win Clean completed..");

}



void cc_loging_win_setup(CCLogingWin * self)
{

    g_message("enter win setup");
    if (self->builder) {
	g_object_unref(self->builder);
	self->builder = NULL;
    }
//    if(self->win){}


//    g_message("enter win setup-1");
    self->builder = gtk_builder_new_from_file(UI_LOGING_WIN);
    GtkBuilder *builder = self->builder;
    GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    self->win = win;

//    g_message("enter win setup0");
    GtkWidget *entry_user =
	GTK_WIDGET(gtk_builder_get_object(builder, "entry_user"));
    GtkWidget *entry_passwd =
	GTK_WIDGET(gtk_builder_get_object(builder, "entry_passwd"));
    GtkWidget *chk_sav =
	GTK_WIDGET(gtk_builder_get_object(builder, "chk_savpasswd"));
    GtkWidget *combo_domain =
	GTK_WIDGET(gtk_builder_get_object(builder, "combo_domain"));
    GtkWidget *but_login =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_login"));


    self->wins = win_widgets_new();

    self->wins->entry_user = entry_user;
    self->wins->entry_passwd = entry_passwd;
    self->wins->sav_passwd = chk_sav;
    self->wins->combo_domain = combo_domain;
    self->wins->login = but_login;



//    CCLogingInfo*info=win->info;
    gchar *default_user = self->info->user;
    g_message("::::default_user:%s", default_user);
    gtk_entry_set_text(GTK_ENTRY(entry_user),
		       default_user ? default_user : "");

    gboolean sav_passwd = self->info->sav_passwd;

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chk_sav), sav_passwd);

    gchar *passwd = self->info->passwd;
    g_message("::::passwd:%s", passwd);
    gtk_entry_set_text(GTK_ENTRY(entry_passwd), sav_passwd ? passwd : "");

    gchar *domain = self->info->domain;
    g_message("::::domain:%s", domain);
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo_domain), 0,
				   domain);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_domain), 0);

#ifdef GET_DOMAIN_ONCE
    if (!domain_getten) {
#endif
	gtk_widget_set_sensitive(combo_domain, FALSE);
	gtk_widget_set_sensitive(but_login, FALSE);
#ifdef GET_DOMAIN_ONCE
    }
#endif

//    CCOvirtInfo *oinfo=g_slice_new(CCOvirtInfo);
//    win->ovirt=oinfo;
//    oinfo->widget=but_login;
//    oinfo->combo_domain=combo_domain;

    gtk_widget_grab_focus(entry_user);
    gtk_editable_select_region(GTK_EDITABLE(entry_user), 0, 0);
    gtk_editable_set_position(GTK_EDITABLE(entry_user), -1);
//    gtk_window_set_keep_above(GTK_WINDOW(win0),TRUE);
//    gtk_widget_activate(win0);
    gtk_grab_add(win);
//    gtk_widget_grab_focus(but_login);

    g_signal_connect(G_OBJECT(but_login), "clicked",
		     G_CALLBACK(cc_loging_win_button_login_cb), self);
    g_signal_connect(G_OBJECT(but_login), "activate",
		     G_CALLBACK(cc_loging_win_button_login_cb), self);
    g_signal_connect(G_OBJECT(entry_user), "activate",
		     G_CALLBACK(cc_loging_win_user_act_cb), self);
    g_signal_connect(G_OBJECT(entry_passwd), "activate",
		     G_CALLBACK(cc_loging_win_passwd_act_cb), self);


//    gtk_window_maximize(GTK_WINDOW(win));
    gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(win);

    g_message("enter win setup1");
/*
    cc_loging_info_set(self->info,PROP_INFO_USER,default_user,
            PROP_INFO_PASSWD,passwd,
            PROP_INFO_DOMAIN,domain,
            PROP_INFO_SAVPASSWD,sav_passwd,NULL);

*/
    g_print("====in Win Setup====\n");
    cc_loging_info_print(self->info);

    cc_ovirt_info_update(self->ovirt, self->info, glink);
//    self->ovirt->combo_domain=combo_domain;
//    self->ovirt->widget=but_login;

//    g_message("combodomain==%x", combo_domain);
//    g_message("buttonlogin==%x", but_login);
    cc_ovirt_info_print(self->ovirt);
//    if(!domain_getten)
//    g_message("enter win setup2");


//#ifdef GET_DOMAIN_ONCE

    CCOvirtVM *ovm = cc_ovirt_vm_new();

    ovm->win = self;
    cc_loging_ovirt_get_domain_r(ovm);

//#endif

    g_message("Win Setup completed..");
}








static void cc_loging_win_init(CCLogingWin * self)
{
//    GtkWidget*win=GTK_WINDOW(self);
//    gtk_window_set_decorated(GTK_WINDOW(win),FALSE);
//    gtk_window_maximize(GTK_WINDOW(win));

//    cc_loging_info_print(&self->info);
//
    self->info = cc_loging_info_new();
    g_print("cc-loging-win:initialized\n");
}

void cc_loging_win_initialize(CCLogingWin * self)
{
    g_message("Entering win initialize..");
    if (glink == NULL)
	glink = cc_link_info_new();	//global variable
    if (0 == access(INI_LINK_CONFIG, 0))
	cc_link_info_read_file(glink, INI_LINK_CONFIG);
    else
	cc_link_info_set(glink, PROP_LINK_NAME, "Default", PROP_LINK_URL,
			 "https://.zjcloud.net:8443", NULL);
    cc_link_info_print(glink);
//    self->builder = gtk_builder_new_from_file(UI_LOGING_WIN);
    if (self->info == NULL)
	self->info = cc_loging_info_new();
    g_message("INIT::self->info==%x", self->info);

    if (0 == access(INI_LOGING_CONFIG, 0)) {
	cc_loging_info_read_file(self->info, INI_LOGING_CONFIG);
	g_message("Exists a loging info config file");
//    cc_loging_info_print(self->info);

    } else {
	g_message("Dose not Exists a loging info config file");
	cc_loging_info_set(self->info, PROP_INFO_USER, "test",
			   PROP_INFO_PASSWD, "",
			   PROP_INFO_DOMAIN, "ccloud",
			   PROP_INFO_SAVPASSWD, FALSE, NULL);
	cc_loging_info_to_file(self->info, INI_LOGING_CONFIG);
    }

    cc_loging_info_print(self->info);


    if (self->ovirt == NULL)
	self->ovirt = cc_ovirt_info_new();
    cc_ovirt_info_update(self->ovirt, self->info, glink);
    g_print("IN initialize...\n");
    cc_ovirt_info_print(self->ovirt);



}



static void cc_loging_win_class_init(CCLogingWinClass * klass)
{

    GObjectClass *base_class = G_OBJECT_CLASS(klass);
    base_class->set_property = cc_loging_win_set_property;
    base_class->get_property = cc_loging_win_get_property;

    GParamSpec *pspec =
	g_param_spec_string("user", "User", "Username for login",
			    "",
			    G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
    g_object_class_install_property(base_class, PROP_USER, pspec);


    pspec = g_param_spec_string("passwd", "Password", "Password for login",
				"", G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
    g_object_class_install_property(base_class, PROP_PASSWD, pspec);


    pspec =
	g_param_spec_string("domain", "Domain", "Domain in which login",
			    "", G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
    g_object_class_install_property(base_class, PROP_DOMAIN, pspec);


    pspec =
	g_param_spec_boolean("sav_passwd", "Save_Password",
			     "Saving password for login", FALSE,
			     G_PARAM_READWRITE | G_PARAM_CONSTRUCT);
    g_object_class_install_property(base_class, PROP_SAV_PASSWD, pspec);

}



CCLogingWin *cc_loging_win_new(void)
{
    CCLogingWin *n = g_object_new(CC_TYPE_LOGINGWIN, NULL);
    g_print("--   Loging_win[%x]", (guint) n);
//    cc_loging_win_setup(n);
    return n;
}

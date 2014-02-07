#include"cc-manager-applet.h"
//#include"cc-manager-panel.h"
//#include"cc-loging-win.h"
#include"cc-link-info.h"

CCLinkInfo*glink=NULL;


G_DEFINE_TYPE(CCManagerApplet, cc_manager_applet, G_TYPE_OBJECT)


static gboolean ccma_confirm_dialog_leave_cb(GtkWidget * widget,
					     GdkEvent * event,
					     gpointer data)
{
    CCManagerApplet *applet = CC_MANAGERAPPLET(data);

    applet->is_confirm_showed = FALSE;
    gtk_widget_destroy(widget);
    return FALSE;
}

static gboolean
ccma_leave_info_cb(GtkWidget * widget, GdkEvent * event, gpointer data)
{
    CCManagerApplet *app = CC_MANAGERAPPLET(data);
    if (app->is_info_showed)
	app->is_info_showed = FALSE;
    gtk_widget_destroy(widget);
    return FALSE;


}



static void ccma_menu_show_info_cb(GtkWidget * applet, gpointer menuit)
{
    if (CC_MANAGERAPPLET(applet)->is_info_showed)
	return;
    GtkWidget *infowin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(infowin), 330, 220);
    GtkWidget *box, *label;
    label = gtk_label_new("CCManagerApplet-0.1");
    gtk_container_add(GTK_CONTAINER(infowin), label);

    g_signal_connect(G_OBJECT(infowin), "delete-event",
		     G_CALLBACK(ccma_leave_info_cb), applet);

    gtk_widget_show(infowin);

}

static void ccma_menu_add_item(GtkWidget * menu,
			       CCManagerApplet * applet, gchar * text)
{
    GtkWidget *menu_item;
    g_return_if_fail(menu != NULL);
    g_return_if_fail(applet != NULL);

    menu_item = gtk_menu_item_new_with_label(text ? text : "More");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    g_signal_connect_swapped(menu_item, "activate",
			     G_CALLBACK(ccma_menu_show_info_cb), applet);



}



static void ccma_menu_show_cb(GtkWidget * menu, CCManagerApplet * applet)
{
    g_return_if_fail(menu != NULL);
    g_return_if_fail(applet != NULL);

    gtk_status_icon_set_tooltip_text(applet->status_icon, NULL);

    ccma_menu_add_item(menu, applet, "INFO");

    gtk_widget_show_all(menu);


}






static void ccma_menu_add_text_item(GtkWidget * menu, gchar * text)
{
    GtkWidget *menu_item;
    g_return_if_fail(menu != NULL);
    g_return_if_fail(text != NULL);

    menu_item = gtk_menu_item_new_with_label(text);
    gtk_widget_set_sensitive(menu_item, FALSE);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    gtk_widget_show(menu_item);

}

static void ccma_menu_add_separator_item(GtkWidget * menu)
{
    g_return_if_fail(menu != NULL);

    GtkWidget *menu_item;

    menu_item = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    gtk_widget_show(menu_item);
}


static gboolean check_passwd(const gchar * passwd)
{
    if (!strcmp(PASSWD, passwd))
	return TRUE;
    else
	return FALSE;

}

static void ccma_confirm_dialog_destroy(CCManagerApplet * app)
{
//    g_print("\n[:%d:] [:%d:]\n",CC_IS_MANAGERAPPLET(app),GTK_IS_WIDGET(app->confirm_dialog));
    gtk_widget_destroy(GTK_WIDGET(app->confirm_dialog));
    app->is_confirm_showed = FALSE;

}

static gboolean ccma_panel_disable_move(GtkWidget * widget,
					GdkEvent * event, gpointer data)
{
    g_print("============================================\n");
    return TRUE;

}



static void ccma_dialog_entry_enter_cb(GtkWidget * widget, gpointer data)
{

//    gtk_window_set_modal(GTK_WINDOW(widget),TRUE);
    CCManagerApplet *applet = CC_MANAGERAPPLET(data);
    GtkWidget *entry = widget;	//GTK_WIDGET(data);
    if (check_passwd(gtk_entry_get_text(GTK_ENTRY(entry)))) {
	ccma_confirm_dialog_destroy(applet);
	g_message("Authentication Successfilly!\n");
	//OK
//manager_panel();
	//spice_page(NULL);

    GtkWidget*parent=GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(applet->loging_win->builder),"window"));
	applet->manager_panel=CC_MPANEL(cc_mpanel_new(GTK_WINDOW(parent)));
    cc_mpanel_setup(applet->manager_panel);


    } else {
	gtk_entry_set_text(GTK_ENTRY(widget), "");

    }

}


static void ccma_dialog_response_cb(GtkDialog * dialog, gint res,
				    gpointer data)
{
//    return ;//

    //GtkWidget*entry=GTK_ENTRY(data);    
    CCManagerApplet *applet = CC_MANAGERAPPLET(data);

    GList *children =
	gtk_container_get_children(GTK_CONTAINER
				   (gtk_dialog_get_content_area(dialog)));
    GtkWidget*box=children->data;//box
    GList*bc=gtk_container_get_children(GTK_CONTAINER(box));
    GtkWidget*entry=GTK_WIDGET(g_list_next(bc)->data);


    /*FIXME get the correct entry widget */
//    GtkWidget *entry = GTK_WIDGET(g_list_next(children)->data);
//    GtWidget* entry=get_child_widget_by_type(dialog,GTK_TYPE_ENTRY);
//    g_list_free(children);


    gboolean passok;

    switch (res) {

    case GTK_RESPONSE_OK:
	passok = check_passwd(gtk_entry_get_text(GTK_ENTRY(entry)));
	if (passok) {
	    //OK
    GtkWidget*parent=GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(applet->loging_win->builder),"window"));
	applet->manager_panel=cc_mpanel_new(GTK_WINDOW(parent));
    cc_mpanel_setup(applet->manager_panel);
	    //
	    //
	    g_message("Authentication Successfilly!\n");
	    ccma_confirm_dialog_destroy(applet);

	} else {
	    gtk_entry_set_text(GTK_ENTRY(entry), "");
	}

	break;
    case GTK_RESPONSE_CANCEL:
	ccma_confirm_dialog_destroy(applet);
	break;
    default:
	break;

    }

}



static GtkWidget *ccma_confirm_dialog_new(CCManagerApplet * applet)
{
    g_message(" Entering ccma_confirm_dialog_new()..");
    GtkWidget *dialog, *label, *entry, *box, *bt_ok, *bt_cancel;
    if (CC_MANAGERAPPLET(applet)->is_confirm_showed == TRUE)
	return NULL;

    CC_MANAGERAPPLET(applet)->confirm_dialog = gtk_dialog_new();
    dialog = CC_MANAGERAPPLET(applet)->confirm_dialog;

    CC_MANAGERAPPLET(applet)->is_confirm_showed = TRUE;

    gtk_window_set_title(GTK_WINDOW(dialog), "管理认证");
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

    bt_ok =
	gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_OK,
			      GTK_RESPONSE_OK);
    bt_cancel =
	gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_CANCEL,
			      GTK_RESPONSE_CANCEL);
    label = gtk_label_new("口令 :");
    entry = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entry), 20);
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
    gtk_box_pack_start(GTK_BOX(box),label, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(box),entry, TRUE, TRUE, 10);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),box);

//    gtk_window_set_has_resize_grip(GTK_WINDOW(dialog),TRUE);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_widget_grab_focus(entry);

    g_signal_connect(dialog, "delete-event",
		     G_CALLBACK(ccma_confirm_dialog_leave_cb), applet);
    g_signal_connect(entry, "activate",
		     G_CALLBACK(ccma_dialog_entry_enter_cb), applet);
    g_signal_connect(dialog, "response",
		     G_CALLBACK(ccma_dialog_response_cb), applet);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_keep_above(GTK_WINDOW(dialog), TRUE);
//    gtk_widget_realize(dialog);
//    gtk_window_present(GTK_WINDOW(dialog));

//    g_message(" Entering ccma_confirm_dialog_new(1)..");
//    GtkWindow *bwin =
//	GTK_WINDOW(gtk_builder_get_object(applet->loging_win->builder, "window"));
//    g_message(" Entering ccma_confirm_dialog_new(2)..");

//    gtk_window_set_transient_for(GTK_WINDOW(dialog), bwin);
//    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    g_message("ccma_confirm_dialog_new()..");
    gtk_widget_show_all(dialog);
    return dialog;

}



static void
status_icon_activate_cb(GtkStatusIcon * icon, CCManagerApplet * applet)
{

    if (applet->menu)
	g_object_unref(applet->menu);
//open a confirm_dialog;

    ccma_confirm_dialog_new(applet);

    //make a fresh menu
/*
    applet->menu=gtk_menu_new();
    g_signal_connect(applet->menu,"show",G_CALLBACK(ccma_menu_show_cb),applet);
//    g_signal_connect(applet->menu,"deactivate",G_CALLBACK(ccma_menu_deactivate_cb),applet);

    gtk_menu_popup(GTK_MENU(applet->menu),NULL,NULL,gtk_status_icon_position_menu,icon,1,gdk_get_current_event_time());
*/
}

gboolean cc_manager_applet_setup_widget(CCManagerApplet * applet)
{
    g_return_val_if_fail(CC_IS_MANAGERAPPLET(applet), FALSE);

    applet->status_icon = gtk_status_icon_new();
    if (!applet->status_icon)
	return FALSE;
    gtk_status_icon_set_from_file(applet->status_icon, ICONPATH);


    g_signal_connect(applet->status_icon, "activate",
		     G_CALLBACK(status_icon_activate_cb), applet);

//setting CCLogingWin
    applet->loging_win = cc_loging_win_new();
//    g_message("dddddddddddddddddddddddddddddd");
    cc_loging_win_initialize(applet->loging_win);
    cc_loging_win_setup(applet->loging_win);
    g_message("win setuped in cc_manager_applet_setup_widget");
//    cc_loging_ovirt_get_domain_r(applet->loging_win->ovirt);
//    cc_loging_switch2win(applet->loging_win);
//    g_message("pppppppppppppppppppppppp%x ",applet->loging_win->info);
    cc_loging_info_print(applet->loging_win->info);

//    g_message("pppppppppppppppppppppppp");
//    g_print("     loging_win [%x]\n",applet->loging_win);
//    g_message("before switch2win  applet setup");
//    g_message("after switch2win  applet setup");
//    cc_loging_win_setup(applet->loging_win);


    return TRUE;
}





static void cc_manager_applet_init(CCManagerApplet * applet)
{
    //g_print("CCMA init...\n");
    applet->confirm_dialog = NULL;
    applet->manager_panel = NULL;
    applet->loging_win=NULL;
    applet->menu = NULL;
    applet->is_info_showed = FALSE;
    applet->is_confirm_showed = FALSE;

}

enum {
    PROP_0,
    PROP_LOOP,
    LAST_PROP
};


static void
cc_manager_applet_set_property(GObject * obj, guint prop_id,
			       const GValue * value, GParamSpec * pspec)
{
    CCManagerApplet *applet = CC_MANAGERAPPLET(obj);

    switch (prop_id) {

    case PROP_LOOP:
	applet->loop = g_value_get_pointer(value);
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
	break;
    }
}

static void cc_manager_applet_class_init(CCManagerAppletClass * klass)
{
//    g_print("----=CLASS\n");
    GObjectClass *oclass = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    oclass->set_property = cc_manager_applet_set_property;

    pspec =
	g_param_spec_pointer("loop", "Loop", "AppletMainloop",
			     G_PARAM_CONSTRUCT | G_PARAM_WRITABLE);
    g_object_class_install_property(oclass, PROP_LOOP, pspec);


}





CCManagerApplet *cc_manager_applet_new(GMainLoop * loop)
{
//    g_print("cc_manager_applet_new\n");
    return g_object_new(CC_TYPE_MANAGERAPPLET, "loop", loop, NULL);

}

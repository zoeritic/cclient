#include"cc-manager-panel.h"
#include"config.h"

G_DEFINE_TYPE(CCMPanel, cc_mpanel, G_TYPE_OBJECT)


typedef enum _CCMPanelProp {

    PROP_0,
    PROP_PARENT,
    PROP_LINK,
    LAST_PROP,

} CCMPanelProp;





static void spice_dialog_entry_name_cb(GtkWidget * widget, gpointer data/*entry_url*/)
{
    GtkWidget *entry_url = GTK_WIDGET(data);
    const gchar *lname = gtk_entry_get_text(GTK_ENTRY(widget));
    gchar str[200];
    g_sprintf(str, "https://%s.zjcloud.net:8443", lname);
    gtk_entry_set_text(GTK_ENTRY(entry_url), str);
    gtk_widget_grab_focus(entry_url);
    gtk_editable_select_region(GTK_EDITABLE(entry_url),0,0);
    gtk_editable_set_position(GTK_EDITABLE(entry_url),-1);

    g_message("entry_name cb");

}

static void spice_dialog_entry_url_cb(GtkWidget * widget, gpointer data/*but_sav*/)
{
    GtkWidget *but_sav = GTK_WIDGET(data);
    gtk_widget_grab_focus(but_sav);

    g_message("entry_url cb");
}

static void spice_dialog_but_sav_cb(GtkWidget * widget, gpointer data/*builder*/)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkWidget *spicewin =
	GTK_WIDGET(gtk_builder_get_object(builder, "winspice"));

//    CCLinkInfo *link = cc_link_info_new();

    GtkWidget *entry_name =
	GTK_WIDGET(gtk_builder_get_object(builder, "entry_lname"));
    GtkWidget *entry_url =
	GTK_WIDGET(gtk_builder_get_object(builder, "entry_url"));

    glink->name = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_name)));
    glink->url = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_url)));
    cc_link_info_set(glink,PROP_LINK_NAME,gtk_entry_get_text(GTK_ENTRY(entry_name)),
            PROP_LINK_URL,gtk_entry_get_text(GTK_ENTRY(entry_url)),NULL);
    cc_link_info_to_file(glink, INI_LINK_CONFIG);
//    cc_link_info_destroy(link);

    g_message("but_sav cb");
    gtk_widget_destroy(spicewin);

}

static void spice_dialog_but_leave_cb(GtkWidget * widget, gpointer data)
{
    gtk_widget_destroy(GTK_WIDGET(data));

    g_message("but)leave cb");
}



GtkWidget *cc_mpanel_spice_dialog_new(GtkWidget*parent)
{

    GtkBuilder *builder = gtk_builder_new_from_file(UI_MPANEL);
    if (builder == NULL) {
	g_error("Loading UI [%s]", UI_MPANEL);
    }
    GtkWidget *dialog =
	GTK_WIDGET(gtk_builder_get_object(builder, "winspice"));
    GtkWidget *entry_lname =
	GTK_WIDGET(gtk_builder_get_object(builder, "entry_lname"));
    GtkWidget *entry_url =
	GTK_WIDGET(gtk_builder_get_object(builder, "entry_url"));
    GtkWidget *but_sav =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_sav"));
    GtkWidget *but_leave =
	GTK_WIDGET(gtk_builder_get_object(builder, "but_leave"));



    //set modal
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);

    g_signal_connect(G_OBJECT(dialog), "delete_event",
		     G_CALLBACK(gtk_false), NULL);

    g_signal_connect(G_OBJECT(entry_lname), "activate",
		     G_CALLBACK(spice_dialog_entry_name_cb), entry_url);
    g_signal_connect(G_OBJECT(entry_url), "activate",
		     G_CALLBACK(spice_dialog_entry_url_cb), but_sav);

    g_signal_connect(G_OBJECT(but_sav), "clicked",
		     G_CALLBACK(spice_dialog_but_sav_cb), builder);
    g_signal_connect(G_OBJECT(but_leave), "clicked",
		     G_CALLBACK(spice_dialog_but_leave_cb), dialog);


gtk_entry_set_text(GTK_ENTRY(entry_lname),glink->name);
gtk_entry_set_text(GTK_ENTRY(entry_url),glink->url);
    gtk_editable_select_region(GTK_EDITABLE(entry_lname),0,0);
    gtk_editable_set_position(GTK_EDITABLE(entry_lname),-1);


//    gtk_widget_show(dialog);
    return dialog;
}




static void get_property(GObject * obj, guint prop_id, GValue * value,
			 GParamSpec * nspec)
{
    CCMPanel *self = CC_MPANEL(obj);

    switch (prop_id) {
    case PROP_PARENT:
	g_value_set_pointer(value, self->topwin);
	break;

    case PROP_LINK:
	g_value_set_pointer(value, self->link);
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, nspec);
	break;
    }

}


static void set_property(GObject * obj, guint prop_id,
			 const GValue * value, GParamSpec * nspec)
{

    CCMPanel *self = CC_MPANEL(obj);
    switch (prop_id) {
    case PROP_PARENT:
	self->topwin = g_value_get_pointer(value);
	GtkWidget *win =
	    GTK_WIDGET(gtk_builder_get_object(self->builder, "window"));
	gtk_window_set_transient_for(GTK_WINDOW(win),
				     GTK_WINDOW(self->topwin));
	gtk_window_set_modal(GTK_WINDOW(win), TRUE);
	break;
    case PROP_LINK:
	self->link = g_value_get_pointer(value);
	break;

    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, nspec);
	break;

    }

}




static void mpanel_but_spice_cb(GtkWidget * widget, gpointer data)
{
    g_message("Open SPICE configuration");
    GtkWidget *spice_dialog = cc_mpanel_spice_dialog_new(data);
    gtk_widget_show(spice_dialog);

}

static void mpanel_but_nm_cb(GtkWidget * widget, gpointer data)
{
    g_message("Open Network-Connection-Editor..");
    GError *err=NULL;
    gboolean rt=g_spawn_command_line_sync("nm-connection-editor",NULL,NULL,NULL,&err);
    if(!rt){
        g_warning("nm-connection-edit terminated with Error!!");
    }
}
static void mpanel_but_nt_cb(GtkWidget * widget, gpointer data)
{
    g_message("Open Gnome-nettool..");
    GError *err=NULL;
    gboolean rt=g_spawn_command_line_sync("gnome-nettool",NULL,NULL,NULL,&err);
    if(!rt){
        g_warning("gnome-nettool terminated with Error!!");
    }
}

static void mpanel_but_tm_cb(GtkWidget * widget, gpointer data)
{
    g_message("Open Terminal [XTerm]");
    GError *err=NULL;
    gboolean rt=g_spawn_command_line_async("xterm",&err);
    if(!rt){
        g_warning("gnome-nettool terminated with Error!!");
    }
}




void cc_mpanel_setup(CCMPanel * self)
{
    g_return_if_fail(NULL != self->builder);

    GtkWidget *win =
	GTK_WIDGET(gtk_builder_get_object(self->builder, "window"));
//    self->topwin = win;

    GtkWidget *but_close =
	GTK_WIDGET(gtk_builder_get_object(self->builder, "but_close"));
    GtkWidget *but_spice =
	GTK_WIDGET(gtk_builder_get_object(self->builder, "but_spice"));
    GtkWidget *but_nm =
	GTK_WIDGET(gtk_builder_get_object(self->builder, "but_nm"));
    GtkWidget *but_nt =
	GTK_WIDGET(gtk_builder_get_object(self->builder, "but_nt"));
    GtkWidget *but_tm =
	GTK_WIDGET(gtk_builder_get_object(self->builder, "but_tm"));


    g_signal_connect(G_OBJECT(but_spice), "clicked",
		     G_CALLBACK(mpanel_but_spice_cb), win);

    g_signal_connect(G_OBJECT(but_nm), "clicked",
		     G_CALLBACK(mpanel_but_nm_cb), win);
    g_signal_connect(G_OBJECT(but_nt), "clicked",
		     G_CALLBACK(mpanel_but_nt_cb), win);

    g_signal_connect(G_OBJECT(but_tm), "clicked",
		     G_CALLBACK(mpanel_but_tm_cb), win);
    g_signal_connect_swapped(G_OBJECT(but_close), "clicked",
		     G_CALLBACK(gtk_widget_destroy), win);

    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
    gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_title(GTK_WINDOW(win),"控制中心");
    gtk_widget_show_all(win);

}


void cc_mpanel_init(CCMPanel * self)
{
    self->builder = gtk_builder_new_from_file(UI_MPANEL);
}

void cc_mpanel_class_init(CCMPanelClass * klass)
{
    GObjectClass *base_class = G_OBJECT_CLASS(klass);
    base_class->set_property = set_property;
    base_class->get_property = get_property;

    GParamSpec *pspec = g_param_spec_pointer("topwin", "toplevel-window",
					     "TopLevel window of the panel",
					     G_PARAM_CONSTRUCT |
					     G_PARAM_READWRITE);
    g_object_class_install_property(base_class, PROP_PARENT, pspec);
    pspec = g_param_spec_pointer("link", "cclinkinfo",
				 "LinkInfo from cc-loging-win",
				 G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
    g_object_class_install_property(base_class, PROP_LINK, pspec);





}

CCMPanel *cc_mpanel_new(GtkWindow * parent)
{
    return g_object_new(CC_TYPE_MPANEL, "topwin", (gpointer) parent, NULL);

}

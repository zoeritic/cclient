#include<gtk/gtk.h>
#include"config.h"
//#define STYLE_PATH "./style/"

void cc_style_setup()
{

    GtkCssProvider* provider=gtk_css_provider_new();

    GdkScreen*screen=gdk_display_get_default_screen(gdk_display_get_default());

    GFile* cssfile=g_file_new_for_path(STYLE_PATH "image.css");
    gtk_css_provider_load_from_file(GTK_CSS_PROVIDER(provider),cssfile,NULL);

    gtk_style_context_add_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);

}



void cc_class_add(GtkWidget*w,gchar*classname)
{
 GtkStyleContext*context = gtk_widget_get_style_context (w);
 if(!gtk_style_context_has_class(context,classname)){
   gtk_style_context_add_class (context, classname);
 }
   g_object_unref(context);
}

void cc_class_set(GtkWidget*w,gchar*classname)
{
    GtkStyleContext*context = gtk_widget_get_style_context (w);
    GList* classes=gtk_style_context_list_classes(context);
    for(;classes!=NULL;classes->next){
        gtk_style_context_remove_class(context,classes->data);
    }
    g_list_free(classes);
//    if(!gtk_style_context_has_class(context,classname)){
    gtk_style_context_add_class (context, classname);
//    }
   g_object_unref(context);
}






void cc_set_css(GtkWidget*w,gchar*file)
{
    GFile *cssfile=g_file_new_for_path(file);
GtkCssProvider*provider=gtk_css_provider_new();
GtkStyleContext*context=gtk_widget_get_style_context(w);
gtk_style_context_add_provider(context,GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
gtk_css_provider_load_from_file(GTK_CSS_PROVIDER(provider),cssfile,NULL);
g_object_unref(provider);

}


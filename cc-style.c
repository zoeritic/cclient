#include<gtk/gtk.h>

#define CSS_PATH "./image.css"

void cc_style_setup()
{

    GtkCssProvider* provider=gtk_css_provider_new();

    GdkScreen*screen=gdk_display_get_default_screen(gdk_display_get_default());

    GFile* cssfile=g_file_new_for_path(CSS_PATH);
    gtk_css_provider_load_from_file(GTK_CSS_PROVIDER(provider),cssfile,NULL);

    gtk_style_context_add_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);

}


#ifndef __CC_STYLE_
#define __CC_STYLE_

#include<gtk/gtk.h>

//#define CSS_PATH "./image.css"

void cc_style_setup();
void cc_class_set(GtkWidget*w,gchar*classname);
void cc_class_add(GtkWidget*w,gchar*classname);
void cc_set_css(GtkWidget*w,gchar*file);

#endif

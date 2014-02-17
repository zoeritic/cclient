#include"cc-ovirt-vm.h"
#include"cc-style.h"
#include"cc-loging-win.h"
#include"cc-loging-ovirt.h"
#include"config.h"

//#define OVIRT_VMOPER "./rest-ovirt/vm-helper.py"

//#define TIMEOUT_COUNT_LIMIT 10
//static guint timeout_ct = 0;

//#define STATING_TIMEOUT_MAX 15


static gboolean domain_get_ok = TRUE;

#ifdef GET_DOMAIN_ONCE
gboolean domain_getten = FALSE;	//only get domain once when run program.
#endif

//static guint vm_stat;

static gboolean vm_stat_ok = TRUE;

static guint vm_stating_cnt = 0;
static gboolean vm_stating = FALSE;
gchar *vm_stat_expect;

static gboolean vm_is_executing = FALSE;	//if TRUE set,mean cant do anything through button from winer.


//static gboolean vm_oper_ok = TRUE;
static gboolean vm_start_ok = TRUE;
static gboolean vm_stop_ok = TRUE;
static gboolean vm_kill_ok = TRUE;
static gboolean vm_view_ok = TRUE;

static gboolean vm_consoling = FALSE;


//get_domain_r
static guint pulse = 0;
static char *pulse_c[5] = { "•", "••", "•••", "••••",
    "•••••"
};

// stat_vm_r
static guint sindex = 0;
static guint pulseinterval[] = { 5, 15, 35, 10 };


static void set_statusbar(CCOvirtVM *ovm,gchar*text)
{

    GtkWidget* statusbar=ovm->win->winers->statusbar;
    guint cid=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),"vm-stating");
    gtk_statusbar_push(GTK_STATUSBAR(statusbar),cid,text);

}

static void set_infolabel(CCOvirtVM *ovm,gchar*text)
{
    GtkWidget* infolabel=ovm->win->winers->infolabel;
    gtk_label_set_text(GTK_LABEL(infolabel),text);
    gtk_label_set_justify(GTK_LABEL(infolabel),GTK_JUSTIFY_CENTER);

}

guint cc_ovirt_stat_pulse()
{
//    g_message("Size of Seqinterval:=%u <--------------",
//            sizeof(pulseinterval));

    guint n_vals = sizeof(pulseinterval) / sizeof(pulseinterval[0]);
    guint val = pulseinterval[n_vals - 1];

    if (sindex >= n_vals)
	return val;
    else
	return pulseinterval[sindex++];
}


CCOvirtVM *cc_ovirt_vm_new(void)
{
    CCOvirtVM *info = g_new0(CCOvirtVM, 1);
    g_print("CCOvirtVM New!");
    return info;
}

CCOvirtVM *cc_ovirt_vm_dup(CCOvirtVM * ovm)
{
    CCOvirtVM *nvm = g_new0(CCOvirtVM, 1);
    nvm->win = ovm->win;
    nvm->argv = g_strdupv(ovm->argv);
    g_print("CCOvirtVM Dup!");
    return nvm;
}



void cc_ovirt_vm_purge(CCOvirtVM * ovm)
{
    g_return_if_fail(ovm != NULL);

    if (ovm->argv) {
	g_strfreev(ovm->argv);
	ovm->argv = NULL;
    }
    //
    g_free(ovm);
    g_message("CCOvirtVM destroyed!!");

}


/* Async Operations..
 *
 *
 *
 * */


static gboolean
cb_get_domain_out_watch(GIOChannel * channel, GIOCondition cond,
			CCOvirtVM * ovm)
{
//    gboolean need_leave = FALSE;
    static int out_cnt;
    out_cnt++;
    g_critical("Callback get domain OUT watch");

    g_print("OUT Watch Count::%d\n", out_cnt);
    if (cond & G_IO_HUP) {
	g_print("---->>:G_IO_HUP::out\n");
	g_io_channel_unref(channel);
	return FALSE;
//    g_io_channel_shutdown(channel,TRUE,NULL);
//      need_leave = TRUE;
    }

    gchar *string;
    gsize size;

    g_print("out_watch\n");
    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
//    g_io_channel_read_to_end(channel, &string, &size, NULL);
//g_print("((((((((((((((((((((((((\n");
    if (NULL == ovm->strout) {
	ovm->strout = g_string_new("");
    }
    ovm->strout = g_string_append(ovm->strout, string);
//    data->strout = g_strdup(g_strstrip(string));
    g_print("soutt::]%s[\n", ovm->strout->str);

    if (string)
	g_free(string);
//    g_print("FREE string\n");
//    if (need_leave) {
//      g_io_channel_unref(channel);
//      return (FALSE);
//   }
    return (TRUE);
}

static gboolean
cb_get_domain_err_watch(GIOChannel * channel, GIOCondition cond,
			CCOvirtVM * ovm)
{
//    gboolean need_leave = FALSE;
    static int out_cnt;
    out_cnt++;

    g_critical("Callback get domain ERR watch");

    g_print("ERR Watch Count::%d\n", out_cnt);
    if (cond & G_IO_HUP) {
	g_print("---->>:G_IO_HUP::err\n");
	g_io_channel_unref(channel);
    g_warning("g_io unrefed..");
	return FALSE;
//    g_io_channel_shutdown(channel,TRUE,NULL);
//      need_leave = TRUE;
    }
//Error was occured;

    gchar *string;
    gsize size;

//    domain_get_ok = FALSE;

    g_print("err_watch\n");
    GIOStatus s=g_io_channel_read_to_end(channel, &string, &size, NULL);
    if(s==G_IO_STATUS_EOF){
        g_warning("G_IO_STATUX_EOF");
    }else if(s==G_IO_STATUS_ERROR){
        g_warning("G_IO_STATUX_ERROR");
    }else if(s==G_IO_STATUS_AGAIN){
        g_warning("G_IO_STATUX_AGAIN");
    }else{
        g_warning("G_IO_STATUX_NNNN");
    
    }
//    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
    if (NULL == ovm->strerr) {
	ovm->strerr = g_string_new("");
    }
//g_print(":::::::[]%s[]\n",ovm->strerr->str);
    ovm->strerr = g_string_append(ovm->strerr, string);
//g_print("=++++++++++++++\n");
    g_print("serrr::]%s[\n", ovm->strerr->str);

//    if (string)
	g_free(string);
//    if (need_leave) {
//      g_io_channel_unref(channel);
//      return (FALSE);
//   }

    return (TRUE);
}

static void cb_get_domain_child_watch(GPid pid, gint status,
				      CCOvirtVM * ovm)
/* if process exited & get nothing from stderr,then the process executed successfully!
 *
 * */
{
    /*clean strout & strerr in CCOvirtInfo */
    gchar *sout = NULL, *serr = NULL;
/*    if (NULL != ovm->strout) {
	sout = g_strstrip(g_string_free(ovm->strout, FALSE));
	ovm->strout = NULL;
	g_message("STROUT::\n%s", sout);
//        g_free(sout);
    }

    */

    if (NULL != ovm->strerr) {
	serr = g_string_free(ovm->strerr, FALSE);
	ovm->strerr = NULL;
	g_message("STRERR::\n%s", serr);
//	ovm->strerr = NULL;
//        g_free(serr);
    }else{
    /*nothing has been read from stderr*/
    serr=g_strdup("DOMAIN-UNKNOW:need retry");
    /*elimate SIGSEGV fault when serr==NULL*/
    }

    if (NULL == strstr(serr, "DOMAIN-OK")) {
	domain_get_ok = FALSE;
    } else {
	domain_get_ok = TRUE;

	sout = cc_get_state(serr);

    }

    g_print("child_watch\n");

    if (domain_get_ok) {
	g_message("Get Domain Successfully.");
	g_source_remove(ovm->timeout_id);

    //gdk_threads_enter();
	GtkWidget *combo_domain = ovm->win->wins->combo_domain;	// data->combo_domain;
	GtkWidget *but_login = ovm->win->wins->login;	//data->widget;
	gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(combo_domain), 0);
	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo_domain), 0,
				       sout);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_domain), 0);
	gtk_widget_set_sensitive(combo_domain, TRUE);
	gtk_widget_set_sensitive(but_login, TRUE);
    //gdk_threads_leave();

#ifdef GET_DOMAIN_ONCE
	domain_getten = TRUE;
#endif

	cc_ovirt_vm_purge(ovm);


    } else {
	g_warning("Get Domain Failed! Retry latter..");

	spawn_async_get_domain_with_delay(ovm, GETDOMAIN_INTERVAL);
//      spawn_async_get_domain(ovm);

    }
    g_message("[[[[[[");
    if (sout != NULL)
	g_free(sout);
    if (serr != NULL)
	g_free(serr);
    g_message("]]]]]");


/* Close pid */
//    g_message("About to close PID");
    g_spawn_close_pid(pid);
    g_message("PID closed");

}




static gboolean timeout_getdomain_cb(CCOvirtVM * ovm)
{
//    timeout_ct += 1;          //static
//    if(timeout_ct>TIMEOUT_COUNT_LIMIT){
//        
//   }

    g_message("Timeout get domain ...");
    /* Bounce progress bar */
    GtkWidget *combo_domain = ovm->win->wins->combo_domain;	//data->combo_domain;

//  gdk_threads_enter();
//    g_message("in Timeout get domain[GDK enter] ...");
    gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(combo_domain), 0);
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo_domain), 0,
				   pulse_c[pulse]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_domain), 0);
    pulse = (pulse + 1) % 5;
//  gdk_threads_leave();
//    g_message("in Timeout get domain[GDK leave] ...");

    return (TRUE);
}


static gboolean spawn_async_get_domain(CCOvirtVM * ovm)
{
    domain_get_ok = TRUE;

    cc_ovirt_info_update(ovm->win->ovirt,ovm->win->info,glink);

    g_message("Spawn get domain....");
    GPid pid;
    gchar *argv[] =
	{ OVIRT_GETDOMAIN, ovm->win->ovirt->maddr,
	NULL
    };


    g_print("Argv-->\n");
    int i;
    for (i = 0; argv[i] != NULL; i++)
	g_print(" :%s\t", argv[i]);
    g_print("\n");
    //  gchar**argv=data->argv;

    gint out, err;
    GIOChannel *out_ch, *err_ch;
    gboolean ret;

    /* Spawn child process */
    ret = g_spawn_async_with_pipes(RESTOVIRT_DIR, argv, NULL,
				   G_SPAWN_DO_NOT_REAP_CHILD, NULL,
				   NULL, &pid, NULL, &out, &err, NULL);
    if (!ret) {
	g_error("SPAWN FAILED");
	return FALSE;
    }
    /* Add watch function to catch termination of the process. This function
     *      * will clean any remnants of process. */
    g_child_watch_add(pid, (GChildWatchFunc) cb_get_domain_child_watch,
		      ovm);

    /* Create channels that will be used to read data from pipes. */
//    out_ch = g_io_channel_unix_new(out);
    err_ch = g_io_channel_unix_new(err);
    /*configure channels */
/*
    g_io_channel_set_encoding (out_ch, NULL, NULL);
    g_io_channel_set_encoding (err_ch, NULL, NULL);
    g_io_channel_set_buffer_size(out_ch,0);
    g_io_channel_set_buffer_size(err_ch,0);
    g_io_channel_set_flags(out_ch,G_IO_FLAG_NONBLOCK,NULL);
    g_io_channel_set_flags(err_ch,G_IO_FLAG_NONBLOCK,NULL);
    */

    /* Add watches to channels */
//    g_io_add_watch(out_ch, G_IO_IN | G_IO_HUP,
//                 (GIOFunc) cb_get_domain_out_watch, ovm);
    g_io_add_watch(err_ch, G_IO_IN | G_IO_HUP,
		   (GIOFunc) cb_get_domain_err_watch, ovm);

    return FALSE;
}



static void spawn_async_get_domain_with_delay(CCOvirtVM * ovm, guint delay)
{
    GSource *sr = g_timeout_source_new_seconds(delay);

    GMainContext *context = g_main_context_default();

    g_source_set_callback(sr, (GSourceFunc) spawn_async_get_domain, ovm,
			  NULL);

    g_source_attach(sr, context);
    g_source_unref(sr);


}





void cc_loging_ovirt_get_domain_r(CCOvirtVM * ovm)
{
#ifdef GET_DOMAIN_ONCE
    if (domain_getten)
	return;
#endif

    //without delay at first time to get domain.
    spawn_async_get_domain(ovm);


    ovm->timeout_id =
	g_timeout_add_seconds(TIMEOUT_INTERVAL,
			      (GSourceFunc) timeout_getdomain_cb, ovm);

}

/*==============Stating VM=============================================================*/



static gboolean
cb_stat_vm_out_watch(GIOChannel * channel, GIOCondition cond,
		     CCOvirtVM * ovm)
{
//    CCLogingWin *win = ovm->win;
    //   CCOvirtInfo *info = win->ovirt;

    gchar *string;
    gsize size;

    if (cond & G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
    g_print("stat:vm_out_watch\n");
    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
//    g_io_channel_read_to_end(channel, &string, &size, NULL);

    if (NULL == ovm->strout) {
	ovm->strout = g_string_new("");
    }
    ovm->strout = g_string_append(ovm->strout, string);

    g_free(string);

    return (TRUE);
}



static gboolean
cb_stat_vm_err_watch(GIOChannel * channel, GIOCondition cond,
		     CCOvirtVM * ovm)
{
//    CCLogingWin *win = ovm->win;
//    CCOvirtInfo *info = win->ovirt;
//
//TODO if error occured,what should to do ?
//STATING_TIMEOUT_MAX limit the behavier of !err!

    gchar *string;
    gsize size;

    if (cond & G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }

    g_print("stat:vm_err_watch\n");
    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
//    g_io_channel_read_to_end(channel, &string, &size, NULL);

    if (NULL == ovm->strerr) {
	ovm->strerr = g_string_new("");
    }
    ovm->strerr = g_string_append(ovm->strerr, string);

//    stat_ok = FALSE;

    g_free(string);

    return (TRUE);
}



static void cb_stat_vm_child_watch(GPid pid, gint status, CCOvirtVM * ovm)
/* if process exited & get nothing from stderr,then the process executed successfully!
 *
 * */
{
    /*clean strout & strerr in CCOvirtInfo */
    gchar *sout = NULL, *serr = NULL;
/*    if (NULL != ovm->strout) {
	sout = g_strstrip(g_string_free(ovm->strout, FALSE));
	ovm->strout = NULL;
	g_message("STROUT::\n%s", sout);
//        g_free(sout);
    }

    */
    if (NULL != ovm->strerr) {
	serr = g_strstrip(g_string_free(ovm->strerr, FALSE));
	ovm->strerr = NULL;
	g_message("STRERR::\n%s", serr);
	ovm->strerr = NULL;
//        g_free(serr);
	sout = cc_get_state(serr);

    }else{
    
    serr=strdup("STAT-UNKNOWN:need re try");
    
    }

    g_print("vm_stat_child_watch\n");

    g_print("SOUT   :[%s] \nEXPECT :[%s]\n", sout, vm_stat_expect);

    if (!g_strcmp0(sout, vm_stat_expect)
	|| vm_stating_cnt >= STATING_TIMEOUT_MAX) {
	gchar *msg = g_strdup_printf("Catch expected status of VM:[%s]",
				     vm_stat_expect);
	g_message("%s", msg);
	g_free(msg);

	g_source_remove(ovm->timeout_id);
    set_infolabel(ovm,"");
    set_statusbar(ovm,"操作成功");

	GtkWidget *viewer = ovm->win->winers->but_view;	// data->combo_domain;

	if (!g_strcmp0(sout, vm_stat_expect)) {
	    if (!strcmp(vm_stat_expect, "up")) {
		gtk_widget_set_sensitive((viewer), TRUE);
		gtk_widget_set_sensitive((ovm->win->winers->but_start), FALSE);
		gtk_widget_set_sensitive((ovm->win->winers->but_shutdown), TRUE);
		gtk_widget_set_sensitive((ovm->win->winers->but_kill), TRUE);
        
        cc_set_css(viewer,STYLE_UP);
        gtk_widget_set_tooltip_text(viewer,"双击进入Windows");

	    } else {		// if(!strcmp(vm_stat_expect,"down")){
		gtk_widget_set_sensitive((viewer), FALSE);
		gtk_widget_set_sensitive((ovm->win->winers->but_start), TRUE);
		gtk_widget_set_sensitive((ovm->win->winers->but_shutdown), FALSE);
		gtk_widget_set_sensitive((ovm->win->winers->but_kill), FALSE);

        cc_set_css(viewer,STYLE_DOWN);
        gtk_widget_set_tooltip_text(viewer,"请先启动Windows");
	    }
	} else if (vm_stating_cnt >= STATING_TIMEOUT_MAX) {
	    g_critical("VM Stating counter Ran out");
//	    gtk_widget_set_sensitive((viewer), FALSE);
	}

	cc_ovirt_vm_purge(ovm);
	g_free(vm_stat_expect);
//    g_print("FREEEEEEEEEEEEEEEE\n");
	vm_stating = FALSE;
	vm_stating_cnt = 0;

    } else {

	g_warning("VM State doesn't meet Expected !Will retry latter..");

	vm_stating_cnt++;
	spawn_async_stat_vm_with_delay(ovm, STATING_INTERVAL);


    }

    if (sout != NULL)
	g_free(sout);
    if (serr != NULL)
	g_free(serr);


/* Close pid */
    g_print("CLOSE PID..\n");
    g_spawn_close_pid(pid);


}




static gboolean timeout_stat_vm_cb(CCOvirtVM * ovm)
{

    g_message("Timeout Stat VM ...");
    /* progress bar */
//    static char *pulse_c[5] =
//      { "•", "••", "•••", "••••",
//"•••••" };

//    GtkWidget *viewer = ovm->win->winers->but_view;	//data->combo_domain;
//    GtkWidget* infolabel=ovm->win->winers->infolabel;
//    guint cid=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),"vm-stating");
//    gtk_statusbar_push(statusbar,cid,pulse_c[pulse]);
    set_infolabel(ovm,pulse_c[pulse]);
//    gtk_label_set_text(GTK_LABEL(infolabel),pulse_c[pulse]);
//    gtk_label_set_justify(GTK_LABEL(infolabel),GTK_JUSTIFY_CENTER);
//    gtk_button_set_label(GTK_BUTTON(viewer), pulse_c[pulse]);

//    gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(combo_domain), 0);
//    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo_domain), 0,
//                                 pulse_c[pulse]);
//    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_domain), 0);
    pulse = (pulse + 1) % 5;

    return (TRUE);
}


static gboolean spawn_async_stat_vm(CCOvirtVM * ovm)
{


    g_message("Spawn stat VM....");
    GPid pid;
/*    gchar *argv[] =
	{ OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u", "syqt503@zjcloud.net",//"admin@internal", 
        "-p", "12345678",//"zj@Cloud)(12",
        "-s", ovm->win->ovirt->vmname,
NULL };
*/
    gchar **argv = ovm->argv;

    g_print("Argv-->\n");
    int i;
    for (i = 0; argv[i] != NULL; i++)
	g_print(" :%s\t", argv[i]);
    g_print("\n");
    //  gchar**argv=data->argv;

    gint out, err;
    GIOChannel *out_ch, *err_ch;
    gboolean ret;

    /* Spawn child process */
    ret = g_spawn_async_with_pipes(RESTOVIRT_DIR, argv, NULL,
				   G_SPAWN_DO_NOT_REAP_CHILD, NULL,
				   NULL, &pid, NULL, &out, &err, NULL);
    if (!ret) {
	g_error("SPAWN FAILED");
	return FALSE;
    }
    /* Add watch function to catch termination of the process. This function
     *      * will clean any remnants of process. */

    g_child_watch_add(pid, (GChildWatchFunc) cb_stat_vm_child_watch, ovm);

    /* Create channels that will be used to read data from pipes. */
//    out_ch = g_io_channel_unix_new(out);
    err_ch = g_io_channel_unix_new(err);
/*
    g_io_channel_set_encoding (out_ch, NULL, NULL);
    g_io_channel_set_encoding (err_ch, NULL, NULL);
    g_io_channel_set_buffer_size(out_ch,0);
    g_io_channel_set_buffer_size(err_ch,0);
    g_io_channel_set_flags(out_ch,G_IO_FLAG_NONBLOCK,NULL);
    g_io_channel_set_flags(err_ch,G_IO_FLAG_NONBLOCK,NULL);
*/
    /* Add watches to channels */
//    g_io_add_watch(out_ch, G_IO_IN | G_IO_HUP,
//                 (GIOFunc) cb_stat_vm_out_watch, ovm);
    g_io_add_watch(err_ch, G_IO_IN | G_IO_HUP,
		   (GIOFunc) cb_stat_vm_err_watch, ovm);

    return FALSE;
}



static void spawn_async_stat_vm_with_delay(CCOvirtVM * ovm, guint delay)
{
    GSource *sr = g_timeout_source_new_seconds(delay);

    GMainContext *context = g_main_context_default();

    g_source_set_callback(sr, (GSourceFunc) spawn_async_stat_vm, ovm,
			  NULL);

    g_source_attach(sr, context);
    g_source_unref(sr);

}



void cc_ovirt_stating_vm_r(CCOvirtVM * ovm)
{

//    if (g_strcmp(vm_stat,"up"))
//      return;
    if (vm_stating)
	return;

    vm_stating = TRUE;

    pulse = 0;

    gchar *argv[] = { OVIRT_STAT, ovm->win->ovirt->maddr,
	ovm->win->ovirt->vmname, NULL
    };

    if (ovm->argv)
	g_strfreev(ovm->argv);

//    g_print("-------------------------------------------------\n");

    ovm->argv = g_strdupv(argv);
    spawn_async_stat_vm(ovm);

    ovm->timeout_id =
	g_timeout_add_seconds(1, (GSourceFunc) timeout_stat_vm_cb, ovm);

}



/*============VM-stat-R OVER===============================================================*/



static gboolean
vm_out_watch(GIOChannel * channel, GIOCondition cond, CCOvirtVM * ovm)
{
    gchar *string;
    gsize size;

    if (cond & G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
    g_print("::vm_out_watch\n");
    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
//    g_io_channel_read_to_end(channel, &string, &size, NULL);

    if (NULL == ovm->strout) {
	ovm->strout = g_string_new("");
    }
    ovm->strout = g_string_append(ovm->strout, string);


    g_free(string);

    return (TRUE);
}

static gboolean
vm_err_watch(GIOChannel * channel, GIOCondition cond, CCOvirtVM * ovm)
{
    gchar *string;
    gsize size;

    if (cond & G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
//    *ovm->op = FALSE;

    g_print("::vm_err_watch\n");
//    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
    g_io_channel_read_to_end(channel, &string, &size, NULL);

    if (NULL == ovm->strerr) {
	ovm->strerr = g_string_new("");
    }
    ovm->strerr = g_string_append(ovm->strerr, string);


    g_free(string);


    return (TRUE);
}





static void vm_child_watch(GPid pid, gint status, CCOvirtVM * ovm)
{
    g_print(":::vm_child_watch\n");

    /*clean strout & strerr in CCOvirtInfo */
    gchar *sout = NULL, *serr = NULL;
/*    if (NULL != ovm->strout) {
	sout = g_strstrip(g_string_free(ovm->strout, FALSE));
	ovm->strout = NULL;
	g_message("STROUT::\n%s", sout);
//        g_free(out);
    }
    */

    if (NULL != ovm->strerr) {
	serr = g_strstrip(g_string_free(ovm->strerr, FALSE));
	ovm->strerr = NULL;
	g_message("STRERR::\n%s", serr);
//        g_free(err);
    }else{
    
        serr=g_strdup("OPER-UNKNOWN:need re try");
    
    }
    if (NULL == strstr(serr, "OPER-OK")) {
	*ovm->op = FALSE;
    } else {
	*ovm->op = TRUE;
	sout = cc_get_state(serr);

    }
    g_critical(">>>>>>>%s<<<<<<[%d]<",sout,*ovm->op);
    
    if (*ovm->op) {
	g_message("VM's Operation Success.-->[%s]", sout);
	//gtk_widget_set_sensitive (, TRUE);
	//
	//
	//TODO

	/*
	 * vm_stat
	 * */
	if (ovm->op == &vm_stat_ok) {
	    g_print("in child watch {vm_stat_ok}..\n");
	    GtkWidget *viewer = ovm->win->winers->but_view;	// data->combo_domain;
	    if (!strcmp(sout, "up")) {
        cc_set_css(viewer,STYLE_UP );
		gtk_widget_set_sensitive(viewer, TRUE);
		gtk_widget_set_sensitive((ovm->win->winers->but_start), FALSE);
		gtk_widget_set_sensitive((ovm->win->winers->but_shutdown), TRUE);
		gtk_widget_set_sensitive((ovm->win->winers->but_kill), TRUE);

        gtk_widget_set_tooltip_text(viewer,"双击进入Windows");
//        cc_class_set(viewer,"up");
//        gtk_widget_set_name(viewer,"")
	    } else {
        cc_set_css(viewer,STYLE_DOWN);
		gtk_widget_set_sensitive(viewer, FALSE);
		gtk_widget_set_sensitive((ovm->win->winers->but_start), TRUE);
		gtk_widget_set_sensitive((ovm->win->winers->but_shutdown), FALSE);
		gtk_widget_set_sensitive((ovm->win->winers->but_kill), FALSE);

        gtk_widget_set_tooltip_text(viewer,"请先启动Windows");
//        cc_class_set(viewer,"down");
	    }
        set_statusbar(ovm,"");
//    g_print("==================\n");
	} else
	    /*
	     *vm_start
	     * */
	if (ovm->op == &vm_start_ok) {

	    CCOvirtVM *svm = cc_ovirt_vm_dup(ovm);
//          svm->win = ovm->win;
	    cc_ovirt_stating_vm_r(svm);
        set_statusbar(ovm,"启动中，请稍后");

	} else
	    /*
	     *vm_stop
	     * */
	if (ovm->op == &vm_stop_ok) {

	    CCOvirtVM *svm = cc_ovirt_vm_dup(ovm);
//          svm->win = ovm->win;
	    cc_ovirt_stating_vm_r(svm);
        set_statusbar(ovm,"等待关闭完成");

	} else
	    /*
	     *vm_kill
	     * */
	if (ovm->op == &vm_kill_ok) {

	    CCOvirtVM *svm = cc_ovirt_vm_dup(ovm);
//          svm->win = ovm->win;
//        svm->argv=g_strdupv(ovm->argv);
	    cc_ovirt_stating_vm_r(svm);
        set_statusbar(ovm,"等待断电完成");


	} else if (ovm->op == &vm_view_ok) {
	    g_print("Open Spice Viewer..\n");
//        vm_consoling=TRUE;
//    g_message();

	}
//      g_print("Before Purge\n");
	cc_ovirt_vm_purge(ovm);
//      g_print("After Purge\n");

    } else {			//operation on VM failed!!

    set_statusbar(ovm,"操作失败，或重试");
        g_warning("VM's Operation Failed!!");

	//TODO

    }

//    g_print("[]][][][\n");

    vm_is_executing = FALSE;
//    *ovm->op = TRUE;

    if (NULL != sout)
	g_free(sout);
//    g_print("[]][][][\n");
    if (NULL != serr)
	g_free(serr);

/* Close pid */
    g_spawn_close_pid(pid);
    g_print("PID CLOSED[operation].\n");


}

static void spawn_async_vm(CCOvirtVM * ovm)
{
    vm_is_executing = TRUE;
    *ovm->op = TRUE;
    g_print("\n\n]]Spawn_....\n");
    GPid pid;

    gchar **argv = ovm->argv;

    gint out, err;
    GIOChannel *out_ch, *err_ch;
    gboolean ret;

    /* Install timeout fnction that will move the progress bar */
// data->timeout_id = g_timeout_add( 1000, (GSourceFunc)cb_timeout, data );
    /* Spawn child process */
    ret = g_spawn_async_with_pipes(RESTOVIRT_DIR, argv, NULL,
				   G_SPAWN_DO_NOT_REAP_CHILD, NULL,
				   NULL, &pid, NULL, &out, &err, NULL);
    if (!ret) {
	g_error("SPAWN FAILED");
	vm_is_executing = FALSE;
	return;
    }
    /* Add watch function to catch termination of the process. This function
     *      * will clean any remnants of process. */
    g_child_watch_add(pid, (GChildWatchFunc) vm_child_watch, ovm);

    /* Create channels that will be used to read data from pipes. */
//    out_ch = g_io_channel_unix_new(out);
    err_ch = g_io_channel_unix_new(err);
/*
    g_io_channel_set_encoding (out_ch, NULL, NULL);
    g_io_channel_set_encoding (err_ch, NULL, NULL);
    g_io_channel_set_buffer_size(out_ch,0);
    g_io_channel_set_buffer_size(err_ch,0);
    g_io_channel_set_flags(out_ch,G_IO_FLAG_NONBLOCK,NULL);
    g_io_channel_set_flags(err_ch,G_IO_FLAG_NONBLOCK,NULL);
*/
    /* Add watches to channels */
//    g_io_add_watch(out_ch, G_IO_IN | G_IO_HUP, (GIOFunc) vm_out_watch,
//                 ovm);
    g_io_add_watch(err_ch, G_IO_IN | G_IO_HUP, (GIOFunc) vm_err_watch,
		   ovm);


}







void cc_ovirt_vm_start_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing || vm_stating) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_start_ok = TRUE;

    gchar *argv[] =
	{ OVIRT_OPER, "-l", ovm->win->ovirt->maddr,
	"-U", ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_start_ok;
    set_statusbar(ovm,"正在启动虚拟机");

    spawn_async_vm(ovm);
    g_print("vm_start_r...........\n");

}


void cc_ovirt_vm_shutdown_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing || vm_stating) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_stop_ok = TRUE;

    gchar *argv[] =
	{ OVIRT_OPER, "-l", ovm->win->ovirt->maddr,
	"-D", ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_stop_ok;
    set_statusbar(ovm,"正在关闭虚拟机");

    spawn_async_vm(ovm);
    g_print("vm_stop_r...........\n");


}


void cc_ovirt_vm_kill_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing || vm_stating) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_kill_ok = TRUE;

    gchar *argv[] =
	{ OVIRT_OPER, "-l", ovm->win->ovirt->maddr, "-K",
	ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_kill_ok;
    set_statusbar(ovm,"强制断电");

    spawn_async_vm(ovm);
    g_print("vm_kill_r...........\n");


}


void cc_ovirt_vm_stat_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_stat_ok = TRUE;

    gchar *argv[] = { OVIRT_STAT, ovm->win->ovirt->maddr,
	ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_stat_ok;

    set_statusbar(ovm,"刷新状态");

    spawn_async_vm(ovm);

    g_print("vm_refresh_r...........\n");

}

void cc_ovirt_vm_stat_sync(CCOvirtVM * ovm)
{

    gchar *cmd = g_strdup_printf(OVIRT_STAT" %s %s",
				 ovm->win->ovirt->maddr,
				 ovm->win->ovirt->vmname);
    gchar *out, *err;
//    gint status;
    GError *e = NULL;		//g_error_new();
    g_message("==-=-=-=::%s:: ", cmd);

    if (!g_spawn_command_line_sync(cmd, &out, &err, NULL, &e)) {
	g_critical("SPAWN ERROR :VM STATING sync!!-%s-", e->message);
	g_error_free(e);
	g_free(cmd);
	return;
    } else {

	g_print("OUT::%s:: \tERR::%s::\n", out, err);
//    g_free(out);
//    g_free(err);
	if (NULL != strstr(err, "STAT-OK"))
	    g_print("VM-status::%s", out);
	g_error_free(e);
	g_free(cmd);
	return;
    }
}


/*
void cc_ovirt_vm_viewer(CCOvirtVM* ovm)
{

    gchar* argv[]={"./rest-ovirt/vm-viewer.py",ovm->win->ovirt->maddr,ovm->win->ovirt->vmname,
                    ovm->win->ovirt->user,ovm->win->ovirt->passwd,NULL};

    ovm->argv=g_strdupv(argv);
    ovm->op


}

*/

void cc_ovirt_vm_viewer(CCOvirtVM * ovm)
{
    if (vm_is_executing) {
//    if (vm_consoling) {
	return;
    }

    vm_view_ok = TRUE;

    gchar *argv[] = { OVIRT_VIEWER, ovm->win->ovirt->maddr,
	ovm->win->ovirt->vmname,
	ovm->win->ovirt->user, ovm->win->ovirt->passwd,
	NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_view_ok;

    spawn_async_vm(ovm);


}


void cc_ovirt_vm_viewer_2(CCOvirtVM*ovm)
{
    if (vm_is_executing) {
//    if (vm_consoling) {
	return;
    }

    vm_view_ok = TRUE;

    gchar *argv[] = { OVIRT_VIEWER, ovm->win->ovirt->maddr,
	ovm->win->ovirt->vmname,
	ovm->win->ovirt->passwd,
	NULL
    };

//    ovm->argv = g_strdupv(argv);
//    ovm->op = &vm_view_ok;
//    GPid pid;
    gint st;
    gboolean rv=g_spawn_sync(RESTOVIRT_DIR,argv,NULL,0,NULL,NULL,NULL,NULL,&st,NULL);


}



//TODO get vm status independently-----------------------------------------

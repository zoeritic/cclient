#include"cc-ovirt-vm.h"

#include"cc-loging-win.h"
#include"cc-loging-ovirt.h"

#define OVIRT_VMOPER "./rest-ovirt/vm-helper.py"

//#define TIMEOUT_COUNT_LIMIT 10
//static guint timeout_ct = 0;

#define STATING_TIMEOUT_MAX 15


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
static gboolean vm_console_ok = TRUE;

static gboolean vm_consoling=FALSE;


//get_domain_r
static guint pulse = 0;
static char *pulse_c[5] = { "•", "••", "•••", "••••",
    "•••••"
};

// stat_vm_r
static guint sindex = 0;
static guint pulseinterval[] = { 5, 15, 35, 10 };



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

CCOvirtVM * cc_ovirt_vm_dup(CCOvirtVM*ovm)
{
    CCOvirtVM *nvm = g_new0(CCOvirtVM, 1);
    nvm->win=ovm->win;
    nvm->argv=g_strdupv(ovm->argv);
    g_print("CCOvirtVM Dup!");
    return nvm;
}



void cc_ovirt_vm_purge(CCOvirtVM * ovm)
{
    g_return_if_fail(ovm != NULL);

    if(ovm->argv){
        g_strfreev(ovm->argv);
        ovm->argv=NULL;
    }

    //
    g_free(ovm);
    g_message("CCOvirtVM destroyed!!");

}


/*
gboolean cc_ovirt_authorize(CCOvirtVM *ovm)
{

    g_return_val_if_fail(ovm != NULL,FALSE);

    

}

*/


/* Async Operations..
 *
 *
 *
 * */


static gboolean
cb_out_watch(GIOChannel * channel, GIOCondition cond, CCOvirtVM * ovm)
{

    gchar *string;
    gsize size;

    if (cond == G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
    g_print("out_watch\n");
//    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
    g_io_channel_read_to_end(channel, &string, &size, NULL);

    if (NULL == ovm->strout) {
	ovm->strout = g_string_new("");
    }
    ovm->strout = g_string_append(ovm->strout, string);
//    data->strout = g_strdup(g_strstrip(string));

    g_free(string);
//    g_print("FREE string\n");

    return (TRUE);
}

static gboolean
cb_err_watch(GIOChannel * channel, GIOCondition cond, CCOvirtVM * ovm)
{

//Error was occured;

    gchar *string;
    gsize size;

    if (cond == G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
    domain_get_ok = FALSE;

    g_print("err_watch\n");
    g_io_channel_read_to_end(channel, &string, &size, NULL);
//    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
    if (NULL == ovm->strerr) {
	ovm->strerr = g_string_new("");
    }
    ovm->strerr = g_string_append(ovm->strerr, string);


    g_free(string);

    return (TRUE);
}

static void cb_child_watch(GPid pid, gint status, CCOvirtVM * ovm)
/* if process exited & get nothing from stderr,then the process executed successfully!
 *
 * */
{
    /*clean strout & strerr in CCOvirtInfo */
    gchar *sout = NULL, *serr = NULL;
    if (NULL != ovm->strout) {
	sout = g_strstrip(g_string_free(ovm->strout, FALSE));
	ovm->strout = NULL;
	g_message("STROUT::\n%s", sout);
//        g_free(sout);
    }
    if (NULL != ovm->strerr) {
	serr = g_strstrip(g_string_free(ovm->strerr, FALSE));
	ovm->strerr = NULL;
	g_message("STRERR::\n%s", serr);
	ovm->strerr = NULL;
//        g_free(serr);
    }

    g_print("child_watch\n");

    if (domain_get_ok) {
	g_message("Get Domain Successfully.");
	g_source_remove(ovm->timeout_id);

	GtkWidget *combo_domain = ovm->win->wins->combo_domain;	// data->combo_domain;
	GtkWidget *but_login = ovm->win->wins->login;	//data->widget;
	gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(combo_domain), 0);
	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo_domain), 0,
				       g_strstrip(sout));
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_domain), 0);
	gtk_widget_set_sensitive(combo_domain, TRUE);
	gtk_widget_set_sensitive(but_login, TRUE);

#ifdef GET_DOMAIN_ONCE
	domain_getten = TRUE;
#endif

	cc_ovirt_vm_purge(ovm);


    } else {
	g_warning("Get Domain Failed! Retry latter..");

	spawn_async_get_domain_with_delay(ovm, GETDOMAIN_INTERVAL);
//      spawn_async_get_domain(ovm);

    }
//    g_message("[[[[[[");
    if (sout != NULL)
	g_free(sout);
    if (serr != NULL)
	g_free(serr);
//    g_message("]]]]]");


/* Close pid */
    g_message("About to close PID");
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

    gtk_combo_box_text_remove(GTK_COMBO_BOX_TEXT(combo_domain), 0);
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo_domain), 0,
				   pulse_c[pulse]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_domain), 0);
    pulse = (pulse + 1) % 5;

    return (TRUE);
}


static gboolean spawn_async_get_domain(CCOvirtVM * ovm)
{
    domain_get_ok = TRUE;

    g_message("Spawn get domain....");
    GPid pid;
    gchar *argv[] = { OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u",
	"admin@internal", "-p", "zj@Cloud)(12", "-d",
	ovm->win->ovirt->vmname, NULL
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
    ret = g_spawn_async_with_pipes(NULL, argv, NULL,
				   G_SPAWN_DO_NOT_REAP_CHILD, NULL,
				   NULL, &pid, NULL, &out, &err, NULL);
    if (!ret) {
	g_error("SPAWN FAILED");
	return FALSE;
    }
    /* Add watch function to catch termination of the process. This function
     *      * will clean any remnants of process. */
    g_child_watch_add(pid, (GChildWatchFunc) cb_child_watch, ovm);

    /* Create channels that will be used to read data from pipes. */
    out_ch = g_io_channel_unix_new(out);
    err_ch = g_io_channel_unix_new(err);

    /* Add watches to channels */
    g_io_add_watch(out_ch, G_IO_IN | G_IO_HUP, (GIOFunc) cb_out_watch,
		   ovm);
    g_io_add_watch(err_ch, G_IO_IN | G_IO_HUP, (GIOFunc) cb_err_watch,
		   ovm);

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

    if (cond == G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
    g_print("stat:vm_out_watch\n");
//    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
    g_io_channel_read_to_end(channel, &string, &size, NULL);

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

    if (cond == G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }

    g_print("stat:vm_err_watch\n");
//    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
    g_io_channel_read_to_end(channel, &string, &size, NULL);

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
    if (NULL != ovm->strout) {
	sout = g_strstrip(g_string_free(ovm->strout, FALSE));
	ovm->strout = NULL;
	g_message("STROUT::\n%s", sout);
//        g_free(sout);
    }
    if (NULL != ovm->strerr) {
	serr = g_strstrip(g_string_free(ovm->strerr, FALSE));
	ovm->strerr = NULL;
	g_message("STRERR::\n%s", serr);
	ovm->strerr = NULL;
//        g_free(serr);
    }

    g_print("vm_stat_child_watch\n");

    g_print("SOUT   :[%s] \nEXPECT :[%s]\n", sout, vm_stat_expect);

    if (!g_strcmp0(sout, vm_stat_expect)
	|| vm_stating_cnt >= STATING_TIMEOUT_MAX) {
	gchar *msg =
	    g_strdup_printf("Catch expected status of VM:[%s]",
			    vm_stat_expect);
	g_message("%s", msg);
	g_free(msg);

	g_source_remove(ovm->timeout_id);

	GtkWidget *console = ovm->win->winers->but_console;	// data->combo_domain;

	if (!g_strcmp0(sout, vm_stat_expect)) {
	    if (!strcmp(vm_stat_expect, "up")) {
		gtk_widget_set_sensitive((console), TRUE);

	    } else {		// if(!strcmp(vm_stat_expect,"down")){
		gtk_widget_set_sensitive((console), FALSE);

	    }
	} else if (vm_stating_cnt >= STATING_TIMEOUT_MAX) {
	    g_critical("VM Stating counter Ran out");
	    gtk_widget_set_sensitive((console), FALSE);
	}

	cc_ovirt_vm_purge(ovm);
	g_free(vm_stat_expect);
	vm_stating = FALSE;
	vm_stating_cnt = 0;

    } else {

	g_warning("Stat VM  Failed!Will retry latter..");

	vm_stating_cnt++;
	spawn_async_stat_vm_with_delay(ovm, REQUEST_INTERVAL);


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

    GtkWidget *console = ovm->win->winers->but_console;	//data->combo_domain;

    gtk_button_set_label(GTK_BUTTON(console), pulse_c[pulse]);

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
    gchar**argv=ovm->argv;

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
    ret = g_spawn_async_with_pipes(NULL, argv, NULL,
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
    out_ch = g_io_channel_unix_new(out);
    err_ch = g_io_channel_unix_new(err);

    /* Add watches to channels */
    g_io_add_watch(out_ch, G_IO_IN | G_IO_HUP,
		   (GIOFunc) cb_stat_vm_out_watch, ovm);
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

    vm_stating=TRUE;

    pulse = 0;
    
    gchar *argv[] = { OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u",
	ovm->win->ovirt->user, "-p", ovm->win->ovirt->passwd, "-s",
	ovm->win->ovirt->vmname, NULL
    };

    if(ovm->argv)
        g_strfreev(ovm->argv);

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

    if (cond == G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
    g_print("::vm_out_watch\n");
//    g_io_channel_read_line(channel, &string, &size, NULL, NULL);
    g_io_channel_read_to_end(channel, &string, &size, NULL);

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

    if (cond == G_IO_HUP) {
	g_io_channel_unref(channel);
	return (FALSE);
    }
    *ovm->op = FALSE;

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
    if (NULL != ovm->strout) {
	sout = g_strstrip(g_string_free(ovm->strout, FALSE));
	ovm->strout = NULL;
	g_message("STROUT::\n%s", sout);
//        g_free(out);
    }
    if (NULL != ovm->strerr) {
	serr = g_strstrip(g_string_free(ovm->strerr, FALSE));
	ovm->strerr = NULL;
	g_message("STRERR::\n%s", serr);
//        g_free(err);
    }



    if (*ovm->op) {
	g_message("VM's Operation Success.");
	//gtk_widget_set_sensitive (, TRUE);
	//
	//
	//TODO

	/*
	 * vm_stat
	 * */
	if (ovm->op == &vm_stat_ok) {
	    GtkWidget *console = ovm->win->winers->but_console;	// data->combo_domain;
	    if (!strcmp(sout, "up")) {
		gtk_widget_set_sensitive(console, TRUE);
	    } else {
		gtk_widget_set_sensitive(console, FALSE);
	    }
	}
	/*
	 *vm_start
	 * */
	if (ovm->op == &vm_start_ok) {

	    CCOvirtVM *svm = cc_ovirt_vm_dup(ovm);
//	    svm->win = ovm->win;
	    cc_ovirt_stating_vm_r(svm);

	}
	/*
	 *vm_stop
	 * */
	if (ovm->op == &vm_stop_ok) {

	    CCOvirtVM *svm = cc_ovirt_vm_dup(ovm);
//	    svm->win = ovm->win;
	    cc_ovirt_stating_vm_r(svm);

	}
	/*
	 *vm_kill
	 * */
	if (ovm->op == &vm_kill_ok) {

	    CCOvirtVM *svm = cc_ovirt_vm_dup(ovm);
//	    svm->win = ovm->win;
//        svm->argv=g_strdupv(ovm->argv);
	    cc_ovirt_stating_vm_r(svm);

	}


    if (ovm->op == &vm_console_ok){
        g_print("About to Open Spice Console..\n");  
//        vm_consoling=TRUE;
//    g_message();
    
    }





	cc_ovirt_vm_purge(ovm);



    } else {			//operation on VM failed!!



	g_warning("VM's operation Failed!!");
	//stop stating_vm..

	//TODO

    }


    vm_is_executing = FALSE;
    *ovm->op = TRUE;

    if (NULL != sout)
	g_free(sout);
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
    g_print("spawn_....\n");
    GPid pid;

    gchar **argv = ovm->argv;

    gint out, err;
    GIOChannel *out_ch, *err_ch;
    gboolean ret;

    /* Install timeout fnction that will move the progress bar */
// data->timeout_id = g_timeout_add( 1000, (GSourceFunc)cb_timeout, data );
    /* Spawn child process */
    ret = g_spawn_async_with_pipes(NULL, argv, NULL,
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
    out_ch = g_io_channel_unix_new(out);
    err_ch = g_io_channel_unix_new(err);

    /* Add watches to channels */
    g_io_add_watch(out_ch, G_IO_IN | G_IO_HUP, (GIOFunc) vm_out_watch,
		   ovm);
    g_io_add_watch(err_ch, G_IO_IN | G_IO_HUP, (GIOFunc) vm_err_watch,
		   ovm);


}







void cc_ovirt_vm_start_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing||vm_stating) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_start_ok = TRUE;

    gchar *argv[] = { OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u",
	ovm->win->ovirt->user, "-p", ovm->win->ovirt->passwd, "-U",
	ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_start_ok;

    spawn_async_vm(ovm);


}


void cc_ovirt_vm_shutdown_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing|| vm_stating) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_stop_ok = TRUE;

    gchar *argv[] = { OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u",
	ovm->win->ovirt->user, "-p", ovm->win->ovirt->passwd, "-D",
	ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_stop_ok;

    spawn_async_vm(ovm);


}


void cc_ovirt_vm_kill_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing|| vm_stating) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_kill_ok = TRUE;

    gchar *argv[] = { OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u",
	"admin@internal", "-p", "zj@Cloud)(12", "-K",
	ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_kill_ok;

    spawn_async_vm(ovm);


}


void cc_ovirt_vm_stat_r(CCOvirtVM * ovm)	//, CCLogingWin * w)
{
//    CCOvirtInfo* info=cc_ovirt_info_copy(ovirt);

    if (vm_is_executing) {
	g_message("IS EXECUTING...");
	return;
    }
    vm_stat_ok = TRUE;

    gchar *argv[] = { OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u",
	ovm->win->ovirt->user, "-p", ovm->win->ovirt->passwd, "-s",
	ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_stat_ok;

    spawn_async_vm(ovm);


}




void cc_ovirt_vm_console(CCOvirtVM* ovm)
{
//    if(vm_is_executing||vm_stating)
    if(vm_consoling){
    
    }

    vm_console_ok=TRUE;

 gchar *argv[] = { OVIRT_VMOPER, "-l", ovm->win->ovirt->maddr, "-u",
	ovm->win->ovirt->user, "-p", ovm->win->ovirt->passwd ,"-S",
	ovm->win->ovirt->vmname, NULL
    };

    ovm->argv = g_strdupv(argv);
    ovm->op = &vm_console_ok;

    spawn_async_vm(ovm);


}






//TODO get vm status independently-----------------------------------------

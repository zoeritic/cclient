#ifndef __H_VM_OVIRT_CC_
#define __H_VM_OVIRT_CC_
#include<gtk/gtk.h>
#include<string.h>
#include<unistd.h>

#include"cc-loging-win.h"
//#include"cc-loging-ovirt.h"

//#define TIMEOUT_INTERVAL 1
//#define STATING_INTERVAL 5
//#define GETDOMAIN_INTERVAL 5



//#define VMSTAT_DOWN 0
//#define VMSTAT_UP 1
//#define VMSTAT_POWERING_UP 2
//#define VMSTAT_POWERING_DOWN 3
//#define VMSTAT_WAIT_FOR_LAUNCH 4
//#define VMSTAT_NON_RESPONDING 5





typedef struct _CCOvirtVM {


    CCLogingWin*win;//reference

    gchar **argv;		//
    GString *strout;		//
    GString *strerr;		//

    gboolean *op;
    guint timeout_id;

} CCOvirtVM;




static gboolean spawn_async_get_domain(CCOvirtVM * ovm);
static void spawn_async_get_domain_with_delay(CCOvirtVM * ovm,guint delay);
static gboolean spawn_async_stat_vm(CCOvirtVM * ovm);
static void spawn_async_stat_vm_with_delay(CCOvirtVM * ovm,guint delay);
static gboolean timeout_getdomain_cb(CCOvirtVM * ovm);




CCOvirtVM *cc_ovirt_vm_new(void);
void cc_ovirt_vm_purge(CCOvirtVM * ovm);
CCOvirtVM * cc_ovirt_vm_dup(CCOvirtVM*ovm);

void cc_ovirt_vm_get_domain_r(CCOvirtVM*ovm);
void cc_ovirt_stating_vm_r(CCOvirtVM*ovm);

void cc_ovirt_vm_start_r(CCOvirtVM*ovm);
void cc_ovirt_vm_shutdown_r(CCOvirtVM*ovm);
void cc_ovirt_vm_kill_r(CCOvirtVM*ovm);
void cc_ovirt_vm_stat_r(CCOvirtVM*ovm);
void cc_ovirt_vm_viewer(CCOvirtVM* ovm);
void cc_ovirt_vm_viewer_2(CCOvirtVM*ovm);

void cc_loging_ovirt_get_domain_r(CCOvirtVM * ovm);




#endif

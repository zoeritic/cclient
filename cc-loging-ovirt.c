#include"cc-loging-ovirt.h"





CCOvirtInfo *cc_ovirt_info_new(void)
{
    CCOvirtInfo *info = g_new0(CCOvirtInfo, 1);
    g_print("CCOvirtInfo New!");
    return info;
}


void cc_ovirt_info_update(CCOvirtInfo * ovirt, CCLogingInfo * info,
			  CCLinkInfo * link)
{
    g_return_if_fail(ovirt != NULL && info != NULL && link != NULL);

    g_message("ovirt info generator..");
    if (ovirt->maddr)
	g_free(ovirt->maddr);
    ovirt->maddr = g_strdup(link->addr);
    g_message("maddr=%s", ovirt->maddr);

    if (ovirt->vmname)
	g_free(ovirt->vmname);
    ovirt->vmname = (g_ascii_strup(info->user, -1));
    g_message("vmname=%s", ovirt->vmname);

    if (ovirt->user)
	g_free(ovirt->user);
    ovirt->user = (g_strconcat(info->user, "@", info->domain, NULL));
    g_message("user=%s", ovirt->user);

    if (ovirt->passwd)
	g_free(ovirt->passwd);
    ovirt->passwd = g_strdup(info->passwd);
    g_message("passwd=%s", ovirt->passwd);

}


void cc_ovirt_info_destroy(CCOvirtInfo * ovirt)
{
    g_return_if_fail(ovirt != NULL);

    if (ovirt->vmname) {
	g_free(ovirt->vmname);
	ovirt->vmname = NULL;
    }
    if (ovirt->user) {
	g_free(ovirt->user);
	ovirt->user = NULL;
    }
    if (ovirt->passwd) {
	g_free(ovirt->passwd);
	ovirt->passwd = NULL;
    }
    if (ovirt->maddr) {
	g_free(ovirt->maddr);
	ovirt->maddr = NULL;
    }
    g_free(ovirt);

}

CCOvirtInfo *cc_ovirt_info_copy(CCOvirtInfo * oinfo)
{
    g_return_val_if_fail(oinfo != NULL, NULL);
    CCOvirtInfo *newinfo = cc_ovirt_info_new();

    newinfo->vmname = g_strdup(oinfo->vmname);
    newinfo->user = g_strdup(oinfo->user);
    newinfo->passwd = g_strdup(oinfo->passwd);
    newinfo->maddr = g_strdup(oinfo->maddr);


    return newinfo;

}




void cc_ovirt_info_print(CCOvirtInfo * oinfo)
{
    g_print("=VMOvirtInfo=\n");
    g_print("VMNAME  : %s\n", oinfo->vmname);
    g_print("USER    : %s\n", oinfo->user);
    g_print("PASSWD  : %s\n", oinfo->passwd);
    g_print("MADDR   : %s\n", oinfo->maddr);


}




CC=clang


SRC=cclient.c cc-manager-applet.c cc-loging-win.c cc-manager-panel.c cc-ovirt-vm.c cc-loging-info.c cc-link-info.c cc-loging-ovirt.c cc-utils.c




cclient: ${SRC}
	${CC} `pkg-config --libs --cflags gtk+-3.0`  $^ -g -o $@




BINARY=../build/cclient

CC=clang
CFLAGS= -g -O2 #-Wunused-function -Wunused-variable
LIBS=`pkg-config --libs --cflags gtk+-3.0`

OBJS=cclient.o cc-manager-applet.o cc-loging-win.o cc-manager-panel.o \
	cc-ovirt-vm.o cc-loging-info.o cc-link-info.o cc-loging-ovirt.o\
   	cc-utils.o cc-style.o

#OBJS=cc-manager-applet.o cc-manager-panel.o cc-loging-win.o cc-loging-info.o\
	 cc-link-info.o cc-link-setting.o cc-ovirt-vm.o\
	 cc-utils.o cc-style.o


all:${OBJS}
	${CC} ${CFLAGS} ${LIBS} $^ -o ${BINARY}


cclient.o:cclient.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@


cc-link-info.o:cc-link-info.c cc-link-info.h config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

#cc-link-setting.o:cc-link-setting.c cc-link-setting.c config.h
#	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

cc-loging-win.o:cc-loging-win.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

cc-loging-info.o:cc-loging-info.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

cc-loging-ovirt.o:cc-loging-ovirt.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

cc-style.o:cc-style.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

cc-utils.o:cc-utils.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@


cc-ovirt-vm.o:cc-ovirt-vm.c  config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@

cc-manager-panel.o:cc-manager-panel.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@


cc-manager-applet.o:cc-manager-applet.c config.h
	${CC} ${CFLAGS} ${LIBS} -c $< -o $@




.PHONY:clean all distclean

distclean:
	-rm ../build/${BINARY}



clean:
	-rm ./*.o
	-rm ${BINARY}





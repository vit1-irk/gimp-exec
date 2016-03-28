CFLAGS+=-ggdb -std=c11
INSTALL=install
INSTALL_DATA=$(INSTALL) -m 644
prefix=/usr
bindir=$(prefix)/bin
docdir=$(prefix)/share/doc/gimp-exec

all:
	${CC} ${CFLAGS} gimp-exec.c -o gimp-exec

clean:
	${RM} gimp-exec

install:
	mkdir -p ${DESTDIR}${bindir}
	mkdir -p ${DESTDIR}${docdir}
	$(INSTALL) gimp-exec ${DESTDIR}${bindir}
	$(INSTALL_DATA) README.md ${DESTDIR}${docdir}

uninstall:
	rm ${DESTDIR}${bindir}/gimp-exec
	rm ${DESTDIR}${docdir}/README.md
	rmdir ${DESTDIR}${docdir}

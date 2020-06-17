# hotkeythingy - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = hotkeythingy.c
OBJ = ${SRC:.c=.o}

all: clean options hotkeythingy

options:
	@echo hotkeythingy build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

hotkeythingy: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f hotkeythingy ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f hotkeythingy ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/hotkeythingy
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < hotkeythingy.1 > ${DESTDIR}${MANPREFIX}/man1/hotkeythingy.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/hotkeythingy.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/hotkeythingy\
		${DESTDIR}${MANPREFIX}/man1/hotkeythingy.1

.PHONY: all options clean dist install uninstall

SUBDIRS = majik_con majik_net majik_wait
bin_PROGRAMS = majik
majik_SOURCES = \
MCP.cpp MCP.h MError.cpp MError.h MKeyboard.cpp MKeyboard.h MMap.cpp MMap.h \
MPlayer.cpp MPlayer.h MTypes.cpp MTypes.h main.cpp majik.h

INCLUDES = -I. -Imajik_con -Imajik_net -Imajik_wait
CLEANFILES = *.debug
LDADD = \
majik_con/CursesCon.o majik_con/MCon.o \
majik_net/MNet.o majik_net/UnixNet.o \
majik_wait/CursesWait.o majik_wait/MWait.o \
-lcurses
toilet  :
	@-wc `find ../../ -regex .*[.][ch]+`

bin_PROGRAMS = majik
majik_SOURCES = \
CursesWait.cpp CursesWait.h MWait.cpp MWait.h

INCLUDES = -I. -I../ -I../majik_con -I../majik_net
CLEANFILES = *.debug
toilet  :
	@-wc `find ../../ -regex .*[.][ch]+`
majik: $(majik_OBJECTS) $(majik_DEPENDENCIES)
#	@rm -f majik
#	$(CXXLINK) $(majik_LDFLAGS) $(majik_OBJECTS) $(majik_LDADD) $(LIBS)

bin_PROGRAMS = majik
majik_SOURCES = \
MNet.cpp MNet.h UnixNet.cpp UnixNet.h

INCLUDES = -I. -I../
CLEANFILES = *.debug
toilet  :
	@-wc `find ../../ -regex .*[.][ch]+`
majik: $(majik_OBJECTS) $(majik_DEPENDENCIES)
#	@rm -f majik
#	$(CXXLINK) $(majik_LDFLAGS) $(majik_OBJECTS) $(majik_LDADD) $(LIBS)

# Makefile for Tokyo Tyrant for Java



#================================================================
# Setting Variables
#================================================================


# Generic settings
SHELL = /bin/sh

# Package information
PACKAGE = java-tokyotyrant
VERSION = 0.1
PACKAGEDIR = $(PACKAGE)-$(VERSION)
PACKAGETGZ = $(PACKAGE)-$(VERSION).tar.gz
LIBVER = 1
LIBREV = 1

# Targets
JARFILES = tokyotyrant.jar
JAVAFILES = TCRDB.java RDBQRY.java Util.java Loader.java TCRDBTest.java
LIBRARYFILES =  libjtokyotyrant.1.1.0.dylib libjtokyotyrant.1.dylib libjtokyotyrant.dylib libjtokyotyrant.jnilib
LIBOBJFILES = tcrdb.o rdbqry.o util.o myconf.o

# Install destinations
prefix = /usr/local
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
LIBDIR = ${exec_prefix}/lib
DESTDIR =

# Building configuration
JAVAC = /Library/Java/Home/bin/javac
JAVACFLAGS = -source 1.4 -d .
JAR = /Library/Java/Home/bin/jar
JAVAH = /Library/Java/Home/bin/javah
JAVADOC = /Library/Java/Home/bin/javadoc
JAVARUN = /Library/Java/Home/bin/java
CC = gcc
CPPFLAGS = -I. -I$(INCLUDEDIR) -L/Users/fmueller/include -L/usr/local/include -DNDEBUG -D_GNU_SOURCE=1 -I/Library/Java/Home/include -I/Library/Java/Home/include/mac
CFLAGS = -std=c99 -Wall -fPIC -fsigned-char -O2
LDFLAGS = -L. -L$(LIBDIR) -L/Users/fmueller/lib -L/usr/local/lib
LIBS = -ltokyotyrant -ltokyocabinet -lbz2 -lz -lpthread -lm -lc 
LDENV = LD_RUN_PATH=/lib:/usr/lib:$(LIBDIR):$(HOME)/lib:/usr/local/lib:$(LIBDIR):.
RUNENV = DYLD_LIBRARY_PATH=.:/lib:/usr/lib:$(LIBDIR):$(HOME)/lib:/usr/local/lib:$(LIBDIR)



#================================================================
# Suffix rules
#================================================================


.SUFFIXES :
.SUFFIXES : .c .o

.c.o :
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<



#================================================================
# Actions
#================================================================


all : $(JARFILES) $(LIBRARYFILES)
	@printf '\n'
	@printf '#================================================================\n'
	@printf '# Ready to install.\n'
	@printf '#================================================================\n'


clean :
	rm -rf $(JARFILES) $(LIBRARYFILES) $(LIBOBJFILES) \
	  *.o a.out *.class check.in check.out gmon.out leak.log \
	  casket casket-* casket.* *.wal *~ hoge moge


untabify :
	ls *.c *.h *.java | while read name ; \
	  do \
	    sed -e 's/\t/        /g' -e 's/ *$$//' $$name > $$name~; \
	    [ -f $$name~ ] && mv -f $$name~ $$name ; \
	  done


install :
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp -Rf $(JARFILES) $(DESTDIR)$(LIBDIR)
	cp -Rf $(LIBRARYFILES) $(DESTDIR)$(LIBDIR)
	@printf '\n'
	@printf '#================================================================\n'
	@printf '# Thanks for using Tokyo Tyrant for Java.\n'
	@printf '#================================================================\n'


uninstall :
	cd $(DESTDIR)$(LIBDIR) && rm -f $(JARFILES)
	cd $(DESTDIR)$(LIBDIR) && rm -f $(LIBRARYFILES)


dist :
	make untabify
	make distclean
	cd .. && tar cvf - $(PACKAGEDIR) | gzip -c > $(PACKAGETGZ)
	sync ; sync


distclean : clean
	rm -rf Makefile config.cache config.log config.status autom4te.cache


head : tokyotyrant.jar
	CLASSPATH=tokyotyrant.jar $(JAVAH) -jni \
	  tokyotyrant.RDBQRY tokyotyrant.TCRDB tokyotyrant.Util


check :
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar \
	  tokyotyrant.TCRDBTest write -ip -is -in 127.0.0.1 10000
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar tokyotyrant.TCRDBTest read 127.0.0.1
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar tokyotyrant.TCRDBTest remove 127.0.0.1
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar tokyotyrant.TCRDBTest misc 127.0.0.1 500
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar \
	  tokyotyrant.TCRDBTest write -tl -is -td 127.0.0.1 10000
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar tokyotyrant.TCRDBTest read -nl 127.0.0.1
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar tokyotyrant.TCRDBTest remove -nb 127.0.0.1
	$(RUNENV) $(JAVARUN) -cp tokyotyrant.jar tokyotyrant.TCRDBTest misc -tl -tb 127.0.0.1 500
	@printf '\n'
	@printf '#================================================================\n'
	@printf '# Checking completed.\n'
	@printf '#================================================================\n'


doc :
	make docclean
	mkdir -p doc
	$(JAVADOC) -locale en -windowtitle tokyotyrant -overview overview.html \
	  -tag note:a:"Note:" -d doc -nodeprecated -nohelp -quiet *.java


docclean :
	rm -rf doc


.PHONY : all clean install casket check doc



#================================================================
# Building binaries
#================================================================


tokyotyrant.jar : $(JAVAFILES)
	$(JAVAC) $(JAVACFLAGS) $(JAVAFILES)
	$(JAR) cvf $@ tokyotyrant/*.class
	rm -rf tokyotyrant


libjtokyotyrant.so.$(LIBVER).$(LIBREV).0 : $(LIBOBJFILES)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libjtokyotyrant.so.$(LIBVER) -o $@ \
	  $(LIBOBJFILES) $(LDFLAGS) $(LIBS)


libjtokyotyrant.so.$(LIBVER) : libjtokyotyrant.so.$(LIBVER).$(LIBREV).0
	ln -f -s libjtokyotyrant.so.$(LIBVER).$(LIBREV).0 $@


libjtokyotyrant.so : libjtokyotyrant.so.$(LIBVER).$(LIBREV).0
	ln -f -s libjtokyotyrant.so.$(LIBVER).$(LIBREV).0 $@


libjtokyotyrant.$(LIBVER).$(LIBREV).0.dylib : $(LIBOBJFILES)
	$(CC) $(CFLAGS) -dynamiclib -o $@ \
	  -install_name $(LIBDIR)/libjtokyotyrant.$(LIBVER).dylib \
	  -current_version $(LIBVER).$(LIBREV).0 -compatibility_version $(LIBVER) \
	  $(LIBOBJFILES) $(LDFLAGS) $(LIBS)


libjtokyotyrant.$(LIBVER).dylib : libjtokyotyrant.$(LIBVER).$(LIBREV).0.dylib
	ln -f -s libjtokyotyrant.$(LIBVER).$(LIBREV).0.dylib $@


libjtokyotyrant.dylib : libjtokyotyrant.$(LIBVER).$(LIBREV).0.dylib
	ln -f -s libjtokyotyrant.$(LIBVER).$(LIBREV).0.dylib $@


libjtokyotyrant.jnilib : libjtokyotyrant.dylib
	ln -f -s libjtokyotyrant.dylib $@


rdbqry.o : tokyotyrant_RDBQRY.h

tcrdb.o : tokyotyrant_TCRDB.h

util.o : tokyotyrant_Util.h

$(LIBOBJFILES) : myconf.h



# END OF FILE

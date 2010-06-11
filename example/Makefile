# Makefile for sample programs of Tokyo Tyrant



#================================================================
# Setting Variables
#================================================================


# Generic settings
SHELL = /bin/sh

# Targets
MYCLASSES = TCRDBEX.class

# Building binaries
JAVAC = javac
JAVACFLAGS = -source 1.4 -cp ../tokyotyrant.jar



#================================================================
# Suffix rules
#================================================================


.SUFFIXES :
.SUFFIXES : .java .class

.java.class :
	$(JAVAC) $(JAVACFLAGS) $<



#================================================================
# Actions
#================================================================


all : $(MYCLASSES)


clean :
	rm -rf $(MYCLASSES) *.class casket* *.tct *.idx.* *~


.PHONY : all clean



# END OF FILE

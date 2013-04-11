# SVN Info: $Id: Makefile 1371 2012-09-26 16:13:52Z werthm $

#####################################################################
##                  ___  ____   ____    _    ____                  ##
##                 / _ \/ ___| / ___|  / \  |  _ \                 ##
##                | | | \___ \| |     / _ \ | |_) |                ##
##                | |_| |___) | |___ / ___ \|  _ <                 ##
##                 \___/|____/ \____/_/   \_\_| \_\                ##
##                                                                 ##
##                                                                 ##
##         OSCAR simplifies coding and analyzing with ROOT         ##
##                                                                 ##
##                                                                 ##
##                        (C) 2007-2012 by                         ##
##      Dominik Werthmueller <dominik.werthmueller@unibas.ch>      ##
##                                                                 ##
#####################################################################



# --------------------------- System and ROOT variables ---------------------------

M             = $(shell grep OSCAR_MODULES main/include/TOVersion.h | cut -f3 -d ' ' | sed 's;\";;g' | sed 's;\,; ;g')
S             = src
L             = lib
O             = obj
I             = include
AI            = $(foreach dir, $(M), -I./$(dir)/$(I))
B             = bin

INCD          = $(foreach dir, $(M), $(wildcard $(dir)/$(I)/TO*.h))
INC           = $(notdir $(INCD))
OBJD          = $(foreach dir, $(M), $(patsubst $(dir)/$(S)/%.cxx, $(dir)/$(O)/%.o, $(wildcard $(dir)/$(S)/TO*.cxx)))
OBJD         += main/$(O)/G__OSCAR.o

OSTYPE       := $(subst -,,$(shell uname))

VERSION      := $(shell grep OSCAR_VERSION main/include/TOVersion.h | cut -f3 -d ' ' | sed 's;\";;g')

ROOTGLIBS    := $(shell root-config --glibs) -lEG -lFoam
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)

DEP_LIB      := libCore.so libHist.so libGraf.so libGpad.so libGui.so libPhysics.so libFoam.so libEG.so libTree.so

BIN_INSTALL_DIR = $(HOME)/$(B)

# ------------------------ Architecture dependent settings ------------------------

ifeq ($(OSTYPE),Darwin)
	LIB_OSCAR = $(L)/libOSCAR.dylib
	SOFLAGS = -dynamiclib -single_module -undefined dynamic_lookup -install_name $(CURDIR)/$(LIB_OSCAR)
	POST_LIB_BUILD = @ln $(L)/libOSCAR.dylib $(L)/libOSCAR.so
endif

ifeq ($(OSTYPE),Linux)
	LIB_OSCAR = $(L)/libOSCAR.so
	SOFLAGS = -shared
	POST_LIB_BUILD = 
endif

# -------------------------------- Compile options --------------------------------

CCCOMP      = g++
CXXFLAGS    = -g -O3 -Wall -fPIC $(ROOTCFLAGS) $(AI)
LDFLAGS     = -g -O3 $(ROOTLDFLAGS)

# ------------------------------------ targets ------------------------------------

all:	begin modules $(LIB_OSCAR) $(L)/libOSCAR.rootmap \
	$(B)/oscar-config headers end

begin:
	@echo
	@echo "Building OSCAR $(VERSION) on a $(OSTYPE) system"
	@echo

end:
	@echo "Done."

modules:
	@for dir in $(M); do make -s -C $$dir ; done

headers:
	@echo "Copying headers"
	@mkdir -p include
	@for dir in $(M); do cp $$dir/include/*.h include ; done

$(B)/oscar-config: $(LIB_OSCAR) main/$(S)/MainOSCARConfig.cxx
	@echo "Building the oscar-config tool"
	@mkdir -p $(B)
	@$(CCCOMP) $(CXXFLAGS) $(ROOTGLIBS) $(CURDIR)/$(LIB_OSCAR) -o $(B)/oscar-config main/$(S)/MainOSCARConfig.cxx

main/$(O)/G__OSCAR.o: main/$(S)/G__OSCAR.cxx
	@echo "Compiling Dict"
	@$(CCCOMP) $(CXXFLAGS) -o main/$(O)/G__OSCAR.o -c main/$(S)/G__OSCAR.cxx

main/$(S)/G__OSCAR.cxx: $(INCD) main/$(I)/LinkDef.h 
	@echo "Creating OSCAR dictionary"
	@rootcint -v -f $@ -c $(AI) -p $(INC) LinkDef.h

$(LIB_OSCAR): $(OBJD)
	@echo
	@echo "Building libOSCAR"
	@mkdir -p $(L)
	@rm -f $(L)/libOSCAR.*
	@$(CCCOMP) $(LDFLAGS) $(SOFLAGS) $(OBJD) -o $(LIB_OSCAR)
	@$(POST_LIB_BUILD)

$(L)/libOSCAR.rootmap: $(LIB_OSCAR)
	@echo "Creating ROOT map"
	@rlibmap -o $(L)/libOSCAR.rootmap -l $(LIB_OSCAR) -d $(DEP_LIB) -c main/$(I)/LinkDef.h

docs:
	@echo "Creating HTML documentation"
	@rm -r -f htmldoc
	#@root -b -n -q $(S)/htmldoc.C > /dev/null 2>&1
	root -b -n -q main/$(S)/htmldoc.C
	@echo "Done."

install: $(B)/OBrowser $(B)/OMCGen $(B)/OAna $(B)/pluto2mkin $(B)/haddf $(B)/checkMC \
        $(B)/oscar-config
	@echo "Installing binaries in $(BIN_INSTALL_DIR)"
	@mkdir -p $(BIN_INSTALL_DIR)
	@cp $(B)/* $(BIN_INSTALL_DIR) 
	@echo "Done."

uninstall:
	@echo "Uninstalling OSCAR applications"
	@rm -f $(BIN_INSTALL_DIR)/OBrowser
	@rm -f $(BIN_INSTALL_DIR)/OMCGen
	@echo "Done."
	
clean:
	@echo "Cleaning OSCAR distribution"
	@for dir in $(M); do make -s -C $$dir $@; done
	@rm -r -f include
	@rm -r -f $(B)
	@rm -r -f $(L)
	@rm -r -f main/$(S)/G__*
	@rm -r -f htmldoc
	@echo "Done."
  

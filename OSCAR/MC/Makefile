# SVN Info: $Id: Makefile 1202 2012-06-06 11:58:07Z werthm $

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
##                        (C) 2007-2010 by                         ##
##      Dominik Werthmueller <dominik.werthmueller@unibas.ch>      ##
##                                                                 ##
#####################################################################



# --------------------------- System and ROOT variables ---------------------------

S             = src
I             = include
EXI           = ../main/include ../utils/include ../analysis/include ../A2/include ../graph/include ../math/include
O             = obj

SRC           = $(wildcard $(S)/TO*.cxx)
INCD          = $(wildcard $(I)/TO*.h)
INC           = $(notdir $(INCD))
OBJD          = $(patsubst $(S)/%.cxx, $(O)/%.o, $(SRC))
OBJ           = $(notdir $(OBJD))

OSTYPE       := $(subst -,,$(shell uname))

ROOTGLIBS    := $(shell root-config --glibs) -lEG -lFoam
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)

vpath %.cxx $(S)
vpath %.h  $(I)
vpath %.o  $(O)

# -------------------------------- Compile options --------------------------------

CCCOMP      = g++
CXXFLAGS    = -g -O3 -Wall -fPIC $(ROOTCFLAGS) -I./$(I) $(foreach dir,$(EXI),-I$(dir))
LDFLAGS     = -g -O3 $(ROOTLDFLAGS)

# ------------------------------------ Targets ------------------------------------

all:	begin $(OBJ) end

begin:
	@echo "Building module MC"

end:
	@echo "Done."
	@echo

%.o: %.cxx
	@echo "Compiling $(patsubst %.cxx,%,$(notdir $<))"
	@mkdir -p $(O)
	@$(CCCOMP) $(CXXFLAGS) -o $(O)/$@ -c $< 

clean:
	@echo "Cleaning module MC"
	@rm -f -r $(O)
  

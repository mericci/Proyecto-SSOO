CC=gcc -std=gnu11
SRC=src
OBJ=obj
OPT=-g

CFLAGS=-Wunused -Wall $(OPT)

MTH=-lm
GTK=`pkg-config --cflags --libs gtk+-3.0`
PNG=-lpng

LIB=$(MTH) $(GTK) $(PNG)

COMMON=
PROGRAMS=crfs

SRCDIR=$(PROGRAMS)
DEPS := $(foreach i, $(COMMON), $(shell find $(SRC)/$(i) -name '*.h'))
HDRFILES := $(shell find $(SRC) -name '*.h')
SRCFILES := $(shell find $(SRC) -name '*.c')
OBJFILES := $(foreach i, $(SRCFILES), $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(i)))
OBJDIR := $(patsubst $(SRC)/%, $(OBJ)/%, $(shell find $(SRC) -type d))

all: $(OBJDIR) $(PROGRAMS)
	
	@echo "done compiling"

clean:
	@rm -fv $(PROGRAMS) && rm -rfv obj && echo "done cleaning"

$(OBJDIR):
	@mkdir -p $@

.SECONDEXPANSION:

LOCAL_DEPS = $(filter $(patsubst $(OBJ)/%, $(SRC)/%, $(dir $(1)))%, $(HDRFILES))

obj/%.o: src/%.c $$(call LOCAL_DEPS,$$@) $(DEPS) Makefile
	@$(CC) $(CFLAGS) $< -c -o $@ $(LIB) && echo "compiled '$@'"

$(PROGRAMS): $$(filter obj/$$@/% $(foreach i, $(COMMON), obj/$(i)/%), $(OBJFILES))
	@$(CC) $(CFLAGS) $^ -o $@ $(LIB) && echo "compiled '$@'"

#all:
#		@gcc -std=gnu11 -o crfs main.c
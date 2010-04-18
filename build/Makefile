ROOT_DIR := ..
BIN_DIR := $(ROOT_DIR)/bin
SRC_DIR := $(ROOT_DIR)/src

VPATH = $(SRC_DIR)

CC := colorgcc
COMMON_CCFLAGS := -g -c -Wall

MAIN_EXE := $(BIN_DIR)/simple-sampler
MAIN_OBJ := $(BIN_DIR)/libsimple-sampler.a

.PHONY: all
all: $(MAIN_EXE)
#all: $(MAIN_EXE) $(MAIN_OBJ)

CCFLAGS := $(COMMON_CCFLAGS) `pkg-config --cflags jack sndfile libxml++-2.6 rubberband gtkmm-2.4 gtk+-2.0 clutter-gtk-0.10 clutter-1.0`
LIBS := `pkg-config --libs jack sndfile libxml++-2.6 rubberband gtkmm-2.4 gtk+-2.0 clutter-gtk-0.10 clutter-1.0` -lm

SRC_FILES := $(wildcard $(SRC_DIR)/*.cc)
OBJECT_FILES := $(notdir $(SRC_FILES:.cc=.o))
DEPENDENCY_FILES := $(notdir $(SRC_FILES:.cc=.d))

#build the main exe
$(MAIN_EXE): $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) -o $(MAIN_EXE) $(LIBS)

#build the main lib
$(MAIN_OBJ): $(OBJECT_FILES)
	ar rvs $(MAIN_OBJ) $(OBJECT_FILES)

#Create a .d file for each .cc source file. The .d file contains a make rule with the dependencies for the .cc file.
$(DEPENDENCY_FILES): %.d: %.cc
	$(CC) -MM $< > $@

#Compile the .o file for each .cc file using the make rules in the .d files.
-include $(DEPENDENCY_FILES)

#Implicit rules to make sure our libraries are included and the desired gcc flags are used when compiling our objects.
$(OBJECT_FILES): %.o: %.cc
	$(CC) $(CCFLAGS) $(LIBS) $< -o $@

.PHONY: clean	
clean:
	rm -f $(DEPENDENCY_FILES) $(OBJECT_FILES) $(MAIN_OBJ) $(MAIN_EXE)

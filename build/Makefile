ROOT_DIR := ..
BIN_DIR := $(ROOT_DIR)/bin
SRC_DIR := $(ROOT_DIR)/src

VPATH = $(SRC_DIR)

COMMON_CCFLAGS := -g -c -Wall

MAIN_EXE := $(BIN_DIR)/simple-sample
MAIN_OBJ := $(BIN_DIR)/libsimple-sample.a

.PHONY: all
all: $(MAIN_OBJ) $(MAIN_EXE)

CCFLAGS := $(COMMON_CCFLAGS)
LIBS := `pkg-config --cflags --libs jack sndfile` -lm

SRC_FILES := $(wildcard $(SRC_DIR)/*.cc)
OBJECT_FILES := $(notdir $(SRC_FILES:.cc=.o))
DEPENDENCY_FILES := $(notdir $(SRC_FILES:.cc=.d))

#build the main exe
$(MAIN_EXE): $(OBJECT_FILES)
	g++ $(OBJECT_FILES) -o $(MAIN_EXE) $(LIBS)

#build the main lib
$(MAIN_OBJ): $(OBJECT_FILES)
	ar rvs $(MAIN_OBJ) $(OBJECT_FILES)

#Create a .d file for each .cc source file. The .d file contains a make rule with the dependencies for the .cc file.
$(DEPENDENCY_FILES): %.d: %.cc
	g++ -MM $< > $@

#Compile the .o file for each .cc file using the make rules in the .d files.
-include $(DEPENDENCY_FILES)

#Implicit rules to make sure our libraries are included and the desired gcc flags are used when compiling our objects.
$(OBJECT_FILES): %.o: %.cc
	g++ $(CCFLAGS) $(LIBS) $< -o $@

.PHONY: clean	
clean:
	rm -f $(DEPENDENCY_FILES) $(OBJECT_FILES) $(MAIN_OBJ) $(MAIN_EXE)

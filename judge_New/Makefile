
ARCH ?= 
CC = $(ARCH)g++
STRIP = $(ARCH)strip

#以下同根目录下的makefile的相同代码的解释
INC_DIR=./include ../common
DIR_SRC=./src
OBJS_DIR=./obj
LD_DIR=./libs


TARGETS=./judge

CUR_SOURCE=$(wildcard ${DIR_SRC}/*.cpp)
CFLAGS  += $(foreach dir,$(INC_DIR),-I$(dir))
CUR_OBJS = $(patsubst $(DIR_SRC)/%.cpp, $(OBJS_DIR)/%.o, $(CUR_SOURCE))

CFLAGS += -Wall -std=c++11
LD=-lpthread $$(mysql_config --cflags --libs) -lzmq -ldl


all:$(TARGETS)
$(OBJS_DIR)/%.o : $(DIR_SRC)/%.cpp
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c  $< -o $@
	@echo $(CC) $(CFLAGS) "-c" $(notdir $<) "-o" $(notdir $@)

$(TARGETS): $(CUR_OBJS)	
	@$(CC) $(CUR_OBJS) -L$(LD_DIR) $(LD) -o $(TARGETS)
#@$(STRIP) $(TARGETS)
    
clean:
	@rm -rf $(CUR_OBJS)
	@rm -rf $(TARGETS)
	@rm -rf $(OBJS_DIR)
 
install:
	$(INSTALL) $(TARGETS) $(BIN_DIR)

print:
	@echo $(CC) $(CFLAGS)
	@echo $(CUR_OBJS) 
	@echo $(CUR_SOURCE)


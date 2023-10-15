CC = gcc
EXT = c
CFLAGS = -g -Wall
# CFLAGS = -g -Wall -fsanitize=address
INCLUDE = include
IFLAGS = -I$(INCLUDE)
LFLAGS =
SRC = src
OBJ = obj
BINDIR = bin
ADDONS = $(BINDIR)/addons
SHARED_LIB = shared_lib
SHARED_LIB_FLAGS = -I$(SHARED_LIB)/include -shared -fPIC
SHARED_LIB_EXT = so
PROJNAME = SecuritySuite
BINNAME = $(PROJNAME).bin
SRCS = $(wildcard $(SRC)/*.$(EXT))
OBJS = $(patsubst $(SRC)/%.$(EXT), $(OBJ)/%.o, $(SRCS))
SHARED_LIBS = $(filter-out $(SHARED_LIB)/$(INCLUDE), $(wildcard $(SHARED_LIB)/*))
BIN = $(BINDIR)/$(BINNAME)

SUBMITNAME = $(PROJECT_NAME).zip
ZIP = zip

all: create_dirs
all: $(BIN)
all: $(SHARED_LIBS)

release: CFLAGS = -O2
release: new

$(SHARED_LIB)/%: $(SHARED_LIB)/%/main.c
	$(CC) $(SHARED_LIB_FLAGS) $(CFLAGS) $(wildcard $@/*.c) -o $(patsubst $(SHARED_LIB)/%, $(ADDONS)/%, $@).$(SHARED_LIB_EXT)

$(BIN): $(OBJS)
	$(CC) $(IFLAGS) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.$(EXT)
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

%.o: $(SRC)/%.$(EXT)
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $(OBJ)/$@	

link: $(OBJS)
	$(CC) $(IFLAGS) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

clean:
	rm -r $(BINDIR) $(OBJ)

create_dirs:
	mkdir -p $(BINDIR) $(ADDONS) $(OBJ)

new: clean
new: all

submit:
	rm -f $(SUBMITNAME)
	$(ZIP) $(SUBMITNAME) $(BIN)

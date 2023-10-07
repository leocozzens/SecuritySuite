CC = gcc
EXT = c
CFLAGS = -g -Wall
IFLAGS = -Iinclude
LFLAGS =
SRC = src
OBJ = obj
BINDIR = bin
PROJNAME = SecuritySuite
BINNAME = $(PROJNAME).bin
SRCS = $(wildcard $(SRC)/*.$(EXT))
OBJS = $(patsubst $(SRC)/%.$(EXT), $(OBJ)/%.o, $(SRCS))
BIN = $(BINDIR)/$(BINNAME)

SUBMITNAME = $(PROJECT_NAME).zip
ZIP = zip

all: create_dirs
all: $(BIN)

release: CFLAGS = -O2
release: new

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
	@mkdir -p $(BINDIR) $(OBJ)

new: clean
new: all

submit:
	rm -f $(SUBMITNAME)
	$(ZIP) $(SUBMITNAME) $(BIN)

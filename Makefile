
SRC = main.c
OBJ := $(SRC:.c=.o)
BIN = chat

# comp library
LIBS = sock/libsock.o utf8/libutf8.o args/libargs.o
CFLAGS += -g --std=c99 $(addprefix -I, $(dir $(LIBS)))

.PHONY: all clean

all: $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $?

$(LIBS):
	$(foreach var,$@, $(MAKE) $(MAKEFLAGS) -C $(dir $(var));)

$(BIN): $(OBJ) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(BIN) $(BINOBJ) $(LIB) $(OBJ)
	$(foreach var,$(LIBS), $(MAKE) $(MAKEFLAGS) -C $(dir $(var)) clean;)

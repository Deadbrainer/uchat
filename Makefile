SRC = src/*.c
INC = inc/*.h
OBJ = obj/*.o
BINARY = uchat

all: $(BINARY)

$(BINARY):
	clang  $(SRC) -o $(BINARY) `pkg-config --cflags --libs gtk+-3.0`

uninstall: clean
	rm -rf $(BINARY)

clean:
	rm -rf $(OBJ)

reinstall: uninstall all

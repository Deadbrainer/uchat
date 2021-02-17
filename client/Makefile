SRC = src/*.c
INC = inc/*.h
OBJ = obj/*.o
BINARY = uchat
FOLDER = obj

all: $(BINARY)

$(BINARY):
	@mkdir $(FOLDER)
	@clang -std=c11 -Wall -Wextra -Werror -Wpedantic -c $(SRC) -I $(INC)
	@mv *.o $(FOLDER)
	@clang $(OBJ) libmx/$(LIB) -o $(BINARY) `pkg-config --cflags --libs gtk+-3.0`

libmx.a: 
	make -C libmx

uninstall: clean
	@rm -rf $(BINARY)

clean:
	@rm -rf $(OBJ)

reinstall: uninstall all
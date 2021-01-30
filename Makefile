compile:
	gcc -Wall -g3 -fsanitize=address -pthread server/*.c -lsqlite3 libmx/libmx.a  -o ser
	gcc -Wall -g3 -fsanitize=address -pthread client/src/*.c libmx/libmx.a -o cl `pkg-config --cflags --libs gtk+-3.0`
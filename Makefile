compile:
	gcc -Wall -g3 -fsanitize=address -pthread server/*.c -lsqlite3 libmx/libmx.a -o ser
	gcc -Wall -g3 -fsanitize=address -pthread client/*.c libmx/libmx.a -o cl
compile:
	gcc -Wall -g3 -fsanitize=address -pthread userver/*.c -lsqlite3 -o server
	gcc -Wall -g3 -fsanitize=address -pthread client.c -o client
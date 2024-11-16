all: libserver-makefile

libserver-makefile:
	g++ -O2 -Wall -std=c++17 ./main.cpp -o libserver-makefile -lcppnet -L./cppnet/lib
	mv ./libserver-makefile ../bin

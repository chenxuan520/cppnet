all: libserver-makefile

libserver-makefile:
	g++ -O2 -Wall -std=c++17 ./main.cpp -o libserver-makefile -DCPPNET_OPENSSL -lcppnet -lssl -lcrypto -L./cppnet/lib

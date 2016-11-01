CS := $(wildcard src/*.cpp)

comp:
	clang++ -std=c++14 -stdlib=libc++ -lsfml-graphics -lsfml-window -lsfml-system -I/usr/local/Cellar/sfml/2.4.0/include -L/usr/local/Cellar/sfml/2.4.0/lib -o pong $(CS)

run:
	./pong


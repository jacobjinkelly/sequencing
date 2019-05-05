FLAGS = -Wall -g -std=c++11

all : main.exe align.exe

main.exe : main.o
	g++ $(FLAGS) -o main.exe main.o

main.o : main.cpp
	g++ $(FLAGS) -c main.cpp

align.exe : align.o
	g++ $(FLAGS) -o align.exe align.o

align.o : align.cpp
	g++ $(FLAGS) -c align.cpp

clean : 
	rm *.o *.exe

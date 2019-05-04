main.exe : main.o
	g++ -o main.exe main.o

main.o : main.cpp
	g++ -c main.cpp

clean : 
	rm *.o *.exe

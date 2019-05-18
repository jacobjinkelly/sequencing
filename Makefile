FLAGS = -Wall -g -std=c++11

all : align.exe

align.exe : align.o
	g++ $(FLAGS) -o $@ $^

%.o : %.cpp %.h
	g++ $(FLAGS) -c $<

clean : 
	rm *.o *.exe

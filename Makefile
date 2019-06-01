FLAGS = -Wall -g -std=c++11

all : align.exe test_align.exe

test_align.exe : align.o
	g++ $(FLAGS) -o $@ $^

align.exe : align.o
	g++ $(FLAGS) -o $@ $^

%.o : %.cpp %.h
	g++ $(FLAGS) -c $<

clean : 
	rm *.o *.exe

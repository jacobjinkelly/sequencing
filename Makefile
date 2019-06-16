FLAGS = -Wall -g -std=c++11

all : align.exe main.exe

%.exe : %.o
	g++ $(FLAGS) -o $@ $^

%.o : %.cpp %.h
	g++ $(FLAGS) -c $<

clean : 
	rm *.exe

CXXFLAGS = -std=c++17 

all: client.o server.o

-include Makefile.deps

Makefile.deps:
	$(CXX) $(CXXFLAGS) -MM *.[ch]pp > Makefile.deps

%.o: %.cpp
	g++ $(CXXFLAGS) $< -o $@ 

clean: 
	rm -f *.d *.o test Makefile.deps a.out
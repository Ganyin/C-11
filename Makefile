TARGET = main
CC = gcc-8
CXX = g++-8
CXXFLAGS = -std=c++11

cppsrc= $(wildcard *.cpp) 
dir=$(notdir $(src))
obj=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

main : $(obj)
	$(CXX) -o $@ $<

$(obj) : $(cppsrc)
	# $(CXX) -c $@ $^ $(CXXFLAGS)
	$(CXX) -c $^ $(CXXFLAGS)

.PHONY: clean
clean:
	-rm *.o  main *.gch
FILE=sample.cc
PROGRAM=app
CXX=clang++
LIBS=`pkg-config opencv --libs`
FLAGS=`pkg-config opencv --cflags`

PROGRAM : 
		$(CXX) -O2 $(FILE) -o $(PROGRAM) $(FLAGS) -std=c++1y $(LIBS)

.PHONY: clean
clean :
	$(RM) -f $(PROGRAM)

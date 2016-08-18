EXENAME = mp2
OBJS = main.o tree.o table.o

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi

all : $(EXENAME)

$(EXENAME) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp tree.h table.h
	$(CXX) $(CXXFLAGS) main.cpp

tree.o : tree.cpp tree.h table.h
	$(CXX) $(CXXFLAGS) tree.cpp

table.o : table.cpp table.h
	$(CXX) $(CXXFLAGS) table.cpp

clean :
	-rm -f *.o $(EXENAME)

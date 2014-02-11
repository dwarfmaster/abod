PROG=learn.prog
TEST=compute.prog
STREAM=stream.prog
LEARN=learnStream.prog
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11 `pkg-config --cflags opencv` -g
LDFLAGS=`pkg-config --libs opencv`
OBJS=abod.o

all : norm test stream learn

norm : $(PROG)

test : $(TEST)

stream : $(STREAM)

learn : $(LEARN)

$(PROG) : $(OBJS) main.o
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST) : $(OBJS) test.o
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(STREAM) : $(OBJS) stream.o
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(LEARN) : $(OBJS) learnStream.o
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

clean :
	rm -vf *.o

rec : clean all

.PHONY:rec clean all test stream learn


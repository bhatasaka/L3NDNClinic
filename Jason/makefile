CC=clang++
CFLAGS= -std=c++14 -Wall -lboost_system -lndn-cxx
TARGET=NDNchat
OBJ = $(TARGET).o Producer.o Consumer.o
DEPS = Producer.h Consumer.h NDNchat.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o $(TARGET)

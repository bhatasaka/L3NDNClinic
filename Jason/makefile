CC=clang++
CFLAGS=-I. -std=c++14 -Wall -lboost_system -lndn-cxx
TARGET=NDNchat
OBJ = $(TARGET).o
DEPS = 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o $(TARGET)

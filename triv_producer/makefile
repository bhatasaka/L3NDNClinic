CC=g++-8
CFLAGS=-I. -std=c++14 -Wall -lboost_system -lndn-cxx
TARGET=triv_producer
OBJ = $(TARGET).o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o $(TARGET)
CC:=/usr/local/clang_8.0.0/bin/clang++
AR:=/usr/bin/ar
CFLAGS:= -lrt -lpthread -lstdc++
TARGET:=chatThreading
LORA_LIB:=loralib
OBJ:=
DEPS:=
BUILD_DIR:=build
ARDUPI_DIR:=arduPi
ARDUPIAPI_DIR:=arduPi-api
LIBRARY_DIR:=libraries/arduPiLoRa

all: $(TARGET)

arduPi: $(ARDUPI_DIR)/arduPi.cpp
	$(CC) -c $(ARDUPI_DIR)/arduPi.cpp -o $(BUILD_DIR)/arduPi.o

arduPiUART: $(ARDUPIAPI_DIR)/arduPiUART.cpp arduPi
	$(CC) -c $(ARDUPIAPI_DIR)/arduPiUART.cpp -o $(BUILD_DIR)/arduPiUART.o

arduPiUtils: $(ARDUPIAPI_DIR)/arduPiUtils.cpp arduPiUART
	$(CC) -c $(ARDUPIAPI_DIR)/arduPiUtils.cpp -o $(BUILD_DIR)/arduPiUtils.o

arduPiMultiprotocol: $(ARDUPIAPI_DIR)/arduPiMultiprotocol.cpp arduPiUtils
	$(CC) -c $(ARDUPIAPI_DIR)/arduPiMultiprotocol.cpp -o $(BUILD_DIR)/arduPiMultiprotocol.o

arduPiLoRa: $(LIBRARY_DIR)/arduPiLoRa.cpp arduPiMultiprotocol
	$(CC) -c $(LIBRARY_DIR)/arduPiLoRa.cpp -I $(ARDUPIAPI_DIR) -I $(ARDUPI_DIR) -o $(BUILD_DIR)/arduPiLoRa.o

main: arduPiLoRa
	$(CC) -c $(TARGET).cpp -o $(BUILD_DIR)/$(TARGET).o \
		-I$(ARDUPI_DIR) \
		-I$(ARDUPIAPI_DIR) \
		-I$(LIBRARY_DIR) \

$(TARGET): main
	$(CC) $(CFLAGS) $(BUILD_DIR)/$(TARGET).o \
		$(BUILD_DIR)/arduPiLoRa.o \
		$(BUILD_DIR)/arduPiUART.o \
		$(BUILD_DIR)/arduPiUtils.o \
		$(BUILD_DIR)/arduPiMultiprotocol.o \
		$(BUILD_DIR)/arduPi.o \
		-I$(ARDUPI_DIR) \
		-I$(ARDUPIAPI_DIR) \
		-I$(LIBRARY_DIR) \
		-o $(TARGET)

library: main
	$(AR) rcvs lib_$(LORA_LIB).a \
		$(BUILD_DIR)/arduPiLoRa.o \
		$(BUILD_DIR)/arduPiUART.o \
		$(BUILD_DIR)/arduPiUtils.o \
		$(BUILD_DIR)/arduPiMultiprotocol.o \
		$(BUILD_DIR)/arduPi.o

withlib: library
	$(CC) $(CFLAGS) $(BUILD_DIR)/$(TARGET).o \
		-L. -l_$(LORA_LIB) \
		-o $(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*.o lib_$(TARGET).a $(TARGET)
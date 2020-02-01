//Chat via commmand line


#include <iostream>
#include <bitset>
#include <stdlib.h>
#include <time.h>
#include "arduPiLoRa.h"
using namespace std;



int e;

char message1 [] = "Packet 1, wanting to see if received packet is the same as sent packet";
char message2 [] = "Packet 2, broadcast test";

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: send packets without ACK\n");

  // Power ON the module
  e = sx1272.ON();
  printf("Setting power ON: state %d\n", e);

  // Set transmission mode
  e |= sx1272.setMode(4);
  printf("Setting Mode: state %d\n", e);

  // Set header
  e |= sx1272.setHeaderON();
  printf("Setting Header ON: state %d\n", e);

  // Select frequency channel
  e |= sx1272.setChannel(CH_01_900);
  printf("Setting Channel: state %d\n", e);

  // Set CRC
  e |= sx1272.setCRC_ON();
  printf("Setting CRC ON: state %d\n", e);

  // Select output power (Max, High or Low)
  e |= sx1272.setPower('H');
  printf("Setting Power: state %d\n", e);

  // Set the node address
  e |= sx1272.setNodeAddress(3);
  printf("Setting Node address: state %d\n", e);

  // Print a success message
  if (e == 0)
    printf("SX1272 successfully configured\n");
  else
    printf("SX1272 initialization failed\n");

  delay(1000);
}

void loop(void)
{
	// Send message1 and print the result
    string input = "";
    getline(cin,input);
    srand(time(0));
    char inputArray [input.length() + 1];
    strcpy(inputArray, input.c_str());
    int v= rand() % 128;
    bitset<8> identity= bitset<8>(v);
    cout << identity;
    unsigned long i = identity.to_ulong(); 
    char c = static_cast<char>( i );
    cout << c << endl;

    if (input.length() < 247)
    {
      char outputArray[input.length()+3];
      outputArray[0]=c;
      char num[2] = " ";
      outputArray[1]= num[0];
      outputArray[2]=num[1];
      cout << bitset<8>(outputArray[1]);
      strcat(outputArray, inputArray);
      e = sx1272.sendPacketTimeoutACK(BROADCAST_0, outputArray);
      cout << "data was sent:" <<  outputArray << endl;
      for (int i=0; i<input.length()+3; i++){
        cout << outputArray[i];
      }
      cout << bitset<8>(outputArray[1]);
    }
    else
    {
      int currentOffset = 0;
      char outputArray [250];
      int A =input.length()/247;
      int B = 0;
      bitset<4> numberOfPackets= bitset<4>(A);

      while(currentOffset < input.length() + 1)
      {
        bitset<4> Packet= bitset<4>(B);
        bitset<8> Arrange;
        Arrange[0] = numberOfPackets[0];
        Arrange[1] = numberOfPackets[1];
        Arrange[2] = numberOfPackets[2];
        Arrange[3] = numberOfPackets[3];
        Arrange[4] = Packet[0];
        Arrange[5] = Packet[1];
        Arrange[6] = Packet[2];
        Arrange[7] = Packet[3];
        cout << Arrange;
        unsigned long z = Arrange.to_ulong(); 
    char d = static_cast<char>( z );
        outputArray[0] = c;
      outputArray[1]= d;
        memcpy(outputArray+2, inputArray + currentOffset,247);
        outputArray[249] = 0;
        e = sx1272.sendPacketTimeoutACK(BROADCAST_0, outputArray);
        currentOffset += 247;
        cout << "data was sent:" <<  outputArray << endl;
        delay(300);
        B +=1;
      }
    }
}

int main (){
  setup();
  while (true){
    loop();

  }
	return (0);
}

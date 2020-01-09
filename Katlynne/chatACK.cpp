//Chat via commmand line


#include <iostream> 
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
  e |= sx1272.setChannel(CH_10_868);
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
    char inputArray [input.length() + 1];
    strcpy(inputArray, input.c_str());
    
    if (input.length() < 99)
    {
      e = sx1272.sendPacketTimeoutACK(BROADCAST_0, inputArray);
      cout << "data was sent:" <<  input << endl;
    }
    else
    {
      int currentOffset = 0;
      char outputArray [100];
      while(currentOffset < input.length() + 1)
      {
        memcpy(outputArray, inputArray + currentOffset,99);
        outputArray[99] = 0;
        e = sx1272.sendPacketTimeoutACK(BROADCAST_0, outputArray);
        currentOffset += 99;
        cout << "data was sent:" <<  outputArray << endl;
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

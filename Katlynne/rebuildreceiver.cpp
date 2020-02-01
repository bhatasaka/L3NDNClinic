/*  
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.2
 *  Design:            David Gascón 
 *  Implementation:    Covadonga Albiñana, Victor Boria, Ruben Martin
 */
 
// Include the SX1272 and SPI library: 
#include "arduPiLoRa.h"
#include <bitset>
#include <iostream>
using namespace std;

int e;
char my_packet[250];

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: receive packets with ACK\n");
  
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
  e |= sx1272.setNodeAddress(8);
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
  char identity = '\0';
  bool error = false;
  int packetlength = 0;
  // Receive message
  e = sx1272.receivePacketTimeoutACK(10000);
  if ( e == 0 )
  {
    printf("Receive packet with ACK, state %d\n",e);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    cout << my_packet;
    identity = my_packet[0];
    //bitset<8> id = bitset<8>(my_packet[0]);
    if (((uint8_t)identity) & 0x80 == 1){  
      char interest[sizeof(my_packet)-2];
      memcpy(interest,my_packet+2, sizeof(my_packet)-2);
      printf("Interest Packet: %s\n", interest);
    }
    else{
      bitset<8> packets =bitset<8>(my_packet[1]);
      cout << packets<< "\n";
      bitset<4> A;
      A[0]= packets[4];
      A[1]= packets[5];
      A[2]= packets[6];
      A[3]= packets[7];
      bitset<4> B;
      B[0]= packets[0];
      B[1]= packets[1];
      B[2]= packets[2];
      B[3]= packets[3];
      int packetnumb =(int)(A.to_ulong());
      cout <<B;
      
        //identity = id;
        packetlength = (int)(B.to_ulong());
        cout << packetlength;
      if(packetlength ==0){
        return;
      }
      char message[(packetlength+1)*247+1];
      cout << sizeof(message);
      
      for(int idx = 0; idx < packetlength; idx++)
      {
        message[idx-1] = my_packet[idx];
        
      }
      message[packetlength]='\0';
      //cout << message << '\n';
      // variable to handle the current packet number we're on
      
      for(int idx = 1; idx < packetlength; idx++) {
        do{
        e = sx1272.receivePacketTimeoutACK(10000);
      }while(e!=0);
        char current_id = (char)sx1272.packet_received.data[0];
        int current_packet_num = uint32_t(((uint8_t)sx1272.packet_received.data[1]) & 0xF0);
        if(current_id != identity)
        {
          error = true;
          break;
        }
        if(idx != current_packet_num){
          error =true;
          break;
        }
      for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      message[(idx*247)+i] = (char)sx1272.packet_received.data[i];
    }
        // check that we're on the net packet number
        // concatenate the new data to message
        cout << message << '\n';
      }
      if(!error){
        cout << message << '\n';
      }
      else{
        cout << "Dropped Message";
      }
      
      // check that error is false still, if it's not
      // then trhow away message
    }
  }
  else {
    printf("Receive packet with ACK, state %d\n",e);
  }
}

int main (){
	setup();
  int identity;
  int packetlength;
	while(1){
		loop();
	}
	return (0);
}

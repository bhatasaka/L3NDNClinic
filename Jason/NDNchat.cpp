// Simple C++ program -- displays text

// Header file - required for terminal input and output
#include <iostream>
#include <string>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>
#include <thread>
#include "Consumer.h"
#include "Producer.h"
#include "NDNchat.h"

// Main Function
int main()
{
  std::string user;
  // Threading
  // 0 - Main

  // Gets input from the user to determine the user name for the chat
  user = getUsername(); // In final version of this program, if possible it will be best to just get the name of the pi (ie pi1, pi2, etc.) rather than user input

  // Status to user that username has been accepted
  std::cout << "Username confirmed. Entering chat..." << std::endl;

  // thread 1 - Producer
  std::thread sendThread(sendMessages, user);
  // thread 2 - Consumer
  // receiveMessages(); // Printing of messages to terminal handled internally in this function, no return type
  std::thread receiveThread(receiveMessages);

  // functions don't return so this shouldn't be called
  sendThread.join();
  receiveThread.join();
}

void receiveMessages()
{
  ndnMessage msgRecieve;          // Create a structure for the user name and message recieved from other pi
  Consumer getter; // Create a consumer object that is used to consume the desired message

  while (true)
  {
    getter.run();

    // std::cout << msgRecieve.user << ": " << msgRecieve.message << endl;
  }
}

void sendMessages(std::string username)
{
  ndnMessage msgSend;             // Create a structure for the user name and message sent from this pi
  Producer sender; // Create a producer object that is used to produce the desired message

  // Set the username of the sender to the username determined when entering the chat program
  msgSend.user = username;

  std::cin.ignore();
  while (true)
  {
    // Take message input from user, "getLine" allows spaces in the message between words
    // std::getline(std::cin, msgSend.message);
    std::getline(std::cin, msgSend.message);

    msgSend.message = msgSend.message;

    // Debug tool, can be deleted
    // std::cout << "The program thinks you typed: " << msgSend.message << std::endl;

    sender.setMessage(msgSend);

    // uses the producer to send the message
    // Note - currently assumed message is small enough to send. There may be size limits to take into account in future messages
    // sender.run();
    // std::cout << "The user name entered is: " << msgSend.user << std::endl;
    // std::cout << "The message entered is: " << msgSend.message << std::endl;
  }
}

// Get user input to pick a username, and returns that name
// The final version may want to change this to just use the name of the pi rather than entering a username
std::string getUsername()
{
  std::string username;
  char confirm;

  while (true)
  {
    // recieve username
    std::cout << "Please enter your username: ";
    // std::cin.ignore();
    std::getline(std::cin, username);
    // Confirm username
    std::cout << "Your username is: " << username << std::endl;

    while (true)
    {
      // Allow user to confirm username
      std::cout << "Confirm username? (type y/n): ";
      // std::cin.ignore();
      std::cin >> confirm;

      // If username confirmed, return it. If username is denied, go back to recreate it. If unknown input, warn the user and try again
      if (confirm == 'y')
      {
        return username;
      }
      else if (confirm == 'n')
      {
        username = "";
        break;
      }
      else
      {
        std::cout << "WARNING - invalid input" << std::endl;
      }
    }
  }
}

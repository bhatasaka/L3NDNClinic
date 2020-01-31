#ifndef NDNCHAT_H
#define NDNCHAT_H
#include <string>

// declare struct to use in program
struct ndnMessage {
	std::string message;
	std::string user;
};

std::string getUsername();
void receiveMessages();
void sendMessages(std::string username);


#endif
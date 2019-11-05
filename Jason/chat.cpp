// Simple C++ program -- displays text

// Header file - requreid for terminal input and output
#include <iostream>
// This include requires the file to be saved in the correct location
// #include <ndn-cxx/face.hpp>

// placeholder: global variable list (not recommended to use this)

// Function declaration
std::string getUsername();
std::string getChatPartner();

// Main Function
int main() {
	// local variable list (variables for MAIN)
	std::string username;
	std::string otherUser;
	std::string sendMessage;
	std::string recievedMessage;
	bool messageRecieved;
	
	username = getUsername();
	
	std::cout << "Username confirmed. Entering chat..." << std::endl;
	
	// Look for a valid chat request from another user
	otherUser = getChatPartner();
	std::cout << "Joining chat with: " << otherUser << std::endl;
	
	while ( true ) {
		// Check for message from other user
		messageRecieved = true;
		recievedMessage = "Response";
		
		if ( messageRecieved ) {
			std::cout << otherUser << ": " << recievedMessage << std::endl;
			messageRecieved = false;
		}
		
		// Send a response
		std::cout << username << ": ";
		std::cin >> sendMessage;
	}
	
	return 0;
}

// Function searches for a chat partner and returns a string containing their name
std::string getChatPartner() {
	std::string username;
	
	username = "Bob";
	
	return username;
}

// Get user input to pick a username, and returns that name
std::string getUsername() {
	std::string username;
	char confirm;
	
	createName:
	
	// recieve username
	std::cout << "Please enter your username: ";
	std::cin >> username;
	
	// Confirm username
	std::cout << "Your username is: " << username << std::endl;
	
	confirmName:
	
	std::cout << "Confirm username? (type y/n): ";
	std::cin >> confirm;
	
	if ( confirm == 'y' ) {
		return username;
	}
	else if ( confirm == 'n' ) {
		goto createName;
	}
	else {
		std::cout << "WARNING - invalid input" << std::endl;
		goto confirmName;
	}
}

// Simple C++ program -- displays text

// Header file - required for terminal input and output
#include <iostream>
#include <string>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>


// declare struct to use in program
struct ndnMessage {
	std::string message;
	std::string user;
};

// Function declaration
void sendMessages(std::string username);
void receiveMessages();
std::string getUsername();

// This class is taken from the NDN tutorial "Trivial Consumer", and modified by Jason Stauffer
// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces should be used to prevent/limit name conflicts
namespace examples {

class Consumer
{
public:
  void
  run()
  {
	// Specify the name for the interest
    Name interestName("/ndn/l3clinic/chat");
    // Append NDN version number to the interest
    // interestName.appendVersion();
	
	// Print the interest name to the terminal
	// std::cout << "Creating interest: " << interestName << std::endl;
	
	// Create an interested with the interest name specified
    Interest interest(interestName);
    
    // Set interest parameters
    interest.setCanBePrefix(false);
    interest.setMustBeFresh(true);
    interest.setInterestLifetime(6_s); // The default is 4 seconds

    // std::cout << "Sending Interest " << interest << std::endl;
    m_face.expressInterest(interest,
                           bind(&Consumer::onData, this,  _1, _2),
                           bind(&Consumer::onNack, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));

    // processEvents will block until the requested data is received or a timeout occurs
    m_face.processEvents();
  }

private:
  void
  onData(const Interest&, const Data& data) const
  {
	ndnMessage message;
    // std::cout << "Received Data " << data << std::endl;
    message = getPayload(data);
    std::cout << message.user << ": " << message.message << std::endl;
  }

  void
  onNack(const Interest&, const lp::Nack& nack) const
  {
    // if nack.getReason() != "NoRoute"
	//	std::cout << "Received Nack with reason " << nack.getReason() << std::endl;
	// }
  }

  void
  onTimeout(const Interest& interest) const
  {
    std::cout << "Timeout for " << interest << std::endl;
  }
  
  // This segment of code was written by Bryan Hatasaka, convert type "data" to ndnMessage
  ndnMessage getPayload(const Data & data) const
  {
    const char* start = reinterpret_cast<const char *>(data.getContent().value());
    long length = data.getContent().value_size();
    std::string ret(start, length);
    
    ndnMessage ndn_message;
    ndn_message.user = ret.substr(0, ret.find("\r\n"));
    ndn_message.message = ret.substr(ret.find("\r\n")+2, ret.length());
    return ndn_message;
    
  }

private:
  Face m_face;
};

// This class is taken from NDN examples "Trivial Producer" and modified by Jason Stauffer

class Producer
{
public:
  void
  run(ndnMessage message)
  {
    m_face.setInterestFilter("/ndn/l3clinic/chat",
                             bind(&Producer::onInterest, this, _1, _2, message),
                             nullptr, // RegisterPrefixSuccessCallback is optional
                             bind(&Producer::onRegisterFailed, this, _1, _2));
    m_face.processEvents();
  }

private:
  void
  onInterest(const InterestFilter&, const Interest& interest, ndnMessage message)
  {
    // std::cout << ">> I: " << interest << std::endl;
	
    //static const ndnMessage content = message;
    //static const std::string content(message.message);

	std::string content = message.user + "\r\n" + message.message;

    // Create Data packet
    auto data = make_shared<Data>(interest.getName());
    data->setFreshnessPeriod(10_s);
    data->setContent(reinterpret_cast<const uint8_t*>(content.data()), content.size());

    // Sign Data packet with default identity
    m_keyChain.sign(*data);
    // m_keyChain.sign(*data, signingByIdentity(<identityName>));
    // m_keyChain.sign(*data, signingByKey(<keyName>));
    // m_keyChain.sign(*data, signingByCertificate(<certName>));
    // m_keyChain.sign(*data, signingWithSha256());

    // Return Data packet to the requester
    // std::cout << "<< D: " << *data << std::endl;
    m_face.put(*data);
  }

  void
  onRegisterFailed(const Name& prefix, const std::string& reason)
  {
    std::cerr << "ERROR: Failed to register prefix '" << prefix
              << "' with the local forwarder (" << reason << ")" << std::endl;
    m_face.shutdown();
  }

private:
  Face m_face;
  KeyChain m_keyChain;
};

} // namespace examples
} // namespace ndn

// Main Function
int main() {
	std::string user;
	// Threading
	// 0 - Main
	
	// Gets input from the user to determine the user name for the chat
	user = getUsername(); // In final version of this program, if possible it will be best to just get the name of the pi (ie pi1, pi2, etc.) rather than user input
	
	// Status to user that username has been accepted
	std::cout << "Username confirmed. Entering chat..." << std::endl;
	
	// thread 1 - Producer
	// sendMessages(user);
	// thread 2 - Consumer
	receiveMessages(); // Printing of messages to terminal handled internally in this function, no return type
	
}

void receiveMessages() {
	ndnMessage msgRecieve; // Create a structure for the user name and message recieved from other pi
	ndn::examples::Consumer getter; // Create a consumer object that is used to consume the desired message
	
	while (true) {
		getter.run();
		
		// std::cout << msgRecieve.user << ": " << msgRecieve.message << endl;
	}
}

void sendMessages(std::string username) {
	ndnMessage msgSend; // Create a structure for the user name and message sent from this pi
	ndn::examples::Producer sender; // Create a producer object that is used to produce the desired message
	
	// Set the username of the sender to the username determined when entering the chat program
	msgSend.user = username;
	
	while (true) {
		// Take message input from user, "getLine" allows spaces in the message between words
		// std::getline(std::cin, msgSend.message);
		std::cin.ignore();
		std::getline(std::cin, msgSend.message);
		
		// Add an empty line at the end of the message (makes it print nicer)
		msgSend.message = msgSend.message += "\n";
		
		// Debug tool, can be deleted
		//std::cout << "The program thinks you typed: " << msgSend.message << std::endl;
		
		// uses the producer to send the message
		// Note - currently assumed message is small enough to send. There may be size limits to take into account in future messages
		sender.run(msgSend);
		std::cout << "The user name entered is: " << msgSend.user << std::endl;
		std::cout << "The message entered is: " << msgSend.message << std::endl;
	}
}

// Get user input to pick a username, and returns that name
// The final version may want to change this to just use the name of the pi rather than entering a username
std::string getUsername() {
	std::string username;
	char confirm;
	
	while (true) {
		// recieve username
		std::cout << "Please enter your username: ";
		// std::cin.ignore();
		std::getline(std::cin, username);
		// Confirm username
		std::cout << "Your username is: " << username << std::endl;
			
		while (true) {
			// Allow user to confirm username
			std::cout << "Confirm username? (type y/n): ";
			// std::cin.ignore();
			std::cin >> confirm;
			
			// If username confirmed, return it. If username is denied, go back to recreate it. If unknown input, warn the user and try again
			if ( confirm == 'y' ) {
				return username;
			}
			else if ( confirm == 'n' ) {
				username = "";
				break;
			}
			else {
				std::cout << "WARNING - invalid input" << std::endl;
			}
		}
	}
}

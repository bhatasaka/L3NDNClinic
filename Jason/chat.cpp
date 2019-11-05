// Simple C++ program -- displays text

// Header file - requreid for terminal input and output
#include <iostream>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>

// placeholder: global variable list

// Function declaration
std::string getUsername();
std::string getChatPartner();

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
    // Append NDN version number to the interested
    interestName.appendVersion();
	
	// Print the interest name to the terminal
	std::cout << "Creating interest: " << interestName << std::endl;
	
	// Create an interested with the interest name specified
    Interest interest(interestName);
    
    // Set interest parameters
    interest.setCanBePrefix(false);
    interest.setMustBeFresh(true);
    interest.setInterestLifetime(6_s); // The default is 4 seconds

    std::cout << "Sending Interest " << interest << std::endl;
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
    std::cout << "Received Data " << data << std::endl;
  }

  void
  onNack(const Interest&, const lp::Nack& nack) const
  {
    std::cout << "Received Nack with reason " << nack.getReason() << std::endl;
  }

  void
  onTimeout(const Interest& interest) const
  {
    std::cout << "Timeout for " << interest << std::endl;
  }

private:
  Face m_face;
};

// This class is taken from NDN examples "Trivial Producer" and modified by Jason Stauffer

class Producer
{
public:
  void
  run()
  {
    m_face.setInterestFilter("/ndn/l3clinic/chat",
                             bind(&Producer::onInterest, this, _1, _2),
                             nullptr, // RegisterPrefixSuccessCallback is optional
                             bind(&Producer::onRegisterFailed, this, _1, _2));
    m_face.processEvents();
  }

private:
  void
  onInterest(const InterestFilter&, const Interest& interest)
  {
    std::cout << ">> I: " << interest << std::endl;

    static const std::string content("Hello, world!");

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
    std::cout << "<< D: " << *data << std::endl;
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
		recievedMessage = "This is an arbitrary response";
		
		if ( messageRecieved ) {
			std::cout << otherUser << ": " << recievedMessage << std::endl;
			messageRecieved = false;
		}
		
		// Send a response
		std::cout << username << ": ";
		std::getline (std::cin, sendMessage);
	}
	
	return 0;
}

// Function searches for a chat partner and returns a string containing their name
std::string getChatPartner() {
	std::string username;
	
	// Start by checking if any active searches for a chat
	// ndn::examples::Consumer consumer;
	// consumer.run();
	
	username = "Bob";
	
	// if no active chat requests
	// ndn::examples::Producer producer;
	// producer.run();
	
	return username;
}

// Get user input to pick a username, and returns that name
std::string getUsername() {
	std::string username;
	char confirm;
	
	// Create name label to jump back in code to the point where the username is chosen
	createName:
	
	// recieve username
	std::cout << "Please enter your username: ";
	std::getline(std::cin, username);
	
	// Confirm username
	std::cout << "Your username is: " << username << std::endl;
	
	// Confirm name label to jump back in code to the point where the username is confirmed
	confirmName:
	
	std::cout << "Confirm username? (type y/n): ";
	std::cin >> confirm;
	
	// If username confirmed, return it. If username is denied, go back to recreate it. If unknown input, warn the user and try again
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

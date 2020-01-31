#include <thread>
#include <chrono>
#include "Consumer.h"


void Consumer::run()
{
  // Specify the name for the interest
  ndn::Name interestName("/ndn/l3clinic/chat");
  // Append NDN version number to the interest
  // interestName.appendVersion();

  // Print the interest name to the terminal
  // std::cout << "Creating interest: " << interestName << std::endl;

  // Create an interested with the interest name specified
  ndn::Interest interest(interestName);

  // Set interest parameters
  interest.setCanBePrefix(false);
  interest.setMustBeFresh(true);
  interest.setInterestLifetime((boost::chrono::milliseconds)100); // The default is 4 seconds

  // std::cout << "Sending ndn::Interest " << interest << std::endl;
  m_face.expressInterest(interest,
                         bind(&Consumer::onData, this, _1, _2),
                         bind(&Consumer::onNack, this, _1, _2),
                         bind(&Consumer::onTimeout, this, _1));

  // processEvents will block until the requested data is received or a timeout occurs
  m_face.processEvents();
}

void Consumer::onData(const ndn::Interest &, const ndn::Data &data) const
{
  ndnMessage message;
  // std::cout << "Received ndn::Data " << data << std::endl;
  message = getPayload(data);
  std::cout << message.user << ": " << message.message << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void Consumer::onNack(const ndn::Interest &, const ndn::lp::Nack &nack) const
{
  // if nack.getReason() != "NoRoute"
  //	std::cout << "Received Nack with reason " << nack.getReason() << std::endl;
  // }
}

void Consumer::onTimeout(const ndn::Interest &interest) const
{
  std::cout << "Timeout for " << interest << std::endl;
}

// This segment of code was written by Bryan Hatasaka, convert type "data" to ndnMessage
ndnMessage Consumer::getPayload(const ndn::Data &data) const
{
  const char *start = reinterpret_cast<const char *>(data.getContent().value());
  long length = data.getContent().value_size();
  std::string ret(start, length);

  ndnMessage ndn_message;
  ndn_message.user = ret.substr(0, ret.find("\r\n"));
  ndn_message.message = ret.substr(ret.find("\r\n") + 2, ret.length());
  return ndn_message;
}
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include "triv_producer.h"
#include <iostream>
#include <chrono>
#include <thread>

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces can be used to prevent/limit name conflicts
namespace examples {

  Producer::Producer(std::string name)
  : _name(name)
  {

  }

  void
  Producer::run(std::string content)
  {
    _content = content;
    m_face.setInterestFilter("/ndn/l3clinic/hatasaka/chat",
                             bind(&Producer::onInterest, this, _1, _2),
                             RegisterPrefixSuccessCallback(),
                             bind(&Producer::onRegisterFailed, this, _1, _2));
    m_face.processEvents();
  }

  void
  Producer::onInterest(const InterestFilter& filter, const Interest& interest)
  {
    std::cout << "<< I: " << interest << std::endl;

    // Create new name, based on Interest's name
    Name dataName(interest.getName());
    dataName
      .append(_name) // add "testApp" component to Interest name
      .appendVersion();  // add "version" component (current UNIX timestamp in milliseconds)

    static const std::string content = _content;

    // Create Data packet
    shared_ptr<Data> data = make_shared<Data>();
    data->setName(dataName);
    data->setFreshnessPeriod(1_s); // 1 seconds

    tlv::ContentTypeValue content_type = tlv::ContentType_Blob;
    data->setContentType(content_type);
    data->setContent(reinterpret_cast<const uint8_t*>(content.data()), content.length());

    // Sign Data packet with default identity
    m_keyChain.sign(*data);
    // m_keyChain.sign(data, <identityName>);
    // m_keyChain.sign(data, <certificate>);

    int sleep_time = 2000;
    std::cout << "sleeping for " << sleep_time << " ms." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    std::cout << "woke fam" << std::endl;

    // Return Data packet to the requester
    std::cout << ">> D: " << *data << std::endl;
    std::cout << ">> C: " << data->getContent().value() << std::endl;
    m_face.put(*data);
  }


  void
  Producer::onRegisterFailed(const Name& prefix, const std::string& reason)
  {
    std::cerr << "ERROR: Failed to register prefix \""
              << prefix << "\" in local hub's daemon (" << reason << ")"
              << std::endl;
    m_face.shutdown();
  }



} // namespace examples
} // namespace ndn

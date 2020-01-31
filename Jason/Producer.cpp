#include "Producer.h"

void Producer::run(ndnMessage message)
{
    m_face.setInterestFilter("/ndn/l3clinic/chat",
                             bind(&Producer::onInterest, this, _1, _2, message),
                             nullptr, // RegisterPrefixSuccessCallback is optional
                             bind(&Producer::onRegisterFailed, this, _1, _2));
    m_face.processEvents();
}

void Producer::onInterest(const ndn::InterestFilter &, const ndn::Interest &interest, ndnMessage message)
{
    // std::cout << ">> I: " << interest << std::endl;

    //static const ndnMessage content = message;
    //static const std::string content(message.message);

    std::string content = message.user + "\r\n" + message.message;

    // Create Data packet
    std::shared_ptr<ndn::Data> data = std::make_shared<ndn::Data>(interest.getName());
    data->setFreshnessPeriod((boost::chrono::milliseconds)1000);
    data->setContent(reinterpret_cast<const uint8_t *>(content.data()), content.size());

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

void Producer::onRegisterFailed(const ndn::Name &prefix, const std::string &reason)
{
    std::cerr << "ERROR: Failed to register prefix '" << prefix
              << "' with the local forwarder (" << reason << ")" << std::endl;
    m_face.shutdown();
}
#ifndef PRODUCER_H
#define PRODUCER_H

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>

#include <iostream>
#include <chrono>
#include <thread>
#include "NDNchat.h"

// This class is taken from NDN examples "Trivial Producer" and modified by Jason Stauffer

class Producer
{
public:
    void run(ndnMessage message);

private:
    ndn::Face m_face;
    ndn::KeyChain m_keyChain;

    void onInterest(const ndn::InterestFilter &, const ndn::Interest &interest, ndnMessage message);

    void onRegisterFailed(const ndn::Name &prefix, const std::string &reason);
};


#endif
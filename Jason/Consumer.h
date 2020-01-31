#ifndef CONSUMER_H
#define CONSUMER_H

#include <iostream>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>
#include "NDNchat.h"

class Consumer
{
public:
  void run();

private:
  ndn::Face m_face;
  void onData(const ndn::Interest&, const ndn::Data& data) const;

  void onNack(const ndn::Interest&, const ndn::lp::Nack& nack) const;

  void onTimeout(const ndn::Interest& interest) const;
  
  // This segment of code was written by Bryan Hatasaka, convert type "data" to ndnMessage
  ndnMessage getPayload(const ndn::Data & data) const;
};

#endif
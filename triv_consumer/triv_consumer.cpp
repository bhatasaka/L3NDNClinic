/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 *
 * @author Alexander Afanasyev <http://lasr.cs.ucla.edu/afanasyev/index.html>
 */

#include <ndn-cxx/face.hpp>
#include "triv_producer.h"
#include <iostream>

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces can be used to prevent/limit name conflicts
namespace examples {

class Consumer : noncopyable
{
public:
  // Consumer(const DataCallback& data_cb, const NackCallback& nack_cb, const TimeoutCallback& timeout_cb)
  // : _data_callback(data_cb), _nack_callback(nack_cb), _timeout_callback(timeout_cb)
  // {
  // }

  Consumer(std::string name)
  {
    producer = new Producer(name);
    producer.run("Bryan has joined");
  }

  void
  run()
  {
    Interest interest(Name("/ndn/l3clinic/hatasaka/chat"));
    interest.setInterestLifetime(10_s); // 10 second lifetime
    interest.setMustBeFresh(true);
    interest.setDefaultCanBePrefix(true);

    m_face.expressInterest(interest,
                           bind(&Consumer::onData, this, _1, _2),
                           bind(&Consumer::onNack, this, _1, _2),
                           bind(&Consumer::onTimeout, this, _1));

    std::cout << "Sending " << interest << std::endl;

    // processEvents will block until the requested data received or timeout occurs
    m_face.processEvents((time::milliseconds)0, false);
  }

private:
  void
  onData(const Interest& interest, const Data& data)
  {
    std::cout << data << std::endl;
    std::cout << "Content: " << getPayload(data) << std::endl;
  }

  void
  onNack(const Interest& interest, const lp::Nack& nack)
  {
    std::cout << "received Nack with reason " << nack.getReason()
              << " for interest " << interest << std::endl;
  }

  void
  onTimeout(const Interest& interest)
  {
    std::cout << "Timeout, retrying " << interest << std::endl;
    run();
  }

  std::string getPayload(const Data & data)
  {
    const char* start = reinterpret_cast<const char *>(data.getContent().value());
    long length = data.getContent().value_size();
    std::string ret(start, length);
    return ret;
  }

private:
  Face m_face;
  ndn::examples::Producer* producer;

  // const DataCallback _data_callback;
  // const NackCallback _nack_callback;
  // const TimeoutCallback _timeout_callback;
};

} // namespace examples
} // namespace ndn

// void onData(const Interest& interest, const Data& data);
// void onNack(const Interest& interest, const lp::Nack& nack);
// void onTimeout(const Interest& interest);


int
main(int argc, char** argv)
{
  if(argc < 2)
  {
    std::cout << "Please provide your name as an argument" << std:: endl;
    return 1;
  }
  ndn::examples::Consumer consumer(argv[1]);//(onData, onNack, onTimeout);
  try {
    consumer.run();
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
  return 0;
}

// void onData(const Interest& interest, const Data& data)
// {
//   std::cout << data << std::endl;
// }

// void onNack(const Interest& interest, const lp::Nack& nack)
// {
//   std::cout << "received Nack with reason " << nack.getReason()
//             << " for interest " << interest << std::endl;
// }

// void onTimeout(const Interest& interest)
// {
//   std::cout << "Timeout " << interest << std::endl;
// }
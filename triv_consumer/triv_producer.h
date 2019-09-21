#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>

#include <iostream>
#include <chrono>
#include <thread>

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces can be used to prevent/limit name conflicts
namespace examples {

class Producer : noncopyable
{
public:
  Producer(std::string name);
  void
  run(std::string);

private:
  void
  onInterest(const InterestFilter& filter, const Interest& interest);

  void
  onRegisterFailed(const Name& prefix, const std::string& reason);

private:
  Face m_face;
  KeyChain m_keyChain;
  std::string _content;
  std::string _name;
};

} // namespace examples
} // namespace ndn

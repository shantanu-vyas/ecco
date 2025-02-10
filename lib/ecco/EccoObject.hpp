#ifndef ECCOOBJECT_HPP
#define ECCOOBJECT_HPP

#include <boost/uuid/random_generator.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <map>

static boost::uuids::uuid generateUuid()
{
  static boost::mutex mutex;
  static boost::uuids::random_generator gen;

  boost::mutex::scoped_lock lock(mutex);
  return gen();
}

namespace ecco
{
    class EccoObject
    {
        public:
            explicit EccoObject(std::string);
            EccoObject() = delete;
            EccoObject(EccoObject&) = delete;
            ~EccoObject();
            std::string GetName();
            std::string GetUUID();

        private:
            boost::uuids::uuid m_uuid;
            std::string m_name;
            static std::map<boost::uuids::uuid, EccoObject> m_allObjects; //store all objects in here?

        };
}



#endif

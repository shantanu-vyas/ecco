#include "EccoObject.hpp"


ecco::EccoObject::EccoObject(std::string name) : m_name(name)
{
}


ecco::EccoObject::~EccoObject()
{

}

std::string GetUUID()
{
    return "notimpl";
}

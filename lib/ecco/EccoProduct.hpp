#ifndef ECCOPRODUCT_HPP
#define ECCOPRODUCT_HPP

#include "EccoObject.hpp"

namespace ecco
{
    class EccoProduct : public EccoObject
    {
        public:
            virtual ~EccoProduct() = default;
            EccoProduct(EccoProduct&) = delete;
            EccoProduct(const EccoProduct&) = delete;
            EccoProduct() = delete;

            virtual void initialize() {};
            virtual void deinitalize() {};

            static std::string GetName() { return "ECCO PRODUCT"; };

        protected:
            explicit EccoProduct(std::string name) : EccoObject(name) {};
    };
}

#endif

#include "EccoManager.hpp"
#include "EccoProduct.hpp"
#include <memory>
#include <type_traits>

//if i put these here and dont inline them im getting "insantation of variable with no definintion" in the FBO manager class even though i am instantiating them here?
template<> std::shared_ptr<ecco::EccoManager<ecco::EccoProduct>> ecco::EccoManager<ecco::EccoProduct>::m_singleton = nullptr;
template<> std::mutex ecco::EccoManager<ecco::EccoProduct>::m_lock;

//construct EccoManager on EccoProduct::GetName()
#include <iostream>
template <>
ecco::EccoManager<ecco::EccoProduct>::EccoManager() :
  ecco::EccoObject(ecco::EccoProduct::GetName() + "-Manager")
{

}

template <>
std::shared_ptr<ecco::EccoManager<ecco::EccoProduct>> ecco::EccoManager<ecco::EccoProduct>::GetInstance() {
  if (!m_singleton) {
    std::lock_guard<std::mutex> lock(m_lock);
    if (!m_singleton) {
        //cant use make_shared since constructor is protected
        m_singleton = std::shared_ptr<EccoManager<EccoProduct>>(new EccoManager<EccoProduct>());
    }
  }
  return m_singleton;
}

//why can i return std:;shared_ptr but not std::make_shared
template<>
std::shared_ptr<ecco::EccoProduct> ecco::EccoManager<ecco::EccoProduct>::CreateProduct() {
  std::lock_guard<std::mutex> lock(m_lock);
  return std::shared_ptr<ecco::EccoProduct>(new ecco::EccoProduct("BASE"));
}

template<>
void ecco::EccoManager<ecco::EccoProduct>::DeleteProduct(const std::shared_ptr<ecco::EccoProduct> &obj) {
  std::lock_guard<std::mutex> lock(m_lock);
  m_products.erase(std::remove(m_products.begin(), m_products.end(), obj),
                   m_products.end());
}





//this is bad and not strongly typed
// template<>
// template <typename... Args>
// std::shared_ptr<ecco::EccoProduct> ecco::EccoManager<ecco::EccoProduct>::CreateProduct(Args &&...args) {

//   std::lock_guard<std::mutex> lock(m_lock);
//   auto product = std::shared_ptr<ecco::EccoProduct>(new ecco::EccoProduct(std::forward<Args>(args)...));

//   product->initialize();

//   m_products.emplace_back(product);
//   return product;
// }

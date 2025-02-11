#ifndef ECCOMANAGER_HPP
#define ECCOMANAGER_HPP

#include "EccoObject.hpp"
#include <vector>
#include <memory>
#include <mutex>

/*
 * template<typename T> is for full generic template
 * template<> is when you want to define the specialization object
 *
 * i want eccoproduct to have pure virtual functions but this fucks with the template specialization
 *
 *
 * the template typename... args is for variadic params
*/

namespace ecco
{
    template<typename T>
    class EccoManager : public EccoObject
    {
    public:

        EccoManager(const EccoManager&) = delete;
        ~EccoManager() = default;
        EccoManager& operator=(const EccoManager&) = delete;

        static std::shared_ptr<EccoManager<T>> GetInstance();

        template<typename... Args>
        std::shared_ptr<T> CreateProduct(Args&&... args) = delete;
        void DeleteProduct(const std::shared_ptr<T>& obj);

    protected:
        EccoManager() : EccoObject("FBO MANAGER") {};
        static inline std::shared_ptr<EccoManager<T>> m_singleton;
        static inline std::mutex m_lock;
        std::vector<std::shared_ptr<T>> m_products;

        /* ENABLE ONLY IF managers need introspection into other managers for debugging */
        // template<typename U>
        // friend class EccoManager;

    };
}

#endif // ECCOMANAGER_HPP

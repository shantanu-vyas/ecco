#include "AttachmentManager.hpp"
#include "Attachment.hpp"

#include <memory>


std::shared_ptr<ecco::EccoManager<ecco::OpenGL::Attachment>> ecco::EccoManager<ecco::OpenGL::Attachment>::GetInstance() {
    static std::shared_ptr<EccoManager<ecco::OpenGL::Attachment>> m_singleton;
    static std::mutex m_lock;

    if (!m_singleton) {
        std::lock_guard<std::mutex> lock(m_lock);
        if (!m_singleton) {
            // Use `new` because constructor is protected
            m_singleton = std::shared_ptr<EccoManager<ecco::OpenGL::Attachment>>(new EccoManager<ecco::OpenGL::Attachment>());
        }
    }
    return m_singleton;
}

// std::shared_ptr<ecco::OpenGL::Attachment> ecco::EccoManager<ecco::OpenGL::Attachment>::CreateProduct(const std::string& name, int width, int height) {
//use our nice cool new alias instead of ecco::EccoManager
std::shared_ptr<ecco::OpenGL::Attachment> ecco::OpenGL::AttachmentManager::CreateProduct(const std::string& name, int width, int height) {
    std::lock_guard<std::mutex> lock(m_lock);
    auto product = std::shared_ptr<OpenGL::Attachment>(new OpenGL::Attachment(name, width, height));
    product->initialize();
    m_products.emplace_back(product);
    return product;
}

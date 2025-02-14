#include "FrameBufferManager.hpp"
#include "FrameBuffer.hpp"

#include <memory>


std::shared_ptr<ecco::EccoManager<ecco::OpenGL::FrameBuffer>> ecco::EccoManager<ecco::OpenGL::FrameBuffer>::GetInstance() {
    static std::shared_ptr<EccoManager<ecco::OpenGL::FrameBuffer>> m_singleton;
    static std::mutex m_lock;

    if (!m_singleton) {
        std::lock_guard<std::mutex> lock(m_lock);
        if (!m_singleton) {
            // Use `new` because constructor is protected
            m_singleton = std::shared_ptr<EccoManager<ecco::OpenGL::FrameBuffer>>(new EccoManager<ecco::OpenGL::FrameBuffer>());
        }
    }
    return m_singleton;
}






// std::shared_ptr<ecco::OpenGL::FrameBuffer> ecco::EccoManager<ecco::OpenGL::FrameBuffer>::CreateProduct(const std::string& name, int width, int height) {
//use our nice cool new alias instead of ecco::EccoManager
std::shared_ptr<ecco::OpenGL::FrameBuffer> ecco::OpenGL::FrameBufferManager::CreateProduct(const std::string& name, int width, int height) {
    std::lock_guard<std::mutex> lock(m_lock);
    auto product = std::shared_ptr<OpenGL::FrameBuffer>(new OpenGL::FrameBuffer(name, width, height));
    product->initialize();
    m_products.emplace_back(product);
    return product;
}

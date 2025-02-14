#ifndef FRAMEBUFFERMANAGER_HPP
#define FRAMEBUFFERMANAGER_HPP

#include "../ecco/EccoManager.hpp"
#include "FrameBuffer.hpp"

namespace ecco
{
    template <>
    class EccoManager<ecco::OpenGL::FrameBuffer> : public EccoManager<ecco::EccoProduct> {
        public:
            std::shared_ptr<ecco::OpenGL::FrameBuffer>
            CreateProduct(const std::string &name, int width, int height);

            static std::shared_ptr<EccoManager<ecco::OpenGL::FrameBuffer>> GetInstance();
    };
}

/* Since the EccoManager has to be in the namespace of ecco since thats where the original template is defined
 * we namespace the specialization out and we only use FrameBufferManager and not EccoManager<FrameBuffer> */
namespace ecco::OpenGL {
    using FrameBufferManager = EccoManager<FrameBuffer>;
}

#endif

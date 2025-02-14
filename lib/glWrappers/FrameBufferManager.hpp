#ifndef FRAMEBUFFERMANAGER_HPP
#define FRAMEBUFFERMANAGER_HPP

#include "../ecco/EccoManager.hpp"
#include "FrameBuffer.hpp"

typedef std::shared_ptr<ecco::OpenGL::FrameBuffer> SP_FrameBuffer;

namespace ecco
{
    template <>
    class EccoManager<ecco::OpenGL::FrameBuffer> : public EccoManager<ecco::EccoProduct> {
        public:
            [[nodiscard]] std::shared_ptr<ecco::OpenGL::FrameBuffer>
            CreateProduct(const std::string &name, int width, int height);

            //Creates a FBO with the same attachments/dimensions etc as another FBO, does not copy the data
            [[nodiscard]] std::shared_ptr<ecco::OpenGL::FrameBuffer>
            CreateProduct(const SP_FrameBuffer fbo);

            [[nodiscard]] static std::shared_ptr<EccoManager<ecco::OpenGL::FrameBuffer>> GetInstance();

            [[nodiscard]] std::shared_ptr<ecco::OpenGL::FrameBuffer> GetCurrentBoundFrameBuffer();
            void BindFrameBuffer(SP_FrameBuffer framebuffer);
            void BindDefaultFrameBuffer();



        private:
            std::shared_ptr<ecco::OpenGL::FrameBuffer> m_currentFBO;

    };
}

/* Since the EccoManager has to be in the namespace of ecco since thats where the original template is defined
 * we namespace the specialization out and we only use FrameBufferManager and not EccoManager<FrameBuffer> */
namespace ecco::OpenGL {
    using FrameBufferManager = EccoManager<FrameBuffer>;
}

#endif

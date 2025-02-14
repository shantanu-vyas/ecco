#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "../ecco/EccoProduct.hpp"
#include "../ecco/EccoManager.hpp"

namespace ecco
{

    namespace OpenGL
    {
        class FrameBuffer : public EccoProduct
        {
            public:
                static std::string GetName() {return "FRAMEBUFFER";};
            //weird, if this constructor is public it can still be called even though its parent constructor is protected?
            protected:

                explicit FrameBuffer(std::string name, int width, int height) :
                    EccoProduct("FBO-" + name),
                    m_width(width),
                    m_height(height) {}

                void DeleteFrameBuffer();
                void ReinitializeFrameBuffer();
                void AddAttachment(int);
                void DeleteAttachment(int);
                //this needs to be attachments
                std::vector<int> m_attachments;

            private:
                int m_width;
                int m_height;

                friend class EccoManager<FrameBuffer>; //Allows EccoManager<FrameBuffer> to call the protected funcs ie the Constructor

        };
    }
}

#endif

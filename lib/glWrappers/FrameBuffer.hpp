#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "../ecco/EccoProduct.hpp"
#include "../ecco/EccoManager.hpp"
#include "Attachment.hpp"

namespace ecco
{

    namespace OpenGL
    {
        class FrameBuffer : public EccoProduct
        {
            public:
                static std::string GetName() {return "FRAMEBUFFER";};
            //weird, if this constructor is public it can still be called even though its parent constructor is protected?

            //Debug functions
            void DebugFBO();
            void SaveFBOAsImage(std::string tag);
            void SaveFBOAttachmentAsImage(std::string tag, std::shared_ptr<ecco::OpenGL::Attachment> attachment);
            void SaveFBOAllAttachmentsAsImage(std::string tag);


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


            private:
                int m_width;
                int m_height;
                std::vector<int> m_attachments;
                //what if we also include attachment sets for pingponging
                //std:map<id, std::vector<m_attachments>

                friend class EccoManager<FrameBuffer>; //Allows EccoManager<FrameBuffer> to call the protected funcs ie the Constructor

        };
    }
}

#endif

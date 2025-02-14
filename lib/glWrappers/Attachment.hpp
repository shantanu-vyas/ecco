#ifndef ATTACHMENT_HPP
#define ATTACHMENT_HPP

#include "../ecco/EccoProduct.hpp"
#include "../ecco/EccoManager.hpp"

namespace ecco
{

    namespace OpenGL
    {
        class Attachment : public EccoProduct
        {
            public:
                static std::string GetName() {return "ATTACHMENT";};
                //weird, if this constructor is public it can still be called even though its parent constructor is protected?
            protected:
                Attachment(std::string name, int width, int height) :
                    EccoProduct("Attachment-" + name),
                    m_width(width),
                    m_height(height) {}


            private:
                int m_width;
                int m_height;
                int type;
                //what if we also include attachment sets for pingponging
                //std:map<id, std::vector<m_attachments>

                friend class EccoManager<Attachment>; //Allows EccoManager<FrameBuffer> to call the protected funcs ie the Constructor

        };
    }
}

#endif

#ifndef ATTACHMENTMANAGET_HPP
#define ATTACHMENTMANAGET_HPP

#include "../ecco/EccoManager.hpp"
#include "Attachment.hpp"

namespace ecco
{
    template <>
    class EccoManager<ecco::OpenGL::Attachment> : public EccoManager<ecco::EccoProduct> {
        public:
            [[nodiscard]] std::shared_ptr<ecco::OpenGL::Attachment>
            CreateProduct(const std::string &name, int width, int height);

            [[nodiscard]]  static std::shared_ptr<EccoManager<ecco::OpenGL::Attachment>> GetInstance();
    };
}

namespace ecco::OpenGL {
    using AttachmentManager = EccoManager<Attachment>;
}

#endif

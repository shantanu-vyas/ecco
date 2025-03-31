#ifndef PREPOSTTESTVIEW_HPP
#define PREPOSTTESTVIEW_HPP

#include "TurboView.hpp"

namespace ecco {
namespace Turbo {

class PrePostTestView : public ecco::Turbo::TurboView {
public:
        PrePostTestView(std::string);
        ~PrePostTestView() = default;

        virtual void OnPrePreRender() override;
        virtual void OnPreRender() override;
        virtual void OnRender() override;
        virtual void OnPostRender() override;
        virtual void OnPostPostRender() override;
};
} // namespace Turbo
} // namespace ecco

#endif

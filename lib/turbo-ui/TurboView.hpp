#ifndef TURBOVIEW_HPP
#define TURBOVIEW_HPP

#import "../ecco/EccoObject.hpp"
#include <memory>

namespace ecco {
namespace Turbo {

struct Rect
{
    float m_width, m_height, m_x, m_y;
};

public class TurboView :
            public EccoObject,
            public std::enable_shared_from_this<TurboView>
{
    public:
        virtual TurboView() = 0;
        ~TurboView() = default;
        virtual void Initialize();
        virtual void DeInitialize();

        //how can i variant this to be either a (float,float,float,float), Rect
        void SetFrame(std::variant<std::tuple<float,float,float,float>, const Rect&>);
        void SetWidth(float w);
        void SetHeight(float h);
        void SetX(float x);
        void SetY(float y);

        void SetMargin();
        void SetTopMargin();
        void SetBottomMargin();
        void SetLeftMargin();
        void SetRightMargin();

        void SetPadding();
        void SetTopPadding();
        void SetBottomPadding();
        void SetLeftPadding();
        void SetRightPadding();

        virtual void PrePreRender() = 0;  //Runs before any of the objects [Prerender, Render, Postrender] cycle
        virtual void PreRender() = 0;
        virtual void Render();
        virtual void PostRender() = 0;
        virtual void PostPostRender() = 0; //Runs after all objects [Prerender, Render, Postrender] cyclex


        private:
        Rect m_bounds;
        Rect m_margins;
        Rect m_padding;
        std::weak_ptr<TurboView> m_parent;
        std::vector<std::shared_ptr<TurboView>> m_children;
        std::float m_frameTime; //this shold be a chrono type

}

}
} // namespace ecco

#endif

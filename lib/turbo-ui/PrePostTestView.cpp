#include "PrePostTestView.hpp"

#include <iostream>

using namespace ecco::Turbo;

#define DEBUG 0

PrePostTestView::PrePostTestView(std::string name) :
    TurboView(name) {

}

void PrePostTestView::OnPrePreRender() {
#if DEBUG
    std::cout << GetName() << " CUSTOM PRE PRE RENDER " << std::endl;
#endif
}
void PrePostTestView::OnPreRender() {
#if DEBUG
    std::cout << GetName() << " CUSTOM PRE RENDER " << std::endl;
#endif
}
void PrePostTestView::OnRender() {
#if DEBUG
    std::cout << GetName() << " CUSTOM RENDER " << std::endl;
#endif
}
void PrePostTestView::OnPostRender() {
#if DEBUG
    std::cout << GetName() << " CUSTOM POST RENDER " << std::endl;
#endif
}
void PrePostTestView::OnPostPostRender() {
#if DEBUG
    std::cout << GetName() << " CUSTOM POST POST RENDER " << std::endl;
#endif
}

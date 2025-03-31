#ifndef TURBOVIEW_HPP
#define TURBOVIEW_HPP

#import "../ecco/EccoObject.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace ecco {
namespace Turbo {

struct Rect {
  Rect(const glm::dvec2 &center, const glm::dvec2 &size)
      : m_center(center), m_size(size) {};
  Rect(float x, float y, float width, float height)
      : m_center(x, y), m_size(width, height) {};

  glm::dvec2 m_center;
  glm::dvec2 m_size;
};


class TurboView : public EccoObject,
                  public std::enable_shared_from_this<TurboView> {
public:
  TurboView(std::string name);// = delete;
  ~TurboView() = default;
  //Temporarily making this class not virtual so we can test out stuff
    virtual void Initialize() {};// = 0;
    virtual void DeInitialize() {};// = 0;

  // how can i variant this to be either a (float,float,float,float), Rect
  void SetFrame(const Rect &rect);
  void SetCenter(const glm::dvec2 &center);
  void SetX(float x);
  void SetY(float y);
  void SetSize(const glm::dvec2 &size);
  void SetWidth(float w);
  void SetHeight(float h);

  // TODO margins and padding

  virtual void OnPrePreRender();
  virtual void OnPreRender();

  virtual void OnRender();
  virtual void OnPostRender();
  virtual void OnPostPostRender();
                                     // Render, Postrender] cyclexa
  void AddChild(const std::shared_ptr<TurboView> child);
  [[nodiscard]] virtual bool HasParent();
  [[nodiscard]] std::weak_ptr<TurboView> GetParent();
  void SetParent(std::shared_ptr<TurboView> parent);
  virtual bool IsRoot();
  void SetOnResizeCBFunction(const std::function<void(void)>& func);

  void PrintTree(int depth = 0);


private:
  void PrePreRender();
  void Render();
  void PostPostRender();


  Rect m_bounds;
  Rect m_margins;
  Rect m_padding;
  std::weak_ptr<TurboView> m_parent;
  std::vector<std::shared_ptr<TurboView>> m_children;
  float m_frameTime; // this shold be a chrono type

  std::function<void(void)> m_onResizeCB;


  //PrePreRender, Render, PostPostRender need to be in TurboView but private
  //a child should never inherit from it
  //However due to the nature of wanting RootTurboView to be the only thing that can call
  //render all it needs access to those priv funcs..
  friend class RootTurboView;
};

class RootTurboView : public TurboView {
    public:
    RootTurboView(std::string name) : TurboView(name) {};
    ~RootTurboView() = default;
    virtual bool IsRoot() override { return true; };
    virtual std::weak_ptr<TurboView> GetParent() = delete;
    virtual bool HasParent() override { return false; };
    virtual void RenderAll() final;


};


} // namespace Turbo
} // namespace ecco

#endif

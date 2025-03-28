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
   TurboView(std::string name) = delete;
  ~TurboView() = default;
  virtual void Initialize() = 0;
  virtual void DeInitialize() = 0;

  // how can i variant this to be either a (float,float,float,float), Rect
  void SetFrame(const Rect &rect);
  void SetCenter(const glm::dvec2 &center);
  void SetX(float x);
  void SetY(float y);
  void SetSize(const glm::dvec2 &size);
  void SetWidth(float w);
  void SetHeight(float h);

  // TODO margins and padding

  void PrePreRender() = final;
  virtual void OnPrePreRender();
  virtual void OnPreRender();
  void Render() = final;
  virtual void OnRender();
  virtual void OnPostRender();
  void PostPostRender() = final;
  virtual void OnPostPostRender();
                                     // Render, Postrender] cyclex
  void RenderAll() = final;
  void AddChild(const std::shared_ptr<TurboView> child);
  [[nodiscard]] bool HasParent();
  [[nodiscard]] std::weak_ptr<TurboView> GetParent();
  virtual bool IsRoot();
  void SetOnResizeCBFunction(const std::function<void(void)>& func);

private:
  Rect m_bounds;
  Rect m_margins;
  Rect m_padding;
  std::weak_ptr<TurboView> m_parent;
  std::vector<std::shared_ptr<TurboView>> m_children;
  float m_frameTime; // this shold be a chrono type

  std::function<void(void)> m_onResizeCB;
};

class RootTurboView : public TurboView {
  virtual bool IsRoot() override { return true; };
  std::weak_ptr<TurboView> GetParent() = delete;

};


} // namespace Turbo
} // namespace ecco

#endif

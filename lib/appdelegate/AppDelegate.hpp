#ifndef APPDELEGATE_HPP
#define APPDELEGATE_HPP

#include "../ecco/EccoObject.hpp"
#include "../turbo-ui/TurboView.hpp"
#include <GLFW/glfw3.h>

namespace ecco {
namespace Base {
class AppDelegate : public Turbo::RootTurboView {
public:
  AppDelegate(const AppDelegate &) = delete;
  ~AppDelegate();
  AppDelegate &operator=(const AppDelegate &) = delete;
  void Run();
  [[nodiscard]] static std::shared_ptr<AppDelegate> GetInstance();
  static void SetAppName(std::string appname);
  static std::string GetAppName();
  void Resize(int width, int height);
  static void SetTargetFrameRate(int fps);
  static void SetVsync(bool on);

  void SetInitializeShadersCB(std::function<void(void)> cb);

private:
  AppDelegate();
  static void initializeAppDelegate();
  static void mainLoop();
  static void handleInput();
  static void resizeWindowCallback(GLFWwindow *window, int width, int height);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void scrollCallBack(GLFWwindow *window, double xoffset,
                             double yoffset);

  static std::shared_ptr<AppDelegate> m_singleton;
  static std::mutex m_lock;
  static int m_width, m_height;
  static bool m_isGLFWInit;
  static std::string m_appName;

  static bool m_isMainLoopRunning; // running or paused
  static bool m_shouldExit;
  static int m_targetFramerate;
  static double m_targetFrameTime;
  static bool m_isVsyncEnabled;

  // callbacks
  std::function<void(void)> m_initializeShadersCB;

  // GLFW
  static std::shared_ptr<GLFWwindow> m_glfwWindow;
};
} // namespace Base
} // namespace ecco

#endif

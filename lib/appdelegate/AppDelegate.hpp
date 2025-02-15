#ifndef APPDELEGATE_HPP
#define APPDELEGATE_HPP

#include <GLFW/glfw3.h>
#include "../ecco/EccoObject.hpp"

namespace ecco {
    namespace Base {
        class AppDelegate : public ecco::EccoObject {
            public:
                AppDelegate(const AppDelegate &) = delete;
                ~AppDelegate();
                AppDelegate &operator=(const AppDelegate &) = delete;

                [[nodiscard]] static std::shared_ptr<AppDelegate> GetInstance();
                static void SetAppName(std::string appname);
                static std::string GetAppName();
                void Resize(int width, int height);


            private:
                AppDelegate();
                static void initializeAppDelegate();
                static void mainLoop();
                static void handleInput();
                static void resizeWindowCallback(GLFWwindow* window, int width, int height);
                static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
                static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
                static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
                static void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);



                static std::shared_ptr<AppDelegate> m_singleton;
                static std::mutex m_lock;
                static int m_width, m_height;
                static bool m_isGLFWInit;
                static std::string m_appName;

                static bool m_isMainLoopRunning; //running or paused
                static bool m_shouldExit;
                static int m_targetFramerate;
                static double m_targetFrameTime;
                static bool m_isVsyncEnabled;

                //GLFW
                static std::shared_ptr<GLFWwindow> m_glfwWindow;

        };
    }
} // namespace ecco

#endif

#include "AppDelegate.hpp"
#include <GLFW/glfw3.h>
#include <memory>

#include <iostream>
#include <thread>
#include <chrono>

std::shared_ptr<ecco::Base::AppDelegate> ecco::Base::AppDelegate::m_singleton = nullptr;
std::mutex ecco::Base::AppDelegate::m_lock;
int ecco::Base::AppDelegate::m_width(500);
int ecco::Base::AppDelegate::m_height(500);
bool ecco::Base::AppDelegate::m_isGLFWInit(false);
std::string ecco::Base::AppDelegate::m_appName("APP");

bool ecco::Base::AppDelegate::m_isMainLoopRunning(true);
bool ecco::Base::AppDelegate::m_shouldExit(false);
int ecco::Base::AppDelegate::m_targetFramerate(60);
double ecco::Base::AppDelegate::m_targetFrameTime(1.0 / m_targetFramerate);
bool ecco::Base::AppDelegate::m_isVsyncEnabled(true);

std::shared_ptr<GLFWwindow> ecco::Base::AppDelegate::m_glfwWindow = nullptr;

ecco::Base::AppDelegate::AppDelegate() :
    ecco::EccoObject("APPDELEGATE")
{
    std::cout << "Initializing App Delegate" << std::endl;
    initializeAppDelegate();

    /* This value for vsync can be
     * 0 vsync off
     * 1 target monitor refresh rate
     * 2 target monitor refresh rate / 2.0
     * ... up to 4
     */

    glfwSwapInterval(m_isVsyncEnabled);
    std::cout << "Main Loop Running" << std::endl;
    mainLoop();

}

ecco::Base::AppDelegate::~AppDelegate() {
    if (m_glfwWindow) {
        glfwTerminate();
    }
}

void ecco::Base::AppDelegate::SetAppName(std::string appname) {
    std::lock_guard<std::mutex> guard(m_lock);
    m_appName = appname;
}

std::string ecco::Base::AppDelegate::GetAppName() {
    std::lock_guard<std::mutex> guard(m_lock);
    return m_appName;
}

std::shared_ptr<ecco::Base::AppDelegate> ecco::Base::AppDelegate::GetInstance() {
    std::lock_guard<std::mutex> guard(m_lock);
    if (!m_singleton) {
        m_singleton = std::shared_ptr<AppDelegate>(new AppDelegate());
    }
    else
    {
        std::cout << "APPDELEGATE ALREADY EXISTS" << std::endl;
    }
    return m_singleton;
}

// Resize Event
void ecco::Base::AppDelegate::Resize(int width, int height) {
    std::lock_guard<std::mutex> guard(m_lock);
    m_width = width;
    m_height = height;

    //resize children UI trigger in render pipeline? dont have uitk thought out yet...

}

void ecco::Base::AppDelegate::SetTargetFrameRate(int fps) {
    m_targetFramerate = fps;
    m_targetFrameTime = m_targetFramerate / 60.0;
    m_isVsyncEnabled = false;
}

void ecco::Base::AppDelegate::SetVsync(bool on) {
    m_isVsyncEnabled = on;
}


//if we move where this is called from and not constructor we need to lock
void ecco::Base::AppDelegate::initializeAppDelegate() {
    if (m_isGLFWInit) {
        std::cout << "Trying to reinit GLFW???" << std::endl;
        return;
    }

    if (!glfwInit()) {
        std::cout << "Failed to create GLFW" << std::endl;
        exit(-1);
    }

    /*
     *  GLFWwindow is forward declared inside of glfw3.hpp, std::make_shared needs to know sizeof which it can't get because of this
     *  THis custom deleter helps us get around that, we should also be setting custom deleters anyways for shared ptrs around gl base stuff
     *  since they are c structs and dont have destructors that properly deinit them
     *  i assume glfwDestroyWindow is deleting the struct so we dont need to know sizeof to free that memory?
     *  */

    m_glfwWindow = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(m_width, m_height, m_appName.c_str(), nullptr, nullptr),
        [](GLFWwindow* window) {
            if (window) {
                glfwDestroyWindow(window);
            }
        });


    if (!m_glfwWindow) {
        std::cout << "Failed to make glfw window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    std::cout << "Created GLFW WINDOW" << std::endl;
    glfwMakeContextCurrent(m_glfwWindow.get());

    glfwSetFramebufferSizeCallback(m_glfwWindow.get(), ecco::Base::AppDelegate::resizeWindowCallback);
    glfwSetKeyCallback(m_glfwWindow.get(), ecco::Base::AppDelegate::keyCallback);
    glfwSetCursorPosCallback(m_glfwWindow.get(), ecco::Base::AppDelegate::mouseCallback);
    glfwSetMouseButtonCallback(m_glfwWindow.get(), ecco::Base::AppDelegate::mouseButtonCallback);
    glfwSetScrollCallback(m_glfwWindow.get(), ecco::Base::AppDelegate::scrollCallBack);

    m_isGLFWInit = true;
}


void ecco::Base::AppDelegate::mainLoop() {

    // double currentTargetFrameTime;
    // if (m_isVsyncEnabled) {
    //     const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    //     double monitorRefreshRate = mode ? mode->refreshRate : 60.0;
    //     currentTargetFrameTime = 1.0 / monitorRefreshRate;
    // } else {
    //     currentTargetFrameTime = m_targetFrameTime;
    // }

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(m_glfwWindow.get())) {
        if (m_shouldExit) return;

        auto frameStart = std::chrono::high_resolution_clock::now();

        // gameUpdate(deltaTime);  // Update game logic
        // render();               // Render frame

        glfwSwapBuffers(m_glfwWindow.get());
        glfwPollEvents();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        double frameTime = std::chrono::duration<double>(frameEnd - frameStart).count();

        //Manually do vsync by getting monitor frame time and capping frame time at that
        //this way i can handle the vsync frame time warning since glfw doesn't let me
        //also glfws clock is shit so i cant even manually check the frametime against glfw vsyn

        // for manual fps cap fps
        if (!m_isVsyncEnabled) {
        //print warning if frametime was longer than target
            if (frameTime > m_targetFrameTime + .0001) {
                std::cout << "Mode: " << (m_isVsyncEnabled ? "VSYNC " : "MANUAL FPS") << std::endl;
                std::cout << "⚠ FRAME TOOK LONGER THAN EXPECTED: "
                          << frameTime << "s (Target: "
                          << m_targetFrameTime << "s)" << std::endl;
            }
            double sleepTime = m_targetFrameTime - frameTime;
            if (sleepTime > 0) {
                glfwWaitEventsTimeout(sleepTime);
            }
        }
    }
}


// Handle Input Events
void ecco::Base::AppDelegate::handleInput() {

}

void ecco::Base::AppDelegate::resizeWindowCallback(GLFWwindow* window, int width, int height) { }
void ecco::Base::AppDelegate::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q) {
            std::cout << "KEYBOARD EXIT" << std::endl;
            m_shouldExit = true;
        }
        else if (key == GLFW_KEY_P) {
            m_isMainLoopRunning = !m_isMainLoopRunning;
            std::cout << "KEYBOARD UNPAUSE/PAUSE: " <<  (m_isMainLoopRunning ? "RUNNING " : "PAUSED") << std::endl;
        }
        else if (key == GLFW_KEY_V) {
            m_isVsyncEnabled = !m_isVsyncEnabled;
            glfwSwapInterval(m_isVsyncEnabled);
            std::cout << "VSYNC ENABLED/DISABLED: " <<  (m_isVsyncEnabled ? "ENABLED " : ("DISABLED: TARGET FPS IS " + std::to_string(m_targetFramerate))) << std::endl;

        }
    }
}

void ecco::Base::AppDelegate::mouseCallback(GLFWwindow* window, double xpos, double ypos) { }
void ecco::Base::AppDelegate::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) { }
void ecco::Base::AppDelegate::scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) { }

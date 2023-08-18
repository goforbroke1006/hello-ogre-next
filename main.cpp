#include <iostream>

#include <Ogre.h>

int entryPoint();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {
    return entryPoint();
}

#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX

int main(int argc, char **argv) {
    return entryPoint();
}

#else

int main(int argc, char **argv) {
    std::cerr << "Unsupported platform" << std::endl;
    return EXIT_FAILURE;
}

#endif

static bool g_ogre_window_should_quit = false;

class MyWindowEventListener : public Ogre::WindowEventListener {
public:
    virtual void windowClosed(Ogre::Window *window) {
        printf("Window closed!\n");
        g_ogre_window_should_quit = true;
    }
};

int entryPoint() {
    auto *gOgreRoot = OGRE_NEW Ogre::Root();

    Ogre::RenderSystem *renderSystem =
            gOgreRoot->getRenderSystemByName("OpenGL 3+ Rendering Subsystem");
    if (!(renderSystem)) {
        printf("Render system not found!\n");
        return EXIT_FAILURE;
    }
    renderSystem->setConfigOption("Full Screen", "No");
    renderSystem->setConfigOption("Video Mode", "800 x 600");
    renderSystem->setConfigOption("sRGB Gamma Conversion", "Yes");
    gOgreRoot->setRenderSystem(renderSystem);

    auto *gOgreWindow = gOgreRoot->initialise(true, "OgreStartProject");

    const size_t numThreads = 1u;
    auto *gSceneManager = gOgreRoot->createSceneManager(Ogre::ST_GENERIC, numThreads, "SceneManager");

    Ogre::Camera *camera = gSceneManager->createCamera("Main Camera");
    {
        camera->setPosition(Ogre::Vector3(0, 5, 15));
        camera->lookAt(Ogre::Vector3(0, 0, 0));
        camera->setNearClipDistance(0.2f);
        camera->setFarClipDistance(1000.0f);
        camera->setAutoAspectRatio(true);
    }

    MyWindowEventListener g_myWindowEventListener;
    Ogre::WindowEventUtilities::addWindowEventListener(gOgreWindow, &g_myWindowEventListener);

    while (true) {
        Ogre::WindowEventUtilities::messagePump();
        if (g_ogre_window_should_quit)
            break;

        if (!(gOgreRoot->renderOneFrame())) {
            break;
        }
    }

    Ogre::WindowEventUtilities::removeWindowEventListener(gOgreWindow, &g_myWindowEventListener);
    OGRE_DELETE (gOgreRoot);
    gOgreRoot = nullptr;

    return EXIT_SUCCESS;
}

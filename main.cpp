#include <iostream>

#include <Ogre.h>
#include <OgreWindow.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreMeshManager2.h>
#include <OgreLight.h>
#include <OgreMesh.h>
#include <OgreItem.h>
#include <OgrePlatformInformation.h>

#include "WindowEventListener.h"
#include "utils.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32 && OGRE_PLATFORM != OGRE_PLATFORM_LINUX
int main(int argc, char **argv) {
    std::cerr << "Unsupported platform: " << OGRE_PLATFORM << std::endl;
    return EXIT_FAILURE;
}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {

#else

int main(int argc, char **argv) {

#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    auto *gOgreRoot = OGRE_NEW Ogre::Root();

    if (!gOgreRoot->restoreConfig())
        if (!gOgreRoot->showConfigDialog())
            return EXIT_FAILURE;

    Ogre::Window *gOgreWindow = gOgreRoot->initialise(true, "Hello Ogre next");


//    const size_t numThreads = std::max<uint32_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
    const size_t numThreads = 1u;
    auto *gSceneManager = gOgreRoot
            ->createSceneManager(Ogre::ST_GENERIC, numThreads, "SM");

    Ogre::Camera *camera = gSceneManager->createCamera("Main Camera");
    camera->setPosition(1000, 1000, 1000);
    camera->lookAt(0, 0, 0);
    camera->setNearClipDistance(0.2f);
    camera->setFarClipDistance(1000.0f);
    camera->setAutoAspectRatio(true);

//    Ogre::Light *light = gSceneManager->createLight();
//    Ogre::SceneNode *lightNode = gSceneManager->getRootSceneNode()->createChildSceneNode();
//    lightNode->attachObject(light);
//    light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
//    light->setType(Ogre::Light::LT_DIRECTIONAL);
//    light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());

    Ogre::CompositorManager2 *compositorManager = gOgreRoot->getCompositorManager2();
    const Ogre::String workspaceName("Demo Workspace");
    const Ogre::ColourValue backgroundColour(0.2f, 0.4f, 0.6f);
    compositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, Ogre::IdString());
    compositorManager->addWorkspace(gSceneManager, gOgreWindow->getTexture(), camera, workspaceName, true);


    WindowEventListener gWindowEventListener;
    Ogre::WindowEventUtilities::addWindowEventListener(gOgreWindow, &gWindowEventListener);

    bool bQuit = false;
    while (!bQuit) {
        Ogre::WindowEventUtilities::messagePump();

        bQuit |= gWindowEventListener.shouldQuit();

        if (gOgreWindow->isVisible() && !bQuit)
            bQuit |= !gOgreRoot->renderOneFrame();
    }

    Ogre::WindowEventUtilities::removeWindowEventListener(gOgreWindow, &gWindowEventListener);

    OGRE_DELETE gOgreRoot;
    gOgreRoot = nullptr;

    return EXIT_SUCCESS;
}

#endif

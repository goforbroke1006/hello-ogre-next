#include <iostream>

#include <Ogre.h>
#include <OgreWindow.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreLight.h>
#include <OgreMesh.h>
#include <OgrePlatformInformation.h>
#include <OGRE/Compositor/OgreCompositorWorkspace.h>

#include "WindowEventListener.h"
#include "utils.h"
#include "Hlms.h"

int main(int argc, char **argv) {
    auto *root = OGRE_NEW Ogre::Root("plugins_d.cfg", "ogre.cfg", "Ogre.log");

    if (!root->restoreConfig() && !root->showConfigDialog())
        return EXIT_FAILURE;

    root->getRenderSystem()->setConfigOption("sRGB Gamma Conversion", "Yes");
    auto *window = root->initialise(true, "Hello Ogre-next");

    registerHlms("resources2.cfg");

    //const size_t numThreads = std::max<uint32_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
    const size_t numThreads = 1u;
    auto *sceneManager = root->createSceneManager(Ogre::ST_GENERIC, numThreads, "Hello Ogre-next SM");

    Ogre::Camera *camera = sceneManager->createCamera("Main Camera");
    camera->setPosition(Ogre::Vector3(100, 100, 100));
    camera->lookAt(Ogre::Vector3(0, 0, 0));
    camera->setNearClipDistance(0.2f);
    camera->setFarClipDistance(1000.0f);
    camera->setAutoAspectRatio(true);

    // The setup for a basic compositor with a blue clear colour
    Ogre::CompositorManager2 *compositorManager = root->getCompositorManager2();
    const Ogre::String workspaceName("Demo Workspace");
    const Ogre::ColourValue backgroundColour(0.2f, 0.4f, 0.6f);
    compositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, Ogre::IdString());
    compositorManager->addWorkspace(sceneManager, window->getTexture(), camera, workspaceName, true);

    loadResources("resources2.cfg");

    WindowEventListener gWindowEventListener;
    Ogre::WindowEventUtilities::addWindowEventListener(window, &gWindowEventListener);

    //

    Ogre::Light *light = sceneManager->createLight();
    auto *lightNode = sceneManager
            ->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(Ogre::SCENE_DYNAMIC);
    lightNode->attachObject(light);
    light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());

    Ogre::Item *item = sceneManager
            ->createItem("ogrehead-2.mesh",
                         Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                         Ogre::SCENE_DYNAMIC);
    auto *mSceneNode = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
            ->createChildSceneNode(Ogre::SCENE_DYNAMIC);
    mSceneNode->attachObject(item);

    //

    bool bQuit = false;
    while (!bQuit) {
        Ogre::WindowEventUtilities::messagePump();

        bQuit |= gWindowEventListener.shouldQuit();

        if (window->isVisible() && !bQuit)
            bQuit |= !root->renderOneFrame();
    }

    Ogre::WindowEventUtilities::removeWindowEventListener(window, &gWindowEventListener);

    OGRE_DELETE root;
    root = nullptr;

    return EXIT_SUCCESS;
}

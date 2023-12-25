#include <iostream>

#include <Ogre.h>
#include <OgreWindow.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreMeshManager2.h>
#include <OgreLight.h>
#include <OgreMesh.h>
#include <OgreItem.h>
#include <OgrePlatformInformation.h>
#include <OGRE/Compositor/OgreCompositorWorkspace.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OGRE/Overlay/OgreOverlayManager.h>

#include "WindowEventListener.h"
#include "Hlms.h"
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
    auto *gOgreRoot = OGRE_NEW Ogre::Root("plugins_d.cfg", "ogre.cfg", "Ogre.log");

    if (!gOgreRoot->restoreConfig())
        if (!gOgreRoot->showConfigDialog())
            return EXIT_FAILURE;

    gOgreRoot->initialise(false);

    Ogre::Window *gOgreWindow = gOgreRoot->initialise(true, "Hello Ogre next");


    {
        // Loading resources
        Ogre::ConfigFile cf;
        cf.load("resources_d.cfg");
//        cf.load("resources2.cfg");
        Ogre::ConfigFile::SectionIterator it = cf.getSectionIterator();
        Ogre::String section, type, arch;
        while (it.hasMoreElements()) {
            section = it.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = it.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i) {
                type = i->first;
                arch = i->second;
                if (type == "DoNotUseAsResource")
                    continue;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, section);
            }
        }


//        auto &resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();
//        resourceGroupManager.addResourceLocation("./Media/Hlms/Common/Any", "FileSystem", "General");
//        resourceGroupManager.addResourceLocation("./Media/Hlms/Common/GLSL", "FileSystem", "General");
//        resourceGroupManager.addResourceLocation("./Media/Hlms/Common/HLSL", "FileSystem", "General");
//        resourceGroupManager.addResourceLocation("./Media/Hlms/Common/Metal", "FileSystem", "General");
//
//        resourceGroupManager.addResourceLocation("./Media/models", "FileSystem", "Models");
////        resourceGroupManager.addResourceLocation("./Media/materials/programs", "FileSystem", "Programs");
//        resourceGroupManager.addResourceLocation("./Media/materials/scripts", "FileSystem", "Models");
//        resourceGroupManager.addResourceLocation("./Media/materials/textures", "FileSystem", "Models");

        // Initialise resources groups
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
    }

    registerHlms();

    auto *mOverlaySystem = OGRE_NEW Ogre::v1::OverlaySystem();

//    const size_t numThreads = std::max<uint32_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
    const size_t numThreads = 1u;
    auto *gSceneManager = gOgreRoot->createSceneManager(Ogre::ST_GENERIC, numThreads, "SM");
    gSceneManager->addRenderQueueListener(mOverlaySystem);
    gSceneManager->getRenderQueue()->setSortRenderQueue(
            Ogre::v1::OverlayManager::getSingleton().mDefaultRenderQueueId,
            Ogre::RenderQueue::StableSort);

    //Set sane defaults for proper shadow mapping
    gSceneManager->setShadowDirectionalLightExtrusionDistance(500.0f);
    gSceneManager->setShadowFarDistance(500.0f);

    Ogre::Camera *camera = gSceneManager->createCamera("Main Camera");
    camera->setPosition(1000, 1000, 1000);
    camera->lookAt(0, 0, 0);
    camera->setNearClipDistance(0.2f);
    camera->setFarClipDistance(1000.0f);
    camera->setAutoAspectRatio(true);


    Ogre::CompositorManager2 *compositorManager = gOgreRoot->getCompositorManager2();
    const Ogre::String workspaceName("Demo Workspace");
    const Ogre::ColourValue backgroundColour(0.2f, 0.4f, 0.6f);
    compositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, Ogre::IdString());
    auto *mWorkspace = compositorManager->addWorkspace(gSceneManager, gOgreWindow->getTexture(), camera, workspaceName,
                                                       true);
    gSceneManager = mWorkspace->getSceneManager();

    Ogre::Item *item = gSceneManager
            ->createItem("ogrehead-2.mesh",
                         Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                         Ogre::SCENE_DYNAMIC);
    auto *mSceneNode = gSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
            ->createChildSceneNode(Ogre::SCENE_DYNAMIC);
    mSceneNode->attachObject(item);

//    Ogre::Light *light = gSceneManager->createLight();
//    Ogre::SceneNode *lightNode = gSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::SCENE_DYNAMIC);
//    lightNode->attachObject(light);
//    light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
//    light->setType(Ogre::Light::LT_DIRECTIONAL);
//    light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());

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

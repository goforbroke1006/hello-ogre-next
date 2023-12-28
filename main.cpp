#include <iostream>

#include <Ogre.h>
#include <OgreWindow.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreMeshManager2.h>
#include <OgreLight.h>
#include <OgreMesh.h>
#include <OGRE/OgreItem.h>
#include <OgrePlatformInformation.h>
#include <OGRE/Compositor/OgreCompositorWorkspace.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OGRE/Overlay/OgreOverlayManager.h>

#include "WindowEventListener.h"
#include "Hlms.h"
#include "utils.h"

void loadResources(const std::string &filepath) {
    // Loading resources
    Ogre::ConfigFile cf;
    cf.load(filepath);

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
    auto *root = OGRE_NEW Ogre::Root("plugins_d.cfg", "ogre.cfg", "Ogre.log");

    if (!root->restoreConfig() && !root->showConfigDialog())
        return EXIT_FAILURE;

    root->getRenderSystem()->setConfigOption("sRGB Gamma Conversion", "Yes");
    auto *window = root->initialise(true, "Hello Ogre-next");

    registerHlms();

//    loadResources("resources2.cfg");
    loadResources("resources_d.cfg");

    //const size_t numThreads = std::max<uint32_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
    const size_t numThreads = 1u;
    auto *sceneManager = root->createSceneManager(Ogre::ST_GENERIC, numThreads, "Hello Ogre-next SM");

    Ogre::Camera *camera = sceneManager->createCamera("Main Camera");
    camera->setPosition(100, 100, 100);
    camera->lookAt(0, 0, 0);
    camera->setNearClipDistance(0.2f);
    camera->setFarClipDistance(1000.0f);
    camera->setAutoAspectRatio(true);

    // The setup for a basic compositor with a blue clear colour
    Ogre::CompositorManager2 *compositorManager = root->getCompositorManager2();
    const Ogre::String workspaceName("Demo Workspace");
    const Ogre::ColourValue backgroundColour(0.2f, 0.4f, 0.6f);
    compositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, Ogre::IdString());
    compositorManager->addWorkspace(sceneManager, window->getTexture(), camera, workspaceName, true);

//    Ogre::Item *item = sceneManager
//            ->createItem("ogrehead-2.mesh",
//                         Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
//                         Ogre::SCENE_DYNAMIC);
//    auto *mSceneNode = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
//            ->createChildSceneNode(Ogre::SCENE_DYNAMIC);
//    if (nullptr == mSceneNode)
//        throw std::runtime_error("can't create new scene node");
//    mSceneNode->attachObject(item);

    Ogre::Light *light = sceneManager->createLight();
    Ogre::SceneNode *lightNode = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(
            Ogre::SCENE_DYNAMIC);
    if (nullptr == lightNode)
        throw std::runtime_error("can't create new light scene node");
    lightNode->attachObject(light);
    light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());

    //

    WindowEventListener gWindowEventListener;
    Ogre::WindowEventUtilities::addWindowEventListener(window, &gWindowEventListener);

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

#endif

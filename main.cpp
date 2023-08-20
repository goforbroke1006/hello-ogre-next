#include <iostream>

#include <Ogre.h>
#include <OgreWindow.h>
#include <Compositor/OgreCompositorManager2.h>
#include <OgreMeshManager2.h>
#include <OgreSharedPtr.h>
#include <OgreLight.h>
#include <OgreMesh.h>
#include <OgreMesh2.h>
#include <OgreItem.h>
#include <OgrePlatformInformation.h>

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
    void windowClosed(Ogre::Window *window) override {
        printf("Window closed!\n");
        g_ogre_window_should_quit = true;
    }
};

// V2 mesh loading
Ogre::Item *ogreLoadMesh(Ogre::SceneManager *const sceneManager, const std::string &filepath) {
//    Ogre::MeshPtr v2Mesh = Ogre::MeshManager::getSingleton().load(
//            name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//    Ogre::Item *item = sceneManager->createItem(
//            name, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_DYNAMIC);
//    return item;

    bool halfPosition = true;
    bool halfUVs = true;
    bool useQtangents = true;

    Ogre::v1::MeshPtr v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
            filepath, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
            Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);
    //Create a v2 mesh to import to, with a different name.
    Ogre::MeshPtr v2Mesh = Ogre::MeshManager::getSingleton().createByImportingV1(
            filepath + " Imported", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            v1Mesh.get(), halfPosition, halfUVs, useQtangents);
    v2Mesh->load();
    v1Mesh->unload();

    Ogre::Item *item = sceneManager->createItem(
            filepath + " Imported", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_DYNAMIC);
    return item;
}

Ogre::SceneNode *ogreNodeFromItem(Ogre::SceneManager *const sceneManager, Ogre::Item *meshHandle) {
//    Ogre::SceneNode *newSceneNode = nullptr;
//    Ogre::SceneNode *rootSceneNode = sceneManager->getRootSceneNode();
//    if (rootSceneNode) {
//        Ogre::SceneNode *sceneNode = ((Ogre::SceneNode *) rootSceneNode->createChild());
//        if (sceneNode) {
//            sceneNode->attachObject(meshHandle);
//            newSceneNode = sceneNode;
//        }
//    }
//    return newSceneNode;

    Ogre::SceneNode *sceneNode = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->
            createChildSceneNode(Ogre::SCENE_DYNAMIC);
    sceneNode->attachObject(meshHandle);
    sceneNode->scale(0.1f, 0.1f, 0.1f);
}

int entryPoint() {
    auto *gOgreRoot = OGRE_NEW Ogre::Root("plugins.cfg", "resources2.cfg", "ogre.cfg", "App Name");

    Ogre::RenderSystem *renderSystem = gOgreRoot->getRenderSystemByName("OpenGL 3+ Rendering Subsystem");
    if (!(renderSystem)) {
        printf("Render system not found!\n");
        return EXIT_FAILURE;
    }
    renderSystem->setConfigOption("Full Screen", "No");
    renderSystem->setConfigOption("Video Mode", "800 x 600");
    renderSystem->setConfigOption("sRGB Gamma Conversion", "Yes");
    gOgreRoot->setRenderSystem(renderSystem);

    auto *gOgreWindow = gOgreRoot->initialise(true, "Hello Ogre next");

    const size_t numThreads = std::max<uint32_t>(1, Ogre::PlatformInformation::getNumLogicalCores());
    auto *gSceneManager = gOgreRoot->createSceneManager(Ogre::ST_GENERIC, numThreads, Ogre::BLANKSTRING);

    Ogre::Camera *camera = gSceneManager->createCamera("Main Camera");
    camera->setPosition(Ogre::Vector3(100, 100, 100));
    camera->lookAt(Ogre::Vector3(0, 0, 0));
    camera->setNearClipDistance(0.2f);
    camera->setFarClipDistance(1000.0f);
    camera->setAutoAspectRatio(true);

    // without light we would just get a black screen
//    Ogre::Light *light = gSceneManager->createLight();
//    Ogre::SceneNode *lightNode = gSceneManager->getRootSceneNode()->createChildSceneNode();
//    lightNode->setPosition(0, 200, 0);
//    lightNode->attachObject(light);

//    Ogre::Light *light = gSceneManager->createLight();
//    Ogre::SceneNode *lightNode = gSceneManager->getRootSceneNode()->createChildSceneNode();
//    lightNode->attachObject(light);
//    light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
//    light->setType(Ogre::Light::LT_DIRECTIONAL);
//    light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());

    auto *compositorManager = gOgreRoot->getCompositorManager2();
    const Ogre::String workspaceName = "Main Workspace";
    const Ogre::ColourValue backgroundColour(0.0302f, 0.03f, 0.03f);
    compositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, Ogre::IdString());

    //

    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);

    //

    const std::string &barrelMeshPath = "Sinbad.mesh";

    bool isBarrelMeshExists = Ogre::ResourceGroupManager::getSingleton().resourceExists("General", barrelMeshPath);
    if (!isBarrelMeshExists) {
        std::cerr << "ERROR: resource not found " << barrelMeshPath << std::endl;
        return EXIT_FAILURE;
    }

    Ogre::Item *item = ogreLoadMesh(gSceneManager, barrelMeshPath);
    Ogre::SceneNode *pBarrelNode = ogreNodeFromItem(gSceneManager, item);

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

    OGRE_DELETE pBarrelNode;
    OGRE_DELETE item;

    Ogre::WindowEventUtilities::removeWindowEventListener(gOgreWindow, &g_myWindowEventListener);
    OGRE_DELETE (gOgreRoot);
    gOgreRoot = nullptr;

    return EXIT_SUCCESS;
}

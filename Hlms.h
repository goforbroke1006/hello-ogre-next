//
// Created by goforbroke on 12/25/23.
//

#ifndef HELLO_OGRE_NEXT_HLMS_H
#define HELLO_OGRE_NEXT_HLMS_H

#include <OgreConfigFile.h>
#include <OgreArchiveManager.h>
#include <OgreHlmsManager.h>
#include <OGRE/Hlms/Pbs/OgreHlmsPbs.h>
#include <OGRE/Hlms/Unlit/OgreHlmsUnlit.h>

template<class T>
T *CreateHlms(const Ogre::String &rootFolder) {
    using namespace Ogre;

    String mainFolderPath;
    StringVector libraryFoldersPaths;
    StringVector::const_iterator libraryFolderPathIt;
    StringVector::const_iterator libraryFolderPathEn;
    ArchiveManager &archiveManager = ArchiveManager::getSingleton();
    T::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
    ArchiveVec archiveLibraryFolders;
    Archive *archive = archiveManager.load(rootFolder + mainFolderPath,
                                           "FileSystem", true);
    libraryFolderPathIt = libraryFoldersPaths.begin();
    libraryFolderPathEn = libraryFoldersPaths.end();
    while (libraryFolderPathIt != libraryFolderPathEn) {
        Archive *archiveLibrary =
                archiveManager.load(rootFolder + *libraryFolderPathIt,
                                    "FileSystem", true);
        archiveLibraryFolders.push_back(archiveLibrary);
        ++libraryFolderPathIt;
    }
    T *hlms = OGRE_NEW T(archive, &archiveLibraryFolders);

    RenderSystem *renderSystem = Root::getSingletonPtr()->getRenderSystem();
    if (renderSystem->getName() == "Direct3D11 Rendering Subsystem") {
        bool supportsNoOverwriteOnTextureBuffers;
        renderSystem->getCustomAttribute("MapNoOverwriteOnDynamicBufferSRV",
                                         &supportsNoOverwriteOnTextureBuffers);
        if (!supportsNoOverwriteOnTextureBuffers)
            hlms->setTextureBufferDefaultSize(512 * 1024);
    }
    return hlms;
}

void registerHlms(void) {
    using namespace Ogre;

//    String resourcePath = "data/";
    String resourcePath = "";
    ConfigFile cf;
    cf.load("resources_d.cfg");
    String rootHlmsFolder =
            resourcePath + cf.getSetting("DoNotUseAsResource", "Hlms", "");
    if (rootHlmsFolder.empty())
        rootHlmsFolder = "./";
    else if (*(rootHlmsFolder.end() - 1) != '/')
        rootHlmsFolder += "/";

    auto mng = Root::getSingleton().getHlmsManager();
    mng->registerHlms(CreateHlms<HlmsUnlit>(rootHlmsFolder));
    mng->registerHlms(CreateHlms<HlmsPbs>(rootHlmsFolder));
}

#endif //HELLO_OGRE_NEXT_HLMS_H

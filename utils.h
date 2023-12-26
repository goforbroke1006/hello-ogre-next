//
// Created by goforbroke on 12/25/23.
//

#ifndef HELLO_OGRE_NEXT_UTILS_H
#define HELLO_OGRE_NEXT_UTILS_H

#include <string>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreMeshManager2.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgreMesh2.h>
#include <OGRE/OgreItem.h>

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

    return sceneNode;
}

#endif //HELLO_OGRE_NEXT_UTILS_H

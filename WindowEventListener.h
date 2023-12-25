//
// Created by goforbroke on 12/25/23.
//

#ifndef HELLO_OGRE_NEXT_WINDOWEVENTLISTENER_H
#define HELLO_OGRE_NEXT_WINDOWEVENTLISTENER_H

#include <OGRE/OgreWindowEventUtilities.h>

class WindowEventListener : public Ogre::WindowEventListener {
public:
    WindowEventListener() : mShouldQuit(false) {}

    void windowClosed(Ogre::Window *window) override {
        printf("Window closed!\n");
        mShouldQuit = true;
    }

    [[nodiscard]] bool shouldQuit() const {
        return mShouldQuit;
    }

private:
    bool mShouldQuit;
};

#endif //HELLO_OGRE_NEXT_WINDOWEVENTLISTENER_H

//
// Created by vincent on 19. 5. 20.
//

#ifndef GLFRONTEND_RESOURCEWRAPPER_H
#define GLFRONTEND_RESOURCEWRAPPER_H

namespace resources {

enum class State {
    NotLoaded,
    Loading,
    FailedToLoad,
    Loaded,
    Unloading,
    Unloaded
};

struct ResourceWrapper
{
    // TODO: think of what should be in the resource wrapper
    // 1. resource
    // 2. state

    // that said: std::lock_guard(mutex) on accessing the resource for a backgroundworker
    // the backgroundworker should have access to all resources, but I would like to lock resource states
    // OR: State is only changed by the backgroundworker in the first place.
    // Q: is volatile important in this one?
};

} // namespace resources

#endif //GLFRONTEND_RESOURCEWRAPPER_H

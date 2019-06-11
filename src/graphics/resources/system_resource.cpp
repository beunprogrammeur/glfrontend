//
// Created by vincent on 19. 4. 21.
//

#include "system_resource.h"

graphics::resources::SystemResource::SystemResource(const std::string &name, graphics::resources::Resource::Type type,
                                                    graphics::drawing::Theme *parent)
        : Resource(name, type, parent)
{

}

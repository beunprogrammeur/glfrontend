//
// Created by vincent on 19. 4. 19.
//

#include "dimensions.h"

namespace graphics {
namespace drawing {

Dimensions::Dimensions()
        : position(glm::vec2(kPositionDefault, kPositionDefault))
          , size(glm::vec2(kSizeDefault, kSizeDefault))
          , displacement(glm::vec2(kDisplacementDefault, kDisplacementDefault))
          , angle(kAngleDefault)
          , opacity(kOpacityDefault)
{

}


} // namespace drawing
} // namespace graphics

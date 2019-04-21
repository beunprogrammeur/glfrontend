//
// Created by vincent on 19. 4. 19.
//

#include "dimensions.h"

namespace graphics {
namespace drawing {

Dimensions::Dimensions()
        : position(glm::vec2(kPositionDefault, kPositionDefault))
          , size(glm::vec2(kSizeDefault, kSizeDefault))
          , margin(glm::vec2(kMarginDefault, kMarginDefault))
          , angle(kAngleDefault)
          , opacity(kOpacityDefault)
{

}


} // namespace drawing
} // namespace graphics

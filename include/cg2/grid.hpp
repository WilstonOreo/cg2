#ifndef GRID_H
#define GRID_H

#include <vector>
#include "SceneObject.hpp"

namespace cg2 {
    class Grid : public SceneObject {
        public:
            std::vector< std::vector<float> > values;
            float min;
            float dist;
            Grid();
    };
}
#endif // GRID_H

#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <includes/glm/glm.hpp>
#include <includes/glm/gtc/matrix_transform.hpp>
#include <includes/glm/gtc/type_ptr.hpp>

#include <vector>

class TerrainGen {
public:
    static std::vector<glm::vec3> getCoords() {
        std::vector<glm::vec3> cubePositions = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  0.0f, -15.0f),
            glm::vec3(-1.5f, 0.0f, -2.5f),
            glm::vec3(-3.8f, 0.0f, -12.3f),
            glm::vec3( 2.4f, 0.0f, -3.5f),
            glm::vec3(-1.7f,  0.0f, -7.5f),
            glm::vec3( 1.3f, 0.0f, -2.5f),
            glm::vec3( 1.5f,  0.0f, -2.5f),
            glm::vec3( 1.5f,  0.0f, -1.5f),
            glm::vec3(-1.3f,  0.0f, -1.5f)
        };
        return cubePositions;
    }
};

#endif
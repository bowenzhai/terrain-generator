#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <includes/glm/glm.hpp>
#include <includes/glm/gtc/matrix_transform.hpp>
#include <includes/glm/gtc/type_ptr.hpp>
#include <includes/PerlinNoise.hpp>

#include <vector>

class TerrainGen {
public:
    static std::vector<glm::vec4> getCoords(int width = 100, int seed = 0) {
        std::vector<glm::vec4> cubePositions;

        int start = (-1) * (width / 2);
        int end = width / 2;

		siv::PerlinNoise perlin;
        if (seed != 0) {
            perlin.reseed(seed);
        }

		const double fx = width / 4;
		const double fz = width / 4;

		for (int z = start; z < end; ++z) {
			for (int x = start; x < end; ++x) {
                cubePositions.emplace_back(glm::vec4( x,  0.0f,  z,  0));

                float f = perlin.octaveNoise0_1(x / fx, z / fz, 8);

                int height = (f * 10) / 2;
                
                for (int h = 1; h <= height; ++h) {
                    if (h = height) {
                        cubePositions.emplace_back(glm::vec4( x,  h,  z,  1));
                    } else {
                        cubePositions.emplace_back(glm::vec4( x,  h,  z,  0));
                    }
                }
			}
		}
        return cubePositions;
    }
};

#endif
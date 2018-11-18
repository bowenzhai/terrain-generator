#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <includes/glm/glm.hpp>
#include <includes/glm/gtc/matrix_transform.hpp>
#include <includes/glm/gtc/type_ptr.hpp>
#include <includes/PerlinNoise.hpp>

#include <vector>

// class to generate coordinates
class Terrain {
private:
    std::vector<glm::vec4> coords;
    int width;
    int seed;
public:
    Terrain(int width = 100, int seed = 0):width(width), seed(seed) {}

    // generate vectors of world space coords.
    // @param width the width of the square of coords to generate
    // @param seed the random seed
    std::vector<glm::vec4> genCoords(glm::vec3 worldPos) {
        coords.clear();

        int xStart = (-1) * (width / 2) + worldPos.x;
        int xEnd = width / 2 + worldPos.x;
        int zStart = (-1) * (width / 2) + worldPos.z;
        int zEnd = width / 2 + worldPos.z;

		siv::PerlinNoise perlin;
        if (seed != 0) {
            perlin.reseed(seed);
        }

		const double fx = width / 4;
		const double fz = width / 4;

		for (int z = zStart; z < zEnd; ++z) {
			for (int x = xStart; x < xEnd; ++x) {
                coords.emplace_back(glm::vec4( x,  0.0f,  z,  0));
                float f = perlin.octaveNoise0_1(x / fx, z / fz, 8);
                int height = f * 5;
                
                for (int h = 1; h <= height; ++h) {
                    if (h = height) {
                        coords.emplace_back(glm::vec4( x,  h,  z,  1));
                    } else {
                        coords.emplace_back(glm::vec4( x,  h,  z,  0));
                    }
                }
			}
		}
        return coords;
    }
};

#endif
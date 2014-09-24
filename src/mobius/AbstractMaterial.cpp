#include "AbstractMaterial.hpp"

AbstractMaterial::AbstractMaterial() : ambient(0.2f, 0.2f, 0.2f), diffuse(0.8f, 0.8f, 0.8f), specular(1.0f, 1.0f, 1.0f), emissive(0.0f, 0.0f, 0.0f), alpha(1.0f), shininess(0.0f), texture("") {
}
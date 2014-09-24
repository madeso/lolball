#include "CommonLightAttributes.hpp"

CommonLightAttributes::CommonLightAttributes() : ambient(1.0f,1.0f,1.0f), diffuse(1.0f,1.0f,1.0f), specular(1.0f,1.0f,1.0f) {
}

Attenuation::Attenuation() : constant(1.0f), linear(0.0f), quadratic(0.0f) {
}
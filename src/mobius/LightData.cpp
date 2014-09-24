#include "PointLightdata.hpp"
#include "DirectionalLightData.hpp"
#include "SpotLightData.hpp"

PointLightData::PointLightData() : position(0,0,0) {
}

DirectionalLightData::DirectionalLightData() : direction(0,0,-1) {
}

SpotLightData::SpotLightData() : position(0,0,0), direction(0,0,-1), exponent(0.0f), cutoff(90.0f) {
}
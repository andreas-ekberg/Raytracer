#ifndef MATERIAL_CPP
#define MATERIAL_CPP

#include "headers/material.hpp"

Material::Material() {};
Material::Material(MaterialType type) : materialType(type) {};

Material Material::getMaterial()
{
    return materialType;
}

#endif

#ifndef MATERIAL_CPP
#define MATERIAL_CPP

#include "headers/material.hpp"

Material::Material() {};
Material::Material(MaterialType type) : materialType(type) {};

Material Material::getMaterial()
{
    return materialType;
}

glm::dvec3 Material::getMaterialColor()
{
    return color;
}
void Material::setMaterialColor(glm::dvec3 _color)
{
    color = _color;
}

#endif

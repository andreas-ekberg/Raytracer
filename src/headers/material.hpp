#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../glmlib/glm.hpp"
#include "../glmlib/gtx/string_cast.hpp"

class Material
{
public:
    enum MaterialType
    {
        Mirror,
        Lambertian,
        Light
    };

    Material();
    Material(MaterialType type);

    Material getMaterial();
    glm::dvec3 getMaterialColor();
    void setMaterialColor(glm::dvec3 color);

    MaterialType materialType;

private:
    glm::dvec3 color;
};

#endif
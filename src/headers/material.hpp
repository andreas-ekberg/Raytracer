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
        Lambertian
    };

    Material();
    Material(MaterialType type);

    Material getMaterial();

    MaterialType materialType;
};

#endif
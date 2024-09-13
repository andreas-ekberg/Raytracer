#include "polygon.hpp"
#include "ray.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Triangle : public Polygon {
public:
    // Constructor
    Triangle() {}
    Triangle(glm::dvec3 point1, glm::dvec3 point2, glm::dvec3 point3, glm::dvec3 _color) {
        p[0] = point1;
        p[1] = point2;
        p[2] = point3;
        color = _color;
    }


    // Destructor
    ~Triangle() {}

    bool isHit(Ray &r, float &t) {
        // First time t is null
        if (t) {
            
        };


        return true; 
    }
private:   
    glm::dvec3 p[3];
    glm::dvec3 color;

};
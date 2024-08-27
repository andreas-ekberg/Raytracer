#include "polygon.hpp"
#include "ray.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Triangle : public Polygon {
public:
    // Constructor
    Triangle() {}
    Triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 _color) {
        p[0] = point1;
        p[1] = point2;
        p[2] = point3;
        color = _color;
    }


    // Destructor
    ~Triangle() {}

    bool isHit(ray &r, float &t) {
        // First time t is null
        if (t) {
            
        };


        return true; 
    }
private:   
    glm::vec3 p[3];
    glm::vec3 color;

};
#include "polygon.hpp"
#include "ray.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Rectangle : public Polygon {
public:
    // Constructor
    Rectangle() {}
    Rectangle(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 _color) {
        p[0] = topLeft;
        p[1] = topRight;
        p[2] = bottomLeft;
        p[3] = bottomRight;
        color = _color;
        normal = glm::cross(p[1] - p[0], p[2] - p[0]);
    }


    // Destructor
    ~Rectangle() {}

    bool isHit(Ray &r, float &t) {
        // First time t is null
        if (glm::dot(r.direction(), normal) < 0){
            return false;
        };
        for (int i = 0; i < 20; i++) {
            glm::vec3 pos = r.position((float)i);
            if (pos.x >=p[0].x && pos.x <= p[1].x && pos.z <= p[0].z && pos.z >= p[2].z && pos.y - p[0].y < 1) {
                t = i;
                return true;
            }
        }
        return false; 
    }
private:   
    glm::vec3 p[4];
    glm::vec3 color;

};
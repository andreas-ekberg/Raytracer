#include "polygon.hpp"
#include "ray.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

class Rectangle : public Polygon
{
public:
    // Constructor
    Rectangle() {}
    Rectangle(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 _color)
    {
        p[0] = topLeft;
        p[1] = topRight;
        p[2] = bottomLeft;
        p[3] = bottomRight;
        color = _color;
        normal = glm::cross(p[3] - p[2], p[0] - p[2]);
    }

    // Destructor
    ~Rectangle() {}

    glm::dvec3 isHit(Ray ray)
    {
        glm::dvec3 v = p[2];
        glm::dvec3 s = ray.rayOrigin;
        glm::dvec3 direction = ray.rayDirection;
        double t = glm::dot((v - s), normal) / glm::dot(direction, normal);
        // std::cout << t << "we go hard";
        if (t < DBL_EPSILON)
        {
            return glm::dvec3(NAN, NAN, NAN);
        }
        glm::dvec3 xi = ray.position(t);
        glm::dvec3 c1 = p[3] - v;
        glm::dvec3 c2 = p[0] - v;

        double a = glm::dot((xi - v), c1) / glm::dot(c1, c1);
        double b = glm::dot((xi - v), c2) / glm::dot(c2, c2);

        if (a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0)
        {
            // std::cout << "Inside the crzy if";
            return xi;
        }
        else
        {
            // std::cout << "Inside the last nan";
            return glm::dvec3(NAN, NAN, NAN);
        }

        /* // First time t is null
        if (glm::dot(r.direction(), normal) < 0)
        {
            return false;
        };
        for (int i = 0; i < 20; i++)
        {
            glm::dvec3 pos = r.position((float)i);
            if (pos.x >= p[0].x && pos.x <= p[1].x && pos.z <= p[0].z && pos.z >= p[2].z && pos.y - p[0].y < 1)
            {
                t = i;
                return true;
            }
        }
        return false; */
    }

private:
    glm::dvec3 p[4];
    glm::dvec3 color;
};
#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <iostream>

class vec3
{
public:
    double elements[3];

    vec3() : elements{0, 0, 0} {}

    vec3(double e1, double e2, double e3)
    {
        elements[0] = e1;
        elements[1] = e2;
        elements[2] = e3;
    };

    double x() const
    {
        return elements[0];
    }

    double y() const
    {
        return elements[1];
    }

    double z() const
    {
        return elements[2];
    }

    vec3 operator-() const
    {
        return vec3(-elements[0], -elements[1], -elements[2]);
    }
    double operator[](int i) const
    {
        return elements[i];
    }

    double &operator[](int i)
    {
        return elements[i];
    }

    vec3 &operator+=(const vec3 &v)
    {
        elements[0] += v.elements[0];
        elements[1] += v.elements[1];
        elements[2] += v.elements[2];
        return *this;
    }
    vec3 &operator*=(const double scalar)
    {
        elements[0] *= scalar;
        elements[1] *= scalar;
        elements[2] *= scalar;
    }
    vec3 &operator/=(const double scalar)
    {
        // Smart move, använd den över för att skriva denna
        return *this *= 1 / scalar;
    }
    double length() const
    {
        double length2 = elements[0] * elements[0] + elements[1] * elements[1] + elements[2] * elements[2];
        return std::sqrt(length2);
    }
};
using point3 = vec3;

std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.elements[0] << " " << v.elements[1] << " " << v.elements[2];
}

vec3 operator+(const vec3 &vector1, const vec3 &vector2)
{
    return vec3(vector1.elements[0] + vector2.elements[0], vector1.elements[1] + vector2.elements[1], vector1.elements[2] + vector2.elements[2]);
}
vec3 operator-(const vec3 &vector1, const vec3 &vector2)
{
    return vec3(vector1.elements[0] - vector2.elements[0], vector1.elements[1] - vector2.elements[1], vector1.elements[2] - vector2.elements[2]);
}

vec3 operator*(const vec3 &vector1, const vec3 &vector2)
{
    return vec3(vector1.elements[0] * vector2.elements[0], vector1.elements[1] * vector2.elements[1], vector1.elements[2] * vector2.elements[2]);
}

vec3 operator*(double s, const vec3 &vector)
{
    return vec3(vector.elements[0] * s, vector.elements[1] * s, vector.elements[2] * s);
}

vec3 operator*(const vec3 &vector, double s)
{
    return s * vector;
}
vec3 operator/(const vec3 &vector, double s)
{
    return (1 / s) * vector;
}

double dot(const vec3 &vector1, const vec3 &vector2)
{
    return vector1.elements[0] * vector2.elements[0] + vector1.elements[1] * vector2.elements[1] + vector1.elements[2] * vector2.elements[2];
}

vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.elements[1] * v.elements[2] - u.elements[2] * v.elements[1],
                u.elements[2] * v.elements[0] - u.elements[0] * v.elements[2],
                u.elements[0] * v.elements[1] - u.elements[1] * v.elements[0]);
}
vec3 unit_vector(const vec3 &v)
{
    return v / v.length();
}
#endif

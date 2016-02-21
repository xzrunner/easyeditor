#ifndef _EASY3D_PARAMETRIC_EQUATIONS_H_
#define _EASY3D_PARAMETRIC_EQUATIONS_H_

#include "ParametricSurface.h"

#include <ee/Math2D.h>

namespace e3d 
{

class Cone : public ParametricSurface 
{
public:
    Cone(float height, float radius) : m_height(height), m_radius(radius)
    {
        ParametricInterval interval = { ee::ivec2(20, 20), ee::vec2(ee::TWO_PI, 1), ee::vec2(30, 20) };
        SetInterval(interval);
    }
    ee::vec3 Evaluate(const ee::vec2& domain) const
    {
        float u = domain.x, v = domain.y;
        float x = m_radius * (1 - v) * cos(u);
        float y = m_height * (v - 0.5f);
        float z = m_radius * (1 - v) * -sin(u);
        return ee::vec3(x, y, z);
    }
private:
    float m_height;
    float m_radius;
};

class Sphere : public ParametricSurface {
public:
    Sphere(float radius) : m_radius(radius)
    {
        ParametricInterval interval = { ee::ivec2(20, 20), ee::vec2(ee::PI, ee::TWO_PI), ee::vec2(20, 35) };
        SetInterval(interval);
    }
    ee::vec3 Evaluate(const ee::vec2& domain) const
    {
        float u = domain.x, v = domain.y;
        float x = m_radius * sin(u) * cos(v);
        float y = m_radius * cos(u);
        float z = m_radius * -sin(u) * sin(v);
        return ee::vec3(x, y, z);
    }
private:
    float m_radius;
};

class Torus : public ParametricSurface {
public:
    Torus(float majorRadius, float minorRadius) :
        m_majorRadius(majorRadius),
        m_minorRadius(minorRadius)
    {
        ParametricInterval interval = { ee::ivec2(20, 20), ee::vec2(ee::TWO_PI, ee::TWO_PI), ee::vec2(40, 10) };
        SetInterval(interval);
    }
    ee::vec3 Evaluate(const ee::vec2& domain) const
    {
        const float major = m_majorRadius;
        const float minor = m_minorRadius;
        float u = domain.x, v = domain.y;
        float x = (major + minor * cos(v)) * cos(u);
        float y = (major + minor * cos(v)) * sin(u);
        float z = minor * sin(v);
        return ee::vec3(x, y, z);
    }
private:
    float m_majorRadius;
    float m_minorRadius;
};

class TrefoilKnot : public ParametricSurface {
public:
    TrefoilKnot(float scale) : m_scale(scale)
    {
        ParametricInterval interval = { ee::ivec2(60, 15), ee::vec2(ee::TWO_PI, ee::TWO_PI), ee::vec2(100, 8) };
        SetInterval(interval);
    }
    ee::vec3 Evaluate(const ee::vec2& domain) const
    {
        const float a = 0.5f;
        const float b = 0.3f;
        const float c = 0.5f;
        const float d = 0.1f;
        float u = (ee::TWO_PI - domain.x) * 2;
        float v = domain.y;
        
        float r = a + b * cos(1.5f * u);
        float x = r * cos(u);
        float y = r * sin(u);
        float z = c * sin(1.5f * u);
        
        ee::vec3 dv;
        dv.x = -1.5f * b * sin(1.5f * u) * cos(u) -
               (a + b * cos(1.5f * u)) * sin(u);
        dv.y = -1.5f * b * sin(1.5f * u) * sin(u) +
               (a + b * cos(1.5f * u)) * cos(u);
        dv.z = 1.5f * c * cos(1.5f * u);
        
        ee::vec3 q = dv.Normalized();
        ee::vec3 qvn = ee::vec3(q.y, -q.x, 0).Normalized();
        ee::vec3 ww = q.Cross(qvn);
        
        ee::vec3 range;
        range.x = x + d * (qvn.x * cos(v) + ww.x * sin(v));
        range.y = y + d * (qvn.y * cos(v) + ww.y * sin(v));
        range.z = z + d * ww.z * sin(v);
        return range * m_scale;
    }
private:
    float m_scale;
};

class MobiusStrip : public ParametricSurface {
public:
    MobiusStrip(float scale) : m_scale(scale)
    {
        ParametricInterval interval = { ee::ivec2(40, 20), ee::vec2(ee::TWO_PI, ee::TWO_PI), ee::vec2(40, 15) };
        SetInterval(interval);
    }
    ee::vec3 Evaluate(const ee::vec2& domain) const
    {
        float u = domain.x;
        float t = domain.y;
        float major = 1.25;
        float a = 0.125f;
        float b = 0.5f;
        float phi = u / 2;
        
        // General equation for an ellipse where phi is the angle
        // between the major axis and the X axis.
        float x = a * cos(t) * cos(phi) - b * sin(t) * sin(phi);
        float y = a * cos(t) * sin(phi) + b * sin(t) * cos(phi);

        // Sweep the ellipse along a circle, like a torus.
        ee::vec3 range;
        range.x = (major + x) * cos(u);
        range.y = (major + x) * sin(u);
        range.z = y;
        return range * m_scale;
    }
private:
    float m_scale;
};

class KleinBottle : public ParametricSurface {
public:
    KleinBottle(float scale) : m_scale(scale)
    {
        ParametricInterval interval = { ee::ivec2(20, 20), ee::vec2(ee::TWO_PI, ee::TWO_PI), ee::vec2(15, 50) };
        SetInterval(interval);
    }
    ee::vec3 Evaluate(const ee::vec2& domain) const
    {
        float v = 1 - domain.x;
        float u = domain.y;
        
        float x0 = 3 * cos(u) * (1 + sin(u)) +
                   (2 * (1 - cos(u) / 2)) * cos(u) * cos(v);
        
        float y0  = 8 * sin(u) + (2 * (1 - cos(u) / 2)) * sin(u) * cos(v);
        
        float x1 = 3 * cos(u) * (1 + sin(u)) +
                   (2 * (1 - cos(u) / 2)) * cos(v + ee::PI);
        
        float y1 = 8 * sin(u);
        
        ee::vec3 range;
        range.x = u < ee::PI ? x0 : x1;
        range.y = u < ee::PI ? -y0 : -y1;
        range.z = (-2 * (1 - cos(u) / 2)) * sin(v);
        return range * m_scale;
    }
    bool InvertNormal(const ee::vec2& domain) const
    {
        return domain.y > 3 * ee::PI / 2;
    }
private:
    float m_scale;
};

}

#endif // _EASY3D_PARAMETRIC_EQUATIONS_H_
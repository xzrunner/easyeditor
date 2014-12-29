#ifndef _RT_GEOMETRIC_OBJECT_H_
#define _RT_GEOMETRIC_OBJECT_H_

#include "maths/Point3D.h"
#include "maths/AABB.h"
#include "utilities/Object.h"

#include <stdio.h>

namespace rt
{

class Ray;
class ShadeRec;
class Material;

class GeometricObject : public Object
{
public:
	GeometricObject();
	GeometricObject(const GeometricObject& obj);
	virtual ~GeometricObject();

	virtual GeometricObject* Clone() const;

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const = 0;	
	virtual bool ShadowHit(const Ray& ray, float& t) const = 0;

	// The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres
	virtual Point3D Sample() const;
	virtual float Pdf(const ShadeRec& sr) const;

	// The following two functions allow us to simplify the code for smooth shaded triangle meshes
	virtual Normal GetNormal(void) const; 
	virtual Normal GetNormal(const Point3D& p) const;

	virtual AABB GetBoundingBox() const;

	// required for compound objects
	virtual void AddObject(GeometricObject* obj) {}

	const Material* GetMaterial() const;
	virtual void SetMaterial(const Material* m) const;

private:
	mutable const Material* m_material;

}; // GeometricObject

inline const Material* GeometricObject::
GetMaterial() const { 
	return m_material; 
}

}

#endif // _RT_GEOMETRIC_OBJECT_H_
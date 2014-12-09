#ifndef _RT_GEOMETRIC_OBJECT_H_
#define _RT_GEOMETRIC_OBJECT_H_

#include <stdio.h>

namespace rt
{

class Ray;
class ShadeRec;
class Material;

class GeometricObject
{
public:
	GeometricObject();
	virtual ~GeometricObject();

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const = 0;	

	const Material* GetMaterial() const;
	void SetMaterial(const Material* m);

private:
	const Material* m_material;

}; // GeometricObject

inline const Material* GeometricObject::
GetMaterial() const { 
	return m_material; 
}

}

#endif // _RT_GEOMETRIC_OBJECT_H_
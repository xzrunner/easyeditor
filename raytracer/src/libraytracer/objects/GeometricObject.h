#ifndef _RT_GEOMETRIC_OBJECT_H_
#define _RT_GEOMETRIC_OBJECT_H_

namespace rt
{

class Ray;
class ShadeRec;
class Material;

class GeometricObject
{
public:
	GeometricObject();
	virtual ~GeometricObject() {}

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const = 0;	

	const Material* GetMaterial() const { return m_material; }

private:
	Material* m_material;

}; // GeometricObject

}

#endif // _RT_GEOMETRIC_OBJECT_H_
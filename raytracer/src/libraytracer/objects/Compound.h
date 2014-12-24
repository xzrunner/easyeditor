#ifndef _RT_COMPOUND_H_
#define _RT_COMPOUND_H_

#include "GeometricObject.h"

#include <vector>

namespace rt
{

class Compound : public GeometricObject
{
public:
	virtual ~Compound();
	
	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	virtual void AddObject(GeometricObject* obj);

protected:
	void ClearObjects();

protected:
	std::vector<GeometricObject*> m_objects;

}; // Compound

}

#endif // _RT_COMPOUND_H_
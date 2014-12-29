#ifndef _RT_INSTANCE_H_
#define _RT_INSTANCE_H_

#include "GeometricObject.h"

#include "maths/Matrix.h"

namespace rt
{

class Instance : public GeometricObject
{
public:
	Instance();
	Instance(GeometricObject* obj);
	virtual ~Instance();

	virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const;	
	virtual bool ShadowHit(const Ray& ray, float& t) const;

	virtual AABB GetBoundingBox() const;

	void ComputeBoundingBox();

	void Translate(const Vector3D& trans);
	void Scale(const Vector3D& scale);
	void RotateX(double r);
	void RotateY(double r);
	void RotateZ(double r);
	void Shear(const Matrix& m);

private:
	GeometricObject*	m_object;					// object to be transformed
	Matrix				m_inv_matrix;		    	// inverse transformation matrix
	static 	Matrix		m_forward_matrix; 			// transformation matrix
	AABB				m_aabb;						// transformed object's bounding box
	bool				m_transform_the_texture;	// do we transform the texture?

}; // Instance

}

#endif // _RT_INSTANCE_H_
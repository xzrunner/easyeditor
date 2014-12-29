#include "Instance.h"

#include "maths/Ray.h"
#include "utilities/Constants.h"
#include "utilities/ShadeRec.h"

#include <float.h>

namespace rt
{

Matrix Instance::m_forward_matrix;

Instance::Instance()
	: m_object(NULL)
	, m_inv_matrix()
	, m_aabb()
	, m_transform_the_texture(true)
{
	m_forward_matrix.SetIdentity();
}

Instance::Instance(GeometricObject* obj)
	: m_inv_matrix()
	, m_aabb()
	, m_transform_the_texture(true)
{
	obj->Retain();
	m_object = obj;

	m_forward_matrix.SetIdentity();
}

Instance::~Instance()
{
	if (m_object) {
		m_object->Release();
	}
}

bool Instance::Hit(const Ray& ray, double& t, ShadeRec& sr) const
{
	Ray inv_ray(ray);  
	inv_ray.ori = m_inv_matrix * inv_ray.ori;   
	inv_ray.dir = m_inv_matrix * inv_ray.dir;

	if (m_object->Hit(inv_ray, t, sr)) {
		sr.normal = m_inv_matrix * sr.normal;
		sr.normal.Normalize();				

		if (m_object->GetMaterial()) {
			SetMaterial(m_object->GetMaterial());
		}
		if (!m_transform_the_texture) 
			sr.local_hit_point = ray.ori + t * ray.dir;  		 

		return true;
	} 

	return false;   
}

bool Instance::ShadowHit(const Ray& ray, float& tmin) const
{
	Ray inv_ray(ray);  
	inv_ray.ori = m_inv_matrix * inv_ray.ori;   
	inv_ray.dir = m_inv_matrix * inv_ray.dir;

	if (m_object->ShadowHit(inv_ray, tmin)) {
		if (m_object->GetMaterial()) {
			SetMaterial(m_object->GetMaterial());
		}

		return true;
	} 

	return false;  
}

AABB Instance::GetBoundingBox() const
{
	return m_aabb;
}

void Instance::ComputeBoundingBox()
{
	// First get the object's untransformed AABB

	AABB object_bbox = m_object->GetBoundingBox();


	// Now apply the affine transformations to the box.
	// We must apply the transformations to all 8 vertices of the original box
	// and then work out the new minimum and maximum values

	// Construct the eight vertices as 3D points:

	Point3D v[8];

	v[0].x = object_bbox.x0; v[0].y = object_bbox.y0; v[0].z = object_bbox.z0;
	v[1].x = object_bbox.x1; v[1].y = object_bbox.y0; v[1].z = object_bbox.z0;
	v[2].x = object_bbox.x1; v[2].y = object_bbox.y1; v[2].z = object_bbox.z0;
	v[3].x = object_bbox.x0; v[3].y = object_bbox.y1; v[3].z = object_bbox.z0;

	v[4].x = object_bbox.x0; v[4].y = object_bbox.y0; v[4].z = object_bbox.z1;
	v[5].x = object_bbox.x1; v[5].y = object_bbox.y0; v[5].z = object_bbox.z1;
	v[6].x = object_bbox.x1; v[6].y = object_bbox.y1; v[6].z = object_bbox.z1;
	v[7].x = object_bbox.x0; v[7].y = object_bbox.y1; v[7].z = object_bbox.z1;


	// Transform these using the forward matrix

	v[0] = m_forward_matrix * v[0];
	v[1] = m_forward_matrix * v[1];
	v[2] = m_forward_matrix * v[2];
	v[3] = m_forward_matrix * v[3];
	v[4] = m_forward_matrix * v[4];
	v[5] = m_forward_matrix * v[5];
	v[6] = m_forward_matrix * v[6];
	v[7] = m_forward_matrix * v[7];


	// Since forward_matrix is a static variable, we must now set it to the unit matrix
	// This sets it up correctly for the next instance object

	m_forward_matrix.SetIdentity();


	// Compute the minimum values

	float x0 = FLT_MAX;
	float y0 = FLT_MAX;
	float z0 = FLT_MAX;

	for (int j = 0; j <= 7; j++)  {
		if (v[j].x < x0)
			x0 = v[j].x;
	}

	for (int j = 0; j <= 7; j++) {
		if (v[j].y < y0)
			y0 = v[j].y;
	}

	for (int j = 0; j <= 7; j++) {
		if (v[j].z < z0)
			z0 = v[j].z;
	}

	// Compute the minimum values

	float x1 = -FLT_MAX;
	float y1 = -FLT_MAX;
	float z1 = -FLT_MAX;   

	for (int j = 0; j <= 7; j++) {
		if (v[j].x > x1)
			x1 = v[j].x;
	}

	for (int j = 0; j <= 7; j++) {
		if (v[j].y > y1)
			y1 = v[j].y;
	}

	for (int j = 0; j <= 7; j++) {
		if (v[j].z > z1)
			z1 = v[j].z;
	}

	// Assign values to the bounding box

	m_aabb.x0 = x0;
	m_aabb.y0 = y0;
	m_aabb.z0 = z0;
	m_aabb.x1 = x1;
	m_aabb.y1 = y1;
	m_aabb.z1 = z1;	
}

void Instance::Translate(const Vector3D& trans)
{
	Matrix inv_translation_matrix;				// temporary inverse translation matrix	

	inv_translation_matrix.m[0][3] = -trans.x;
	inv_translation_matrix.m[1][3] = -trans.y;
	inv_translation_matrix.m[2][3] = -trans.z;

	m_inv_matrix = m_inv_matrix * inv_translation_matrix;

	Matrix translation_matrix;					// temporary translation matrix	

	translation_matrix.m[0][3] = trans.x;
	translation_matrix.m[1][3] = trans.y;
	translation_matrix.m[2][3] = trans.z;

	m_forward_matrix = translation_matrix * m_forward_matrix; 
}

void Instance::Scale(const Vector3D& s)
{
	Matrix	inv_scaling_matrix;			// temporary inverse scaling matrix

	inv_scaling_matrix.m[0][0] = 1.0 / s.x;
	inv_scaling_matrix.m[1][1] = 1.0 / s.y;
	inv_scaling_matrix.m[2][2] = 1.0 / s.z;

	m_inv_matrix = m_inv_matrix * inv_scaling_matrix;			

	Matrix	scaling_matrix;				// temporary scaling matrix

	scaling_matrix.m[0][0] = s.x;
	scaling_matrix.m[1][1] = s.y;
	scaling_matrix.m[2][2] = s.z;

	m_forward_matrix = scaling_matrix * m_forward_matrix; 		
}

void Instance::RotateX(double theta)
{
	double sin_theta = sin(theta * PI_ON_180);
	double cos_theta = cos(theta * PI_ON_180);

	Matrix inv_x_rotation_matrix;					// temporary inverse rotation matrix about x axis

	inv_x_rotation_matrix.m[1][1] = cos_theta;
	inv_x_rotation_matrix.m[1][2] = sin_theta;   	
	inv_x_rotation_matrix.m[2][1] = -sin_theta;  
	inv_x_rotation_matrix.m[2][2] = cos_theta;		

	m_inv_matrix = m_inv_matrix * inv_x_rotation_matrix;	   	

	Matrix x_rotation_matrix;						// temporary rotation matrix about x axis

	x_rotation_matrix.m[1][1] = cos_theta;
	x_rotation_matrix.m[1][2] = -sin_theta;
	x_rotation_matrix.m[2][1] = sin_theta;
	x_rotation_matrix.m[2][2] = cos_theta;

	m_forward_matrix = x_rotation_matrix * m_forward_matrix; 
}

void Instance::RotateY(double theta)
{
	double sin_theta = sin(theta * PI / 180.0);
	double cos_theta = cos(theta * PI / 180.0);

	Matrix inv_y_rotation_matrix;					// temporary inverse rotation matrix about y axis

	inv_y_rotation_matrix.m[0][0] = cos_theta;   
	inv_y_rotation_matrix.m[0][2] = -sin_theta;  
	inv_y_rotation_matrix.m[2][0] = sin_theta;
	inv_y_rotation_matrix.m[2][2] = cos_theta;		

	m_inv_matrix = m_inv_matrix * inv_y_rotation_matrix;	   	

	Matrix y_rotation_matrix;						// temporary rotation matrix about x axis

	y_rotation_matrix.m[0][0] = cos_theta;
	y_rotation_matrix.m[0][2] = sin_theta;
	y_rotation_matrix.m[2][0] = -sin_theta;
	y_rotation_matrix.m[2][2] = cos_theta;

	m_forward_matrix = y_rotation_matrix * m_forward_matrix; 
}

void Instance::RotateZ(double theta)
{
	double sin_theta = sin(theta * PI / 180.0);
	double cos_theta = cos(theta * PI / 180.0);

	Matrix inv_z_rotation_matrix;					// temporary inverse rotation matrix about y axis	

	inv_z_rotation_matrix.m[0][0] = cos_theta;
	inv_z_rotation_matrix.m[0][1] = sin_theta;   	
	inv_z_rotation_matrix.m[1][0] = -sin_theta;  
	inv_z_rotation_matrix.m[1][1] = cos_theta;	

	m_inv_matrix = m_inv_matrix * inv_z_rotation_matrix;

	Matrix z_rotation_matrix;						// temporary rotation matrix about y axis

	z_rotation_matrix.m[0][0] = cos_theta;
	z_rotation_matrix.m[0][1] = -sin_theta;
	z_rotation_matrix.m[1][0] = sin_theta;
	z_rotation_matrix.m[1][1] = cos_theta;

	m_forward_matrix = z_rotation_matrix * m_forward_matrix; 
}

void Instance::Shear(const Matrix& s)
{
	Matrix inverse_shearing_matrix;    // inverse shear matrix

	// discriminant

	double d = 1.0 	- s.m[1][0] * s.m[0][1] - s.m[2][0] * s.m[0][2]  - s.m[2][1] * s.m[1][2]
	+ s.m[1][0] * s.m[2][1] * s.m[0][2] + s.m[2][0] * s.m[0][1] * s.m[2][1];

	// diagonals

	inverse_shearing_matrix.m[0][0] = 1.0 - s.m[2][1] * s.m[1][2];
	inverse_shearing_matrix.m[1][1] = 1.0 - s.m[2][0] * s.m[0][2];
	inverse_shearing_matrix.m[2][2] = 1.0 - s.m[1][0] * s.m[0][1];
	inverse_shearing_matrix.m[3][3] = d;

	// first row

	inverse_shearing_matrix.m[0][1] = -s.m[1][0] + s.m[2][0] * s.m[1][2];
	inverse_shearing_matrix.m[0][2] = -s.m[2][0] + s.m[1][0] * s.m[2][1];

	// second row

	inverse_shearing_matrix.m[1][0] = -s.m[0][1] + s.m[2][1] * s.m[0][2];
	inverse_shearing_matrix.m[1][2] = -s.m[2][1] + s.m[2][0] * s.m[0][1];

	// third row

	inverse_shearing_matrix.m[2][0] = -s.m[0][2] + s.m[0][1] * s.m[1][2];
	inverse_shearing_matrix.m[2][1] = -s.m[1][2] + s.m[1][0] * s.m[0][2] ;

	// divide by discriminant

	inverse_shearing_matrix = inverse_shearing_matrix / d;

	m_inv_matrix = m_inv_matrix * inverse_shearing_matrix;	

	m_forward_matrix = s * m_forward_matrix; 
}

}
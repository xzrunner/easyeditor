#ifndef _RT_SHADER_REC_H_
#define _RT_SHADER_REC_H_

#include "maths/Point3D.h"
#include "maths/Ray.h"
#include "maths/Normal.h"
#include "utilities/RGBColor.h"

namespace rt
{

class World;
class Material;

class ShadeRec 
{
public:
	bool				hit_an_object;		// Did the ray hit an object?
	const Material* 	material;			// Pointer to the nearest object's material
	Point3D 			hit_point;			// World coordinates of intersection
	Point3D				local_hit_point;	// World coordinates of hit point on generic object (used for texture transformations)
	Normal				normal;				// Normal at hit point
	Ray					ray;				// Required for specular highlights and area lights
	int					depth;				// recursion depth
	Vector3D			dir;				// for area lights
	double				t;					// ray parameter
	const World&		w;					// World reference
	RGBColor			color;

public:
	ShadeRec(const World& world)
		: hit_an_object(false), material(0), depth(0), t(0), w(world) {}

}; // ShadeRec

}

#endif // _RT_SHADER_REC_H_
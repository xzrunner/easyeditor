#ifndef _RT_WORLD_H_
#define _RT_WORLD_H_

#include "ViewPlane.h"

#include "utilities/ShadeRec.h"

#include <vector>

namespace rt
{

class GeometricObject;
class Light;
class Tracer;
class Ray;
class Camera;
class RenderOutput;

class World
{
public:
	World();
	~World();

	void Build();

	void RenderScene();

	ShadeRec HitObjects(const Ray& ray) const;

	const RGBColor& GetBackgroundColor() const { return m_background_color; }

	const std::vector<Light*>& GetLights() const { return m_lights; }

	const Light* GetAmbient() const { return m_ambient; }

	const Camera* GetCamera() const { return m_camera; }

	const ViewPlane& GetViewPlane() const { return m_vp; }

	Tracer* GetTracer() const { return m_tracer; }

	void SetRenderOutput(RenderOutput* output);

	void DisplayPixel(const int row, const int column, const RGBColor& pixel_color) const;

private:
	void AddObject(GeometricObject* obj);

	void AddLight(Light* light);

	void SetTracer(Tracer* tracer);

	void SetCamera(Camera* camera);

private:
	ViewPlane m_vp;

	RGBColor m_background_color;

	std::vector<GeometricObject*> m_objects;

	std::vector<Light*> m_lights;

	Light* m_ambient;

	Tracer* m_tracer;

	Camera* m_camera;

	RenderOutput* m_output;

}; // World

}

#endif // _RT_WORLD_H_
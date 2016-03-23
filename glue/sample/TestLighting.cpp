#include "TestLighting.h"

#include <m3_mesh.h>
#include <m3_parametric_surface.h>

namespace test
{

void TestLighting::Init()
{
	glue::ShaderMgr::Instance()->Init();

	m_mesh = new m3_mesh;
//	m3_create_sphere(100, m_mesh);
//	m3_create_mobius_strip(100, m_mesh);
	m3_create_cone(200, 100, m_mesh);
//	m3_create_torus(100, 10, m_mesh);
//	m3_create_trefoil_knot(100, m_mesh);

	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
		mgr->GetShader(glue::ShaderMgr::LIGHTING));
	shader->SetMaterial(glue::vec3(0.04f, 0.04f, 0.04f), glue::vec3(0.75f, 0.25f, 0), glue::vec3(0.5f, 0.5f, 0.5f), 50);
	shader->SetLightPosition(glue::vec3(0.25, 0.25, 1));

	m_angle = 0;
	m_z = 0;
}

void TestLighting::Resize(int width, int height)
{
	glue::ShaderMgr::Instance()->OnSize(width, height);
}

void TestLighting::Draw() const
{
	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
	mgr->SetShader(glue::ShaderMgr::LIGHTING);
	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
		mgr->GetShader(glue::ShaderMgr::LIGHTING));
	shader->Draw(m_mesh);
}

void TestLighting::Update()
{
// 	glue::mat4 mat = glue::mat4::RotateX(m_angle) * glue::mat4::Translate(m_z, 0, 0);
// 	
// 	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
// 	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
// 		mgr->GetShader(glue::ShaderMgr::LIGHTING));
// 	shader->SetModelView(mat);

//	m_angle += 1;

	m_z -= 0.1f;
}

}
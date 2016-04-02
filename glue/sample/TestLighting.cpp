#include "TestLighting.h"

#include <m3_mesh.h>
#include <m3_parametric_surface.h>

namespace test
{

void TestLighting::Init()
{
	glue::ShaderMgr::Instance()->Init();

	m_mesh = new m3_mesh;
//	m3_create_cone(200, 100, m_mesh);
//	m3_create_sphere(100, m_mesh);
//	m3_create_torus(100, 50, m_mesh);
//	m3_create_mobius_strip(100, m_mesh);
	m3_create_trefoil_knot(100, m_mesh);

	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
		mgr->GetShader(glue::ShaderMgr::LIGHTING));
	shader->SetMaterial(glue::vec3(0.04f, 0.04f, 0.04f), glue::vec3(0.75f, 0.25f, 0), glue::vec3(0.5f, 0.5f, 0.5f), 50);
	shader->SetLightPosition(glue::vec3(0.25, 0.25, 1));

	m_angle = 0;
	m_z = -300;
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
	sm_mat4 rot_mat;
	sm_mat4_rotymat(&rot_mat, m_angle);

	sm_mat4 trans_mat;
	sm_mat4_transmat(&trans_mat, 0, 0, m_z);

	sm_mat4 mv_mat;
	sm_mat4_mul(&mv_mat, &trans_mat, &rot_mat);

	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
		glue::ShaderMgr::Instance()->GetShader(glue::ShaderMgr::LIGHTING));
	shader->SetModelView(mv_mat);

 	m_angle += 0.01;
}

}
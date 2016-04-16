//#include "TestLighting.h"
//
//#include <glue.h>
//
//#include <m3_model.h>
//#include <m3_parametric_surface.h>
//#include <ds_array.h>
//
//namespace test
//{
//
//void TestLighting::Init()
//{
//	glue::ShaderMgr::Instance()->Init();
//
////	LoadParametricSurface();
//	LoadFromFile("meshes21/soldier.X");
//}
//
//void TestLighting::Resize(int width, int height)
//{
//	glue::RenderContext::Instance()->OnSize(width, height);
//}
//
//void TestLighting::Draw() const
//{
//	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
//	mgr->SetShader(glue::ShaderMgr::LIGHTING);
//	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
//		mgr->GetShader(glue::ShaderMgr::LIGHTING));
//	shader->Draw(m_model);
//}
//
//void TestLighting::Update()
//{
//	sm_mat4 rot_mat;
//	sm_mat4_rotymat(&rot_mat, m_angle);
//
//	sm_mat4 trans_mat;
//	sm_mat4_transmat(&trans_mat, m_pos.x, m_pos.y, m_pos.z);
//
//	sm_mat4 mv_mat;
//	sm_mat4_mul(&mv_mat, &trans_mat, &rot_mat);
//
//	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
//		glue::ShaderMgr::Instance()->GetShader(glue::ShaderMgr::LIGHTING));
//	shader->SetModelView(mv_mat);
//
// 	m_angle += 0.01f;
//}
//
//void TestLighting::LoadParametricSurface()
//{
//	m_model = m3_model_create(16);
//
//	m3_mesh* mesh = new m3_mesh;
//	//m3_create_cone(200, 100, mesh);
//	//m3_create_sphere(100, mesh);
//	m3_create_torus(100, 50, mesh);
//	//m3_create_mobius_strip(100, mesh);
//	//m3_create_trefoil_knot(100, mesh);
//	ds_array_add(m_model->meshes, &mesh);
//
//	mesh->material.ambient.x = 0.04f;
//	mesh->material.ambient.y = 0.04f;
//	mesh->material.ambient.z = 0.04f;
//	mesh->material.diffuse.x = 0.75f;
//	mesh->material.diffuse.y = 0.25f;
//	mesh->material.diffuse.z = 0;
//	mesh->material.specular.x = 0.5f;
//	mesh->material.specular.y = 0.5f;
//	mesh->material.specular.z = 0.5f;
//	mesh->material.shininess = 50;
//
//	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
//	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
//		mgr->GetShader(glue::ShaderMgr::LIGHTING));
//	shader->SetLightPosition(glue::vec3(0, 0, 0));
//
//	m_angle = 0;
//	m_pos.x = m_pos.y = 0;
//	m_pos.z = -300;
//}
//
//void TestLighting::LoadFromFile(const char* filepath)
//{
//	m_model = glue::AssimpHelper::Load(filepath);
//
//	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
//	glue::LightingShader* shader = static_cast<glue::LightingShader*>(
//		mgr->GetShader(glue::ShaderMgr::LIGHTING));
//	shader->SetLightPosition(glue::vec3(0.25, 0.25, 1));
//
//	m_angle = 0;
//	m_pos.x = 0;
//	m_pos.y = -0.8f;
//	m_pos.z = -2;
//}
//
//}
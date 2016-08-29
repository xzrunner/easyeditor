//#include "TestModel3.h"
//
//#include <glue.h>
//
//#include <m3_model.h>
//#include <m3_parametric_surface.h>
//#include <m3_typedef.h>
//#include <ds_array.h>
//#include <shaderlab.h>
//
//namespace test
//{
//
//void TestModel3::Init()
//{
//	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
//	sl_mgr->CreateContext(4096);
//	sl::RenderContext* sl_rc = sl_mgr->GetContext();
//	sl_mgr->CreateShader(sl::MODEL3, new sl::Model3Shader(sl_rc));
//
////	LoadParametricSurface();
//	LoadFromFile("meshes21/soldier.X");
//}
//
//void TestModel3::Resize(int width, int height)
//{
//	glue::RenderContext::Instance()->OnSize(width, height);
//	glue::RenderContext::Instance()->SetCamera(0, 0, 1, 1);
//}
//
//void TestModel3::Draw() const
//{
//	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
//	mgr->SetShader(sl::MODEL3);
//	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader(sl::MODEL3));
//	for (int i = 0, n = ds_array_size(m_model->meshes); i < n; ++i) 
//	{
//		m3_mesh* mesh = *(m3_mesh**)ds_array_fetch(m_model->meshes, i);
//		shader->SetMaterial(&mesh->material.ambient, &mesh->material.diffuse, 
//			&mesh->material.specular, mesh->material.shininess, mesh->material.tex);
//		bool has_normal = mesh->vertex_type & M3_VERTEX_FLAG_NORMALS,
//			 has_texcoord = mesh->vertex_type & M3_VERTEX_FLAG_TEXCOORDS;
//		shader->Draw(mesh->vertices, mesh->indices, has_normal, has_texcoord);
//	}
//}
//
//void TestModel3::Update()
//{
//	sm::mat4 rot_mat;
//	sm_mat4_rotymat(&rot_mat, m_angle);
//
//	sm::mat4 trans_mat;
//	sm_mat4_transmat(&trans_mat, m_pos.x, m_pos.y, m_pos.z);
//
//	sm::mat4 mv_mat;
//	sm_mat4_mul(&mv_mat, &trans_mat, &rot_mat);
//
//	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
//	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader(sl::MODEL3));
//	shader->SetModelView(mv_mat);
//
// 	m_angle += 0.01f;
//}
//
//void TestModel3::LoadParametricSurface()
//{
//	m_model = m3_model_create(16);
//
//	m3_mesh* mesh = new m3_mesh;
//	//m3_create_cone(200, 100, mesh);
//	//m3_create_sphere(100, mesh);
//	//m3_create_torus(100, 50, mesh);
//	m3_create_mobius_strip(100, mesh);
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
//	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
//	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader(sl::MODEL3));
//	struct sm::vec3 pos = { 0.25, 0.25, 1 };
//	shader->SetLightPosition(pos);
//
//	m_angle = 0;
//	m_pos.x = m_pos.y = 0;
//	m_pos.z = -300;
//}
//
//void TestModel3::LoadFromFile(const char* filepath)
//{
//	m_model = glue::AssimpHelper::Load(filepath);
//
//	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
//	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader(sl::MODEL3));
//	struct sm::vec3 pos = { 0.25, 0.25, 1 };
//	shader->SetLightPosition(pos);
//
//	m_angle = 0;
//	m_pos.x = 0;
//	m_pos.y = -0.8f;
//	m_pos.z = -2;
//}
//
//}
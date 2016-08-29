//#include "ImageSymbol.h"
//#include "Texture.h"
//#include "render/RenderContext.h"
//
//#include <shaderlab.h>
//
//namespace glue
//{
//
//ImageSymbol::ImageSymbol(const std::string& filepath)
//	: m_filepath(filepath)
//	, m_tex(NULL)
//{
//	Load();
//}
//
//void ImageSymbol::Draw(const sm::mat4& mt) const
//{
//	RID texid = m_tex->GetID();
//
//	sm::vec2 positions[4];
//	float hw = m_tex->GetWidth() * 0.5f,
//		  hh = m_tex->GetHeight() * 0.5f;
//	positions[0] = sm::vec2(-hw, -hh);
//	positions[1] = sm::vec2( hw, -hh);
//	positions[2] = sm::vec2( hw,  hh);
//	positions[3] = sm::vec2(-hw,  hh);
//	for (int i = 0; i < 4; ++i) {
//		positions[i] = mt * positions[i];
//	}
//
//	sm::vec2 texcoords[4];
//	texcoords[0] = sm::vec2(0, 0);
//	texcoords[1] = sm::vec2(1, 0);
//	texcoords[2] = sm::vec2(1, 1);
//	texcoords[3] = sm::vec2(0, 1);
//
//// 	if (smgr->IsBlendShader())
//// 	{
//// 		sm::vec2 vertices_scr[4];
//// 		float img_hw = m_tex->GetWidth() * 0.5f,
//// 			  img_hh = m_tex->GetHeight() * 0.5f;
//// 		vertices_scr[0] = Math::TransVector(sm::vec2(-img_hw, -img_hh), mt);
//// 		vertices_scr[1] = Math::TransVector(sm::vec2( img_hw, -img_hh), mt);
//// 		vertices_scr[2] = Math::TransVector(sm::vec2( img_hw,  img_hh), mt);
//// 		vertices_scr[3] = Math::TransVector(sm::vec2(-img_hw,  img_hh), mt);
//// 
//// 		RenderContext* ctx = RenderContext::Instance();
//// 		sm::vec2 tex_coords_base[4];
//// 		int w = ctx->GetWidth(),
//// 			h = ctx->GetHeight();
////  		for (int i = 0; i < 4; ++i) {
////  			tex_coords_base[i] = ctx->GetCamera()->TransPosProjectToScreen(vertices_scr[i], w, h);
////  			tex_coords_base[i].y = h - 1 - tex_coords_base[i].y;
////  			tex_coords_base[i].x /= w;
////  			tex_coords_base[i].y /= h;
////  			tex_coords_base[i].x = std::min(std::max(0.0f, tex_coords_base[i].x), 1.0f);
////  			tex_coords_base[i].y = std::min(std::max(0.0f, tex_coords_base[i].y), 1.0f);
////  		}
//// 		smgr->BlendDraw(vertices_scr, texcoords, tex_coords_base, texid, );
//// 	}
//// 	else
//// 	{
//		sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
//		switch (sl_mgr->GetShaderType())
//		{
//		case sl::SPRITE2:
//			{
//				sl_mgr->SetShader(sl::SPRITE2);
//				sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
//				sl_shader->Draw(&positions[0].x, &texcoords[0].x, texid);
//			}
//			break;
//		case sl::FILTER:
//			{
//				sl_mgr->SetShader(sl::FILTER);
//				sl::FilterShader* sl_shader = static_cast<sl::FilterShader*>(sl_mgr->GetShader());
//				sl_shader->Draw(&positions[0].x, &texcoords[0].x, texid);
//			}
//			break;
//		}
//
////	}
//}
//
//void ImageSymbol::Draw25(const sm::mat4& mt) const
//{
//	RID texid = m_tex->GetID(); 
//
//	sm::vec2 positions[4];
//	float hw = m_tex->GetWidth() * 0.5f,
//		hh = m_tex->GetHeight() * 0.5f;
//	positions[0] = sm::vec2(-hw, -hh);
//	positions[1] = sm::vec2( hw, -hh);
//	positions[2] = sm::vec2( hw,  hh);
//	positions[3] = sm::vec2(-hw,  hh);
//	for (int i = 0; i < 4; ++i) {
//		positions[i] = mt * positions[i];
//	}
//
//	float z[4];
//	z[0] = 250;
//	z[1] = 0;
//	z[2] = 0;
//	z[3] = 250;
//
//	std::vector<sm::vec3> positions3;
//	positions3.push_back(sm::vec3(positions[0].x, positions[0].y, z[0]));
//	positions3.push_back(sm::vec3(positions[1].x, positions[1].y, z[1]));
//	positions3.push_back(sm::vec3(positions[2].x, positions[2].y, z[2]));
//	positions3.push_back(sm::vec3(positions[0].x, positions[0].y, z[0]));
//	positions3.push_back(sm::vec3(positions[2].x, positions[2].y, z[2]));
//	positions3.push_back(sm::vec3(positions[3].x, positions[3].y, z[3]));
//
//	std::vector<sm::vec2> texcoords;
//	texcoords.push_back(sm::vec2(0, 0));
//	texcoords.push_back(sm::vec2(1, 0));
//	texcoords.push_back(sm::vec2(1, 1));
//	texcoords.push_back(sm::vec2(0, 0));
//	texcoords.push_back(sm::vec2(1, 1));
//	texcoords.push_back(sm::vec2(0, 1));
//
//	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
//	sl_mgr->SetShader(sl::SPRITE3);
//	sl::Sprite3Shader* sl_shader = static_cast<sl::Sprite3Shader*>(sl_mgr->GetShader());
//	sl_shader->Draw(&positions3[0].x, &texcoords[0].x, texid);
//}
//
//void ImageSymbol::Load()
//{
//	m_tex = new Texture();
//	m_tex->Load(m_filepath);
//}
//
//}
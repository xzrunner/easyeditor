#include "Symbol.h"
#include "OceanMesh.h"
#include "FileIO.h"

#include <ee/ImageSymbol.h>
#include <ee/FileHelper.h>

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <sprite2/RenderParams.h>
#include <gum/FilepathHelper.h>

#include <algorithm>

namespace eterrain2d
{

Symbol::Symbol()
	: m_time(0)
	, m_update(false)
	, m_angle(0)
{	
}

Symbol::~Symbol()
{
	Clear();
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	clock_t curr = clock();
	if (m_time == 0) {
		m_time = curr;
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.GetMul().ToABGR(), p.color.GetAdd().ToABGR());
	shader->SetColorMap(p.color.GetMapR().ToABGR(), p.color.GetMapG().ToABGR(), p.color.GetMapB().ToABGR());

	float dt = (float)(curr - m_time) / CLOCKS_PER_SEC;
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		OceanMesh* ocean = m_oceans[i];
		if (m_update) {
			ocean->Update(dt);
		}
		ocean->Draw(p, false);
	}

	m_time = curr;
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	sm::rect ret;
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		ret.Combine(m_oceans[i]->GetRegion());
	}
	return ret; 
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		OceanMesh* ocean = m_oceans[i];
		const ee::ImageSymbol* img0 = ocean->GetImage0();
		if (img0) {
			img0->ReloadTexture();
		}
		const ee::ImageSymbol* img1 = ocean->GetImage1();
		if (img1) {
			img1->ReloadTexture();
		}
	}	
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Clear();

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);
	int i = 0;
	Json::Value ocean_val = value["ocean"][i++];
	while (!ocean_val.isNull()) {
		OceanMesh* ocean = FileIO::LoadOceanMesh(dir, ocean_val);
		if (ocean) {
			m_angle = ocean->GetTexcoordSpdAngle();
			ocean->Rotate(m_angle);

			m_oceans.push_back(ocean);
// 			insertShape(const_cast<eshape::PolygonShape*>(ocean->GetBounding()));
// 			library->AddSymbol(const_cast<ee::ImageSymbol*>(ocean->GetImage0()));
// 			if (const ee::Symbol* tex1 = ocean->GetImage1()) {
// 				library->AddSymbol(const_cast<ee::Symbol*>(tex1));
// 			}
// 			toolbar->SetControlersValue(ocean);
		}
		ocean_val = value["ocean"][i++];
	}

	return true;
}

void Symbol::Clear()
{
	for_each(m_oceans.begin(), m_oceans.end(), cu::RemoveRefFunctor<OceanMesh>());
	m_oceans.clear();
}

}
#include "Symbol.h"
#include "OceanMesh.h"
#include "FileIO.h"

namespace eterrain2d
{

Symbol::Symbol()
	: m_time(0)
	, m_update(true)
	, m_angle(0)
{	
}

Symbol::Symbol(const Symbol& s)
	: m_time(s.m_time)
	, m_update(s.m_update)
	, m_angle(0)
{
}

Symbol::~Symbol()
{
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		delete m_oceans[i];
	}
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		OceanMesh* ocean = m_oceans[i];
		const d2d::ImageSymbol* img0 = ocean->GetImage0();
		if (img0) {
			img0->ReloadTexture();
		}
		const d2d::ImageSymbol* img1 = ocean->GetImage1();
		if (img1) {
			img1->ReloadTexture();
		}
	}	
}

void Symbol::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
	clock_t curr = clock();
	if (m_time == 0) {
		m_time = curr;
		return;
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->SetSpriteColor(mul, add);
	shader->SetSpriteColorTrans(r_trans, g_trans, b_trans);

	float dt = (float)(curr - m_time) / CLOCKS_PER_SEC;
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		OceanMesh* ocean = m_oceans[i];
		if (m_update) {
			ocean->Update(dt);
		}
		ocean->Draw(mt, mul, add, false);
	}

	m_time = curr;
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
	d2d::Rect ret;
	for (int i = 0, n = m_oceans.size(); i < n; ++i) {
		ret.combine(m_oceans[i]->GetRegion());
	}
	return ret; 
}

void Symbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(m_filepath).ToStdString();
	int i = 0;
	Json::Value ocean_val = value["ocean"][i++];
	while (!ocean_val.isNull()) {
		OceanMesh* ocean = FileIO::LoadOceanMesh(dir, ocean_val);
		if (ocean) {
			m_angle = ocean->GetTexcoordSpdAngle();
			ocean->Rotate(m_angle);

			m_oceans.push_back(ocean);
// 			insertShape(const_cast<libshape::PolygonShape*>(ocean->GetBounding()));
// 			library->AddSymbol(const_cast<d2d::ImageSymbol*>(ocean->GetImage0()));
// 			if (const d2d::ISymbol* tex1 = ocean->GetImage1()) {
// 				library->AddSymbol(const_cast<d2d::ISymbol*>(tex1));
// 			}
// 			toolbar->SetControlersValue(ocean);
		}
		ocean_val = value["ocean"][i++];
	}
}

}
#include "Symbol.h"
#include "PackageMgr.h"
#include "EJSprite.h"
#include "EJScreen.h"
#include "EJShader.h"

namespace eejoy2d
{

Symbol::Symbol()
	: m_spr(NULL)
{
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
{
	EJSprite* spr = s.m_spr;
	ee::obj_assign<EJSprite>(m_spr, spr);
}

Symbol::~Symbol()
{
	PackageMgr::Instance()->Remove(m_filepath);

	if (m_spr) {
		m_spr->Release();
	}
}

Symbol* Symbol::Clone() const
{
	return new Symbol(*this);
}

void Symbol::ReloadTexture() const
{
}

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
	if (!m_spr) {
		return;
	}

	DrawToEJScreen();
	DrawFromEJScreen();
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	return ee::Rect(200, 200);
}

void Symbol::LoadResources()
{
	eejoy2d::EJScreen::Create();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string filepath = value["filepath"].asString();
	std::string name = value["export name"].asString();

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);

	dtex_package* pkg = PackageMgr::Instance()->Fetch(dir + "\\" + filepath);
	assert(pkg);
	m_spr = new eejoy2d::EJSprite(pkg, name.c_str());
}

void Symbol::DrawToEJScreen() const
{
	eejoy2d::EJShader::Reset();

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);
	scr->Bind();
	scr->Clear();

	m_spr->Draw(0, 0);

	scr->UnBind();

	assert(eejoy2d::EJScreen::Instance());
	eejoy2d::EJScreen::Instance()->DebugDraw();
}

void Symbol::DrawFromEJScreen() const
{
	float vb[16];

	vb[2] = 0, vb[3] = 0;
	vb[6] = 0, vb[7] = 1;
	vb[10] = 1, vb[11] = 1;
	vb[14] = 1, vb[15] = 0;

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);

	int left = 0, right = scr->GetWidth();
	int down = 0, up = scr->GetHeight();

	vb[0] = left, vb[1] = down;
	vb[4] = left, vb[5] = up;
	vb[8] = right, vb[9] = up;
	vb[12] = right, vb[13] = down;

	ee::ShaderMgr* shader = ee::ShaderMgr::Instance();
	// reset
	shader->SetTexture(0);
	shader->null();

	shader->sprite();
	shader->Draw(vb, scr->GetTexID());
}

}
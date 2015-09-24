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
	: d2d::ISymbol(s)
{
	EJSprite* spr = s.m_spr;
	d2d::obj_assign<EJSprite>(m_spr, spr);
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

void Symbol::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite) const
{
	if (!m_spr) {
		return;
	}

	DrawToEJScreen();
	DrawFromEJScreen();
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(200, 200);
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

	std::string dir = d2d::FilenameTools::getFileDir(m_filepath);

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

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	// reset
	shader->SetTexture(0);
	shader->null();

	shader->sprite();
	shader->Draw(vb, scr->GetTexID());
}

}
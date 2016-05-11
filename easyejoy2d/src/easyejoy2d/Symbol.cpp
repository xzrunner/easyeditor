#include "Symbol.h"
#include "PackageMgr.h"
#include "EJSprite.h"
#include "EJScreen.h"
#include "EJShader.h"

#include <ee/FileHelper.h>

#include <shaderlab.h>

#include <json/json.h>

#include <fstream>

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

void Symbol::Draw(const s2::RenderParams& trans, const s2::Sprite* spr) const
{
	if (!m_spr) {
		return;
	}

	DrawToEJScreen();
	DrawFromEJScreen();
}

void Symbol::ReloadTexture() const
{
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
	sm::vec2 vertices[4], texcoords[4];

	texcoords[0].x = 0, texcoords[0].y = 0;
	texcoords[1].x = 0, texcoords[1].y = 1;
	texcoords[2].x = 1, texcoords[2].y = 1;
	texcoords[3].x = 1, texcoords[3].y = 0;

	eejoy2d::EJScreen* scr = eejoy2d::EJScreen::Instance();
	assert(scr);

	int left = 0, right = scr->GetWidth();
	int down = 0, up = scr->GetHeight();

	vertices[0].x = left,  vertices[0].y = down;
	vertices[1].x = left,  vertices[1].y = up;
	vertices[2].x = right, vertices[2].y = up;
	vertices[3].x = right, vertices[3].y = down;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	// reset
	mgr->GetContext()->SetTexture(0, 0);
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, scr->GetTexID());
}

}
#define NOMINMAX 
#include <gl/GLee.h>

#include "StageCanvas.h"
#include "StagePanel.h"

#include "Symbol.h"
#include "config.h"

namespace libsketch
{

StageCanvas::StageCanvas(StagePanel* stage, d2d::LibraryPanel* library)
	: e3d::StageCanvas(stage)
	, m_stage(stage)
	, m_library(library)
{
}

ivec2 StageCanvas::TransPos3ProjectToScreen(const vec3& proj) const
{
	vec3 pos = GetCamera3().GetModelViewMat() * proj;
	return e3d::ViewFrustum::TransPos3ProjectToScreen(pos, m_screen_width, m_screen_height);
}

vec3 StageCanvas::TransPos3ScreenToProject(const ivec2& scr, float proj_z) const
{
	vec3 pos = GetCamera3().GetModelViewMat() * vec3(0, 0, proj_z);
	return e3d::ViewFrustum::TransPos3ScreenToProject(scr, pos.z, m_screen_width, m_screen_height);
}

vec3 StageCanvas::TransPos3ScreenToDir(const ivec2& screen) const
{
	return e3d::ViewFrustum::TransPos3ScreenToDir(screen, m_screen_width, m_screen_height);
}

void StageCanvas::initGL()
{
	e3d::StageCanvas::initGL();
	InitDefaultSymbol();
}

void StageCanvas::onSize(int w, int h)
{
	e3d::StageCanvas::onSize(w, h);
	m_screen_width = w;
	m_screen_height = h;
}

void StageCanvas::onDraw()
{
	e3d::ShaderMgr::Instance()->SetModelView(GetCamera3().GetModelViewMat());
	DrawBackground();
	DrawSprites();

 	m_editPanel->drawEditTemp();
}

void StageCanvas::InitDefaultSymbol()
{
	e3d::ISurface* surface;

	surface = new e3d::Cone(2, 1);
	LoadSymbolToLib(surface, "Cone");

	surface = new e3d::Sphere(1);
	LoadSymbolToLib(surface, "Sphere");

	surface = new e3d::Torus(0.5f, 0.2f);
	LoadSymbolToLib(surface, "Torus");

	surface = new e3d::TrefoilKnot(1);
	LoadSymbolToLib(surface, "TrefoilKnot");

	surface = new e3d::MobiusStrip(0.2f);
	LoadSymbolToLib(surface, "MobiusStrip");

	surface = new e3d::KleinBottle(0.1f);
	LoadSymbolToLib(surface, "KleinBottle");
}

void StageCanvas::LoadSymbolToLib(e3d::ISurface* surface, const char* name)
{
	e3d::AABB aabb;
	e3d::ModelParametric* model = new e3d::ModelParametric(surface, aabb);

	libsketch::Symbol* symbol = new libsketch::Symbol();
	symbol->SetAABB(aabb);
	symbol->setName(name);
	symbol->SetModel(model);

	wxString filepath = FILE_TAG;
	filepath += ".json";
	symbol->SetFilepath(filepath);

	m_library->AddSymbol(symbol);
}

void StageCanvas::DrawBackground() const
{
	e3d::DrawCross(vec3(0, 0, 0), vec3(10, 10, 10), d2d::LIGHT_RED);
//  	e3d::DrawGrids(vec3(-10, -10, 0), vec3(10, 10, 0), vec3(0.5f, 0.5f, FLT_MAX), 
//  		d2d::LIGHT_RED);
}

void StageCanvas::DrawSprites() const
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteDraw::drawSprite(sprite);
	}
}

}
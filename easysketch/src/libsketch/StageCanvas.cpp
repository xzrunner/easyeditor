#define NOMINMAX 
#include <gl/GLee.h>

#include "StageCanvas.h"
#include "StagePanel.h"

#include "Symbol.h"
#include "config.h"

#include "ViewFrustum.h"

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
	return e3d::ViewFrustum::TransPos3ProjectToScreen(proj, m_width, m_height);
}

void StageCanvas::initGL()
{
	e3d::StageCanvas::initGL();
	InitDefaultSymbol();
}

void StageCanvas::onSize(int w, int h)
{
	e3d::StageCanvas::onSize(w, h);
	m_width = w;
	m_height = h;
}

void StageCanvas::onDraw()
{
//	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

 	std::vector<d2d::ISprite*> sprites;
 	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
 
 	for (size_t i = 0, n = sprites.size(); i < n; ++i)
 	{
 		d2d::ISprite* sprite = sprites[i];
 		if (!sprite->visiable)
 			continue;
 		d2d::SpriteDraw::drawSprite(sprite);
 	}

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

	surface = new e3d::MobiusStrip(0.6f);
	LoadSymbolToLib(surface, "MobiusStrip");

	surface = new e3d::KleinBottle(0.1f);
	LoadSymbolToLib(surface, "KleinBottle");
}

void StageCanvas::LoadSymbolToLib(e3d::ISurface* surface, const char* name)
{
	e3d::Cube aabb;
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

}
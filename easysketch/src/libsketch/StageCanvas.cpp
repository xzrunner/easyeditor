#define NOMINMAX 
#include <gl/GLee.h>

#include "StageCanvas.h"
#include "StagePanel.h"

#include "Model.h"
#include "ModelParametric.h"
#include "ParametricEquations.h"
#include "Symbol.h"
#include "config.h"

#include "ViewFrustum.h"

namespace libsketch
{

StageCanvas::StageCanvas(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
	, m_library(library)
{
}

ivec2 StageCanvas::TransPos3ProjectToScreen(const vec3& proj) const
{
	return ViewFrustum::TransPos3ProjectToScreen(proj, m_width, m_height);
}

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	InitDefaultSymbol();
}

void StageCanvas::onSize(int w, int h)
{
	d2d::OrthoCanvas::onSize(w, h);
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
	z3d::ISurface* surface;

	surface = new z3d::Cone(2, 1);
	LoadSymbolToLib(surface, "Cone");

	surface = new z3d::Sphere(1);
	LoadSymbolToLib(surface, "Sphere");

	surface = new z3d::Torus(0.5f, 0.2f);
	LoadSymbolToLib(surface, "Torus");

	surface = new z3d::TrefoilKnot(1);
	LoadSymbolToLib(surface, "TrefoilKnot");

	surface = new z3d::MobiusStrip(0.6f);
	LoadSymbolToLib(surface, "MobiusStrip");

	surface = new z3d::KleinBottle(0.1f);
	LoadSymbolToLib(surface, "KleinBottle");
}

void StageCanvas::LoadSymbolToLib(z3d::ISurface* surface, const char* name)
{
	z3d::Cube aabb;
	z3d::ModelParametric* model = new z3d::ModelParametric(surface, aabb);

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
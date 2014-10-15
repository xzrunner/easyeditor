#define NOMINMAX 
#include <gl/GLee.h>

#include "StageCanvas.h"
#include "StagePanel.h"

#include "Model.h"
#include "ModelParametric.h"
#include "ParametricEquations.h"
#include "Symbol.h"
#include "config.h"

namespace libsketch
{

StageCanvas::StageCanvas(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
	, m_library(library)
{
}

void StageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	InitDefaultSymbol();
}

void StageCanvas::onDraw()
{
	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

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
	z3d::ModelParametric* model;
	libsketch::Symbol* symbol;

	wxString filepath = FILE_TAG;
	filepath += ".json";

	surface = new z3d::Cone(2, 1.5f);
	model = new z3d::ModelParametric(surface);
	symbol = new libsketch::Symbol();
	symbol->setName("Cone");
	symbol->SetModel(model);
	symbol->SetFilepath(filepath);
	m_library->AddSymbol(symbol);

	surface = new z3d::Sphere(1.5f);
	model = new z3d::ModelParametric(surface);
	symbol = new libsketch::Symbol();
	symbol->setName("Sphere");
	symbol->SetModel(model);
	symbol->SetFilepath(filepath);
	m_library->AddSymbol(symbol);

	surface = new z3d::Torus(1.2f, 0.8f);
	model = new z3d::ModelParametric(surface);
	symbol = new libsketch::Symbol();
	symbol->setName("Torus");
	symbol->SetModel(model);
	symbol->SetFilepath(filepath);
	m_library->AddSymbol(symbol);

	surface = new z3d::TrefoilKnot(2);
	model = new z3d::ModelParametric(surface);
	symbol = new libsketch::Symbol();
	symbol->setName("TrefoilKnot");
	symbol->SetModel(model);
	symbol->SetFilepath(filepath);
	m_library->AddSymbol(symbol);

	surface = new z3d::MobiusStrip(1.2f);
	model = new z3d::ModelParametric(surface);
	symbol = new libsketch::Symbol();
	symbol->setName("MobiusStrip");
	symbol->SetModel(model);
	symbol->SetFilepath(filepath);
	m_library->AddSymbol(symbol);

	surface = new z3d::KleinBottle(0.2f);
	model = new z3d::ModelParametric(surface);
	symbol = new libsketch::Symbol();
	symbol->setName("KleinBottle");
	symbol->SetModel(model);
	symbol->SetFilepath(filepath);
	m_library->AddSymbol(symbol);
}

}
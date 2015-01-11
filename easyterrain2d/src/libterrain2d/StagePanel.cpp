#include "StagePanel.h"
#include "StageCanvas.h"
#include "OceanMesh.h"

#include <easyshape.h>

namespace eterrain2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, d2d::ShapesPanelImpl(this)
{
	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);

 	d2d::Frame* _frame = static_cast<d2d::Frame*>(m_frame);
 	SetDropTarget(new StageDropTarget(_frame, this, library));
}

StagePanel::~StagePanel()
{	
	for_each(m_oceans.begin(), m_oceans.end(), DeletePointerFunctor<OceanMesh>());
	m_oceans.clear();
}

void StagePanel::clear()
{
}

void StagePanel::AddOcean(const libshape::PolygonShape* shape, const d2d::ImageSymbol* image)
{
	bool is_new = true;
	std::vector<OceanMesh*>::iterator itr = m_oceans.begin();
	for ( ; itr != m_oceans.end(); ) {
		OceanMesh* mesh = *itr;
		if (mesh->GetShape() == shape) {
			if (mesh->IsBlendOpen()) {
				is_new = false;
				mesh->SetImage1(image);
				++itr;
			} else {
				delete mesh;
				itr = m_oceans.erase(itr);
			}
		} else {
			++itr;
		}
	}

	if (is_new) {
		m_oceans.push_back(new OceanMesh(shape, image));
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(d2d::Frame* frame, StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::StageDropTarget(frame, stage, NULL, library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		d2d::IShape* shape = m_stage->queryShapeByPos(pos);
		if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape)) {
			poly->SetMaterialTexture(image);
			m_stage->AddOcean(poly, image);
		}
		return true;
	}

	return false;
}

}
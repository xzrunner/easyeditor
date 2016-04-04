#include "StagePanel.h"
#include "StageCanvas.h"
#include "dataset/Mesh.h"

#include <ee/SelectShapesOP.h>
#include <ee/subject_id.h>

#include <easyshape.h>

namespace enav
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library, ee::PropertySettingPanel* property)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->Release();

	ee::OneFloatValue* capture_val = new ee::OneFloatValueStatic(10);
	ee::EditOP* editop = new eshape::EditPolylineOP<eshape::DrawPolygonOP, ee::SelectShapesOP>(this, GetStageImpl(), this, property, capture_val, NULL);
	SetEditOP(editop);
	editop->Release();

	const int HW = 512;
	const int HH = 512;
	m_region.xmin = -HW;
	m_region.xmax =  HW;
	m_region.ymin = -HH;
	m_region.ymax =  HH;

	m_mesh = new Mesh(m_region);
}

StagePanel::~StagePanel()
{
	delete m_mesh;
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::SpritesPanelImpl::OnNotify(sj_id, ud);
	ee::ShapesPanelImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SHAPE:
		{
			eshape::PolygonShape* poly = (eshape::PolygonShape*)ud;
			m_mesh->AddHole(poly->GetVertices());
		}
		break;
	}
}

}
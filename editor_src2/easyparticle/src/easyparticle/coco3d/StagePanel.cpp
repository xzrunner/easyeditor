
#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"

namespace eparticle
{
namespace coco3d
{

	StagePanel::StagePanel(wxWindow* parent)
		: d2d::EditPanel(parent)
		, m_particle(NULL)
	{
		xRot = yRot = 0;

		m_canvas = new StageCanvas(this);
		m_editOP = new EditOP(this);
	}

	StagePanel::~StagePanel()
	{
	}

	void StagePanel::clear()
	{
		d2d::EditPanel::clear();
	}

}
}
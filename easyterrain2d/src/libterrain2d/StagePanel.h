#ifndef _EASYTERRAIN2D_STAGE_PANEL_H_
#define _EASYTERRAIN2D_STAGE_PANEL_H_

#include <drag2d.h>
#include <easyshape.h>

namespace eterrain2d
{

class OceanMesh;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, 
	public d2d::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

	const std::vector<OceanMesh*>& GetOceans() const {
		return m_oceans;
	}
	std::vector<OceanMesh*>& GetOceans() {
		return m_oceans;
	}

private:
	void AddOcean(const libshape::PolygonShape* shape, const d2d::ImageSymbol* image);

 private:
 	class StageDropTarget : public d2d::StageDropTarget
 	{
 	public:
 		StageDropTarget(d2d::Frame* frame, StagePanel* stage, 
 			d2d::LibraryPanel* library);
 
 		virtual bool OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos);
 
 	private:
 		StagePanel* m_stage;
 
 	}; // StageDropTarget 

private:
	std::vector<OceanMesh*> m_oceans;

}; // StagePanel

}

#endif // _EASYTERRAIN2D_STAGE_PANEL_H_
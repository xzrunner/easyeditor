#ifndef _EASYTERRAIN2D_STAGE_PANEL_H_
#define _EASYTERRAIN2D_STAGE_PANEL_H_

#include <drag2d.h>
#include <easyshape.h>

namespace eterrain2d
{

class Symbol;
class OceanMesh;
class ToolbarPanel;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, 
	public d2d::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites, 
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	void Store(const std::string& dir, Json::Value& value) const;
	void Load(const std::string& dir, const Json::Value& value, 
		d2d::LibraryPanel* library, ToolbarPanel* toolbar);

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
 		StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library);
 
 		virtual bool OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos);
 
 	private:
 		StagePanel* m_stage;
 
 	}; // StageDropTarget 

private:
	std::vector<OceanMesh*> m_oceans;

//	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYTERRAIN2D_STAGE_PANEL_H_
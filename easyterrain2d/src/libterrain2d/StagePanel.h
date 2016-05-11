#ifndef _EASYTERRAIN2D_STAGE_PANEL_H_
#define _EASYTERRAIN2D_STAGE_PANEL_H_

#include <easyshape.h>

#include <ee/SpritesPanelImpl.h>
#include <ee/ShapesPanelImpl.h>
#include <ee/StageDropTarget.h>

namespace eterrain2d
{

class Symbol;
class OceanMesh;
class ToolbarPanel;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, 
	public ee::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, ee::LibraryPanel* library);
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* bg_sprites, ee::LibraryPanel* library);
	virtual ~StagePanel();

	virtual bool Update(float dt);

	void Store(const std::string& dir, Json::Value& value) const;
	void Load(const std::string& dir, const Json::Value& value, 
		ee::LibraryPanel* library, ToolbarPanel* toolbar);

	const std::vector<OceanMesh*>& GetOceans() const {
		return m_oceans;
	}
	std::vector<OceanMesh*>& GetOceans() {
		return m_oceans;
	}

private:
	void AddOcean(const eshape::PolygonShape* shape, const ee::ImageSymbol* image);

 private:
 	class StageDropTarget : public ee::StageDropTarget
 	{
 	public:
 		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);
 
 		virtual bool OnDropSymbol(ee::Symbol* symbol, const sm::vec2& pos);
 
 	private:
 		StagePanel* m_stage;
 
 	}; // StageDropTarget 

private:
	std::vector<OceanMesh*> m_oceans;

//	Symbol* m_symbol;

}; // StagePanel

}

#endif // _EASYTERRAIN2D_STAGE_PANEL_H_
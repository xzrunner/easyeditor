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
		wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* bg_sprites, ee::LibraryPanel* library);
	virtual ~StagePanel();

	virtual bool UpdateStage() override;

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
	void AddOcean(const eshape::PolygonShape* shape, const std::shared_ptr<ee::ImageSymbol>& image);

 private:
 	class StageDropTarget : public ee::StageDropTarget
 	{
 	public:
 		StageDropTarget(StagePanel* stage, ee::LibraryPanel* library);
 
 		virtual bool OnDropSymbol(const ee::SymPtr& sym, const sm::vec2& pos) override;
 
 	private:
 		StagePanel* m_stage;
 
 	}; // StageDropTarget 

private:
	std::vector<OceanMesh*> m_oceans;

//	std::shared_ptr<Symbol> m_sym;

}; // StagePanel

}

#endif // _EASYTERRAIN2D_STAGE_PANEL_H_
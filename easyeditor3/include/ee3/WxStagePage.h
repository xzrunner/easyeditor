#pragma once

#include <ee0/WxStagePage.h>

#include <SM_Vector.h>

namespace ee0 { class WxLibraryPanel; }

namespace ee3
{

class WxStagePage : public ee0::WxStagePage
{
public:
	WxStagePage(wxWindow* parent, ee0::WxLibraryPanel* library);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	sm::vec3 TransPosScrToProj3d(int x, int y) const;

	const std::vector<n0::SceneNodePtr>& GetAllNodes() const { return m_nodes; }

private:
	void InsertSceneNode(const ee0::VariantSet& variants);
	void DeleteSceneNode(const ee0::VariantSet& variants);

private:
	std::vector<n0::SceneNodePtr> m_nodes;
	
}; // WxStagePage

}
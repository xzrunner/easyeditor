#ifndef _LR_LIBRARY_PAGE_H_
#define _LR_LIBRARY_PAGE_H_

#include "dataset/layer_type.h"

#include <ee/LibraryPage.h>
#include <ee/CameraModes.h>

#include <vector>

namespace ee { class EditOP; }

namespace lr
{

class LibraryPanel;
class Layer;

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(LibraryPanel* library, const std::string& name, 
		LayerType type, int id, pt2::CameraMode cam);
	virtual ~LibraryPage();

	//
	//	interface ee::LibraryPage
	//
	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

	void UpdateStatusFromLayer();

	Layer* GetLayer() { return m_layer; }
	void SetLayer(Layer* layer);

	void AddEditOP(const std::shared_ptr<ee::EditOP>& editop);
	std::shared_ptr<ee::EditOP> GetNextEditOP();

	LayerType GetLayerType() const { return m_layer_type; }
	pt2::CameraMode GetLayerCameraMode() const { return m_cam_mode; }

protected:
	virtual void InitLayoutExtend(wxSizer* sizer) override;

	virtual void OnAddPress(wxCommandEvent& event) override;

private:
	void OnChangeVisible(wxCommandEvent& event);
	void OnChangeEditable(wxCommandEvent& event);
	void OnChangeNameVisible(wxCommandEvent& event);

	void OnResetSpritesVisibleEditable(wxCommandEvent& event);

	void VisibleAllSprites(bool visible);
	void EditableAllSprites(bool visible);

private:
	LayerType m_layer_type;
	pt2::CameraMode m_cam_mode;

	Layer* m_layer;

	wxCheckBox* m_name_ctrl;
	wxCheckBox* m_visible_ctrl;
	wxCheckBox* m_editable_ctrl;

	wxButton* m_reset;

	int m_curr_op_idx;
	std::vector<std::shared_ptr<ee::EditOP>> m_editops;

}; // LibraryPage 

}

#endif // _LR_LIBRARY_PAGE_H_
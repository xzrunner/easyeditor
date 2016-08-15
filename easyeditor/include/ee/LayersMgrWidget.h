#ifndef _EASYEDITOR_LAYERS_MGR_WIDGET_H_
#define _EASYEDITOR_LAYERS_MGR_WIDGET_H_

#include <wx/wx.h>

#include <vector>

namespace ee
{

class Layer;

class LayersMgrWidget : public wxPanel
{
	class LayerWidget;

public:
	LayersMgrWidget(wxWindow* parent);
	virtual ~LayersMgrWidget();

	void TraverseEditableLayers(Visitor<ee::Layer>& visitor);
	void TraverseVisibleLayers(Visitor<ee::Layer>& visitor);
	void TraverseSelectableLayers(Visitor<ee::Layer>& visitor);
	void TraverseAllLayers(Visitor<ee::Layer>& visitor);

	void SetEditable(LayerWidget* layer);

	Layer* GetEditedLayer() { return m_edited_layer; }

	void GetAllLayers(std::vector<Layer*>& layers) const;

	void AddLayer(Layer* layer);
	void AddLayerBottom(Layer* layer);

	void Clear();

private:
	class LayerWidget : public wxPanel
	{
	public:
		LayerWidget(wxWindow* parent, Layer* layer);
		
		bool IsVisible() const;
		bool IsSelectable() const;

		void SetEditable(bool editable);

		Layer* GetLayer() { return m_layer; }

	protected:
		void OnSetVisible(wxCommandEvent& event);
		void OnSetEditable(wxCommandEvent& event);
		void OnSetSelectable(wxCommandEvent& event);
		void OnSetName(wxCommandEvent& event);

	private:
		void InitLayout();

	private:
		wxRadioButton* m_editable;

		wxCheckBox* m_visible;

		wxCheckBox* m_selectable;

		wxTextCtrl* m_name;

		Layer* m_layer;
	};

private:
	void InitLayout();

	wxSizer* InitLayersPanel();
	wxSizer* InitButtonsPanel();

	void OnAddLayer(wxCommandEvent& event);
	void OnDelLayer(wxCommandEvent& event);
	void OnLayerUp(wxCommandEvent& event);
	void OnLayerDown(wxCommandEvent& event);

	int GetEditLayerIndex() const;

private:
	std::vector<LayerWidget*> m_layers;

	Layer* m_edited_layer;

	wxSizer* m_layers_sz;

	wxButton *m_btn_add, *m_btn_del;
	wxButton *m_btn_up, *m_btn_down;

}; // LayersMgrWidget

}

#endif // _EASYEDITOR_LAYERS_MGR_WIDGET_H_
#pragma once

#include <wx/wx.h>
#include <vector>

namespace d2d
{
	class Layer;
	class IVisitor;
	class EditPanel;

	class LayersMgrWidget : public wxPanel
	{
		class LayerWidget;

	public:
		LayersMgrWidget(wxWindow* parent, EditPanel* stage);
		virtual ~LayersMgrWidget();

		void traverseEditableLayers(IVisitor& visitor);
		void traverseVisibleLayers(IVisitor& visitor);
		void traverseSelectableLayers(IVisitor& visitor);
		void traverseAllLayers(IVisitor& visitor);

		void setEditable(LayerWidget* layer);

		Layer* getEditedLayer() { return m_editedLayer; }

		void getAllLayers(std::vector<Layer*>& layers) const;

		void addLayer(Layer* layer);
		void addLayerBottom(Layer* layer);

		void clear();

	private:
		class LayerWidget : public wxPanel
		{
		public:
			LayerWidget(wxWindow* parent, Layer* layer);
			
			bool isVisible() const;
			bool isSelectable() const;

			void setEditable(bool editable);

			Layer* getLayer() { return m_layer; }

		protected:
			void onSetVisible(wxCommandEvent& event);
			void onSetEditable(wxCommandEvent& event);
			void onSetSelectable(wxCommandEvent& event);
			void onSetName(wxCommandEvent& event);

		private:
			void initLayout();

		private:
			wxRadioButton* m_editable;

			wxCheckBox* m_visible;

			wxCheckBox* m_selectable;

			wxTextCtrl* m_name;

			Layer* m_layer;
		};

	private:
		void initLayout();

		wxSizer* initLayersPanel();
		wxSizer* initButtonsPanel();

		void onAddLayer(wxCommandEvent& event);
		void onDelLayer(wxCommandEvent& event);
		void onLayerUp(wxCommandEvent& event);
		void onLayerDown(wxCommandEvent& event);

		int getEditLayerIndex() const;

	private:
		d2d::EditPanel* m_stage;

		std::vector<LayerWidget*> m_layers;

		Layer* m_editedLayer;

		wxSizer* m_layersSizer;

		wxButton *m_btnAdd, *m_btnDel;
		wxButton *m_btnUp, *m_btnDown;

	}; // LayersMgrWidget
}


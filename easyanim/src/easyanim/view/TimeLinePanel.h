#pragma once

#include <wx/wx.h>

namespace eanim
{
	class Layer;
	class LayersMgr;
	class LayersPanel;
	class KeysPanel;

	class TimeLinePanel : public wxPanel
	{
	public:
		TimeLinePanel(wxWindow* parent);
		virtual ~TimeLinePanel();

		void loadFromTextFile(std::ifstream& fin);
		void storeToTextFile(std::ofstream& fout) const;

		LayersMgr* getLayerMgr() const;
		Layer* getCurrLayer() const;
		int getCurrFrame() const;

		int getFPSVal() const; 

		// bad
		LayersPanel* getLayersPanel();
		KeysPanel* getKeysPanel();

		void clear();

	private:
		void initLayout();

		void initTitle(wxSizer* sizer);
		void initSplitterWindow(wxSizer* sizer);

	private:
		static const float SASH_GRAVITY_VERT;

	private:
		LayersMgr* m_layersMgr;

		LayersPanel* m_layersPanel;
		KeysPanel* m_keysPanel;

	}; // TimeLinePanel
}


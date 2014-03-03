
#ifndef EBUILDER_LAYER_ITEM_H
#define EBUILDER_LAYER_ITEM_H

#include <drag2d.h>

namespace ebuilder
{
	class Layer;
	class Actor;

	class LayerItem : public d2d::ISymbol
	{
	public:
		LayerItem(Layer* layer);
		virtual ~LayerItem();

		//
		// d2d::ICloneable interface
		//	
		virtual LayerItem* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const d2d::ISprite* sprite = NULL) const;
		virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

		//
		// ListItem interface
		//
		virtual void refresh();

		Layer* getLayer() const {
			return m_layer;
		}

		const wxString& getName() const;
		void setName(const wxString& name);

	protected:
		virtual void loadResources() {}

	private:
		void refreshThumbnail();

	private:
		Layer* m_layer;

	}; // LayerItem
}

#endif // EBUILDER_LAYER_ITEM_H

#ifndef EBUILDER_LAYER_ITEM_H
#define EBUILDER_LAYER_ITEM_H



namespace ebuilder
{
	class Layer;
	class Actor;

	class LayerItem : public ee::ISymbol
	{
	public:
		LayerItem(Layer* layer);
		virtual ~LayerItem();

		//
		// ee::ICloneable interface
		//	
		virtual LayerItem* clone();

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const ee::Matrix& mt,
			const ee::Colorf& mul = ee::Colorf(1, 1, 1, 1), 
			const ee::Colorf& add = ee::Colorf(0, 0, 0, 0),
			const ee::ISprite* sprite = NULL) const;
		virtual ee::Rect getSize(const ee::ISprite* sprite = NULL) const;

		Layer* getLayer() const {
			return m_layer;
		}

		const wxString& getName() const;
		void setName(const wxString& name);

	protected:
		virtual void loadResources() {}

	private:
		Layer* m_layer;

	}; // LayerItem
}

#endif // EBUILDER_LAYER_ITEM_H
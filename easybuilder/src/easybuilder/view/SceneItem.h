
#ifndef EBUILDER_SCENE_ITEM_H
#define EBUILDER_SCENE_ITEM_H



namespace ebuilder
{
	class Layer;
	class Scene;

	class SceneItem : public ee::ISymbol
	{
	public:
		SceneItem(Scene* scene);
		virtual ~SceneItem();

		//
		// ee::ICloneable interface
		//	
		virtual SceneItem* clone();

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const ee::Matrix& mt,
			const ee::Colorf& mul = ee::Colorf(1, 1, 1, 1), 
			const ee::Colorf& add = ee::Colorf(0, 0, 0, 0),
			const ee::ISprite* sprite = NULL) const;
		virtual ee::Rect getSize(const ee::ISprite* sprite = NULL) const;

		Scene* getScene() const {
			return m_scene;
		}

		const wxString& getName() const;
		void setName(const wxString& name);

	protected:
		virtual void loadResources() {}

	private:
		Scene* m_scene;

	}; // SceneItem
}

#endif // EBUILDER_SCENE_ITEM_H


#ifndef EBUILDER_SCENE_ITEM_H
#define EBUILDER_SCENE_ITEM_H

#include <drag2d.h>

namespace ebuilder
{
	class Layer;
	class Scene;

	class SceneItem : public d2d::ISymbol
	{
	public:
		SceneItem(Scene* scene);
		virtual ~SceneItem();

		//
		// d2d::ICloneable interface
		//	
		virtual SceneItem* clone();

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

		Scene* getScene() const {
			return m_scene;
		}

		const wxString& getName() const;
		void setName(const wxString& name);

	protected:
		virtual void loadResources() {}

	private:
		void refreshThumbnail();

	private:
		Scene* m_scene;

	}; // SceneItem
}

#endif // EBUILDER_SCENE_ITEM_H

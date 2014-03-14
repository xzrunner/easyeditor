
#ifndef EBUILDER_ACTOR_ITEM_H
#define EBUILDER_ACTOR_ITEM_H

#include <drag2d.h>

namespace ebuilder
{
	class Actor;

	class ActorItem : public d2d::ISymbol
	{
	public:
		ActorItem(Actor* actor);
		virtual ~ActorItem() {}

		//
		// d2d::ICloneable interface
		//	
		virtual ActorItem* clone() { return NULL; }

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// d2d::ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
			const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
			const d2d::ISprite* sprite = NULL) const;
		virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

		Actor* getActor() const {
			return m_actor;
		}

	protected:
		virtual void loadResources();

	private:
		Actor* m_actor;

	}; // ActorItem
}

#endif // EBUILDER_ACTOR_ITEM_H

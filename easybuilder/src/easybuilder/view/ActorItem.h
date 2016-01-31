
#ifndef EBUILDER_ACTOR_ITEM_H
#define EBUILDER_ACTOR_ITEM_H



namespace ebuilder
{
	class Actor;

	class ActorItem : public ee::ISymbol
	{
	public:
		ActorItem(Actor* actor);
		virtual ~ActorItem() {}

		//
		// ee::ICloneable interface
		//	
		virtual ActorItem* clone() { return NULL; }

		//
		// ee::ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const ee::Matrix& mt,
			const ee::Colorf& mul = ee::Colorf(1, 1, 1, 1), 
			const ee::Colorf& add = ee::Colorf(0, 0, 0, 0),
			const ee::ISprite* sprite = NULL) const;
		virtual ee::Rect getSize(const ee::ISprite* sprite = NULL) const;

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

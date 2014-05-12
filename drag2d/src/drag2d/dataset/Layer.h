#pragma once

#include <wx/wx.h>
#include <vector>

#include "interfaces.h"
#include "common/Object.h"

namespace d2d
{	
	class ISprite;
	class IShape;

	class Layer : public Object
 	{
 	public:
 		Layer();
 		~Layer();

		const wxString& getName() const { return m_name; }
		void setName(const wxString& name);

		// use IVisitor if has other implementation
		const std::vector<ISprite*>& getSprites() const { return m_sprites; }
		const std::vector<IShape*>& getShapes() const { return m_shapes; }

		void traverseSprites(IVisitor& visitor) const;
		void traverseShapes(IVisitor& visitor) const;

		void insert(ISprite* shape);
		void remove(ISprite* shape);
		void resetOrder(ISprite* sprite, bool up);

		void insert(IShape* sprite);
		void remove(IShape* sprite);

 		void clear();
 
 	private:
		wxString m_name;

 		std::vector<ISprite*> m_sprites;
 		std::vector<IShape*> m_shapes;
 
 	}; // Layer
}


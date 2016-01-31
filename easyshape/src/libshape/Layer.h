#ifndef _LIBSHAPE_LAYER_H_
#define _LIBSHAPE_LAYER_H_

#include <wx/wx.h>
#include <vector>

namespace libshape
{	

class Layer : public ee::Object
{
public:
	Layer();
	~Layer();

	const wxString& getName() const { return m_name; }
	void setName(const wxString& name);

	// use IVisitor if has other implementation
	const std::vector<ee::ISprite*>& getSprites() const { return m_sprites; }
	const std::vector<ee::IShape*>& getShapes() const { return m_shapes; }

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

	std::vector<ee::ISprite*> m_sprites;
	std::vector<ee::IShape*> m_shapes;

}; // Layer

}

#endif // _LIBSHAPE_LAYER_H_
#ifndef _EASYMESH_SYMBOL_H_
#define _EASYMESH_SYMBOL_H_

#include <ee/Symbol.h>
#include <ee/Rect.h>

namespace ee { class Image; }

namespace emesh
{

class Mesh;
class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	Symbol(ee::Image* image);
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const;

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const ee::RenderParams& trans, const ee::Sprite* spr = NULL, 
		const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const {
		return m_region;
	}

 	const Mesh* GetMesh() const { return m_mesh; }
 	Mesh* GetMesh() { return m_mesh; }
	void SetMesh(Mesh* mesh);

	const ee::Image* GetImage() const { return m_image; }
	ee::Image* GetImage() { return m_image; }

	std::string GetImagePath() const;
	void LoadImage2(const std::string& filepath);

	void SetPause(bool pause) { m_pause = pause; }

	void CreateShape();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void InitBounding();

private:
	ee::Image* m_image;

	Mesh* m_mesh;

	bool m_pause;

	ee::Rect m_region;

}; // Symbol

}

#endif // _EASYMESH_SYMBOL_H_
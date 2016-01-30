#ifndef _EASYTERRAIN2D_SYMBOL_H_
#define _EASYTERRAIN2D_SYMBOL_H_

#include <drag2d.h>

namespace eterrain2d
{

class OceanMesh;

class Symbol : public d2d::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	std::vector<OceanMesh*>& GetOceans() { return m_oceans; }
	const std::vector<OceanMesh*>& GetOceans() const { return m_oceans; }

	void SetUpdateOpen(bool open) { m_update = open; }

	float GetOceanAngle() const { return m_angle; }

	static d2d::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void Clear();

private:
	bool m_update;

	std::vector<OceanMesh*> m_oceans;

	float m_angle;

	mutable clock_t m_time;

}; // Symbol

}

#endif // _EASYTERRAIN2D_SYMBOL_H_
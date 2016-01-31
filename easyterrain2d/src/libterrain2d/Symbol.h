#ifndef _EASYTERRAIN2D_SYMBOL_H_
#define _EASYTERRAIN2D_SYMBOL_H_



namespace eterrain2d
{

class OceanMesh;

class Symbol : public ee::Symbol
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
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color = ee::ColorTrans(), 
		const ee::Sprite* spr = NULL, const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	std::vector<OceanMesh*>& GetOceans() { return m_oceans; }
	const std::vector<OceanMesh*>& GetOceans() const { return m_oceans; }

	void SetUpdateOpen(bool open) { m_update = open; }

	float GetOceanAngle() const { return m_angle; }

	static ee::Symbol* Create() { return new Symbol(); }

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
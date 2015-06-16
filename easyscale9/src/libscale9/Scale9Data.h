#ifndef _EASYSCALE9_SCALE9_DATA_H_
#define _EASYSCALE9_SCALE9_DATA_H_

#include <drag2d.h>

namespace escale9
{

enum Scale9Type
{
	e_null = 0,
	e_9Grid,
	e_3GridHor,
	e_3GridVer,
	e_6GridUpper,
	e_9GridHollow
};

class Scale9Data
{
public:
	Scale9Data();
	Scale9Data(const Scale9Data& data);
	~Scale9Data();

	void ReloadTexture() const;

	void Draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0),
		const d2d::ISprite* sprite = NULL) const;

	void GetSize(float& width, float& height) const;

	void Resize(float width, float height);
	void Compose(d2d::ISprite* sprites[3][3], float width, float height);

	void LoadFromFile(const std::string& filepath);

	Scale9Type GetType() const { return m_type; }
	
	d2d::ISprite* GetSprite(int i, int j) const { return m_sprites[i][j]; }

	static void DrawScale9(Scale9Type type,
		d2d::ISprite* const sprites[3][3],
		const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0));

	static void ResizeScale9(Scale9Type type, d2d::ISprite* const sprites[3][3],
		float width, float height);
	static void ResizeSprite(d2d::ISprite* sprite, const d2d::Vector& center, 
		float width, float height);

	static Scale9Type CheckType(d2d::ISprite* sprites[3][3]);

private:
	static void InitSprite(const Json::Value& spr_val,
		d2d::ISprite** pSprite, const std::string& dir);	

private:
	Scale9Type m_type;

	// 2
	// 1
	// 0 1 2
	d2d::ISprite* m_sprites[3][3];

	float m_width, m_height;

}; // Scale9Data

}

#endif // _EASYSCALE9_SCALE9_DATA_H_
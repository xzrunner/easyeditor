#ifndef _EASYEJOY2D_EJSPRITE_H_
#define _EASYEJOY2D_EJSPRITE_H_

struct ej_package;
struct ej_sprite;
struct animation;
struct ej_extra;
struct screen_coord;

#include <stdint.h>

#include <drag2d.h>

namespace eejoy2d
{

class EJSprite : public d2d::Object
{
public:
	EJSprite(ej_package* pkg, const char* name);
	EJSprite(ej_sprite* spr);
	virtual ~EJSprite();

	void Update();
	void Draw(int x, int y) const;

	void Translate(int dx, int dy);

	void SetFrame(int frame);

private:
	void LoadSprite(ej_package* pkg, const char* name);

	void Draw(const ej_sprite* spr, animation* opt_ani, int frame, int x, int y, 
		uint32_t color, uint32_t additive, int *mat, ej_extra *extra, screen_coord * screen) const;

private:
	ej_package* m_pkg;

	ej_sprite* m_spr;

}; // EJSprite

}

#endif // _EASYEJOY2D_EJSPRITE_H_
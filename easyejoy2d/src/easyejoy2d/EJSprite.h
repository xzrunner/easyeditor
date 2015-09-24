#ifndef _EASYEJOY2D_EJSPRITE_H_
#define _EASYEJOY2D_EJSPRITE_H_

#include <drag2d.h>
#include <ejoy2d.h>

#include <stdint.h>

struct dtex_package;

namespace eejoy2d
{

class EJSprite : public d2d::Object
{
public:
	EJSprite(dtex_package* pkg, const char* name);
	EJSprite(ej_sprite* spr);
	virtual ~EJSprite();

	void Update();
	void Draw(int x, int y) const;

	void Translate(int dx, int dy);

	void SetFrame(int frame);

	ej_sprite* GetEjSpr() { return m_ej_spr; }

private:
	void LoadSprite(dtex_package* pkg, const char* name);

	ej_sprite* CreateSprite(int id);

	static void Draw(struct ej_sprite* spr, struct ej_srt* srt, struct ej_sprite_trans* ts);
	static void DrawQuad(struct ej_pack_picture* pic, const struct ej_srt* srt, const struct ej_sprite_trans* arg);
	static void DrawAnim(struct ej_sprite* spr, struct ej_srt* srt, struct ej_sprite_trans* t);

	static int GetSpriteFrame(struct ej_sprite* spr);

private:
	ej_sprite_pack* m_ej_pkg;

	ej_sprite* m_ej_spr;

	ej_srt m_ej_srt;

}; // EJSprite

}

#endif // _EASYEJOY2D_EJSPRITE_H_
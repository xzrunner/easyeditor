#ifndef _EASYEJOY2D_EJSPRITE_H_
#define _EASYEJOY2D_EJSPRITE_H_

#include <CU_RefCountObj.h>

#include <stdint.h>

struct dtex_package;

namespace eejoy2d
{

	class EJSprite : public cu::RefCountObj
{
public:
	EJSprite(dtex_package* pkg, const char* name);
//	EJSprite(ej_sprite* spr);
	virtual ~EJSprite();

	void Update();
	void Draw(int x, int y) const;

//	void Translate(int dx, int dy);

	void SetFrame(int frame);

//	ej_sprite* GetEjSpr() { return m_ej_spr; }

private:
	dtex_package* m_pkg;

//	ej_sprite* m_ej_spr;

//	ej_srt m_ej_srt;

}; // EJSprite

}

#endif // _EASYEJOY2D_EJSPRITE_H_
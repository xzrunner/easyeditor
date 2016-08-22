#ifndef _EASYSKETCH_SPRITE_H_
#define _EASYSKETCH_SPRITE_H_

#include <ee/Sprite.h>

#include <ee/DummySprite.h>

namespace esketch
{

class Sprite : public s2::DummySprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(Symbol* sym);
	Sprite(const Sprite& spr);
	virtual ~Sprite();

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }
	virtual bool Update(const s2::RenderParams& params, float dt);

	const sm::vec3& GetPos3() const { return m_pos3; }
	void SetPos3(const sm::vec3& pos) { m_pos3 = pos; }
	void Translate3(const sm::vec3& offset) { m_pos3 += offset; }

	const sm::Quaternion& GetOri3() const { return m_ori3; }
	void SetOri3(const sm::Quaternion& ori) { m_ori3 = ori; }
	void Rotate3(const sm::Quaternion& delta) {
		m_ori3.Rotate(delta);
		m_ori3 = delta.Rotated(m_ori3);
	}

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	sm::vec3 m_pos3;
	sm::Quaternion m_ori3;

}; // Sprite

}

#endif // _EASYSKETCH_SPRITE_H_
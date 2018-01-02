#ifndef _EASYCOMPLEX3D_SPRITE_H_
#define _EASYCOMPLEX3D_SPRITE_H_

#include <ee/Sprite.h>

#include <ee/DummySprite.h>

namespace ecomplex3d
{

class Sprite : public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);
	virtual ~Sprite();

	const sm::vec3& GetPos3() const { return m_pos3; }
	void SetPos3(const sm::vec3& pos) { m_pos3 = pos; }
	void Translate3(const sm::vec3& offset) { m_pos3 += offset; }

	const sm::Quaternion& GetOri3() const { return m_ori3; }
	void SetOri3(const sm::Quaternion& ori) { m_ori3 = ori; }
	void Rotate3(const sm::Quaternion& delta) {
		m_ori3.Rotate(delta);
		m_ori3 = delta.Rotated(m_ori3);
	}

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

private:
	sm::vec3 m_pos3;
	sm::Quaternion m_ori3;

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYCOMPLEX3D_SPRITE_H_

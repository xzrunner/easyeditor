#ifndef _EASYEDITOR_SHADER_MGR_H_
#define _EASYEDITOR_SHADER_MGR_H_

#include "Color.h"
#include "ShaderMgrBase.h"
#include "BlendModes.h"

namespace ee
{

struct ColorTrans;
class Vector;
class ShaderImpl;
class IShader;
class ShapeShader;
class SpriteShader;
class ScreenShader;
class RVGShader;
class BlendShader;

class ShaderMgr : public ShaderMgrBase
{
public:
	static ShaderMgr* Instance();

	virtual void NullProg();

	void SetSpriteColor(const ColorTrans& color);
	void SetShapeColor(const Colorf& col);
	void SetBlendMode(BlendMode mode);

	void sprite();
	void shape();
	void Screen();
	void RVG();
	void SpriteBlend();
	void null();

	int GetTexID() const;
	int GetFboID() const;

	void SetTexture(int tex);
	void SetFBO(int fbo);

	void Draw(const float vb[16], int texid);
	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);
	void DrawScreen(int texid);

	int GetVersion() const;

	void SetBufferData(bool open);
	bool IsOpenBufferData() const;

	void SetModelView(const Vector& offset, float scale);

	int AddShapeShader(ShapeShader* shader);
	void SetShapeShader(int idx);

	int AddSpriteShader(SpriteShader* shader);
	void SetSpriteShader(int idx);
	void SetSpriteShader(SpriteShader* shader, bool delete_old = true);

	ShapeShader* GetShapeShader() { return m_shape_shader; }
	SpriteShader* GetSpriteShader() { return m_sprite_shader; }
	BlendShader* GetBlendShader() { return m_blend_shader; }

private:
	ShaderMgr();
	~ShaderMgr();

private:
	static ShaderMgr* m_instance;

private:
	ShapeShader* m_shape_shader;
	SpriteShader* m_sprite_shader;
	ScreenShader* m_screen_shader;
	RVGShader* m_rvg_shader;
	BlendShader* m_blend_shader;

	std::vector<SpriteShader*> m_all_sprite_shader;
	std::vector<ShapeShader*> m_all_shape_shader;

	int m_version;

}; // ShaderMgr

}

#endif // _EASYEDITOR_SHADER_MGR_H_
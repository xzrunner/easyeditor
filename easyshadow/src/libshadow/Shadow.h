#ifndef _EASYSHADOW_SHADOW_H_
#define _EASYSHADOW_SHADOW_H_

#include <SM_Matrix.h>
#include <SM_Rect.h>
#include <sprite2/Color.h>
#include <CU_RefCountObj.h>

#include <sprite2/pre_defined.h>
#include S2_MAT_HEADER

#include <json/json.h>

namespace eshadow
{

class Shadow : public cu::RefCountObj
{
public:
	Shadow(float radius);
	~Shadow();

	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value);

	void Draw(const S2_MAT& mt, float alpha) const;

	void BuildFace();
	void BuildOutterLine();
	void BuildInnerLine(const std::vector<sm::vec2>& loop);

	const sm::rect& GetRegion() const { return m_region; }

	const s2::Color& GetInnerColor() const { return m_inner_color; }
	const s2::Color& GetOuterColor() const { return m_outer_color; }
	void SetInnerColer(const s2::Color& col);
	void SetOuterColer(const s2::Color& col);

	void SetRadius(float r) { m_radius = r; }

	const std::vector<sm::vec2>& GetInnerLoop() const { return m_inner_loop; }

	static void InitShader();

private:
	void BuildInnerLoop(const std::vector<sm::vec2>& loop);
	void BuildOuterLoop();

private:
	float m_radius;

	std::vector<sm::vec2> m_inner_loop;
	std::vector<sm::vec2> m_outer_loop;

	s2::Color m_inner_color, m_outer_color;

	std::vector<sm::vec2> m_tris;	
	std::vector<s2::Color> m_colors;	

	sm::rect m_region;

	static int m_shader_idx;

}; // Shadow

}

#endif // _EASYSHADOW_SHADOW_H_
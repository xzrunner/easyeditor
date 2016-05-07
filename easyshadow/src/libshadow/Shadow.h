#ifndef _EASYSHADOW_SHADOW_H_
#define _EASYSHADOW_SHADOW_H_

#include <ee/Object.h>
#include <ee/Vector.h>
#include <ee/Rect.h>
#include <ee/Color.h>

#include <SM_Matrix.h>

#include <json/json.h>

namespace eshadow
{

class Shadow : public ee::Object
{
public:
	Shadow(float radius);
	~Shadow();

	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value);

	void Draw(const sm::mat4& mt, float alpha) const;

	void BuildFace();
	void BuildOutterLine();
	void BuildInnerLine(const std::vector<ee::Vector>& loop);

	const ee::Rect& GetRegion() const { return m_region; }

	const ee::Colorf& GetInnerColor() const { return m_inner_color; }
	const ee::Colorf& GetOuterColor() const { return m_outer_color; }
	void SetInnerColer(const ee::Colorf& col);
	void SetOuterColer(const ee::Colorf& col);

	void SetRadius(float r) { m_radius = r; }

	const std::vector<ee::Vector>& GetInnerLoop() const { return m_inner_loop; }

	static void InitShader();

private:
	void BuildInnerLoop(const std::vector<ee::Vector>& loop);
	void BuildOuterLoop();

private:
	float m_radius;

	std::vector<ee::Vector> m_inner_loop;
	std::vector<ee::Vector> m_outer_loop;

	ee::Colorf m_inner_color, m_outer_color;

	std::vector<ee::Vector> m_tris;	
	std::vector<ee::Colorf> m_colors;	

	ee::Rect m_region;

	static int m_shader_idx;

}; // Shadow

}

#endif // _EASYSHADOW_SHADOW_H_
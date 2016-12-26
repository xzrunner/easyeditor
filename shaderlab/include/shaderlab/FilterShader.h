#ifndef _SHADERLAB_FILTER_SHADER_H_
#define _SHADERLAB_FILTER_SHADER_H_

#include "Shader.h"
#include "FilterMode.h"

#include <unirender/VertexAttrib.h>

#include <stdint.h>

namespace sl
{

class FilterProgram;
class RenderBuffer;

class FilterShader : public Shader
{
public:
	FilterShader(ur::RenderContext* rc);
	virtual ~FilterShader();
	
	virtual void Bind() const;
	virtual void UnBind() const;
	virtual void Commit() const;

	void SetColor(uint32_t color, uint32_t additive);

	void UpdateTime(float dt);
	void ClearTime();
	
	void SetMode(FILTER_MODE mode);
	FilterProgram* GetProgram(FILTER_MODE mode);
	FILTER_MODE GetMode() const { return m_curr_mode; }

	void Draw(const float* positions, const float* texcoords, int texid) const;

private:
	void InitVAList();
	void InitProgs();

	void UpdateTime();

private:
	enum PROG_IDX {
		PI_EDGE_DETECTION = 0,
		PI_RELIEF,
		PI_OUTLINE,

		PI_GRAY,
		PI_BLUR,
		PI_GAUSSIAN_BLUR_HORI,
		PI_GAUSSIAN_BLUR_VERT,

		PI_HEAT_HAZE,
		PI_SHOCK_WAVE,
		PI_SWIRL,
		PI_BURNING_MAP,

		PI_COL_GRADING,

		PROG_COUNT
	};

	enum PROG_TYPE {
		PT_NULL				= 0,
		PT_MULTI_ADD_COLOR	= 1,
	};

	enum VA_TYPE {
		POSITION = 0,
		TEXCOORD,
		COLOR,
		ADDITIVE,
		VA_MAX_COUNT
	};

	struct Vertex
	{
		float vx, vy;
		float tx, ty;
		uint32_t color, additive;
	};

private:
	FilterProgram* InitProgWithColor(int idx) const;

private:
	ur::VertexAttrib m_va_list[VA_MAX_COUNT];

	FilterProgram* m_programs[PROG_COUNT];
	mutable FilterProgram* m_programs_with_color[PROG_COUNT];

	float m_time;

	FILTER_MODE m_curr_mode;
	int m_mode2index[256];

	mutable int m_texid;

	Vertex* m_vertex_buf;
	mutable int m_quad_sz;

	RenderBuffer* m_index_buf;

	uint32_t m_color, m_additive;

	mutable int m_prog_type;

}; // FilterShader

}

#endif // _SHADERLAB_FILTER_SHADER_H_
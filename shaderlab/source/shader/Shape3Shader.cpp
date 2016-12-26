#include "Shape3Shader.h"
#include "ShaderProgram.h"
#include "SubjectMVP3.h"
#include "RenderShader.h"
#include "StackAllocator.h"

namespace sl
{

static const int MAX_VERTICES = 4096;

Shape3Shader::Shape3Shader(ur::RenderContext* rc)
	: ShapeShader(rc)
{
	InitProg(3, MAX_VERTICES);
}

void Shape3Shader::Draw(const float* positions, int count) const
{
	StackAllocator* alloc = StackAllocator::Instance();
	int sz = m_prog->GetVertexSize() * count;
	alloc->Reserve(sz);
	void* buf = alloc->Alloc(sz);
	uint8_t* ptr = (uint8_t*)buf;
 	for (int i = 0; i < count; ++i) 
 	{
 		memcpy(ptr, &positions[i * 3], sizeof(float));
 		ptr += sizeof(float);
 		memcpy(ptr, &positions[i * 3 + 1], sizeof(float));
 		ptr += sizeof(float);
		memcpy(ptr, &positions[i * 3 + 2], sizeof(float));
		ptr += sizeof(float);
 		memcpy(ptr, &m_color, sizeof(m_color));
 		ptr += sizeof(m_color);
 	}
 	m_prog->GetShader()->Draw(buf, count);
	alloc->Free(buf);
}

void Shape3Shader::Draw(float x, float y, float z, bool dummy) const
{
	uint8_t buf[sizeof(float) * 3 + sizeof(int)];
	uint8_t* ptr = buf;
	memcpy(ptr, &x, sizeof(float));
	ptr += sizeof(float);
	memcpy(ptr, &y, sizeof(float));
	ptr += sizeof(float);
	memcpy(ptr, &z, sizeof(float));
	ptr += sizeof(float);
	if (dummy) {
		memset(ptr, 0, sizeof(uint32_t));
	} else {
		memcpy(ptr, &m_color, sizeof(uint32_t));
	}
	ptr += sizeof(uint32_t);
	m_prog->GetShader()->Draw(buf, 1);
}

void Shape3Shader::InitMVP(ObserverMVP* mvp) const
{
	SubjectMVP3::Instance()->Register(mvp);
}

}
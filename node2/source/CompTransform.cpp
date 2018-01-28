#include "node2/CompTransform.h"

namespace n2
{

const char* const CompTransform::TYPE_NAME = "transform2d";

CompTransform::CompTransform()
{ 
	Reset();
}

void CompTransform::SetPosition(const sm::vec2& pos)
{
	if (pos != m_srt.position) {
		m_srt.position = pos;
		UpdateCenter();
		UpdateMatrix();
	}
}

void CompTransform::Reset()
{
	m_srt.Reset();
	m_mat.Identity();
}

}
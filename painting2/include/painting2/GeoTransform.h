#pragma once

#include "painting2/SRT.h"

#include <SM_Matrix2D.h>

namespace pt2
{

class GeoTransform
{
public:
	GeoTransform();

	void Reset();

	const sm::vec2& GetPosition() const { return m_srt.position; }
	float GetAngle() const              { return m_srt.angle; }
	const sm::vec2& GetScale() const    { return m_srt.scale; }
	const sm::vec2& GetShear() const    { return m_srt.shear; }
	const sm::vec2& GetOffset() const   { return m_srt.offset; }
	const sm::vec2& GetCenter() const   { return m_srt.center; }

	SRT& GetSRT()                         { return m_srt; }
	const SRT& GetSRT() const             { return m_srt; }
	const sm::Matrix2D& GetMatrix() const { return m_mat; }

	void SetPosition(const sm::vec2& pos);
	void SetAngle(float angle);
	void SetScale(const sm::vec2& scale);
	void SetShear(const sm::vec2& shear);
	void SetOffset(const sm::vec2& offset);

	void SetSRT(const SRT& srt);

private:
	void UpdateSrt();
	void UpdateMatrix();

private:
	SRT          m_srt;
	sm::Matrix2D m_mat;

}; // GeoTransform

}

#include "painting2/GeoTransform.inl"
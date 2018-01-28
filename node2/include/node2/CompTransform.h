#pragma once

#include "predef.h"

#include <SM_Vector.h>
#include <SM_Matrix2D.h>
#include <node0/NodeComponent.h>

namespace n2
{

class CompTransform : public n0::NodeComponent
{
public:
	CompTransform();

	virtual const char* Type() const override { return TYPE_NAME; }

	void SetPosition(const sm::vec2& pos);

	const sm::Matrix2D& GetTransformMat() const { return m_mat; }

	void Reset();

	static const char* const TYPE_NAME;

private:
	void UpdateCenter();
	void UpdateMatrix();

private:
	struct SRT
	{
		sm::vec2 position;
		float    angle;
		sm::vec2 scale;
		sm::vec2 shear;
		sm::vec2 offset;
		sm::vec2 center;

		SRT();

		void Reset();

	}; // SRT

private:
	SRT    m_srt;
	N2_MAT m_mat;

}; // CompTransform

}

#include "node2/CompTransform.inl"
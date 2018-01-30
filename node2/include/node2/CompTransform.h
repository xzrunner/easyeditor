#pragma once

#include "node2/predef.h"
#include "node2/SRT.h"

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
	const SRT& GetTransformSRT() const { return m_srt; }

	void Reset();

	static const char* const TYPE_NAME;

private:
	void UpdateCenter();
	void UpdateMatrix();

private:
	SRT    m_srt;
	N2_MAT m_mat;

}; // CompTransform

}

#include "node2/CompTransform.inl"
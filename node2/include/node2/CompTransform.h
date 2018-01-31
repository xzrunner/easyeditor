#pragma once

#include <node0/NodeComponent.h>
#include <painting2/GeoTransform.h>

namespace n2
{

class CompTransform : public n0::NodeComponent
{
public:
	CompTransform();

	virtual const char* Type() const override { return TYPE_NAME; }

	static const char* const TYPE_NAME;

private:
	pt2::GeoTransform m_transform;

}; // CompTransform

}
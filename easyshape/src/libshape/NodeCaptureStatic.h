#ifndef _LIBSHAPE_NODE_CAPTURE_STATIC_H_
#define _LIBSHAPE_NODE_CAPTURE_STATIC_H_

#include "INodeCapture.h"

namespace libshape
{
	
class NodeCaptureStatic : public INodeCapture
{
public:
	NodeCaptureStatic(float scope) : m_scope(scope) {}

	virtual float GetScope() const { return m_scope; }

private:
	float m_scope;

}; // NodeCaptureStatic

}

#endif // _LIBSHAPE_NODE_CAPTURE_STATIC_H_

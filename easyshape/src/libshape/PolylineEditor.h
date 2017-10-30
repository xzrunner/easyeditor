#ifndef _EASYSHAPE_POLYLINE_EDITOR_H_
#define _EASYSHAPE_POLYLINE_EDITOR_H_

#include <cu/cu_stl.h>

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>

namespace eshape
{

class PolylineEditor
{
public:
	static bool AddVertex(CU_VEC<sm::vec2>& vertices, sm::rect& bounding,
		int index, const sm::vec2& pos);

	static bool RemoveVertex(CU_VEC<sm::vec2>& vertices, sm::rect& bounding,
		const sm::vec2& pos);

	static bool ChangeVertex(CU_VEC<sm::vec2>& vertices, sm::rect& bounding,
		const sm::vec2& from, const sm::vec2& to);

private:
	static bool IsVertexOnRect(const sm::vec2& v, const sm::rect& r);

	static sm::rect CalcBounding(const CU_VEC<sm::vec2>& vertices);

}; // PolylineEditor

}

#endif // _EASYSHAPE_POLYLINE_EDITOR_H_
#include "PolylineEditor.h"

namespace eshape
{

bool PolylineEditor::AddVertex(std::vector<sm::vec2>& vertices, sm::rect& bounding,
							   int index, const sm::vec2& pos)
{
	if (index == vertices.size()) {
		vertices.push_back(pos);
		bounding.Combine(pos);
		return true;		
	} else if (index >= 0 && index < vertices.size()) {
		vertices.insert(vertices.begin() + index, pos);
		bounding.Combine(pos);
		return true;
	} else {
		return false;
	}
}

bool PolylineEditor::RemoveVertex(std::vector<sm::vec2>& vertices, sm::rect& bounding,
								  const sm::vec2& pos)
{
	std::vector<sm::vec2>::iterator itr = vertices.begin();
	for ( ; itr != vertices.end(); ++itr) 
	{
		if (*itr != pos) {
			continue;
		}
		vertices.erase(itr);
		if (IsVertexOnRect(pos, bounding)) {
			bounding = CalcBounding(vertices);
		}
		return true;
	}
	return false;
}

bool PolylineEditor::ChangeVertex(std::vector<sm::vec2>& vertices, sm::rect& bounding,
								  const sm::vec2& from, const sm::vec2& to)
{
	int index = 0;
	for (int n = vertices.size(); index < n; ++index)
	{
		if (vertices[index] == from)
			break;
	}

	if (index == vertices.size()) {
		return false;
	}

	vertices[index] = to;
	
	if (IsVertexOnRect(from, bounding)) {
		bounding = CalcBounding(vertices);
	} else {
		bounding.Combine(to);
	}

	return true;
}

bool PolylineEditor::IsVertexOnRect(const sm::vec2& v, const sm::rect& r)
{
	return (v.x == r.xmin || v.x == r.xmax ||
		    v.y == r.ymin || v.y == r.ymax);
}

sm::rect PolylineEditor::CalcBounding(const std::vector<sm::vec2>& vertices)
{
	sm::rect bounding;
	bounding.MakeEmpty();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		bounding.Combine(vertices[i]);
	}
	return bounding;
}

}
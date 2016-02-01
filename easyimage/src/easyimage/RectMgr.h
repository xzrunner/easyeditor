#ifndef _EASYIMAGE_RECT_MGR_H_
#define _EASYIMAGE_RECT_MGR_H_

#include <ee/Vector.h>

#include <json/json.h>

namespace ee { class Rect; }

namespace eimage
{

class StagePanel;

class RectMgr
{
public:
	struct Node
	{
		const ee::Rect* rect;
		ee::Vector pos;

		Node() : rect(NULL) {}
	};

public:
	~RectMgr();

	void Load(const Json::Value& value);
	void Store(Json::Value& value) const;

	void Draw() const;

	void Insert(const ee::Rect& rect, bool force = false);
	bool Remove(const ee::Vector& pos);

	ee::Vector QueryNearestAxis(const ee::Vector& pos, const ee::Rect* except = NULL) const;

	Node QueryNode(const ee::Vector& pos) const;

	ee::Rect* QueryRect(const ee::Vector& pos) const;

	bool MoveNode(const Node& node, const ee::Vector& pos);
	void MoveRect(const ee::Rect* rect, const ee::Vector& from, const ee::Vector& to);

	const std::vector<ee::Rect*>& GetAllRect() const {
		return m_rects;
	}

	void Clear();

private:
	std::vector<ee::Rect*> m_rects;

}; // RectMgr

}

#endif // _EASYIMAGE_RECT_MGR_H_

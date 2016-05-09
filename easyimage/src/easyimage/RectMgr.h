#ifndef _EASYIMAGE_RECT_MGR_H_
#define _EASYIMAGE_RECT_MGR_H_

#include <SM_Vector.h>

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
		sm::vec2 pos;

		Node() : rect(NULL) {}
	};

public:
	~RectMgr();

	void Load(const Json::Value& value);
	void Store(Json::Value& value) const;

	void Draw() const;

	void Insert(const ee::Rect& rect, bool force = false);
	bool Remove(const sm::vec2& pos);

	sm::vec2 QueryNearestAxis(const sm::vec2& pos, const ee::Rect* except = NULL) const;

	Node QueryNode(const sm::vec2& pos) const;

	ee::Rect* QueryRect(const sm::vec2& pos) const;

	bool MoveNode(const Node& node, const sm::vec2& pos);
	void MoveRect(const ee::Rect* rect, const sm::vec2& from, const sm::vec2& to);

	const std::vector<ee::Rect*>& GetAllRect() const {
		return m_rects;
	}

	void Clear();

private:
	std::vector<ee::Rect*> m_rects;

}; // RectMgr

}

#endif // _EASYIMAGE_RECT_MGR_H_

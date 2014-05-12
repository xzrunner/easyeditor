#ifndef _EASYIMAGE_RECT_MGR_H_
#define _EASYIMAGE_RECT_MGR_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class RectMgr
{
public:
	struct Node
	{
		const d2d::Rect* rect;
		d2d::Vector pos;

		Node() : rect(NULL) {}
	};

public:
	~RectMgr();

	void load(const Json::Value& value);
	void store(Json::Value& value) const;

	void draw() const;

	void insert(const d2d::Rect& rect);
	bool remove(const d2d::Vector& pos);

	d2d::Vector queryNearestAxis(const d2d::Vector& pos, const d2d::Rect* except = NULL) const;

	Node queryNode(const d2d::Vector& pos) const;

	d2d::Rect* queryRect(const d2d::Vector& pos) const;

	bool moveNode(const Node& node, const d2d::Vector& pos);
	void moveRect(const d2d::Rect* rect, const d2d::Vector& from, const d2d::Vector& to);

	const std::vector<d2d::Rect*>& getAllRect() const {
		return m_rects;
	}

private:
	void clear();

private:
	std::vector<d2d::Rect*> m_rects;

}; // RectMgr

}

#endif // _EASYIMAGE_RECT_MGR_H_

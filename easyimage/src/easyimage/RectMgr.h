#ifndef _EASYIMAGE_RECT_MGR_H_
#define _EASYIMAGE_RECT_MGR_H_



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

	void load(const Json::Value& value);
	void store(Json::Value& value) const;

	void draw() const;

	void insert(const ee::Rect& rect, bool force = false);
	bool remove(const ee::Vector& pos);

	ee::Vector queryNearestAxis(const ee::Vector& pos, const ee::Rect* except = NULL) const;

	Node queryNode(const ee::Vector& pos) const;

	ee::Rect* queryRect(const ee::Vector& pos) const;

	bool moveNode(const Node& node, const ee::Vector& pos);
	void moveRect(const ee::Rect* rect, const ee::Vector& from, const ee::Vector& to);

	const std::vector<ee::Rect*>& getAllRect() const {
		return m_rects;
	}

	void clear();

private:
	std::vector<ee::Rect*> m_rects;

}; // RectMgr

}

#endif // _EASYIMAGE_RECT_MGR_H_

#ifndef _EASYEDITOR_TEXTURE_PACKER_NODE_H_
#define _EASYEDITOR_TEXTURE_PACKER_NODE_H_

namespace ee
{

class TPNode
{
public:
	TPNode();
	TPNode(int width, int height);
	~TPNode() {
		// todo
	}

	TPNode* Insert(int w, int h);
	void Clear();

	bool IsRoomEnough(int w, int h) const;

	// fixme
	void SetChild(TPNode* n) { m_child = n; }

	float GetCenterX() const { return 0.5f * (m_xmin + m_xmax); }
	float GetCenterY() const { return 0.5f * (m_ymin + m_ymax); }

	bool IsRotated() const { return m_is_rotated; }

	int GetMinX() const { return m_xmin; }
	int GetMaxX() const { return m_xmax; }
	int GetMinY() const { return m_ymin; }
	int GetMaxY() const { return m_ymax; }

	int GetWidth() const { return m_xmax - m_xmin; }
	int GetHeight() const { return m_ymax - m_ymin; }

private:
	TPNode* Split(int w, int h);

	float GetArea() const;
	float GetMaxLength() const;

	void UpdateRemain();

private:
	bool m_used;

	int m_xmin, m_ymin, m_xmax, m_ymax;
	bool m_is_rotated;
	bool m_is_split_y;

	TPNode* m_next;
	TPNode* m_child;
	TPNode* m_parent;

	int m_remain_area;
	int m_remain_len;
	int m_remain_space;

}; // TPNode

}

#endif // _EASYEDITOR_TEXTURE_PACKER_NODE_H_
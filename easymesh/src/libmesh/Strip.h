#ifndef _EASYMESH_STRIP_H_
#define _EASYMESH_STRIP_H_

#include "EditShape.h"

namespace emesh
{

class Node;
class Triangle;

class Strip : public EditShape
{
public:
	Strip();
	Strip(const Strip& strip);
	Strip(const d2d::Image& image);

	//
	// Cloneable interface
	//
	virtual Strip* Clone() const;

	//
	// Shape interface
	//
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;

	virtual void OffsetUV(float dx, float dy);

	//
	// EditShape interface
	//
	virtual void InsertNode(const d2d::Vector& p);
	virtual void RemoveNode(const d2d::Vector& p);
	virtual d2d::Vector* FindNode(const d2d::Vector& p);
	virtual void MoveNode(d2d::Vector* src, const d2d::Vector& dst);

	virtual void TraverseShape(d2d::Visitor& visitor) const {}
	virtual bool RemoveShape(d2d::Shape* shape) { return false; }
	virtual bool InsertShape(d2d::Shape* shape) { return false; }
	virtual bool ClearShape() { return false; }

	virtual void Reset();
	virtual void Clear();

	static const char* GetType() { return "strip"; }

private:
	void InitBound();
	
	void RefreshTriangles();
	void CopyTriangles(const Strip& strip);

	void AbsorbNodeToRegion(d2d::Vector& node);
	void RemoveCornerFromNodes();

	void GetTransList(std::vector<std::pair<d2d::Vector, d2d::Vector> >& trans_list) const;

	static void TranslateNode(Node* node, const std::vector<std::pair<d2d::Vector, d2d::Vector> >& trans_list);
	static void TranslateNode(d2d::Vector& node, const std::vector<std::pair<d2d::Vector, d2d::Vector> >& trans_list);

	static void MapUV2XY(const std::vector<d2d::Vector>& nodes, int index, const d2d::Vector& pos, 
		std::vector<std::pair<d2d::Vector, d2d::Vector> >& trans_list);

private:
	class NodeList
	{
	public:
		void Reset(const d2d::Vector& begin,
			const d2d::Vector& end);

		void Insert(const d2d::Vector& p);
		void Remove(int idx);

		int GetNodeInsertPos(const d2d::Vector& p, d2d::Vector& nearest);

		int QueryIndex(const d2d::Vector& p, float radius) const;
		d2d::Vector* QueryPointer(const d2d::Vector& p, float radius);

		bool IsRegionContain(const d2d::Vector& p) const;

		int Size() const { return m_ori.size(); }

		void Sort();

	private:
		static void Insert(std::vector<d2d::Vector>& nodes, const d2d::Vector& p);

	public:
		std::vector<d2d::Vector> m_ori, m_ext;

	}; // NodeList

private:
	NodeList m_left_nodes, m_right_nodes;

	float m_uv_offset;

}; // Strip 

}

#endif // _EASYMESH_STRIP_H_
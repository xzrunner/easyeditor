#ifndef _EASYMESH_STRIP_H_
#define _EASYMESH_STRIP_H_

#include "EditableMesh.h"

namespace emesh
{

class Node;
class Triangle;

class Strip : public EditableMesh
{
public:
	Strip();
	Strip(const Strip& strip);
	Strip(const ee::Image& image);

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
	// EditableMesh interface
	//
	virtual void InsertNode(const ee::Vector& p);
	virtual void RemoveNode(const ee::Vector& p);
	virtual ee::Vector* FindNode(const ee::Vector& p);
	virtual void MoveNode(ee::Vector* src, const ee::Vector& dst);

	virtual void TraverseMesh(ee::Visitor& visitor) const {}
	virtual bool RemoveMesh(ee::Shape* shape) { return false; }
	virtual bool InsertMesh(ee::Shape* shape) { return false; }
	virtual bool ClearMesh() { return false; }

	virtual void Reset();
	virtual void Clear();

	static const char* GetType() { return "strip"; }

private:
	void InitBound();
	
	void RefreshTriangles();
	void CopyTriangles(const Strip& strip);

	void AbsorbNodeToRegion(ee::Vector& node);
	void RemoveCornerFromNodes();

	void GetTransList(std::vector<std::pair<ee::Vector, ee::Vector> >& trans_list) const;

	static void TranslateNode(Node* node, const std::vector<std::pair<ee::Vector, ee::Vector> >& trans_list);
	static void TranslateNode(ee::Vector& node, const std::vector<std::pair<ee::Vector, ee::Vector> >& trans_list);

	static void MapUV2XY(const std::vector<ee::Vector>& nodes, int index, const ee::Vector& pos, 
		std::vector<std::pair<ee::Vector, ee::Vector> >& trans_list);

private:
	class NodeList
	{
	public:
		void Reset(const ee::Vector& begin,
			const ee::Vector& end);

		void Insert(const ee::Vector& p);
		void Remove(int idx);

		int GetNodeInsertPos(const ee::Vector& p, ee::Vector& nearest);

		int QueryIndex(const ee::Vector& p, float radius) const;
		ee::Vector* QueryPointer(const ee::Vector& p, float radius);

		bool IsRegionContain(const ee::Vector& p) const;

		int Size() const { return m_ori.size(); }

		void Sort();

	private:
		static void Insert(std::vector<ee::Vector>& nodes, const ee::Vector& p);

	public:
		std::vector<ee::Vector> m_ori, m_ext;

	}; // NodeList

private:
	NodeList m_left_nodes, m_right_nodes;

	float m_uv_offset;

}; // Strip 

}

#endif // _EASYMESH_STRIP_H_
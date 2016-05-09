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
	Strip(const ee::Symbol* base);

	//
	// Cloneable interface
	//
	virtual Strip* Clone() const;

	//
	// Mesh interface
	//
	virtual void Load(const Json::Value& value);
	virtual void Store(Json::Value& value) const;

	virtual void OffsetUV(float dx, float dy);

	virtual void TraverseMesh(ee::Visitor& visitor) const {}
	virtual bool RemoveMesh(ee::Shape* shape) { return false; }
	virtual bool InsertMesh(ee::Shape* shape) { return false; }
	virtual bool ClearMesh() { return false; }

	virtual void Reset();
	virtual void Clear();

	void InsertNode(const sm::vec2& p);
	void RemoveNode(const sm::vec2& p);
	sm::vec2* FindNode(const sm::vec2& p);
	void MoveNode(sm::vec2* src, const sm::vec2& dst);

	static const char* GetType() { return "strip"; }

private:
	void InitBound();
	
	void RefreshTriangles();
	void CopyTriangles(const Strip& strip);

	void AbsorbNodeToRegion(sm::vec2& node);
	void RemoveCornerFromNodes();

	void GetTransList(std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list) const;

	static void TranslateNode(Node* node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);
	static void TranslateNode(sm::vec2& node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);

	static void MapUV2XY(const std::vector<sm::vec2>& nodes, int index, const sm::vec2& pos, 
		std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);

private:
	class NodeList
	{
	public:
		void Reset(const sm::vec2& begin,
			const sm::vec2& end);

		void Insert(const sm::vec2& p);
		void Remove(int idx);

		int GetNodeInsertPos(const sm::vec2& p, sm::vec2& nearest);

		int QueryIndex(const sm::vec2& p, float radius) const;
		sm::vec2* QueryPointer(const sm::vec2& p, float radius);

		bool IsRegionContain(const sm::vec2& p) const;

		int Size() const { return m_ori.size(); }

		void Sort();

	private:
		static void Insert(std::vector<sm::vec2>& nodes, const sm::vec2& p);

	public:
		std::vector<sm::vec2> m_ori, m_ext;

	}; // NodeList

private:
	NodeList m_left_nodes, m_right_nodes;

	float m_uv_offset;

}; // Strip 

}

#endif // _EASYMESH_STRIP_H_
#ifndef _EASYMESH_STRIP_H_
#define _EASYMESH_STRIP_H_

#include "Shape.h"

#include <drag2d.h>

namespace emesh
{

class Node;
class Triangle;
class Strip : public Shape
{
public:
	Strip();
	Strip(const Strip& strip);
	Strip(const d2d::Image& image);

	//
	// ICloneable interface
	//
	virtual Strip* clone() const;

	//
	// IShape interface
	//
	virtual void Insert(const d2d::Vector& p);
	virtual void Remove(const d2d::Vector& p);
	virtual d2d::Vector* Find(const d2d::Vector& p);
	virtual void Move(d2d::Vector* src, const d2d::Vector& dst);

	virtual void Reset();
	virtual void Clear();

	virtual void ResetUVOffset(float dx, float dy);

private:
	void InitBound();
	
	void RefreshTriangles();

	void CopyTriangles(const Strip& strip);

	void AbsorbNodeToRegion(d2d::Vector& node);
	void RemoveCornerFromNodes();

	void GetTransList(std::vector<std::pair<d2d::Vector, d2d::Vector> >& trans_list) const;
	void TransTriangles(const std::vector<std::pair<d2d::Vector, d2d::Vector> >& trans_list);

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
		int InsertExt(const d2d::Vector& p, const std::vector<std::pair<d2d::Vector, d2d::Vector> >& trans_list, d2d::Vector& nearest);
		int InsertExt(const d2d::Vector& p, d2d::Vector& nearest);
		void Remove(int idx);

		int QueryIndex(const d2d::Vector& p) const;
		d2d::Vector* QueryPointer(const d2d::Vector& p);

		bool IsRegionContain(const d2d::Vector& p) const;

		int Size() const { return m_ori.size(); }

		void Sort();

		const std::vector<d2d::Vector>& GetExtNodes() const {
			return m_ext;
		}
		std::vector<d2d::Vector>& GetExtNodes() {
			return m_ext;
		}

	private:
		static void Insert(std::vector<d2d::Vector>& nodes, const d2d::Vector& p);

	private:
		std::vector<d2d::Vector> m_ori, m_ext;

	}; // NodeList

private:
	NodeList m_left_nodes, m_right_nodes;

	float m_uv_offset;

}; // Strip 

}

#endif // _EASYMESH_STRIP_H_
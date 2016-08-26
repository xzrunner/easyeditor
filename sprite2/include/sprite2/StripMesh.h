#ifndef _SPRITE2_STRIP_MESH_H_
#define _SPRITE2_STRIP_MESH_H_

#include "Mesh.h"

#include <SM_Vector.h>

namespace s2
{

class StripMesh : public VIRTUAL_INHERITANCE Mesh
{
public:
	StripMesh();
	StripMesh(const StripMesh& strip);
	StripMesh(const Symbol* base);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual StripMesh* Clone() const { return new StripMesh(*this); }

protected:
	void InitBound();

	void RefreshTriangles();
	void CopyTriangles(const StripMesh& strip);

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

protected:
	NodeList m_left_nodes, m_right_nodes;

	VI_DUMMY_FUNC

}; // StripMesh

}

#endif // _SPRITE2_STRIP_MESH_H_
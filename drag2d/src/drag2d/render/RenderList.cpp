#include "RenderList.h"
#include "ShaderNew.h"

namespace d2d
{

RenderList* RenderList::m_instance = NULL;
RenderList* RenderList::Instance()
{
	if (!m_instance) {
		m_instance = new RenderList();
	}
	return m_instance;
}

void RenderList::Insert(int tex, Vector vertices[4], Vector texcoords[4])
{
	Node n;
	n.tex = tex;
	for (int i = 0; i < 4; ++i)
	{
		n.vertices[i] = vertices[i];
		n.texcoords[i] = texcoords[i];
	}
	m_nodes.push_back(n);
}

void RenderList::Flush()
{
	ShaderNew* shader = ShaderNew::Instance();
	shader->sprite();

	for (int i = 0, sz = m_nodes.size(); i < sz; ++i)
	{
		const Node& n = m_nodes[i];
		float vb[16];
		for (int j = 0; j < 4; ++j)
		{
			vb[j*4] = n.vertices[j].x;
			vb[j*4+1] = n.vertices[j].y;
			vb[j*4+2] = n.texcoords[j].x;
			vb[j*4+3] = n.texcoords[j].y;
		}
		shader->Draw(vb, n.tex);
	}
	m_nodes.clear();
}

}
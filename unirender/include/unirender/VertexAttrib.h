#ifndef _UNIRENDER_VERTEX_ATTRIB_H_
#define _UNIRENDER_VERTEX_ATTRIB_H_

#include <string>

namespace ur
{

struct VertexAttrib
{
	std::string name;
	int n;
	int size;
	int tot_size;

	VertexAttrib() : n(0), size(0), tot_size(0) {}
	VertexAttrib(const std::string& name, int n, int size) {
		Assign(name, n, size);
	}
	void Assign(const std::string& name, int n, int size) {
		this->name = name;
		this->n = n;
		this->size = size;
		this->tot_size = size * n;
	}
};

}

#endif // _UNIRENDER_VERTEX_ATTRIB_H_
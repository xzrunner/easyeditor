#ifndef _EASYDB_NODE_H_
#define _EASYDB_NODE_H_

#include <string>

namespace edb
{

class Node
{
public:
	Node();
	Node(const std::string& path);
	virtual ~Node();

	virtual int Type() const = 0;

	virtual void Store(std::ofstream& fout) const;
	virtual void Load(std::ifstream& fin);

	const std::string& GetPath() const { return m_path; }
	void SetPath(const std::string& path) { m_path = path; }

	int GetID() const { return m_id; }
	void SetID(int id) { m_id = id; }

private:
	std::string m_path;

	int m_id;

}; // Node

}

#endif // _EASYDB_NODE_H_
#include "Database.h"
#include "LeafNode.h"
#include "IndexNode.h"
#include "Serializer.h"

#include <ee/std_functor.h>

#include <gum/FilepathHelper.h>

#include <wx/dir.h>
#include <wx/filename.h>
//#include <wx/log.h>

#include <algorithm>
#include <fstream>

namespace edb
{

//SINGLETON_DEFINITION(Database);

Database::Database()
	: m_root(-1)
{
}

Database::~Database()
{
	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<Node>());
}

void Database::Store(const std::string& filepath) const
{
	std::ofstream fout(filepath.c_str(), std::ios::binary);

	// dir path
	Serializer::WriteStr(fout, m_dir_path);

	// root
	fout.write(reinterpret_cast<const char*>(&m_root), sizeof(int));

	// nodes
	int n = m_nodes.size();
	fout.write(reinterpret_cast<const char*>(&n), sizeof(int));	
	for (int i = 0; i < n; ++i) 
	{
		const Node* node = m_nodes[i];

		int type = node->Type();
		fout.write(reinterpret_cast<const char*>(&type), sizeof(uint8_t));

		m_nodes[i]->Store(fout);
	}

	fout.close();
}

void Database::Load(const std::string& filepath)
{
	Clear();

	std::ifstream fin(filepath.c_str(), std::ios::binary);

	// dir path
	Serializer::ReadStr(fin, m_dir_path);

	// root
	fin.read(reinterpret_cast<char*>(&m_root), sizeof(int));

	// nodes
	int n = 0;
	fin.read(reinterpret_cast<char*>(&n), sizeof(int));
	for (int i = 0; i < n; ++i)
	{
		int type = 0;
		fin.read(reinterpret_cast<char*>(&type), sizeof(uint8_t));

		Node* node = NULL;
		switch (type)
		{
		case NODE_INDEX:
			node = new IndexNode;
			break;
		case NODE_LEAF:
			node = new LeafNode;
			break;
		}
		node->Load(fin);
		node->SetID(i);

		m_nodes.push_back(node);

		std::string path = gum::FilepathHelper::Absolute(m_dir_path, node->GetPath());
		m_map.insert(std::make_pair(path, node->GetID()));
	}

	fin.close();
}

void Database::Build(const std::string& dir_path)
{
	if (dir_path == m_dir_path) {
		return;
	}

	Clear();

	m_dir_path = dir_path;

	m_root = BuildNode(dir_path);

	BuildConnection();
}

int Database::Query(const std::string& path) const
{
	std::map<std::string, int>::const_iterator itr = m_map.find(path);
	if (itr == m_map.end()) {
		return -1;
	} else {
		return itr->second;
	}
}

const Node* Database::Fetch(int idx) const
{
	if (idx < 0 || idx >= m_nodes.size()) {
		return NULL;
	} else {
		return m_nodes[idx];
	}
}

void Database::Clear()
{
	m_dir_path.clear();

	m_root = -1;

	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<Node>());
	m_nodes.clear();

	m_map.clear();
}

int Database::BuildNode(const std::string& path)
{
	Node* node = NULL;

	std::string relative_path = gum::FilepathHelper::Relative(m_dir_path, path);

	if (wxFileName::DirExists(path))
	{
//		wxLogDebug("++ dir: %s", path.c_str());

		IndexNode* idx_node = new IndexNode(relative_path);

		std::vector<int> children;
	 	wxDir dir(path);
	 	if (dir.IsOpened()) 
		{
		 	wxString filename;
		 	bool cont = dir.GetFirst(&filename);
		 	while (cont) {
				int child = BuildNode(path + "\\" + filename.ToStdString());
				children.push_back(child);
		 		cont = dir.GetNext(&filename);
		 	}
	 	}
		idx_node->SetChildren(children);

		node = idx_node;
	}
	else 
	{
//		wxLogDebug("file: %s", path.c_str());
		node = new LeafNode(relative_path);
	}

	int id = m_nodes.size();
	node->SetID(id);
	m_nodes.push_back(node);
	m_map.insert(std::make_pair(gum::FilepathHelper::Format(path), id));	

	return id;
}

void Database::BuildConnection()
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i)
	{
		Node* node = m_nodes[i];
		if (node->Type() == NODE_LEAF) {
			static_cast<LeafNode*>(node)->BuildConnection(*this);
		}
	}
}

}
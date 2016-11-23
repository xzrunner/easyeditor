#ifndef _EASYSPRPACKER_PACK_TO_BIN_H_
#define _EASYSPRPACKER_PACK_TO_BIN_H_

#include <string>
#include <vector>

namespace ee { class TexturePacker; }

namespace esprpacker
{

class PackNode;

class PackToBin
{
public:
	static void Pack(const std::string& filepath, const ee::TexturePacker& tp, 
		bool compress, float scale);

private:
	class Page
	{
	public:
		Page();
		~Page();

		void Insert(PackNode* node);

		int NodeSize() const { return m_nodes.size(); }

	public:
		int m_id_min, m_id_max;
		std::vector<PackNode*> m_nodes;

	}; // Page

	static void PageIndex(const std::string& filepath, 
		const std::vector<Page*>& pages, bool compress);
	static void PackPage(const std::string& filepath, const Page& page, 
		const ee::TexturePacker& tp, bool compress, float scale);

}; // PackToBin

}

#endif // _EASYSPRPACKER_PACK_TO_BIN_H_
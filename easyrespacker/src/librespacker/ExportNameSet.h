#ifndef _LIBRESPACKER_EXPORT_NAME_SET_H_
#define _LIBRESPACKER_EXPORT_NAME_SET_H_



namespace librespacker
{

class IPackNode;

class ExportNameSet
{
public:
	
	void LoadExport(const ee::Symbol* symbol, IPackNode* node);

	const std::map<std::string, int>& GetData() const { return m_map; }

private:
	std::map<std::string, int> m_map;

}; // ExportNameSet

}

#endif // _LIBRESPACKER_EXPORT_NAME_SET_H_
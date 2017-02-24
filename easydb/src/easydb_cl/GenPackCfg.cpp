//#include "GenPackCfg.h"
//
//namespace edb
//{
//
//void GenPackCfg::FetchAllDirs(const std::string& src, std::vector<std::string>& dst)
//{
//	class DirTraverser : public wxDirTraverser
//	{
//	public:
//		DirTraverser(wxArrayString& files) 
//			: m_files(files) {}
//
//		virtual wxDirTraverseResult OnFile(const wxString& filename)
//		{
//			m_files.Add(filename);
//			return wxDIR_IGNORE;
//		}
//
//		virtual wxDirTraverseResult OnDir(const wxString& dirname)
//		{
//			return wxDIR_IGNORE;
//		}
//
//	private:
//		wxArrayString& m_files;
//
//	}; // DirTraverser
//
//	wxArrayString dirs;
//	DirTraverser traverser(dirs);
//
//	wxDir dir(src);
//	dir.Traverse(traverser);
//
//	dst.reserve(dirs.size());
//	for (int i = 0, n = dirs.size(); i < n; ++i) {
//		dst.push_back(dirs[i]);
//	}
//}
//
//void GenPackCfg::GenIDCfg(const std::string& dir, const std::string& path, int start_id)
//{
//	Json::Value val;
//
//	
//}
//
//}
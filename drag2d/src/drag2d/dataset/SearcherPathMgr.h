#ifndef _D2D_SEARCHER_PATH_MGR_H_
#define _D2D_SEARCHER_PATH_MGR_H_

#include <wx/string.h>
#include <vector>

namespace d2d
{

class SearcherPathMgr
{	
public:
	static SearcherPathMgr* Instance();

	// absolute path
	bool IsExist(const wxString& filepath) const;

	// only one packed res for cocpack
	void ResetPackRes(const std::string& dirpath);

private:
	SearcherPathMgr();
	~SearcherPathMgr();

	void Clear();

	void LoadConfig();

private:
	struct PackedRes
	{
	public:
		PackedRes(const std::string& res_dir,
			const std::string& dirpath);
		PackedRes(const std::string& dirpath);

		bool IsExist(const wxString& filepath) const;

		bool CanHandleFilepath(const wxString& filepath) const;

	private:
		void LoadCfgDir(const std::string& dirpath, bool need_trans = false);
		void LoadCfgFile(const std::string& filepath);

	private:
		std::string m_res_dir;
		wxString m_res_dir_absolute;

		// todo map
		std::vector<std::string> m_packed_files;

	}; // PackedRes

	std::vector<PackedRes*> m_search_path;

private:
	static SearcherPathMgr* m_instance;

}; // SearcherPathMgr

}

#endif // _D2D_SEARCHER_PATH_MGR_H_
#ifndef _EASYCOMPLEX_SEARCH_PATH_MGR_H_
#define _EASYCOMPLEX_SEARCH_PATH_MGR_H_

#include <drag2d.h>

namespace ecomplex
{

class SearchPathMgr
{	
public:
	static SearchPathMgr* Instance();

	// absolute path
	bool IsExist(const wxString& filepath) const;

private:
	SearchPathMgr();
	~SearchPathMgr();

	void LoadConfig();

private:
	struct PackedRes
	{
	public:
		PackedRes(const std::string& res_dir,
			const std::string& dirpath);

		bool IsExist(const wxString& filepath) const;

		const wxString& GetDirAbsolute() const {
			return m_res_dir_absolute;
		}

	private:
		void LoadCfgDir(const std::string& dirpath);
		void LoadCfgFile(const std::string& filepath);

	private:
		std::string m_res_dir;
		wxString m_res_dir_absolute;

		// todo map
		std::vector<std::string> m_packed_files;

	}; // PackedRes

	std::vector<PackedRes*> m_search_path;

private:
	static SearchPathMgr* m_instance;

}; // SearchPathMgr

}

#endif // _EASYCOMPLEX_SEARCH_PATH_MGR_H_
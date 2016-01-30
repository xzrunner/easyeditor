#ifndef _EASYEDITOR_PATH_MGR_H_
#define _EASYEDITOR_PATH_MGR_H_

#include <vector>

namespace ee
{

class SearcherPathMgr
{	
public:
	static SearcherPathMgr* Instance();

	// absolute path
	bool IsExist(const std::string& filepath) const;

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

		bool IsExist(const std::string& filepath) const;

		bool CanHandleFilepath(const std::string& filepath) const;

	private:
		void LoadCfgDir(const std::string& dirpath, bool need_trans = false);
		void LoadCfgFile(const std::string& filepath);

	private:
		std::string m_res_dir;
		std::string m_res_dir_absolute;

		// todo map
		std::vector<std::string> m_packed_files;

	}; // PackedRes

	std::vector<PackedRes*> m_search_path;

private:
	static SearcherPathMgr* m_instance;

}; // SearcherPathMgr

}

#endif // _EASYEDITOR_PATH_MGR_H_
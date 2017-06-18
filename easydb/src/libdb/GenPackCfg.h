//#ifndef _EASYDB_GEN_PACK_CFG_H_
//#define _EASYDB_GEN_PACK_CFG_H_
//
//namespace edb
//{
//
//class GenPackCfg : public ICommand
//{
//public:
//	GenPackCfg() {}
//
//	//
//	// interface ICommand
//	//
//	virtual std::string Command() const;
//	virtual std::string Description() const;
//	virtual std::string Usage() const;
//	virtual int Run(int argc, char *argv[]);
//
//	static ICommand* Create() { return new GenPackCfg(); }
//
//private:
//	void FetchAllDirs(const std::string& src, std::vector<std::string>& dst);
//
//	void GenIDCfg(const std::string& dir, const std::string& path, int start_id);
//
//}; // GenPackCfg
//
//}
//
//#endif // _EASYDB_GEN_PACK_CFG_H_
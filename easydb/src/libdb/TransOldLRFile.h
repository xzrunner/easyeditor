#ifndef _EASYDB_TRANS_OLD_LR_FILE_H_
#define _EASYDB_TRANS_OLD_LR_FILE_H_

#include "ICommand.h"

#include <json/json.h>

#include <set>

namespace edb
{

class TransOldLRFile : public ICommand
{
public:
	TransOldLRFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransOldLRFile(); }

private:
	void Run(const std::string& filepath);

private:
	class Layer
	{
	public:
		Layer(const std::string& name, const Json::Value& old_val);

		bool InsertSprite(const std::string& filepath, const Json::Value& spr_val);

		void BuildNewJsonVal(Json::Value& new_val) const;

	private:
		std::string m_name;
		bool m_editable, m_visible;
		std::set<std::string> m_symbol_paths;

		std::vector<Json::Value> m_spr_values;

	}; // Layer

}; // TransOldLRFile

}

#endif // _EASYDB_TRANS_OLD_LR_FILE_H_
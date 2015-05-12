#ifndef _EASYDB_LR_LAYERS_PACK_H_
#define _EASYDB_LR_LAYERS_PACK_H_

#include "ICommand.h"

#include <drag2d.h>

namespace lr { class Grids; }

namespace edb
{

class LRLayersPack : public ICommand
{
public:
	LRLayersPack() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRLayersPack(); }

private:
	void Run(const std::string& filepath);

	void ParserPolygon(const Json::Value& src_val, const std::string& dir,
		const lr::Grids& grids, int layer_idx, const char* name, Json::Value& out_val);
	void ParserPoint(const Json::Value& src_val, const std::string& dir,
		int layer_idx, const char* name, Json::Value& out_val);
	void ParserCamera(const Json::Value& src_val, int layer_idx, 
		const char* name, Json::Value& out_val);
	void ParserCharacter(const Json::Value& src_val, int layer_idx, 
		const char* name, Json::Value& out_val);

}; // LRLayersPack

}

#endif // _EASYDB_LR_LAYERS_PACK_H_
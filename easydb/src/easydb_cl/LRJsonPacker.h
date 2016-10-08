#ifndef _EASYDB_LR_JSON_PACKER_H_
#define _EASYDB_LR_JSON_PACKER_H_

#include "ICommand.h"

#include <SM_Vector.h>

#include <json/json.h>

#include <set>

namespace lr { class Grids; }
namespace ee { class Shape; }

namespace edb
{

class LRJsonPacker : public ICommand
{
public:
	LRJsonPacker() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRJsonPacker(); }

	void Run(const std::string& filepath);

private:
	void PackGraphics(const std::string& filepath);
	void PackLogic(const std::string& filepath);

	void ParserShapeLayer(const Json::Value& src_val, const lr::Grids& grids, bool force_grids,
		int layer_idx, const char* name, Json::Value& out_val);
	void ParserShapeFromSprite(const Json::Value& src_val, const lr::Grids& grids, bool force_grids,
		const char* name, Json::Value& out_val);
	void ParserShapeFromShape(const Json::Value& src_val, const lr::Grids& grids, bool force_grids,
		const char* name, Json::Value& out_val);	
	void ParserShape(const ee::Shape* shape, const sm::vec2& offset, float angle,
		const lr::Grids& grids, bool force_grids, Json::Value& out_val);
	
	void ParserPoint(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val);
	void ParserPointFromSprite(const Json::Value& src_val, const char* name, Json::Value& out_val);

	void ParserCamera(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val);
	void ParserCameraFromSprite(const Json::Value& src_val, const char* name, Json::Value& out_val);

	void ParserCharacter(const Json::Value& src_val, const lr::Grids& grids, int layer_idx, const char* name, Json::Value& out_val);
	void ParserCharacterFromSprite(const Json::Value& src_val, const lr::Grids& grids, const char* name, Json::Value& out_val);

	void ParserLevel(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val);
	void ParserLevelFromSprite(const Json::Value& src_val, const char* name, Json::Value& out_val);

	void ParserSpecial(const Json::Value& src_val, const std::string& name, Json::Value& out_val);
	void ParserSpecialFromSprite(const Json::Value& src_val, const std::string& name, bool is_layer2, Json::Value& out_val);
	void ParserSpecialLayer(const Json::Value& spr_val, const std::string& name, Json::Value& out_val);
	void ParserChildren(const Json::Value& spr_val, std::set<std::string>& children_names);

	void ParserParticleLayer(const Json::Value& spr_val, Json::Value& out_val, bool top_layer);

	void ParserSprTag(const std::string& tag, const std::string& symbol_path, Json::Value& out_val);
	void ParserSprTag(const std::string& tag, Json::Value& out_val);

private:
	std::string m_dir;

}; // LRJsonPacker

}

#endif // _EASYDB_LR_JSON_PACKER_H_
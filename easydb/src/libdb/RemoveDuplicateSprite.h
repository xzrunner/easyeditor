#ifndef _EASYDB_REMOVE_DUPLICATE_SPRITE_H_
#define _EASYDB_REMOVE_DUPLICATE_SPRITE_H_

#include "ICommand.h"

#include <SM_Vector.h>

#include <json/json.h>

namespace edb
{

class RemoveDuplicateSprite : public ICommand
{
public:
	RemoveDuplicateSprite() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new RemoveDuplicateSprite(); }

private:
	void Run(const std::string& filepath, const std::string& key);

	void RemoveDuplicate(const Json::Value& old_val, Json::Value& new_val) const;

private:
	struct Sprite
	{
		Sprite(const Json::Value& json_val);

		bool operator == (const Sprite& spr) const;

		int id;

		std::string filepath;

		sm::vec2 pos;
		float angle;
		sm::vec2 scale;
		sm::vec2 shear;

		Json::Value json_val;

	}; // Sprite

	class SprCmp
	{
	public:
		bool operator() (const Sprite* lhs, const Sprite* rhs) const {
			return lhs->id < rhs->id;
		}
	}; // SprCmp


}; // RemoveDuplicateSprite

}

#endif // _EASYDB_REMOVE_DUPLICATE_SPRITE_H_
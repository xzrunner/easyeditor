#ifndef _EASYDB_TRANSLATE_JSON_H_
#define _EASYDB_TRANSLATE_JSON_H_

#include "ICommand.h"

#include <SM_Vector.h>

#include <json/json.h>

namespace edb
{

class TranslateJson : public ICommand
{
public:
	TranslateJson() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TranslateJson(); }

private:
	void Trigger(const std::string& dir, const sm::vec2& offset) const;

	void TranslateComplex(const std::string& filepath, const sm::vec2& offset) const;

	void TranslateSprite(Json::Value& spr_val, const sm::vec2& offset) const;

}; // TranslateJson

}

#endif // _EASYDB_TRANSLATE_JSON_H_
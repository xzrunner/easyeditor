#pragma once

#include <ee/Sprite.h>

#include <rapidjson/document.h>

#include <string>

namespace ecomplex
{

class Symbol;

class StoreToJson
{
public:
	static void StoreToFile(const std::string& filepath, const Symbol& sym, 
		const std::string& dir, bool store_history);

	static void StoreToMem(rapidjson::Document& doc, const Symbol& sym,
		const std::string& dir);

private:
	static void CenterSymbol(Symbol& sym);
	static rapidjson::Value StoreSpr(const ee::SprPtr& spr, const std::string& dir,
		rapidjson::MemoryPoolAllocator<>& alloc);

	static void CheckName(const Symbol& sym);

	static void CheckDuplicateName(const Symbol& sym);
	static void CheckNameDiff(const Symbol& sym);
	static void CheckAnchorName(const Symbol& sym);

//	static void StoreAction(const Symbol& sym, Json::Value& val);

}; // StoreToBin

}
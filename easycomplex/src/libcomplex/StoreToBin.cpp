#include "StoreToBin.h"
#include "StoreToJson.h"
#include "Symbol.h"

#include <sns/NodeFactory.h>
#include <sns/NodeSym.h>
#include <memmgr/LinearAllocator.h>

#include <json/value.h>

#include <string>
#include <fstream>

namespace ecomplex
{

void StoreToBin::Store(const std::string& filepath, const Symbol& sym, const std::string& dir)
{
	// store to json
	rapidjson::Value val;
	StoreToJson::StoreToMem(val, sym, dir);

	// load sns from json
	mm::LinearAllocator alloc;
	sns::NodeSym* sns_sym = sns::NodeFactory::CreateNodeSym(alloc, val, sns::NODE_COMPLEX);

	// store sns to bin
	uint8_t* data = nullptr;
	size_t len = 0;
	sns_sym->StoreToBin(&data, len);
	delete sns_sym;

	// bin to file
	std::ofstream fout(filepath, std::ios::binary);
	fout.write(reinterpret_cast<const char*>(data), len);
	fout.close();
	delete data;
}

}
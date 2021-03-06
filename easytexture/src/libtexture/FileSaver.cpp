#include "FileSaver.h"
#include "Symbol.h"

#include <easyshape.h>

#include <ee/FileHelper.h>
#include <ee/Exception.h>

namespace etexture
{

void FileSaver::Store(const char* filepath, const std::shared_ptr<Symbol>& sym)
{
	auto& shapes = sym->GetPolygons();
	std::string dir = ee::FileHelper::GetFileDir(filepath);

	Json::Value value;
	for (size_t i = 0; i < shapes.size(); ++i) {
		auto shape = std::dynamic_pointer_cast<ee::Shape>(shapes[i]);
		value["shapes"][i] = eshape::FileIO::StoreShape(dir, *shape);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	if (fout.fail()) {
		throw ee::Exception("Can't save file: %s !", filepath);
	}
	writer.write(fout, value);
	fout.close();
}

}
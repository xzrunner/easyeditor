#include "FileSaver.h"
#include "Symbol.h"

#include <easyshape.h>

#include <ee/FileHelper.h>
#include <ee/Exception.h>

namespace etexture
{

void FileSaver::Store(const char* filepath, const Symbol* symbol)
{
	const std::vector<ee::Shape*>& shapes = symbol->GetAllShapes();
	std::string dir = ee::FileHelper::GetFileDir(filepath);

	Json::Value value;
	for (size_t i = 0; i < shapes.size(); ++i) {
		ee::Shape* shape = shapes[i];
		value["shapes"][i] = eshape::FileIO::StoreShape(dir, shape);
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
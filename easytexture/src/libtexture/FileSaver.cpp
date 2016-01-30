#include "FileSaver.h"
#include "Symbol.h"

#include <drag2d.h>
#include <easyshape.h>

namespace etexture
{

void FileSaver::Store(const char* filepath, const Symbol* symbol)
{
	const std::vector<d2d::Shape*>& shapes = symbol->GetAllShapes();
	std::string dir = d2d::FileHelper::GetFileDir(filepath);

	Json::Value value;
	for (size_t i = 0; i < shapes.size(); ++i) {
		d2d::Shape* shape = shapes[i];
		value["shapes"][i] = libshape::FileIO::StoreShape(dir, shape);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	if (fout.fail()) {
		throw d2d::Exception("Can't save file: %s !", filepath);
	}
	writer.write(fout, value);
	fout.close();
}

}
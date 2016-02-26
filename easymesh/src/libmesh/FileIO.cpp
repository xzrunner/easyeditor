#include "FileIO.h"
#include "Symbol.h"
#include "Strip.h"
#include "Mesh.h"

#include <ee/FileHelper.h>
#include <ee/panel_msg.h>

#include <fstream>

namespace emesh
{

void FileIO::Store(const char* filepath, const Symbol* symbol)
{
	Json::Value value;

	value["name"] = symbol->name;

	const Shape* shape = symbol->getShape();
	if (shape) {
		shape->Store(value);
	} else {
		return;
	}

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	value["image"] = ee::FileHelper::GetRelativePath(dir, symbol->GetImagePath());

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const char* filepath, Symbol* symbol)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (!value["image"].isNull())
	{
		std::string dir = ee::FileHelper::GetFileDir(filepath);
		std::string path = ee::FileHelper::GetAbsolutePath(dir, value["image"].asString());
		// todo Release symbol
		//symbol = ee::SymbolMgr::Instance()->fetchSymbol(path);
		symbol->LoadImage(path);
// 		symbol->Release();
	}
	else
	{
		return;
	}

	std::string type = value["type"].asString();
	Shape* shape = NULL;
	if (type == Strip::GetType()) {
		shape = new Strip(*symbol->getImage());
	} else if (type == Mesh::GetType()) {
		shape = new Mesh(*symbol->getImage());		
	}
	if (shape)
	{
		shape->Load(value);
		symbol->SetShape(shape);
		shape->Release();
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
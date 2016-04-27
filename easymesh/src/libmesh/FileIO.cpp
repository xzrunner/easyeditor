#include "FileIO.h"
#include "Symbol.h"
#include "Strip.h"
#include "Network.h"

#include <ee/FileHelper.h>
#include <ee/panel_msg.h>

#include <fstream>

namespace emesh
{

void FileIO::Store(const char* filepath, const Symbol* symbol)
{
	Json::Value value;

	value["name"] = symbol->name;

	const Mesh* mesh = symbol->GetMesh();
	if (mesh) {
		mesh->Store(value);
	} else {
		return;
	}

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	value["base_symbol"] = ee::FileHelper::GetRelativePath(dir, symbol->GetImagePath());

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

	if (!value["base_symbol"].isNull())
	{
		std::string dir = ee::FileHelper::GetFileDir(filepath);
		std::string path = ee::FileHelper::GetAbsolutePath(dir, value["base_symbol"].asString());
		// todo Release symbol
		//symbol = ee::SymbolMgr::Instance()->fetchSymbol(path);
		symbol->LoadImage2(path);
// 		symbol->Release();
	}
	else
	{
		return;
	}

	std::string type = value["type"].asString();
	Mesh* mesh = NULL;
	if (type == Strip::GetType()) {
		mesh = new Strip(*symbol->GetImage());
	} else if (type == Network::GetType()) {
		mesh = new Network(*symbol->GetImage());		
	}
	if (mesh)
	{
		mesh->Load(value);
		symbol->SetMesh(mesh);
		mesh->Release();
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
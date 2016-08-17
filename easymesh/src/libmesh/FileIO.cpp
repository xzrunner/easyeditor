#include "FileIO.h"
#include "Symbol.h"
#include "Strip.h"
#include "Network.h"

#include <ee/FileHelper.h>
#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>

#include <fstream>

namespace emesh
{

void FileIO::Store(const char* filepath, const Symbol* sym)
{
	Json::Value value;

	value["name"] = sym->name;

	const Mesh* mesh = sym->GetMesh();
	if (mesh) {
		mesh->Store(value);
	} else {
		return;
	}

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	value["base_symbol"] = ee::FileHelper::GetRelativePath(dir, sym->GetMesh()->GetBaseSymbol()->GetFilepath());

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const char* filepath, Symbol* sym)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	ee::Symbol* base_symbol = NULL;
	if (!value["base_symbol"].isNull())
	{
		std::string dir = ee::FileHelper::GetFileDir(filepath);
		std::string path = ee::FileHelper::GetAbsolutePath(dir, value["base_symbol"].asString());
		base_symbol = ee::SymbolMgr::Instance()->FetchSymbol(path);
	}
	else
	{
		return;
	}

	std::string type = value["type"].asString();
	Mesh* mesh = NULL;
	if (type == Strip::GetType()) {
		mesh = new Strip(base_symbol);
	} else if (type == Network::GetType()) {
		mesh = new Network(base_symbol);
	}
	if (mesh)
	{
		mesh->Load(value);
		sym->SetMesh(mesh);
		mesh->RemoveReference();
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
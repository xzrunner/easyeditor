#include "FileIO.h"
#include "OceanMesh.h"
#include "Symbol.h"

#include <ee/JsonSerializer.h>
#include <ee/FileHelper.h>
#include <ee/ImageSymbol.h>
#include <ee/SymbolMgr.h>

namespace eterrain2d
{

void FileIO::StoreOceanMesh(const OceanMesh* ocean, const std::string& dir, Json::Value& value)
{
	if (const eshape::PolygonShape* shape = ocean->GetBounding()) {
		ee::JsonSerializer::Store(shape->GetVertices(), value["bound"]);
	}
	if (const ee::ImageSymbol* img = ocean->GetImage0()) {
		value["tex0"] = ee::FileHelper::GetRelativePath(dir, img->GetFilepath());
	}
	if (const ee::ImageSymbol* img = ocean->GetImage1()) {
		value["tex1"] = ee::FileHelper::GetRelativePath(dir, img->GetFilepath());
	}

	value["wave"]["open"] = ocean->IsWaveOpen();
	value["wave"]["row"] = ocean->GetWaveGridRow();
	value["wave"]["col"] = ocean->GetWaveGridCol();
	value["wave"]["speed"] = ocean->GetWaveSpeed() / 0.1f;
	value["wave"]["height"] = ocean->GetWaveHeight();

	value["uv_move"]["open"] = ocean->IsUVMoveOpen();
	value["uv_move"]["speed"]["x"] = ocean->GetUVMoveSpeed().x / 0.01f;
	value["uv_move"]["speed"]["y"] = ocean->GetUVMoveSpeed().y / 0.01f;

	value["tex_blend"]["open"] = ocean->IsTexBlendOpen();
	value["tex_blend"]["speed"] = ocean->GetTexBlendSpeed() / 0.01f;

	value["bound_lock"] = ocean->IsBoundLockOpen();
}

OceanMesh* FileIO::LoadOceanMesh(const std::string& dir, const Json::Value& value)
{
	if (value["bound"].isNull() || value["tex0"].isNull()) {
		return NULL;
	}

	std::vector<ee::Vector> bounding;
	ee::JsonSerializer::Load(value["bound"], bounding);
	eshape::PolygonShape* shape = new eshape::PolygonShape(bounding);

	std::string tex0_path = dir + "\\" + value["tex0"].asString();
	ee::Symbol* tex0 = ee::SymbolMgr::Instance()->FetchSymbol(tex0_path);
	shape->SetMaterialTexture(static_cast<ee::ImageSymbol*>(tex0));

	OceanMesh* ocean = new OceanMesh(shape, static_cast<ee::ImageSymbol*>(tex0));

	if (!value["tex1"].isNull()) {
		std::string tex1_path = dir + "\\" + value["tex1"].asString();
		ee::Symbol* tex1 = ee::SymbolMgr::Instance()->FetchSymbol(tex1_path);
		ocean->SetImage1(static_cast<ee::ImageSymbol*>(tex1));
	}

	ocean->OpenWave(value["wave"]["open"].asBool());
	ocean->SetSmallGridSize(value["wave"]["row"].asInt(), value["wave"]["col"].asInt());
	ocean->SetWaveInfo(value["wave"]["speed"].asDouble() * 0.1f, value["wave"]["height"].asDouble());

	ocean->OpenUVMove(value["uv_move"]["open"].asBool());
	float x = value["uv_move"]["speed"]["x"].asDouble() * 0.01f,
		y = value["uv_move"]["speed"]["y"].asDouble() * 0.01f;
	ocean->SetTexcoordsSpeed(ee::Vector(x, y));

	ocean->OpenBlend(value["tex_blend"]["open"].asBool());
	ocean->SetBlendSpeed(value["tex_blend"]["speed"].asDouble() * 0.01f);

	ocean->SetBoundLock(value["bound_lock"].asBool());

	return ocean;
}

void FileIO::StoreSymbol(const char* filepath, const Symbol* symbol)
{
	Json::Value src_value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, src_value);

	Json::Value dst_value;
	dst_value["bg"] = src_value["bg"];

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	std::vector<OceanMesh*> meshes = symbol->GetOceans();
	for (int i = 0, n = meshes.size(); i < n; ++i) {
		FileIO::StoreOceanMesh(meshes[i], dir, dst_value["ocean"][i]);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, dst_value);
	fout.close();
}

}
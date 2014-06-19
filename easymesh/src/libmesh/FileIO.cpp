#include "FileIO.h"
#include "Symbol.h"
#include "Mesh.h"
#include "Triangle.h"

namespace emesh
{

void FileIO::store(const char* filepath, const Symbol* symbol)
{
	Json::Value value;

	value["name"] = symbol->name;

	const d2d::Image* image = symbol->getImage();
	if (image)
	{
		value["image"] = image->filepath().ToStdString();
	}

	const Mesh* mesh = symbol->getMesh();
	if (mesh)
	{
		const Mesh::Region& r = mesh->m_region;
		value["bound"]["xmin"] = r.rect.xMin;
		value["bound"]["xmax"] = r.rect.xMax;
		value["bound"]["ymin"] = r.rect.yMin;
		value["bound"]["ymax"] = r.rect.yMax;
		d2d::JsonTools::store(r.nodes, value["nodes"]);
		storeTransform(mesh, value["transform"]);
	}

	wxString dir = d2d::FilenameTools::getFileDir(filepath) + "\\";
	value["image"] = d2d::FilenameTools::getRelativePath(dir,
		symbol->getImage()->filepath()).ToStdString();

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::load(const char* filepath, Symbol* symbol)
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
		wxString dir = d2d::FilenameTools::getFileDir(filepath);
		wxString path = d2d::FilenameTools::getAbsolutePath(dir, value["image"].asString());
		// todo release symbol
		//symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
 		d2d::BitmapMgr::Instance()->getItem(path, &symbol->m_bitmap);
 		d2d::ImageMgr::Instance()->getItem(path, &symbol->m_image);
// 		symbol->release();
	}
	else
	{
		return;
	}

	if (!value["bound"].isNull())
	{
		Mesh* mesh = new Mesh(*symbol->m_image, false);
		d2d::Rect& r = mesh->m_region.rect;
		r.xMin = value["bound"]["xmin"].asDouble();
		r.xMax = value["bound"]["xmax"].asDouble();
		r.yMin = value["bound"]["ymin"].asDouble();
		r.yMax = value["bound"]["ymax"].asDouble();
		d2d::JsonTools::load(value["nodes"], mesh->m_region.nodes);
		mesh->refreshTriangles();
		loadTransform(value["transform"], mesh);

		if (symbol->m_mesh)
 			symbol->m_mesh->release();
 		symbol->m_mesh = mesh;
	}
}

void FileIO::loadStruct(const char* filepath, Mesh& mesh)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

 	mesh.m_region.nodes.clear();

	d2d::JsonTools::load(value["nodes"], mesh.m_region.nodes);
	mesh.refreshTriangles();

	// todo: need set triangles if need tween
	//       or index nodes
}

void FileIO::storeTransform(const Mesh* mesh, Json::Value& value)
{
	std::vector<d2d::Vector> transform;
	for (int i = 0, n = mesh->m_tris.size(); i < n; ++i)
	{
		Triangle* tri = mesh->m_tris[i];
		for (int i = 0; i < 3; ++i)
			transform.push_back(tri->nodes[i]->xy);
	}
	d2d::JsonTools::store(transform, value);
}

void FileIO::loadTransform(const Json::Value& value, Mesh* mesh)
{
	std::vector<d2d::Vector> transform;
	d2d::JsonTools::load(value, transform);
	int itr = 0;
	for (int i = 0, n = mesh->m_tris.size(); i < n; ++i)
	{
		Triangle* tri = mesh->m_tris[i];
		for (int i = 0; i < 3; ++i)
			tri->nodes[i]->xy = transform[itr++];
	}
}

}
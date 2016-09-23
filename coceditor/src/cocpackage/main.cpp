#include <easycoco.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyicon.h>

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>
#include <ee/Config.h>
#include <ee/SearcherPathMgr.h>
#include <ee/Exception.h>

#include <iostream>

#define CHARACTER

std::vector<const ee::Symbol*> SYMBOLS;

ecoco::epd::TextureMgr TEX_MGR;

std::set<std::string> IGNORE_LIST;

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &eanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &eanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);
}

void LoadAllFilesSorted(const std::string& dir, std::set<std::string>& files_sorted)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		files_sorted.insert(filepath);
	}
}

void LoadFromDir(const std::string& dir)
{
	std::set<std::string> files_sorted;
	LoadAllFilesSorted(dir, files_sorted);

	std::set<std::string>::iterator itr = files_sorted.begin();
	for ( ; itr != files_sorted.end(); ++itr) 
	{
		if (ee::FileType::IsType(*itr, ee::FILE_COMPLEX)
			|| ee::FileType::IsType(*itr, ee::FILE_ANIM))
		{
			// todo release symbol
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(*itr);
			SYMBOLS.push_back(sym);
		}
	}
}

void LoadFromList(const std::string& list)
{
	std::set<std::string> names;

	std::string ext = ee::FileHelper::GetExtension(list);
	ee::StringHelper::ToLower(ext);
	if (ext == "txt")
	{
		std::locale::global(std::locale(""));
		std::ifstream fin(list.c_str());
		std::locale::global(std::locale("C"));

		std::string str;
		while (getline(fin, str)) {
			names.insert(str);
		}

		fin.close();
	}
	else if (ext == "json")
	{
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(list.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int i = 0;
		Json::Value itemValue = value["items"][i++];
		while (!itemValue.isNull()) {
			std::string name = itemValue["name"].asString();
			names.insert(name);
			itemValue = value["items"][i++];
		}
	}
	else 
	{
		std::cerr << "unsupported format " << ext << std::endl;
		return;
	}

	std::string dir = ee::FileHelper::GetFileDir(list);

	std::set<std::string> files_sorted;
	LoadAllFilesSorted(dir, files_sorted);

	std::set<std::string>::iterator itr = files_sorted.begin();
	for ( ; itr != files_sorted.end(); ++itr) 
	{
		if (ee::FileType::IsType(*itr, ee::FILE_COMPLEX) || 
			ee::FileType::IsType(*itr, ee::FILE_ANIM))
		{
			// todo release symbol
			ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(*itr);
			std::set<std::string>::iterator itr = names.find(sym->name);
			if (itr == names.end()) {
//					sym->release();
			} else {
				SYMBOLS.push_back(sym);
			}
		}
	}
}

void LoadTexturePacker(std::string texpackerpath)
{
	size_t i = 1;
	while (true)
	{
		std::string path = texpackerpath + ee::StringHelper::ToString(i) + ".json";
		if (wxFileName::FileExists(path))
			TEX_MGR.Add(path, i-1);
		else
			break;
		++i;
	}
}

void DetectScale(int idx, char *argv[], float& scale)
{
	std::string opt = argv[idx];
	if (opt == "-s" || opt == "--scale") {
		scale = atof(argv[idx+1]);
	}
}

void DetectIgnoreFile(int idx, char *argv[], std::string& ignore_file)
{
	std::string opt = argv[idx];
	if (opt == "-i" || opt == "--ignore") {
		ignore_file = argv[idx+1];
	}	
}

void ParamsDetection(int argc, char *argv[], float& scale, std::string& ignore_file)
{
	for (int i = 5; i < argc; i += 2)
	{
		DetectScale(i, argv, scale);
		DetectIgnoreFile(i, argv, ignore_file);
	}
}

void LoadIgnoreList(const std::string& filename)
{
	std::string ext = ee::FileHelper::GetExtension(filename);
	ee::StringHelper::ToLower(ext);

	std::set<std::string> list;

	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str());
	std::locale::global(std::locale("C"));

	std::string str;
	while (getline(fin, str)) {
		list.insert(str);
	}

	fin.close();

	IGNORE_LIST = list;
}

int main(int argc, char *argv[])
{
	wxInitializer initializer;
	if ( !initializer )
	{
		fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
		return -1;
	}

	if (argc < 5) {
		std::cerr << "Need Resource Dir & Texture Packer Filepath & TP dir & Output filepath !" << std::endl;
		return 1;
	}

	ee::Config::Instance()->EnableUseDTex(false);
	ee::Config::Instance()->EnableRender(false);

	InitSymbolCreators();

	float gscale = 1.0f;
	std::string ignore_file;
	if (argc > 5) {
		ParamsDetection(argc, argv, gscale, ignore_file);
	}
	if (!ignore_file.empty()) {
		LoadIgnoreList(ignore_file);
	}

	std::string tp_path = argv[2];
	ee::SearcherPathMgr::Instance()->ResetPackRes(tp_path);

	std::string path = argv[1];
	bool is_dir = false;
	int pos_dot = path.find_last_of('.');
	int pos_divide = std::max((int)path.find_last_of('/'), (int)path.find_last_of('\\'));
	if (pos_dot == std::string::npos || pos_dot < pos_divide) {
		is_dir = true;
	}

	try {
		std::cout << "Load json data \n";
		if (is_dir) {
			LoadFromDir(path);
		} else {
			LoadFromList(path);
		}

		std::cout << "Load tp data \n";

		std::string tp_dir = argv[3];
		TEX_MGR.SetSrcDataDir(tp_dir);

		LoadTexturePacker(tp_path);
		ecoco::epd::CocoPacker packer(SYMBOLS, TEX_MGR);
		packer.Parser();
		packer.Output(argv[4]);
	} catch (ee::Exception& e) {
		std::cerr << e.What() << std::endl;
		return 1;
	}

	return 0;
}
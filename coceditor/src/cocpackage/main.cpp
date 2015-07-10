#include <iostream>

#include <easycoco.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyicon.h>

#define CHARACTER

std::vector<const d2d::ISymbol*> SYMBOLS;

libcoco::TextureMgr TEX_MGR;

std::set<std::string> IGNORE_LIST;

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);
}

void LoadAllFilesSorted(const std::string& dir, std::set<std::string>& files_sorted)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);

	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		files_sorted.insert(filepath.ToStdString());
	}
}

void LoadFromDir(const std::string& dir)
{
	std::set<std::string> files_sorted;
	LoadAllFilesSorted(dir, files_sorted);

	try {
		std::set<std::string>::iterator itr = files_sorted.begin();
		for ( ; itr != files_sorted.end(); ++itr) 
		{
			if (d2d::FileNameParser::isType(*itr, d2d::FileNameParser::e_complex)
				|| d2d::FileNameParser::isType(*itr, d2d::FileNameParser::e_anim))
			{
				// todo release symbol
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(*itr);
				SYMBOLS.push_back(symbol);
			}
		}
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void LoadFromList(const std::string& list)
{
	std::set<std::string> names;

	wxString ext = d2d::FilenameTools::getExtension(list).Lower();
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

	wxString dir = d2d::FilenameTools::getFileDir(list);

	std::set<std::string> files_sorted;
	LoadAllFilesSorted(dir.ToStdString(), files_sorted);

	try {
		std::set<std::string>::iterator itr = files_sorted.begin();
		for ( ; itr != files_sorted.end(); ++itr) 
		{
			if (d2d::FileNameParser::isType(*itr, d2d::FileNameParser::e_complex)
				|| d2d::FileNameParser::isType(*itr, d2d::FileNameParser::e_anim))
			{
				// todo release symbol
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(*itr);
				std::set<std::string>::iterator itr = names.find(symbol->name);
				if (itr == names.end()) {
//					symbol->release();
				} else {
					SYMBOLS.push_back(symbol);
				}
			}
		}
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void LoadTexturePacker(std::string texpackerpath)
{
	size_t i = 1;
	while (true)
	{
		std::string path = texpackerpath + wxString::FromDouble(i) + ".json";
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
	for (int i = 4; i < argc; i += 2)
	{
		DetectScale(i, argv, scale);
		DetectIgnoreFile(i, argv, ignore_file);
	}
}

void LoadIgnoreList(const std::string& filename)
{
	wxString ext = d2d::FilenameTools::getExtension(filename).Lower();

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

	if (argc < 4) {
		std::cerr << "Need Resource Dir & Texture Packer Filepath & Output filepath !" << std::endl;
		return 1;
	}

	d2d::Config::Instance()->EnableRender(false);

	InitSymbolCreators();

	float gscale = 1.0f;
	std::string ignore_file;
	if (argc > 4) {
		ParamsDetection(argc, argv, gscale, ignore_file);
	}
	if (!ignore_file.empty()) {
		LoadIgnoreList(ignore_file);
	}

	std::string texpackerpath = argv[2];
	d2d::SearcherPathMgr::Instance()->ResetPackRes(texpackerpath);

	std::string path = argv[1];
	bool is_dir = false;
	int pos_dot = path.find_last_of('.');
	int pos_divide = std::max((int)path.find_last_of('/'), (int)path.find_last_of('\\'));
	if (pos_dot == std::string::npos || pos_dot < pos_divide) {
		is_dir = true;
	}

	std::cout << "Load json data \n";
	if (is_dir) {
		LoadFromDir(path);
	} else {
		LoadFromList(path);
	}

	std::cout << "Load tp data \n";

	LoadTexturePacker(texpackerpath);

	try {
		libcoco::CocoPacker packer(SYMBOLS, TEX_MGR);
		packer.Parser();
		packer.Output(argv[3]);
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
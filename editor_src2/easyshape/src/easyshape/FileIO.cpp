
#include "FileIO.h"
#include "LibraryPanel.h"
#include "LibraryItem.h"
#include "Context.h"

#include <easyshape.h>

using namespace eshape;

void FileIO::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	LibraryPanel* library = Context::Instance()->library;
	{
		int i = 0;
		Json::Value shapeValue = value["shape"][i++];
		d2d::LibraryList* list = library->getShapePage()->getList();
		while (!shapeValue.isNull()) {
			LibraryItem* item = loadShapeItem(shapeValue.asString());
			list->insert(item);
			shapeValue = value["shape"][i++];
		}
	}
	{
		int i = 0;
		Json::Value imageValue = value["image"][i++];
		d2d::LibraryList* list = library->getImagePage()->getList();
		while (!imageValue.isNull()) {
			LibraryItem* item = loadImageItem(imageValue.asString());
			list->insert(item);
			imageValue = value["image"][i++];
		}
	}

	int index = static_cast<wxNotebook*>(library->getNotebook())->GetSelection();
	wxBookCtrlEvent event;
 	event.SetSelection(index);
 	library->onPageChanged(event);
}

void FileIO::store(const char* filename)
{
	Json::Value value;

	std::string dlg = d2d::FilenameTools::getFileDir(filename);

	{
		std::vector<LibraryItem*> items;
		Context::Instance()->library->getShapePage()->getList()->
			traverse(d2d::FetchAllVisitor<LibraryItem>(items));
		for (size_t i = 0, n = items.size(); i < n; ++i)
		{
			if (items[i]->getFilepath().empty())
				value["shape"][i] = (dlg + "\\" + items[i]->getName() + 
					"_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json").ToStdString();
			else
				value["shape"][i] = items[i]->getFilepath().ToStdString();
			store(items[i], dlg);
		}
	}
	{
		std::vector<LibraryItem*> items;
		Context::Instance()->library->getImagePage()->getList()->
			traverse(d2d::FetchAllVisitor<LibraryItem>(items));
		for (size_t i = 0, n = items.size(); i < n; ++i)
		{
			value["image"][i] = items[i]->getFilepath().ToStdString();
			store(items[i], dlg);
		}
	}

	Json::StyledStreamWriter writer;
	std::ofstream fout(filename);
	writer.write(fout, value);
	fout.close();
}

LibraryItem* FileIO::loadShapeItem(const std::string& filepath)
{
	LibraryItem* item = new LibraryItem(filepath);

	std::vector<d2d::IShape*>* shapes = new std::vector<d2d::IShape*>;
	item->setUserData(shapes);

	libshape::FileAdapter adapter(*shapes);
	adapter.load(filepath.c_str());

	return item;
}

LibraryItem* FileIO::loadImageItem(const std::string& filepath)
{
	LibraryItem* item = new LibraryItem(filepath);

	std::vector<d2d::IShape*>* shapes = new std::vector<d2d::IShape*>;
	item->setUserData(shapes);

	wxString shape_filepath = d2d::FilenameTools::getFilePathExceptExtension(filepath) 
		+"_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
	libshape::FileAdapter adapter(*shapes);
	adapter.load(shape_filepath);

	return item;
}

void FileIO::store(LibraryItem* item, const std::string& dlg)
{
	Json::Value value;

 	std::vector<d2d::IShape*>* shapes 
 		= static_cast<std::vector<d2d::IShape*>*>(item->getUserData());
 	for (size_t i = 0, n = shapes->size(); i < n; ++i)
 		value["shapes"][i] = libshape::FileAdapter::store((*shapes)[i]);

	wxString filepath;
	wxString ext = "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape) + ".json";
	if (item->getFilepath().empty())
		filepath = dlg + "\\" + item->getName() + ext;
	else
		filepath = d2d::FilenameTools::getFilePathExceptExtension(item->getFilepath()) + ext;

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath.fn_str());
	writer.write(fout, value);
	fout.close();
}

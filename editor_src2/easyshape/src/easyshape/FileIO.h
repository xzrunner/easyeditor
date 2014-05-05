
#ifndef ESHAPE_FILE_IO_H
#define ESHAPE_FILE_IO_H

#include <json/json.h>

#include <drag2d.h>

namespace eshape
{
	class LibraryPanel;
	class LibraryItem;

	class FileIO
	{
	public:
		static void load(const char* filename, LibraryPanel* library);
		static void store(const char* filename, LibraryPanel* library);

	private:
		static LibraryItem* loadShapeItem(const std::string& filepath);
		static LibraryItem* loadImageItem(const std::string& filepath);

		static void store(LibraryItem* item, const std::string& dlg);

	}; // FileIO
}

#endif // ESHAPE_FILE_IO_H
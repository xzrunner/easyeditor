#ifndef FORMATION_FILE_IO_H
#define FORMATION_FILE_IO_H

#include <json/json.h>

#include <drag2d.h>

namespace formation
{
	class StagePanel;

	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static d2d::ISprite* load(const Json::Value& value,
			const std::string& dlg);
		static Json::Value store(const d2d::ISprite* sprite,
			const std::string& dlg);

	}; // FileIO
}

#endif // FORMATION_FILE_IO_H
#ifndef FORMATION_FILE_IO_H
#define FORMATION_FILE_IO_H

#include <json/json.h>



namespace formation
{
	class StagePanel;

	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static ee::ISprite* load(const Json::Value& value,
			const std::string& dlg);
		static Json::Value store(const ee::ISprite* sprite,
			const std::string& dlg);

	}; // FileIO
}

#endif // FORMATION_FILE_IO_H
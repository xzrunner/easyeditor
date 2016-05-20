#ifndef _EASYDB_TRIM_IMAGE_H_
#define _EASYDB_TRIM_IMAGE_H_

#include "ICommand.h"

#include <SM_Rect.h>

#include <json/json.h>

#include <stdint.h>

namespace ee { class Rect; class ImageData; }

namespace edb
{

class TrimImage : public ICommand
{
public:
	TrimImage() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new TrimImage(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_dir);

	void StoreBoundInfo(const ee::ImageData& img, const sm::rect& r, Json::Value& val) const;
	bool IsTransparent(const ee::ImageData& img, int x, int y) const;

	void Trim(const std::string& filepath);

	static int64_t GetFileModifyTime(const std::string& filepath);

private:
	class JsonConfig
	{
	public:	
		~JsonConfig();

		void LoadFromFile(const std::string& filepath);

		void Insert(const std::string& filepath, const Json::Value& val, int64_t time);

		void OutputToFile(const std::string& filepath, const std::string& dst_dir) const;

		int64_t QueryTime(const std::string& filepath) const;

	private:
		struct Item
		{
			Item() : time(0), used(false) {}

			Json::Value val;
			int64_t time;

			mutable bool used;
		};

	private:
		std::map<std::string, Item*> m_map_items;

	}; // JsonConfig

private:
	std::string m_src_dir, m_dst_dir;

	JsonConfig m_json_cfg;

}; // TrimImage

}

#endif // _EASYDB_TRIM_IMAGE_H_
#ifndef _EASYSCALE9_FILEADAPTER_H_
#define _EASYSCALE9_FILEADAPTER_H_

#include <drag2d.h>

namespace escale9
{

class FileAdapter
{
public:
	struct Entry
	{
		std::string filepath;

		std::string name;

		d2d::Vector pos;
		float angle;
		float xscale, yscale;
		float xshear, yshear;
		bool xMirror, yMirror;
	};

public:
	virtual void load(const char* filename);

private:
	Entry load(const Json::Value& value, const wxString& dir);

public:
	int type;

	float width, height;

	std::vector<Entry> m_data;

}; // FileAdapter

}

#endif // _EASYSCALE9_FILEADAPTER_H_
#ifndef _EASYDB_CHANGE_COMPLEX_ORIGIN_H_
#define _EASYDB_CHANGE_COMPLEX_ORIGIN_H_

#include "ICommand.h"

#include <SM_Vector.h>

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class ChangeComplexOrigin : public ICommand
{
public:
	ChangeComplexOrigin();

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new ChangeComplexOrigin(); }

private:
	void Trigger(const std::string& dir) const;

	void FixComplex(const std::string& filepath) const;
	void FixLR(const std::string& filepath) const;
	
	bool FixLRSpr(Json::Value& val) const;

private:
	struct Item
	{
		Item(const std::string& filename, float x, float y) 
			: filename(filename), trans(x, y) {}

		std::string filename;
		sm::vec2 trans;		// for offset point
	};

	const Item* QueryItem(const std::string& filepath) const;

private:
	std::vector<Item> m_items;
	
}; // ChangeComplexOrigin 

}

#endif // _EASYDB_CHANGE_COMPLEX_ORIGIN_H_
#ifndef _EASYEDITOR_GTXT_H_
#define _EASYEDITOR_GTXT_H_

#include "Matrix.h"

#include <string>

namespace ee
{

//class Sprite;

class GTxt
{
public:
	void LoadFont(const char* filepath);

// 	void Draw(const Sprite* spr, const Matrix& mt, 
// 		const Colorf& mul, const Colorf& add) const;
	void Draw(const Matrix& mt, const std::string& str) const;

//	void Reload(const Sprite* spr);

	static GTxt* Instance();

private:
	GTxt();

	void Init();

private:
	static GTxt* m_instance;

}; // GTxt

}

#endif // _EASYEDITOR_GTXT_H_
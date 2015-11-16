#ifndef _EASYTEXT_GTXT_H_
#define _EASYTEXT_GTXT_H_

#include <drag2d.h>

namespace etext
{

class Sprite;

class GTxt
{
public:
	void LoadFont(const char* filepath);

	void Draw(const d2d::Matrix& mt, const Sprite* spr) const;

	void ReloadTexture();
	void Clear();

	static GTxt* Instance();

private:
	GTxt();

	void Init();

private:
	static GTxt* m_instance;

}; // GTxt

}

#endif // _EASYTEXT_GTXT_H_
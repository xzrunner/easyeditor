#ifndef _SPRITE2_GTXT_H_
#define _SPRITE2_GTXT_H_

#include <CU_Singleton.h>
#include <SM_Matrix.h>

#include <string>

struct gtxt_label_style;

namespace s2
{

class Color;

class GTxt
{
public:
	void LoadFont(const std::string& name, const std::string& filepath);
	
	void Draw(const gtxt_label_style& style, const sm::mat4& mt, const Color& mul, 
		const Color& add, const std::string& text, int time, bool richtext) const;
	void Draw(const sm::mat4& mt, const std::string& str) const;

//	void Reload(const Sprite* spr);

private:
	void Init();
	
	SINGLETON_DECLARATION(GTxt)

}; // GTxt
	
}

#endif // _SPRITE2_GTXT_H_
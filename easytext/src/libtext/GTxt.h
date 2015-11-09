#ifndef _EASYTEXT_GTXT_H_
#define _EASYTEXT_GTXT_H_

namespace etext
{

class GTxt
{
public:
	void LoadFont(const char* filepath);

	void Draw(const char* str) const;

	static GTxt* Instance();

private:
	GTxt();

	void Init();

private:
	static GTxt* m_instance;

}; // GTxt

}

#endif // _EASYTEXT_GTXT_H_
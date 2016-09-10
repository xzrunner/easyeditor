#ifndef _GUM_DTEX_H_
#define _GUM_DTEX_H_

#include <CU_Singleton.h>

#include <string>
#include <map>

struct dtex_cg;

namespace s2 { class Texture; }

namespace gum
{

class DTex
{
public:
	void InitHook(void (*draw_begin)(), void (*draw_end)());

	void LoadBegin();
	int Load(const std::string& filepath, const s2::Texture* tex);
	void LoadEnd();

	float* Query(const std::string& filepath, const s2::Texture* tex, int* id);

	void Clear();

	dtex_cg* GetDtexCG();

	void OnSize(int w, int h);

	void DebugDraw() const;

protected:
	std::map<std::string, int> m_map_filepath;

	SINGLETON_DECLARATION(DTex)

}; // DTex

}

#endif // _GUM_DTEX_H_

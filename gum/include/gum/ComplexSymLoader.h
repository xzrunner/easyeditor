#ifndef _GUM_COMPLEX_SYM_LOADER_H_
#define _GUM_COMPLEX_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class ComplexSymbol; }

namespace gum
{

class ComplexSymLoader : private cu::Uncopyable
{
public:
	ComplexSymLoader(s2::ComplexSymbol* sym);
	~ComplexSymLoader();

	void LoadJson(const std::string& filepath);

public:
	struct Action
	{
		std::string name;
		std::vector<int> idx;
	};

	static void LoadJsonAction(const Json::Value& val, s2::ComplexSymbol* sym);
	static void LoadJsonAction(const Json::Value& val, std::vector<Action>& actions);

private:
	s2::ComplexSymbol* m_sym;

}; // ComplexSymLoader

}

#endif // _GUM_COMPLEX_SYM_LOADER_H_

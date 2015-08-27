#ifndef _LIBCOCO_COMPLEX_BUILDER_H_
#define _LIBCOCO_COMPLEX_BUILDER_H_

#include <map>

namespace ecomplex { class Symbol; }

namespace libcoco
{

class IPackNode;
class PackAnimation;

class ComplexBuilder
{
public:
	ComplexBuilder();
	~ComplexBuilder();

	const IPackNode* Create(const ecomplex::Symbol* symbol);

private:
	void Load(const ecomplex::Symbol* symbol, PackAnimation* anim);

private:
	std::map<const ecomplex::Symbol*, const PackAnimation*> m_map_complex;

}; // ComplexBuilder

}

#endif // _LIBCOCO_COMPLEX_BUILDER_H_
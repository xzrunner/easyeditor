#ifndef _EASYMODELING_LOVE2D_CODE_H_
#define _EASYMODELING_LOVE2D_CODE_H_

#include <vector>

namespace ebuilder { class CodeGenerator; }

namespace emodeling
{

class Body;
class Joint;

class Love2dCode
{
public:
	Love2dCode(ebuilder::CodeGenerator& gen);

	void Resolve();

private:
	void ResolveHeader();

	void ResolveLoad();
	void ResolveLoadImages();
	void ResolveLoadWorld();
	void ResolveLoadBodies();
	void ResolveLoadFixtures(Body* body);
	void ResolveLoadJoints();
	void ResolveLoadJoint(const std::vector<Joint*>& joints, 
		size_t index, const std::vector<Body*>& bodies);

	void ResolveUpdate();

	void ResolveDraw();

private:
	ebuilder::CodeGenerator& m_gen;
	
	ebuilder::CodeGenerator *m_gen_header, *m_gen_load, 
		*m_gen_update, *m_gen_draw;

}; // Love2dCode

}

#endif // _EASYMODELING_LOVE2D_CODE_H_
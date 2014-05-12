
#ifndef EMODELING_LOVE2D_CODE_H
#define EMODELING_LOVE2D_CODE_H

#include <easybuilder.h>

namespace libmodeling
{
	class Body;
	class Joint;
}

namespace emodeling
{
	class Love2dCode
	{
	public:
		Love2dCode(ebuilder::CodeGenerator& gen);

		void resolve();

	private:
		void resolveHeader();

		void resolveLoad();
		void resolveLoadImages();
		void resolveLoadWorld();
		void resolveLoadBodies();
		void resolveLoadFixtures(libmodeling::Body* body);
		void resolveLoadJoints();
		void resolveLoadJoint(const std::vector<libmodeling::Joint*>& joints, 
			size_t index, const std::vector<libmodeling::Body*>& bodies);

		void resolveUpdate();

		void resolveDraw();

	private:
		ebuilder::CodeGenerator& m_gen;
		
		ebuilder::CodeGenerator *m_genHeader, *m_genLoad, 
			*m_genUpdate, *m_genDraw;

	}; // Love2dCode
}

#endif // EMODELING_LOVE2D_CODE_H
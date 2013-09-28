#pragma once

#include <vector>

#include "common/Vector.h"

#include "AbstractAtomicOP.h"

namespace d2d
{
	class ChainShape;

	namespace edit_nodes
	{
		class ModifyNodesAOP : public AbstractAtomicOP
		{
		public:
			ModifyNodesAOP(const std::vector<ChainShape*>& chains, 
				const std::vector<std::vector<Vector> >& dstChains);

			virtual void undo();
			virtual void redo();

		private:
			std::vector<ChainShape*> m_chains;
			std::vector<std::vector<Vector> > m_src, m_dst;

		}; // ModifyNodesAOP
	}
}


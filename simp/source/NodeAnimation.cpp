#include "NodeAnimation.h"
#include "simp_define.h"
#include "ImportStream.h"
#include "Allocator.h"
#include "NodeTrans.h"
#include "from_int.h"

namespace simp
{

NodeAnimation::NodeAnimation(Allocator& alloc, ImportStream& is)
{
	n = is.UInt16();
	alloc.Alloc(LayerSize() * n);
	for (int i = 0; i < n; ++i) 
	{
		Layer* layer = &layers[i];
		layer->n = is.UInt16();
		layer->frames = static_cast<Frame**>(alloc.Alloc(SIZEOF_POINTER * layers->n));
		memset(layer->frames, 0, SIZEOF_POINTER * layers->n);
 		for (int j = 0; j < layer->n; ++j) 
		{
			void* ptr = alloc.Alloc(ALIGN_4BYTE(FrameSize()));
			Frame* frame = new (ptr) Frame();
			frame->index = is.UInt16();
			frame->tween = int2bool(is.UInt8());
			frame->n = is.UInt16();
			frame->actors = static_cast<Actor**>(alloc.Alloc(SIZEOF_POINTER * frame->n));
			memset(frame->actors, 0, SIZEOF_POINTER * frame->n);
			for (int k = 0; k < frame->n; ++k) 
			{
				void* ptr = alloc.Alloc(ALIGN_4BYTE(ActorSize()));
				Actor* actor = new (ptr) Actor();
				actor->spr = is.UInt32();
				actor->trans = NodeTrans::LoadTrans(alloc, is);
				frame->actors[k] = actor;
			}
			layer->frames[j] = frame;
		}
	}
}

int NodeAnimation::Size()
{
	return sizeof(NodeAnimation) - sizeof(Layer);
}

int NodeAnimation::ActorSize()
{
	return sizeof(Actor) + PTR_SIZE_DIFF;
}

int NodeAnimation::FrameSize()
{
	return sizeof(Frame) + PTR_SIZE_DIFF;
}

int NodeAnimation::LayerSize()
{
	return sizeof(Layer) + PTR_SIZE_DIFF;		
}

}
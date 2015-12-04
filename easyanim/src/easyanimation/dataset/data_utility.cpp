#include "data_utility.h"
#include "DataMgr.h"
#include "Layer.h"
#include "KeyFrame.h"

#include "view/ViewMgr.h"
#include "view/KeysPanel.h"

namespace eanim
{

Layer* get_curr_layer(int& col)
{
	int row;
	ViewMgr::Instance()->keys->GetSelectPos(row, col);
	if (row == -1 || col == -1) {
		return NULL;
	}

	LayersMgr& layers = DataMgr::Instance()->GetLayers();
	int index = layers.Size() - row - 1;
	Layer* layer = layers.GetLayer(index);
	return layer;
}

KeyFrame* get_curr_frame()
{
	int col;
	Layer* layer = get_curr_layer(col);
	if (!layer) {
		return NULL;
	} else {
		return layer->GetCurrKeyFrame(col + 1);
	}
}

SkeletonData* get_curr_skeleton()
{
	KeyFrame* frame = get_curr_frame();
	if (!frame) {
		return NULL;
	} else {
		return &frame->GetSkeletonData();
	}
}

}
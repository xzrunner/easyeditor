#ifndef _EASYANIM_VIEW_UTILITY_H_
#define _EASYANIM_VIEW_UTILITY_H_

namespace eanim
{

class KeyFrame;

void reload_viewlist(const KeyFrame* frame);

int get_curr_layer_index();
int get_curr_frame_index();
void get_curr_index(int& layer_idx, int& frame_idx);

}

#endif // _EASYANIM_VIEW_UTILITY_H_
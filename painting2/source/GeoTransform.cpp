#include "painting2/GeoTransform.h"

namespace pt2
{

GeoTransform::GeoTransform()
{
	Reset();
}

void GeoTransform::Reset()
{
	m_srt.Reset();
	m_mat.Identity();
}

}
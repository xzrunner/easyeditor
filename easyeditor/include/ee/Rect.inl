#ifndef _EASYEDITOR_RECT_INL_
#define _EASYEDITOR_RECT_INL_

namespace ee
{

inline
Rect::Rect() 
{ 
	MakeInfinite(); 
}

inline
Rect::Rect(const Vector& p0, const Vector& p1) 
{
	xmin = std::min(p0.x, p1.x);
	ymin = std::min(p0.y, p1.y);
	xmax = std::max(p0.x, p1.x);
	ymax = std::max(p0.y, p1.y);
}

inline
Rect::Rect(const Vector& center, float hw, float hh) 
{
	xmin = center.x - hw;
	xmax = center.x + hw;
	ymin = center.y - hh;
	ymax = center.y + hh;
}

inline
Rect::Rect(float width, float height) {
	xmin = -width * 0.5f;
	ymin = -height * 0.5f;
	xmax = -xmin;
	ymax = -ymin;
}

inline
const Rect& Rect::operator = (const Rect& aabb)
{
	xmin = aabb.xmin;
	xmax = aabb.xmax;
	ymin = aabb.ymin;
	ymax = aabb.ymax;
	return *this;
}

inline
bool Rect::IsValid() const 
{
	return xmin != FLT_MAX && ymin != FLT_MAX
		&& xmax != - FLT_MAX && ymax != - FLT_MAX
		&& xmin <= xmax && ymin <= ymax;
}

inline
void Rect::MakeInfinite() 
{
	xmin = ymin = FLT_MAX;
	xmax = ymax = - FLT_MAX;
}

inline
float Rect::Width() const 
{
	return xmax - xmin; 
}

inline
float Rect::Height() const 
{ 
	return ymax - ymin; 
}

inline
float Rect::CenterX() const 
{ 
	return (xmin + xmax) * 0.5f; 
}

inline
float Rect::CenterY() const 
{ 
	return (ymin + ymax) * 0.5f; 
}

inline
Vector Rect::Center() const 
{ 
	return Vector(CenterX(), CenterY()); 
}

inline
void Rect::Translate(const Vector& offset)
{
	xmin += offset.x;
	xmax += offset.x;
	ymin += offset.y;
	ymax += offset.y;
}

inline
void Rect::Scale(float sx, float sy)
{
	xmin *= sx;
	xmax *= sx;
	ymin *= sy;
	ymax *= sy;
}

inline
void Rect::Shear(float sx, float sy)
{
	// x' = x + y*kx
	// y' = x*ky + y
	Vector v[4];
	v[0].x = xmin + ymin*sx;
	v[0].y = xmin*sy + ymin;
	v[1].x = xmax + ymin*sx;
	v[1].y = xmax*sy + ymin;
	v[2].x = xmax + ymax*sx;
	v[2].y = xmax*sy + ymax;
	v[3].x = xmin + ymax*sx;
	v[3].y = xmin*sy + ymax;
	for (int i = 0; i < 4; ++i)
	{
		if (v[i].x < xmin)
			xmin = v[i].x;
		else if (v[i].x > xmax)
			xmax = v[i].x;
		if (v[i].y < ymin)
			ymin = v[i].y;
		else if (v[i].y > ymax)
			ymax = v[i].y;
	}
}

inline
void Rect::Combine(const Vector& pos)
{
	if (pos.x < xmin) xmin = pos.x;
	if (pos.x > xmax) xmax = pos.x;
	if (pos.y < ymin) ymin = pos.y;
	if (pos.y > ymax) ymax = pos.y;
}

inline
void Rect::Combine(const Rect& aabb)
{
	if (aabb.xmin < xmin) xmin = aabb.xmin;
	if (aabb.xmax > xmax) xmax = aabb.xmax;
	if (aabb.ymin < ymin) ymin = aabb.ymin;
	if (aabb.ymax > ymax) ymax = aabb.ymax;
}

}

#endif // _EASYEDITOR_RECT_INL_
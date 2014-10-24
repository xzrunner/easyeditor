#ifndef _EASYPARTICLE2D_VALUE_REGION_H_
#define _EASYPARTICLE2D_VALUE_REGION_H_

namespace eparticle2d
{

template<class T>
class ValueRegion
{
public:

	void Set(T begin, T end);

	T Interpolating(float p) const;
	T Random() const;

private:
	T m_begin, m_end;

}; // ValueRegion

template<class T>
inline void ValueRegion<T>::Set(T begin, T end)
{
	m_begin = begin;
	m_end = end;
}

template<class T>
inline T ValueRegion<T>::Interpolating(float p) const
{
	return m_begin + (m_end - m_begin) * p;
}

template<class T>
inline T ValueRegion<T>::Random() const
{
	float p = rand() / (float(RAND_MAX) + 1);
	return Interpolating(p);
}

}

#endif // _EASYPARTICLE2D_VALUE_REGION_H_
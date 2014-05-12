#ifndef D2D_MODULE_H
#define D2D_MODULE_H

namespace d2d
{
	class Module
	{
	public:
		virtual ~Module() {}
		virtual const char* getName() const = 0;
	};
}

#endif // D2D_MODULE_H
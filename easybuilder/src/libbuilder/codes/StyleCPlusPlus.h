#ifndef _EASYBUILDER_STYLE_CPLUSPLUS_H_
#define _EASYBUILDER_STYLE_CPLUSPLUS_H_

#include "CodeStyle.h"
#include "CodeGenerator.h"

#include <ee/StringHelper.h>

namespace ebuilder
{
namespace cpp
{

enum Type
{
	e_pointer = 0,
	e_reference,
	e_struct,
	e_null
};

static void assign(CodeGenerator& gen, 
	const std::string& name, const std::string& val)
{
	gen.line(name+" = "+val+";");
}

static void define(CodeGenerator& gen, const std::string& type, 
	const std::string& name, const std::string& val = "")
{
	if (val.empty())
		gen.line(type+" "+name+";");
	else
		gen.line(type+" "+name+" = "+val+";");
}

static void call(CodeGenerator& gen, const std::string& caller,
	Type type, const std::string func, size_t n, ...)
{
	std::string str;

	str += caller;

	switch (type)
	{
	case e_pointer:
		str += "->";
		break;
	case e_reference:
	case e_struct:
		str += ".";
		break;
	}

	str += func + "(";
	PARSER_PARAMS(n, str);
	str += ");";

	gen.line(str);
}

static std::string call(const std::string& caller, Type type, 
	const std::string func, size_t n, ...)
{
	std::string str;

	str += caller;

	switch (type)
	{
	case e_pointer:
		str += "->";
		break;
	case e_reference:
	case e_struct:
		str += ".";
		break;
	}

	str += func + "(";
	PARSER_PARAMS(n, str);
	str += ")";

	return str;
}

static std::string call(const std::string func, size_t n, ...)
{
	std::string str;
	str += func + "(";
	PARSER_PARAMS(n, str);
	str += ")";
	return str;
}

// 	static std::string create(const std::string name, size_t n, ...)
// 	{
// 		std::string str;
// 
// 		str += "new " + name;
// 
// 		str += "(";
// 		PARSER_PARAMS(n, str);
// 		str += ")";
// 
// 		return str;
// 	}

static void funcDef(CodeGenerator& gen, const std::string& type,
	const std::string& name, size_t n, ...)
{
	std::string str;
	if (type.empty())
		str = name;
	else
		str = type + " " + name; 
	str += "(";
	PARSER_PARAMS(n, str);
	str += ");";

	gen.line(str);
}

struct HeaderFile
{
public:
	HeaderFile(CodeGenerator& gen, const std::string& project,
		const std::string& file)
		: m_gen(gen)
	{
		m_flag = project + "_" + file + "_h";
		ee::StringHelper::ToUpper(m_flag);

		m_gen.line("#ifndef " + m_flag);
		m_gen.line("#define " + m_flag);
		m_gen.line();
	}
	~HeaderFile()
	{
		m_gen.line();
		m_gen.line("#endif // " + m_flag);
	}

private:
	CodeGenerator& m_gen;

	std::string m_flag;

}; // HeaderFile

struct ClassDef
{
public:
	enum Type
	{
		e_public,
		e_protected,
		e_private
	};

public:
	ClassDef(CodeGenerator& gen, const std::string& name, 
		const std::string& inherit)
		: m_gen(gen)
		, m_name(name)
	{
		if (inherit.empty())
			m_gen.line("class " + m_name);
		else
			m_gen.line("class " + m_name + " : " + inherit);
		m_gen.line("{");
	}
	~ClassDef()
	{
		m_gen.line("}; //" + m_name);
	}

	struct AccessControl
	{
		AccessControl(CodeGenerator& gen, Type type)
			: m_gen(gen)
		{
			switch (type)
			{
			case e_public:
				m_gen.line("public:");
				break;
			case e_protected:
				m_gen.line("protected:");
				break;
			case e_private:
				m_gen.line("private:");
				break;
			}
			m_gen.tab();
		}
		~AccessControl()
		{
			m_gen.detab();
		}

	private:
		CodeGenerator& m_gen;
	};

private:
	CodeGenerator& m_gen;

	std::string m_name;

}; // ClassDef

struct Namespace
{
public:
	Namespace(CodeGenerator& gen, const std::string& name)
		: m_gen(gen)
		, m_name(name)
	{
		m_gen.line("namespace " + m_name);
		m_gen.line("{");
		m_gen.tab();
	}
	~Namespace()
	{
		m_gen.detab();
		m_gen.line("} //" + m_name);
	}

private:
	CodeGenerator& m_gen;

	std::string m_name;

}; // Namespace

struct FuncImpl
{
public:
	FuncImpl(CodeGenerator& gen,
		const std::string& ret,
		const std::string& name,
		size_t n, ...)
		: m_gen(gen)
	{
		std::string str;

		if (!ret.empty()) 
			str += ret + " ";
		str += name;
		str += "(";
		PARSER_PARAMS(n, str);
		str += ")";
		m_gen.line(str);

		m_gen.line("{");
		m_gen.tab();
	}
	FuncImpl(CodeGenerator& gen,
		const std::string& ret,
		const std::string& name,
		std::string defList,
		size_t n, ...)
		: m_gen(gen)
	{
		std::string str;

		if (!ret.empty()) 
			str += ret + " ";
		str += name;
		str += "(";
		PARSER_PARAMS(n, str);
		str += ")";
		m_gen.line(str);

		m_gen.tab();
		m_gen.line(": " + defList);
		m_gen.detab();

		m_gen.line("{");
		m_gen.tab();
	}
	~FuncImpl() 
	{
		m_gen.detab();
		m_gen.line("}");
	}

private:
	CodeGenerator& m_gen;

}; // FuncImpl

struct If
{
public:
	If(CodeGenerator& gen, const std::string& condition)
		: m_gen(gen)
	{
		m_gen.line("if ("+condition+")");
		m_gen.line("{");
		m_gen.tab();
	}
	~If()
	{
		m_gen.detab();
		m_gen.line("}");
	}

private:
	CodeGenerator& m_gen;

}; // If

struct DoWhile
{
public:
	DoWhile(CodeGenerator& gen, const std::string& condition)
		: m_gen(gen)
		, m_condition(condition)
	{
		m_gen.line("do");
		m_gen.line("{");
		m_gen.tab();
	}
	~DoWhile() 
	{
		m_gen.detab();
		m_gen.line("} while ("+m_condition+");");
	}

private:
	CodeGenerator& m_gen;

	std::string m_condition;

}; // DoWhile

}
}

#endif // _EASYBUILDER_STYLE_CPLUSPLUS_H_

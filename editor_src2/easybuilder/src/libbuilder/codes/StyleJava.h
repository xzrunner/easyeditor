
#ifndef LIBBUILDER_STYLE_JAVA_H
#define LIBBUILDER_STYLE_JAVA_H

#include "CodeStyle.h"
#include "CodeGenerator.h"

namespace ebuilder
{
namespace java
{
	enum CompetenceType
	{
		e_public,
		e_private
	};

	static void assign(CodeGenerator& gen, 
		const std::string& name, const std::string& val)
	{
		gen.line(name+" = "+val+";");
	}

	static void declare(CodeGenerator& gen, 
		const std::string& type, const std::string& name)
	{
		gen.line(type+" "+name+";");
	}

	static void call(CodeGenerator& gen, const std::string& caller,
		const std::string func, size_t n, ...)
	{
		std::string str;

		if (!caller.empty())
		{
			str += caller;
			str += ".";
		}

		str += func + "(";
		PARSER_PARAMS(n, str);
		str += ");";

		gen.line(str);
	}

	static std::string call(const std::string func, size_t n, ...)
	{
		std::string str;
		str += func + "(";
		PARSER_PARAMS(n, str);
		str += ")";
		return str;
	}

	static std::string create(const std::string name, size_t n, ...)
	{
		std::string str;

		str += "new " + name;

		str += "(";
		PARSER_PARAMS(n, str);
		str += ")";

		return str;
	}

	static void funcDef(CodeGenerator& gen, 
		CompetenceType competence, const std::string& type,
		const std::string& name, size_t n, ...)
	{
		std::string str;
		switch (competence)
		{
		case e_public:
			str += "public";
			break;
		case e_private:
			str += "private";
			break;
		}
		if (type.empty())
			str += " " + name;
		else
			str += " " + type + " " + name; 
		str += "(";
		PARSER_PARAMS(n, str);
		str += ");";

		gen.line(str);
	}

	struct FuncImpl
	{
	public:
		FuncImpl(CodeGenerator& gen, 
			CompetenceType competence, 
			const std::string& ret,
			const std::string& name,
			bool isOverride,
			size_t n, ...)
			: m_gen(gen)
		{
			if (isOverride)
				m_gen.line("@Override");
			std::string str;
			switch (competence)
			{
			case e_public:
				str += "public";
				break;
			case e_private:
				str += "private";
				break;
			}
			if (!ret.empty())
				str += " "+ret;
			str += " "+name;
			str += "(";
			PARSER_PARAMS(n, str);
			str += ") {";

			m_gen.line(str);
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
			m_gen.line("if ("+condition+") {");
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

	struct Listener
	{
	public:
		Listener(CodeGenerator& gen, const std::string& caller, 
			const std::string& func, const std::string& listener)
			: m_gen(gen)
		{
			m_gen.line(caller+"."+func+"(new "+listener+"() {");
			m_gen.tab();
		}
		~Listener()
		{
			m_gen.detab();
			m_gen.line("});");
		}

	private:
		CodeGenerator& m_gen;

	}; // Listener

	struct ClassDef
	{
	public:
		ClassDef(CodeGenerator& gen, const std::string& type,
			const std::string& name)
			: m_gen(gen)
		{
			m_gen.line(type+" class "+name+" {");
			m_gen.tab();
		}
		~ClassDef()
		{
			m_gen.detab();
			m_gen.line("}");
		}

	private:
		CodeGenerator& m_gen;

	}; // ClassDef

}
}

#endif // LIBBUILDER_STYLE_JAVA_H

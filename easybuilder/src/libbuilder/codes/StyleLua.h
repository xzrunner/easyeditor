
#ifndef LIBBUILDER_STYLE_LUA_H
#define LIBBUILDER_STYLE_LUA_H

#include "CodeStyle.h"
#include "CodeGenerator.h"

namespace ebuilder
{
namespace lua
{
	static void require(CodeGenerator& gen, const std::string& name)
	{
		gen.line("require(\""+name+"\")");
	}

	static std::string require(const std::string& name)
	{
		return "require(\""+name+"\")";
	}

	static void assign(CodeGenerator& gen, 
		const std::string& name, const std::string& val)
	{
		gen.line(name+" = "+val);
	}

	static void assign_with_end(CodeGenerator& gen, 
		const std::string& name, const std::string& val)
	{
		gen.line(name+" = "+val+",");
	}

	static void assign_with_end(CodeGenerator& gen, 
		const std::string& name, float val)
	{
		gen.line(name+" = "+wxString::FromDouble(val).ToStdString()+",");
	}

	static std::string assign(const std::string& name, const std::string& val)
	{
		return name+" = "+val;
	}

	static void assign(CodeGenerator& gen, const std::string& type,
		const std::string& name, const std::string& val)
	{
		gen.line(type+" "+name+" = "+val);
	}

	static void tableassign(CodeGenerator& gen, const std::string& name, size_t n, ...)
	{
		std::string str;

		if (!name.empty())
		{
			str += name;
			str += " = ";
		}

		str += "{ ";
		PARSER_PARAMS(n, str);
		str += " },";

		gen.line(str);
	}

	static std::string tableassign(const std::string& name, size_t n, ...)
	{
		std::string str;

		if (!name.empty())
		{
			str += name;
			str += " = ";
		}

		str += "{";
		PARSER_PARAMS(n, str);
		str += "}";

		return str;
	}

	static std::string tableassign(const std::string& name, const std::vector<std::string>& params)
	{
		std::string str;

		if (!name.empty())
		{
			str += name;
			str += " = ";
		}

		str += "{ ";
		for (int i = 0, n = params.size(); i < n; ++i) {
			str += params[i];
			if (i != params.size() - 1) {
				str += ", ";
			}
		}
		str += " },";

		return str;
	}

	static void tableassign(CodeGenerator& gen, const std::string& name, const std::vector<std::string>& params)
	{
		gen.line(tableassign(name, params));
	}

	static void call(CodeGenerator& gen, const std::string& caller,
		const std::string func, size_t n, ...)
	{
		std::string str;

		if (!caller.empty())
		{
			str += caller;
			str += ":";
		}

		str += func + "(";
		PARSER_PARAMS(n, str);
		str += ")";

		gen.line(str);
	}

	static std::string call(const std::string& caller, const std::string func, 
		size_t n, ...)
	{
		std::string str;

		if (!caller.empty())
		{
			str += caller;
			str += ":";
		}

		str += func + "(";
		PARSER_PARAMS(n, str);
		str += ")";

		return str;
	}

	struct Function
	{
	public:
		Function(CodeGenerator& gen, const std::string& name, size_t n, ...) 
			: m_gen(gen)
		{
			std::string str;
			str += "function "+name;
			str += "(";
			PARSER_PARAMS(n, str);
			str += ")";
			m_gen.line(str);
			m_gen.tab();
		}
		~Function() 
		{
			m_gen.detab();
			m_gen.line("end");
		}

	private:
		CodeGenerator& m_gen;

	}; // Function

	struct TableAssign
	{
	public:
		TableAssign(CodeGenerator& gen, const std::string& name, bool needEnd = false, bool hasEqual = true)
			: m_gen(gen)
			, m_needEnd(needEnd)
		{
			if (!name.empty())
			{
				if (hasEqual)
					m_gen.line(name+" = {");
				else
					m_gen.line(name+" {");
			}
			else
				m_gen.line("{");
			m_gen.tab();
		}
		~TableAssign()
		{
			m_gen.detab();
			if (m_needEnd)
				m_gen.line("},");
			else
				m_gen.line("}");
		}

	private:
		CodeGenerator& m_gen;

		bool m_needEnd;

	}; // TableAssign

	struct If
	{
	public:
		If(CodeGenerator& gen, const std::string& condition) 
			: m_gen(gen)
		{
			m_gen.line("if "+condition+" then");
			m_gen.tab();
		}
		~If() 
		{
			m_gen.detab();
			m_gen.line("end");
		}

	private:
		CodeGenerator& m_gen;

	}; // If

	struct For
	{
	public:
		For(CodeGenerator& gen, const std::string& condition)
			: m_gen(gen)
		{
			m_gen.line("for "+condition+" do");
			m_gen.tab();
		}
		~For()
		{
			m_gen.detab();
			m_gen.line("end");
		}

	private:
		CodeGenerator& m_gen;

	}; // For
}
}

#endif // LIBBUILDER_STYLE_LUA_H

#ifndef _EASYBUILDER_CODE_GENERATOR_H_
#define _EASYBUILDER_CODE_GENERATOR_H_

#include <vector>
#include <string>

namespace ebuilder
{

class CodeNode
{
public:
	virtual ~CodeNode() {}

	virtual bool existLine(const std::string& line) const = 0;

	virtual void toText(std::string& text) const = 0;
};

class CodeLine : public CodeNode
{
public:
	CodeLine(const std::string& line) : str(line) {}

	virtual bool existLine(const std::string& line) const {
		return str == line;
	}

	virtual void toText(std::string& text) const {
		text += str + "\n";
	}

private:
	std::string str;
};

class CodeBlock : public CodeNode
{
public:
	virtual ~CodeBlock();

	virtual bool existLine(const std::string& line) const;

	virtual void toText(std::string& text) const;

	void push_back(const CodeNode* node) {
		children.push_back(node);
	}

	void setHeader(const std::string& header) {
		m_header = header;
	}

private:
	std::vector<const CodeNode*> children;

	std::string m_header;
};

class CodeGenerator
{
public:
	void line(const std::string& s = "");
	void block(CodeGenerator& gen);

	bool existLine(const std::string& line) const;

	void tab();
	void detab();

	std::string toText() const;

private:
	CodeBlock m_block;

	std::string m_header;
};

}

#endif // _EASYBUILDER_CODE_GENERATOR_H_

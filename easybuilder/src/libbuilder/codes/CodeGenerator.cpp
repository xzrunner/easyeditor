
#include "CodeGenerator.h"

using namespace ebuilder;

CodeBlock::~CodeBlock()
{
	for (size_t i = 0, n = children.size(); i < n; ++i)
		delete children[i];
	children.clear();
}

bool CodeBlock::existLine(const std::string& line) const
{
	for (size_t i = 0, n = children.size(); i < n; ++i)
	{
		if (children[i]->existLine(line))
			return true;
	}
	return false;
}

void CodeBlock::toText(std::string& text) const
{
	if (m_header.empty())
	{
		for (size_t i = 0, n = children.size(); i < n; ++i)
			children[i]->toText(text);
	}
	else
	{
		for (size_t i = 0, n = children.size(); i < n; ++i)
		{
			text += m_header;
			children[i]->toText(text);
		}
	}
}

void CodeGenerator::line(const std::string& s/* = ""*/)
{
	m_block.push_back(new CodeLine(m_header + s));
}

void CodeGenerator::block(CodeGenerator& gen)
{
	gen.m_block.setHeader(m_header);
	m_block.push_back(&gen.m_block);
}

bool CodeGenerator::existLine(const std::string& line) const
{
	return m_block.existLine(line);
}

void CodeGenerator::tab()
{
	m_header += "\t";
}

void CodeGenerator::detab()
{
	m_header = m_header.substr(0, m_header.find_last_of("\t"));
}

std::string CodeGenerator::toText() const
{
	std::string str;
	m_block.toText(str);
	return str;
}

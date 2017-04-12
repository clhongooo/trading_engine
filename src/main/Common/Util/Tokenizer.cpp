#include "Tokenizer.h"

const string Tokenizer::DELIMITERS(" \t\n\r,-/:");

Tokenizer::Tokenizer(const std::string& s) :
	m_offset(0),
	m_string(s), 
	m_delimiters(DELIMITERS),
	_bSafeToGetNextToken(false) {}

Tokenizer::Tokenizer(const std::string& s, const std::string& delimiters) :
	m_offset(0),
	m_string(s),
	m_delimiters(delimiters),
	_bSafeToGetNextToken(false) {}

bool Tokenizer::NextToken() 
{
	return NextToken(m_delimiters);
}

bool Tokenizer::NextToken(const std::string& delimiters) 
{
	size_t i = m_string.find_first_not_of(delimiters, m_offset);
	if (string::npos == i) 
	{
		m_offset = m_string.length();
		return false;
	}

	size_t j = m_string.find_first_of(delimiters, i);
	if (string::npos == j) 
	{
		m_token = m_string.substr(i);
		m_offset = m_string.length();
		_bSafeToGetNextToken = true;
		return true;
	}

	m_token = m_string.substr(i, j - i);
	m_offset = j;
	_bSafeToGetNextToken = true;
	return true;
}

const string Tokenizer::GetToken()
{
	if (_bSafeToGetNextToken)
	{
		_bSafeToGetNextToken = false;
		return m_token;
	}
  else string("");
	return string("");
}

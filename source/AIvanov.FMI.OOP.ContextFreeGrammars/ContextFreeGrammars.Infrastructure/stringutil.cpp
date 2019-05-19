#include "stdafx.h"
#include "stringutil.h"
#include <deque>
using namespace std;

std::string concatenate(const vector<string> segments, const string &delimeter)
{
	if (segments.empty())return "";

	string result;

	int length = 0;
	for (const string &segment : segments)length += segment.size();
	length += (segments.size() - 1)*delimeter.size();

	result.reserve(length);
	for (int i = 0; i < segments.size(); i++)
	{
		result += segments[i];
		if (i + 1 < segments.size())result += delimeter;
	}

	return result;
}

std::string to_lower(const std::string &text)
{
	string result = text;

	for (char &character : result)
	{
		if (isupper(character))character = tolower(character);
	}

	return result;
}

vector<string> split(const string &text, char delimeter)
{
	vector<string> result;
	bool hadDelimeter = false;

	for (char character : text)
	{
		if (character == delimeter)hadDelimeter = true;
		else
		{
			if (hadDelimeter || result.empty())
			{
				result.emplace_back("");
				hadDelimeter = false;
			}
			result.back().push_back(character);
		}
	}

	result.shrink_to_fit();
	for (string &s : result)s.shrink_to_fit();
	return result;
}

string trim(const string &text, char trimmedCharacter)
{
	if (text.empty())return "";
	if (text.front() != trimmedCharacter && text.back() != trimmedCharacter)return text;

	int prefixLength = 0;
	while (prefixLength < text.size() && text[prefixLength] == trimmedCharacter)prefixLength++;

	if (prefixLength == text.size())return "";

	int suffixLength = 0;
	while (suffixLength < text.size() && text[text.size() - 1 - suffixLength] == trimmedCharacter)suffixLength++;

	return string{ text.begin() + prefixLength, text.end() - suffixLength };
}

vector<string> split_trim(const string &text, char delimeter, char trimmedCharacter)
{
	vector<string> result = split(text, delimeter);
	for (string &substring : result)substring = trim(substring, trimmedCharacter);
	return result;
}

bool string_contains(const string &text, char character)
{
	for (char textCharacter : text)
	{
		if (textCharacter == character)return true;
	}
	return false;
}

#include "stdafx.h"
#include <sstream>
#include "text_position.h"
using namespace std;


std::string text_position::build_under_line_pointer()const
{
	stringstream text;

	if (data.column > 1)text << string(column() - 1, ' ');
	text << string(character_span(), '^');

	return text.str();
}

text_position text_position::get_next(const std::string &nextText) const
{
	int characterSpan = nextText.size();
	int linesAppended = 0;
	int row = data.row;
	int column = data.row + data.characterSpan;

	for (char character : nextText)
	{
		if (character == '\n')
		{
			linesAppended++;
			row++;
			column = 1;
		}
		else column++;
	}

	return text_position{ row, column, characterSpan, linesAppended };
}

text_position::text_position(int row, int column, int characterSpan, int linesAppended)
{
	data.row = row;
	data.column = column;
	data.characterSpan = characterSpan;
	data.linesAppended = linesAppended;
}

text_position::~text_position()
{
}

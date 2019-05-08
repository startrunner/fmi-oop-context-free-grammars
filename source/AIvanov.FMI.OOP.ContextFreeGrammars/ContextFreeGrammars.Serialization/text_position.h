#pragma once
#include <string>
class text_position
{
	struct {
		int row = 1, column = 1;
		int characterSpan = 0;
		int linesAppended = 0;
	}data;

public:
	text_position(const text_position &toCopy)
		:text_position(toCopy.row(), toCopy.column(), toCopy.character_span(), toCopy.lines_appended()) {}
	text_position(int row, int column, int characterSpan, int linesAppended);
	text_position() {}

	int row()const { return data.row; }
	int column()const { return data.column; }
	int character_span()const { return data.characterSpan; }
	int lines_appended()const { return data.linesAppended; }

	std::string build_under_line_pointer()const;
	text_position get_next(const std::string &nextText)const;

	~text_position();
};


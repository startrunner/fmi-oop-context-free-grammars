#pragma once

struct document_position
{
private:
	struct 
	{
		int row, column;
		int characterSpan, rowsAdded;
	}my;

public:
	document_position() :
		document_position(1, 1, 1) {}

	document_position(int row, int column, int characterSpan) :
		document_position(row, column, characterSpan, 0) {}

	document_position(int row, int column, int characterSpan, int rowsAdded) :
		my({ row, column, characterSpan, rowsAdded }) {}

	int row()const { return my.row; }
	void set_row(int row) { my.row = row; }

	int column()const { return my.column; }
	void set_column(int column) { my.column = column; }

	int character_span()const { return my.characterSpan; }
	void set_character_span(int characterSpan) { my.characterSpan = characterSpan; }

	int rows_added()const { return my.rowsAdded; }
	void set_rows_added(int rowsAdded) { my.rowsAdded = rowsAdded; }
};
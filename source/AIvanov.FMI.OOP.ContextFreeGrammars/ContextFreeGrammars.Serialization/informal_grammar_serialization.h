#pragma once
#include "stdafx.h"
#include <ostream>
#include <istream>
#include <string>
#include "../ContextFreeGrammars.Grammars/grammar.h"

std::ostream& operator <<(std::ostream& stream, const grammar &grammar);
std::ostream& operator <<(std::ostream& stream, const production &production);
std::ostream& operator <<(std::ostream& stream, const concatenation &concatenation);
std::ostream& operator <<(std::ostream& stream, const term &term);
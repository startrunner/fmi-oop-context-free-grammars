#pragma once
#include "stdafx.h"

std::string concatenate(const std::vector<std::string> segments, const std::string &delimeter="");
std::string to_lower(const std::string &text);
std::vector<std::string> split(const std::string &text, char delimeter);
std::string trim(const std::string &text, char trimmedCharacter);
std::vector<std::string> split_trim(const std::string &text, char delimeter, char trimmedCharacter);
bool string_contains(const std::string &text, char character);
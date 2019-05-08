#pragma once
#include <string>
#include <deque>
#include <map>
#include <functional>
#include <memory>
#include <sstream>
#include "token.h"
class tokenizer
{
	std::shared_ptr<std::istream> characters = nullptr;
	text_position lastPosition;

public:
	tokenizer(std::shared_ptr<std::istream> characters);
	tokenizer& refeed(std::shared_ptr<std::istream> characters);

	bool can_get()const;
	bool try_get(token *outToken);
	token get();

	~tokenizer();
};


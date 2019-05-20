#pragma once
#include <string>

template<typename T> class entry_list
{
	struct node
	{
		T item;
		node *next = nullptr;
		~node() { delete next; }
	};

public:
	class iterator
	{
		node *current = nullptr;
	public:

		iterator(node *current) :current(current) {}

		T& operator *() { return current->item; }
		const T& operator*()const { return current->item; }

		bool operator == (const iterator &other)const { return current == other.current; }
		bool operator != (const iterator &other)const { return current != other.current; }

		T* operator->() { return &current->item; }
		const T* operator->()const { return &current->item; }

		iterator& operator++(int)
		{
			current = current->next;
			return *this;
		}

		iterator operator++()
		{
			iterator value = *this;
			current = current->next;
			return value;
		}
	};

	using const_iterator = const iterator;

private:
	size_t itemCount = 0;
	node *head = nullptr;
	node *last = nullptr;

public:

	size_t size()const { return itemCount; }
	bool empty()const { return itemCount == 0; }

	iterator begin() { return { head }; }
	const_iterator begin()const { return { head }; }

	iterator end() { return { nullptr }; }
	const_iterator end()const { return{ nullptr }; }

	entry_list() {}
	entry_list(const entry_list &toCopy)
	{
		for (const T &item : toCopy)add(item);
	}

	entry_list& operator=(const entry_list &other)
	{
		if (this == &other)return *this;
		destroy();
		for (const T &item : other)add(item);
		return *this;
	}

	void add(const T& item)
	{
		if (itemCount == 0)
		{
			head = last = new node({ item, nullptr });
			itemCount = 1;
			return;
		}

		last->next = new node({ item, nullptr });
		last = last->next;
		itemCount++;
	}

	bool contains(const T& item)
	{
		for (node *i = head; i != nullptr; i = i->next)
		{
			if (i->item == item)return true;
		}
		return false;
	}

	bool remove(const T& item)
	{
		node *previous = nullptr, *node = head;
		while (node != nullptr && node->item != item)
		{
			previous = node;
			node = node->next;
		}

		if (node == nullptr)return false;

		if (previous != nullptr)previous->next = node->next;
		delete node;
		itemCount--;

		if (node == head)head = last = nullptr;

		return true;
	}

	~entry_list() { destroy(); }

private:
	void destroy()
	{
		delete head;
		head = last = nullptr;
		itemCount = 0;
	}
};
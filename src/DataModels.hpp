#pragma once

#include <string>

struct TodoItem
{
    std::string title;
    std::uint64_t id;
    bool completed = false;

    bool operator==(const TodoItem& other) const
    {
        return title == other.title && id == other.id;
    }
};
#ifndef MOCKPERSONSTORAGE_H
#define MOCKPERSONSTORAGE_H

#include <map>
#include "logic/PersonStorage.h"

class MockPersonStorage : public PersonStorage
{
    public:
    std::vector<std::shared_ptr<Person>> getAll() override 
    {
        std::vector<std::shared_ptr<Person>> output;
        for (const auto& [key, value] : this->data)
        {
            output.emplace_back(value);
        }
        return output;
    }

    private:
    std::map<int, std::shared_ptr<Person>> data = {
        {0, std::make_shared<Person>(0, "John")},
        {1, std::make_shared<Person>(1, "Alice")}
    };
};
#endif
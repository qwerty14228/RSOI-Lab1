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
    int Create(std::string name, std::string work="", std::string address="", int age=20) override
    {
        int new_id = this->data.size();
        this->data[new_id] = std::make_shared<Person>(new_id, name, work, address, age);
        return new_id;
    }

    private:
    std::map<int, std::shared_ptr<Person>> data = {
        {0, std::make_shared<Person>(0, "John")},
        {1, std::make_shared<Person>(1, "Alice")}
    };
};
#endif
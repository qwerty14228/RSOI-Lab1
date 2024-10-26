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

    std::shared_ptr<Person> getById(int id) override
    {
        if(this->data.find(id) == this->data.end()) {
            return nullptr;
        }
        return this->data[id];
    }

    int Create(std::string name, std::string work="", std::string address="", int age=20) override
    {
        int new_id = this->next_id++;
        this->data[new_id] = std::make_shared<Person>(new_id, name, work, address, age);
        return new_id;
    }

    bool Delete(int id) override
    {
        if(this->data.find(id) == this->data.end()) {
            return false;
        }
        this->data.erase(id);
        return true;
    }

    bool Update(int id, std::string name, std::string work="", std::string address="", int age=20) override
    {
        if(this->data.find(id) == this->data.end()) {
            return false;
        }
        this->data[id]->name = name;
        this->data[id]->work = work;
        this->data[id]->address = address;
        this->data[id]->age = age;
        return true;
    }

    private:
    std::map<int, std::shared_ptr<Person>> data = {
        {0, std::make_shared<Person>(0, "John")},
        {1, std::make_shared<Person>(1, "Alice")}
    };
    int next_id = 2;
};
#endif
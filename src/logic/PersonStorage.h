#ifndef PERSONSTORAGE_H
#define PERSONSTORAGE_H

#include <vector>
#include <memory>

class PersonStorage
{
    public:
    virtual std::vector<std::shared_ptr<Person>> getAll() = 0;
    virtual int Create(std::string name, std::string work="", std::string address="", int age=20) = 0;
};
#endif
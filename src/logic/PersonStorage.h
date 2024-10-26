#ifndef PERSONSTORAGE_H
#define PERSONSTORAGE_H

#include <vector>
#include <memory>

class PersonStorage
{
    public:
    virtual std::vector<std::shared_ptr<Person>> getAll() = 0;
    virtual std::shared_ptr<Person> getById(int id) = 0;
    virtual int Create(std::string name, std::string work="", std::string address="", int age=20) = 0;
    virtual bool Delete(int id) = 0;
};
#endif
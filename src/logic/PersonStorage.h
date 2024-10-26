#ifndef PERSONSTORAGE_H
#define PERSONSTORAGE_H

#include <vector>
#include <memory>

class PersonStorage
{
    public:
    virtual std::vector<std::shared_ptr<Person>> getAll() = 0;
};
#endif
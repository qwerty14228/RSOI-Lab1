#ifndef PERSONLOGIC_H
#define PERSONLOGIC_H

#include <vector>
#include <memory>
#include "logic/Person.h"
#include "logic/PersonStorage.h"

class PersonLogic
{
    public:
    std::vector<std::shared_ptr<Person>> getAll()
    {
        return this->storage->getAll();
    }

    std::shared_ptr<Person> getById(int id)
    {
        return this->storage->getById(id);
    }

    std::shared_ptr<Person> Create(std::string name, std::string work="", std::string address="", int age=20)
    {
        return this->storage->Create(name, work, address, age);
    }

    bool Delete(int id)
    {
        return this->storage->Delete(id);
    }

    bool Update(int id, std::string name, std::string work="", std::string address="", int age=20)
    {
        return this->storage->Update(id, name, work, address, age);
    }

    PersonLogic(std::shared_ptr<PersonStorage> &storage): storage(storage) {}
    private:
    std::shared_ptr<PersonStorage> storage;
};
#endif
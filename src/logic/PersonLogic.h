#ifndef PERSONLOGIC_H
#define PERSONLOGIC_H

#include <vector>
#include <memory>
#include "logic/PersonStorage.h"

class PersonLogic
{
    public:
    std::vector<Person> getAll()
    {
        return this->storage->getAll();
    }

    PersonLogic(std::shared_ptr<PersonStorage> &storage): storage(storage) {}
    private:
    std::shared_ptr<PersonStorage> storage;
};
#endif
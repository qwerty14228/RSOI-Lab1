#include <iostream>
#include <memory>
#include "logic/Person.h"
#include "storage/MockPersonStorage.h"

int main () 
{
    std::cout << "it works!" << std::endl;
    std::shared_ptr<MockPersonStorage> storage = std::make_shared<MockPersonStorage>();
    storage->Create ("Dima");
    storage->Delete (0);
    for(auto &person : storage->getAll()) {
        std::cout << person->name << std::endl;
    }
    std::cout << storage->getById(1)->name << std::endl;
    return 0;
}
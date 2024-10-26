#include <iostream>
#include <memory>
#include "logic/Person.h"
#include "logic/PersonLogic.h"
#include "storage/MockPersonStorage.h"

int main () 
{
    std::cout << "it works!" << std::endl;
    std::shared_ptr<PersonStorage> storage = std::make_shared<MockPersonStorage>();
    std::shared_ptr<PersonLogic> personLogic = std::make_shared<PersonLogic>(storage);

    personLogic->Create ("Dima");
    personLogic->Delete (0);
    auto dima = personLogic->Create("Dima", "student");
    std::cout << dima->name << std::endl;
    personLogic->Update(dima->id, "Vlad");
    std::cout << dima->name << std::endl;
    for(auto &person : personLogic->getAll()) {
        std::cout << person->name << std::endl;
    }
    std::cout << personLogic->getById(1)->name << std::endl;
    return 0;
}
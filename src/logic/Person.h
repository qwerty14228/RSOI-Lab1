#ifndef PERSON_H 
#define PERSON_H
    class Person
    {
        public:
        Person(int id, std::string name, std::string work="", std::string address="", int age=20): 
        id(id), name(name), work(work), address(address), age(age) {}

        std::string name, work, address; 
        int id;
        int age;
    }; 
#endif
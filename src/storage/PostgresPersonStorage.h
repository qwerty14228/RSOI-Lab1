#ifndef POSTGRESPERSONSTORAGE_H
#define POSTGRESPERSONSTORAGE_H

#include <map>
#include <pqxx/pqxx>
#include "logic/PersonStorage.h"

class PostgresPersonStorage : public PersonStorage
{
    public:
    PostgresPersonStorage(const std::string& connection_string) : conn(connection_string){
        this->init();
    }

    std::vector<std::shared_ptr<Person>> getAll() override 
    {
        std::vector<std::shared_ptr<Person>> output;

        try
        {
            pqxx::work w(this->conn);

            pqxx::result r = w.exec1("SELECT * FROM Persons;");

            w.commit();

            for (auto &row : r) {
                output.emplace_back(std::make_shared<Person>(
                    row[0].as<int>(), 
                    row[1].as<std::string>(), 
                    row[3].as<std::string>(), 
                    row[4].as<std::string>(),
                    row[2].as<int>()
                ));
            }
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
        }

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

    std::shared_ptr<Person> Create(std::string name, std::string work="", std::string address="", int age=20) override
    {
        int new_id = this->next_id++;
        this->data[new_id] = std::make_shared<Person>(new_id, name, work, address, age);
        return this->data[new_id];
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

    protected: 
    void init(){
        try
        {
            pqxx::work w(this->conn);

            w.exec1("CREATE TABLE IF NOT EXISTS Persons (
            id SERIAL PRIMARY KEY, 
            name TEXT NOT NULL,
            age INTEGER NULL,
            work TEXT NULL,
            address TEXT NULL
            );"
            );

            w.commit();
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    private:
    std::map<int, std::shared_ptr<Person>> data;
    int next_id = 0;
    pqxx::connection conn;
};
#endif
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

            pqxx::result r = w.exec_prepared("getAll");

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
        try
        {
            pqxx::work w(this->conn);

            pqxx::row r = w.exec_prepared("Create", name, age, work, address);

            w.commit();
            
            return std::make_shared<Person>(
                r[0].as<int>(), 
                name, work, address, age
            );
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
        }

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

            this->conn.prepare("getAll", "SELECT * FROM Persons;");
            this->conn.prepare("get", "SELECT * FROM Persons WHERE id = $1;");
            this->conn.prepare("Create", "INSERT INTO Persons (name, age, work, address) VALUES ($1, $2, $3, $4) RETURNING id;");
            this->conn.prepare("Delete", "DELETE FROM Persons WHERE id = $1;");
            this->conn.prepare("Update", "UPDATE Persons SET name = $1, work = $2, address = $3, age = $4 WHERE id = $5;");
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
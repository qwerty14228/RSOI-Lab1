#ifndef POSTGRESPERSONSTORAGE_H
#define POSTGRESPERSONSTORAGE_H

#include <iostream>
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

            for (auto row : r) {
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
        return output;
    }

    std::shared_ptr<Person> getById(int id) override
    {
        try
        {
            pqxx::work w(this->conn);

            pqxx::result r = w.exec_prepared("get", id);

            w.commit();

            auto row = r[0];

            return std::make_shared<Person>(
                row[0].as<int>(), 
                row[1].as<std::string>(), 
                row[3].as<std::string>(), 
                row[4].as<std::string>(),
                row[2].as<int>()
            );
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
        }
        return nullptr;
    }

    std::shared_ptr<Person> Create(std::string name, std::string work="", std::string address="", int age=20) override
    {
        try
        {
            pqxx::work w(this->conn);

            pqxx::result r = w.exec_prepared("Create", name, age, work, address);

            w.commit();

            return std::make_shared<Person>(
                r[0][0].as<int>(), 
                name, work, address, age
            );
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
        }
        return nullptr;
    }

    bool Delete(int id) override
    {
        try
        {
            pqxx::work w(this->conn);

            pqxx::result r = w.exec_prepared("Delete", id);

            w.commit();

            return true;
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    bool Update(int id, std::string name, std::string work="", std::string address="", int age=20) override
    {
        try
        {
            pqxx::work w(this->conn);

            pqxx::result r = w.exec_prepared("Update", name, work, address, age, id);

            w.commit();

            return true; 
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    protected: 
    void init(){
        try
        {
            pqxx::work w(this->conn);

            w.exec(
                "CREATE TABLE IF NOT EXISTS Persons ("
                "id SERIAL PRIMARY KEY,"
                "name TEXT NOT NULL,"
                "age INTEGER NULL,"
                "work TEXT NULL,"
                "address TEXT NULL"
                ");"
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
    pqxx::connection conn;
};
#endif
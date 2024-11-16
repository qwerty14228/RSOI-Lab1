#include <memory>
#include "logic/Person.h"
#include "logic/PersonLogic.h"
#include "storage/PostgresPersonStorage.h"
#include "server/Server.h"

int main () 
{

    std::string connectionString = std::getenv("POSTGRES_CONNECTION_STRING");
    std::shared_ptr<PersonStorage> storage = std::make_shared<PostgresPersonStorage>(connectionString);
    std::shared_ptr<PersonLogic> personLogic = std::make_shared<PersonLogic>(storage);
    std::shared_ptr<Server> server = std::make_shared<Server>(personLogic, 8000);

    server->run();

    return 0;
}
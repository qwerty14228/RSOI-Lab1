#include <memory>
#include "logic/Person.h"
#include "logic/PersonLogic.h"
#include "storage/MockPersonStorage.h"
#include "server/Server.h"

int main () 
{

    std::shared_ptr<PersonStorage> storage = std::make_shared<MockPersonStorage>();
    std::shared_ptr<PersonLogic> personLogic = std::make_shared<PersonLogic>(storage);
    std::shared_ptr<Server> server = std::make_shared<Server>(personLogic, 8000);

    server->run();

    return 0;
}
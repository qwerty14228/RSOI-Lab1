#include <gtest/gtest.h>
#include "logic/PersonLogic.h"
#include "storage/MockPersonStorage.h"

class PersonLogicTest : public testing::Test {
 protected:
  void SetUp() override {
    std::shared_ptr<PersonStorage> storage = std::make_shared<MockPersonStorage>();
    this->personLogic = std::make_shared<PersonLogic>(storage);
  }

  std::shared_ptr<PersonLogic> personLogic;
};

TEST_F(PersonLogicTest, CreatesAPerson) {
    auto person = this->personLogic->Create("John", "student", "Moscow Bauman street 10", 18);

    EXPECT_EQ("John", person->name);
    EXPECT_EQ("student", person->work);
    EXPECT_EQ("Moscow Bauman street 10", person->address);
    EXPECT_EQ(18, person->age);
}

TEST_F(PersonLogicTest, GetsAPerson) {
    auto person = this->personLogic->Create("John", "student", "Moscow Bauman street 10", 18);
    auto retrievedPerson = this->personLogic->getById(person->id);

    EXPECT_EQ(retrievedPerson->id, person->id);
    EXPECT_EQ(retrievedPerson->name, "John");
    EXPECT_EQ(retrievedPerson->work, "student");
    EXPECT_EQ(retrievedPerson->address, "Moscow Bauman street 10");
    EXPECT_EQ(retrievedPerson->age, 18);
}
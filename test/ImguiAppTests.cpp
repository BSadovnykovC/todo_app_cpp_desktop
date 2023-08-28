#include "IDataProviderMock.hpp"
#include <gtest/gtest.h>
#include "../src/ImguiApp.hpp"

using namespace ::testing;

class ImguiAppTests : public Test {
public:
    ImguiAppTests() {
        dataProviderMock = new IDataProviderMock();
        app = std::make_unique<ImguiApp>(std::unique_ptr<IDataProvider>(dataProviderMock));
    }

    std::unique_ptr<ImguiApp> app;
    IDataProviderMock* dataProviderMock;

    const std::string testItem = "test item";
};

//TEST_F(ImguiAppTests, Run) {
//    EXPECT_CALL(*dataProviderMock, GetTodoItems()).Times(1);
//    EXPECT_CALL(*dataProviderMock, AddTodoItem(_)).Times(1);
//    EXPECT_CALL(*dataProviderMock, CompleteTodoItem(_)).Times(1);
//    EXPECT_CALL(*dataProviderMock, RemoveTodoItem(_)).Times(1);
//    app->Run();
//};

TEST_F(ImguiAppTests, AddTodoItem) {
    EXPECT_CALL(*dataProviderMock, AddTodoItem(TodoItem{ testItem })).Times(1);
    app->EmulateAddTodoItem(testItem);
};
// is there any bug in the code?
// if you run the test, you will see that the test fails
// the test fails because the test is not written correctly
// q: why is the test not written correctly?
// a: the test is not written correctly because the test is not written from the perspective of the user
// q: what is the perspective of the user?
// a: the perspective of the user is that the user wants to add a todo item
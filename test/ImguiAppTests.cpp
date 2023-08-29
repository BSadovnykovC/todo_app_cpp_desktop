#include "IDataProviderMock.hpp"
#include <gtest/gtest.h>
#include "../src/ImguiApp.hpp"

using namespace ::testing;

class ImguiAppTests : public Test {
public:
    void SetUp() override {
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

TEST_F(ImguiAppTests, AddTodoItem)
{
    EXPECT_CALL(*dataProviderMock, AddTodoItem(TodoItem{ testItem })).Times(1);
    app->EmulateAddTodoItem(testItem);
};

// add unit tests for GetTodoItems
TEST_F(ImguiAppTests, GetTodoItems)
{
    auto dataProviderMockLocal = new IDataProviderMock();
    EXPECT_CALL(*dataProviderMockLocal, GetTodoItems()).Times(1);
    auto appLocal = std::make_unique<ImguiApp>(std::unique_ptr<IDataProvider>(dataProviderMockLocal));
};

TEST_F(ImguiAppTests, GetTodoItemAfterDbUpdate)
{
    EXPECT_CALL(*dataProviderMock, GetTodoItems()).Times(1);
    app->EmulateAddTodoItem(testItem);
    app->Run();
};
#include "../src/IDataProvider.hpp"
#include <gmock/gmock.h>

/// generate mock class for IDataProvider interface
class IDataProviderMock : public IDataProvider {
public:
    ~IDataProviderMock() override = default;
    MOCK_METHOD(std::vector<TodoItem>, GetTodoItems, (), (override));
    MOCK_METHOD(void, AddTodoItem, (const TodoItem& item), (override));
    MOCK_METHOD(void, CompleteTodoItem, (std::uint64_t idemId), (override));
    MOCK_METHOD(void, RemoveTodoItem, (std::uint64_t idemId), (override));
    
};
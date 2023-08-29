#pragma once
#include <memory>
#include "D3D11ResourceHolder.hpp"
#include <string>
#include "IDataProvider.hpp"

class ImguiApp
{
public:
    ImguiApp(std::unique_ptr<IDataProvider> dataProvider);
    ~ImguiApp()
    {
        
    }
    void Run();
#ifdef UNIT_TEST_MODE
    void EmulateAddTodoItem(const std::string& item);
    void EmulateCompleteTodoItem(uint64_t id);
    void EmulateDeleteTodoItem(uint64_t id);
    std::vector<TodoItem> EmulateGetTodoItems();
#endif // UNIT_TEST_MODE

private:
    void InitAppWindow();
    void InitImgui();
    void AddTodoItem();
    void CompleteTodoItem(std::uint64_t itemId);
    void RemoveTodoItem(std::uint64_t itemId);
private:
    HWND m_hwnd;
    std::unique_ptr<D3D11ResourceHolder> m_d3d11ResourceHolder;
    std::string m_todoText = "";
    std::unique_ptr<IDataProvider> m_dataProvider;
    std::vector<TodoItem> m_todoItems;
    bool m_todoItemsDirty = false;
};
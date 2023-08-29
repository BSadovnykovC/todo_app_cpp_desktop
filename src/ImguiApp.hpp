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
    std::vector<TodoItem> EmulateGetTodoItems();
#endif // UNIT_TEST_MODE

private:
    void InitAppWindow();
    void InitImgui();
    void AddTodoItem();
private:
    HWND m_hwnd;
    std::unique_ptr<D3D11ResourceHolder> m_d3d11ResourceHolder;
    std::string m_todoText = "";
    std::unique_ptr<IDataProvider> m_dataProvider;
    std::vector<TodoItem> m_todoItems;
    bool m_todoItemsDirty = false;
};
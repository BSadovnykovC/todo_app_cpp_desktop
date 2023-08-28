#pragma once
#include <memory>
#include "D3D11ResourceHolder.hpp"
#include <string>
#include "IDataProvider.hpp"

class ImguiApp
{
public:
    ImguiApp(std::unique_ptr<IDataProvider> dataProvider);
    void Run();
    
private:
    void InitAppWindow();
    void InitImgui();
private:
    HWND m_hwnd;
    std::unique_ptr<D3D11ResourceHolder> m_d3d11ResourceHolder;
    std::string m_todoText = "";
    std::unique_ptr<IDataProvider> m_dataProvider;
};
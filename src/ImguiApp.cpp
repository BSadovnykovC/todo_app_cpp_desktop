#include "ImguiApp.hpp"
#include "imgui.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <string>

namespace
{
    const std::string appName = "Todo app";
    const uint64_t windowWidth = 640;
    const uint64_t windowHeight = 480;

    //uint64_t g_ResizeWidth = 0;
    //uint64_t g_ResizeHeight = 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        //g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        //g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


ImguiApp::ImguiApp(std::unique_ptr<IDataProvider> dataProvider) : m_dataProvider(std::move(dataProvider))
{
    m_todoText.resize(256);
    InitAppWindow();
    m_d3d11ResourceHolder = std::make_unique<D3D11ResourceHolder>(m_hwnd);
}

void ImguiApp::Run()
{
    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hwnd);
    InitImgui();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    auto [d3Device, d3DeviceContext] = m_d3d11ResourceHolder->GetDeviceAndContext();
    auto renderTarget = m_d3d11ResourceHolder->GetRenderTargetView();
    auto swapChain = m_d3d11ResourceHolder->GetSwapChain();
    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        /*if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            swapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = 0;
            g_ResizeHeight = 0;
            m_d3d11ResourceHolder->CreateRenderTarget();
            renderTarget = m_d3d11ResourceHolder->GetRenderTargetView();
        }*/

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoResize;

            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            ImGui::Begin(appName.c_str(), false, window_flags);
            ImGui::Text("Add Task");
            ImGui::InputText(" ", const_cast<char*>(m_todoText.c_str()), m_todoText.size());
            ImGui::SameLine();
            if (ImGui::Button("Add"))
            {

            }
            
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        d3DeviceContext->OMSetRenderTargets(1, &renderTarget, nullptr);
        d3DeviceContext->ClearRenderTargetView(renderTarget, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        swapChain->Present(1, 0);
    }
}

void ImguiApp::InitAppWindow()
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Todo app", nullptr };
    ::RegisterClassExW(&wc);
    m_hwnd = ::CreateWindowW(wc.lpszClassName, L"Todo app", WS_OVERLAPPEDWINDOW, 100, 100, windowWidth, windowHeight, nullptr, nullptr, wc.hInstance, nullptr);
}

void ImguiApp::InitImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls


    float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(m_hwnd);
    // get window size from hwnd
    RECT rect;
    ::GetClientRect(m_hwnd, &rect);
    io.DisplaySize.x = static_cast<float>(rect.right - rect.left) / dpiScale ;
    io.DisplaySize.y = static_cast<float>(rect.bottom - rect.top) / dpiScale;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hwnd);
    auto [d3Device, d3DeviceContext] = m_d3d11ResourceHolder->GetDeviceAndContext();
    ImGui_ImplDX11_Init(d3Device, d3DeviceContext);
}
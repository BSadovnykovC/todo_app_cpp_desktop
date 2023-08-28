#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <tuple>

class D3D11ResourceHolder
{
public:
    D3D11ResourceHolder(HWND hwnd);
    std::pair<ID3D11Device*, ID3D11DeviceContext*> GetDeviceAndContext();
    ID3D11RenderTargetView* GetRenderTargetView();
    IDXGISwapChain* GetSwapChain();
    void CreateRenderTarget();
private:
    void CreateDeviceAndContext(HWND hwnd);
private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};
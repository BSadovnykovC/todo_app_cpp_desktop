#include "D3D11ResourceHolder.hpp"

D3D11ResourceHolder::D3D11ResourceHolder(HWND hwnd)
{
    CreateDeviceAndContext(hwnd);
}

std::pair<ID3D11Device*, ID3D11DeviceContext*> D3D11ResourceHolder::GetDeviceAndContext()
{
    return std::pair<ID3D11Device*, ID3D11DeviceContext*>(m_device.Get(), m_context.Get());
}

ID3D11RenderTargetView* D3D11ResourceHolder::GetRenderTargetView()
{
    return m_renderTargetView.Get();
}

IDXGISwapChain* D3D11ResourceHolder::GetSwapChain()
{
    return m_swapChain.Get();
}

void D3D11ResourceHolder::CreateDeviceAndContext(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 
                                                2, D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, &featureLevel, &m_context);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, &featureLevel, &m_context);

    CreateRenderTarget();
}

void D3D11ResourceHolder::CreateRenderTarget()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    m_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_renderTargetView);
}

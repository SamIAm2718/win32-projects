#pragma once
#include "BaseScene.h"

class GameWindow;

class BorderScene : public BaseScene
{
public:
	BorderScene();

    // Methods required by BaseScene
    HRESULT CreateDeviceIndependentResources();
    void    DiscardDeviceIndependentResources();
    HRESULT CreateDeviceDependentResources();
    void    DiscardDeviceDependentResources();
    void    CalculateLayout();
    void    RenderScene();

private:
    GameWindow* m_pGame{ nullptr };

    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightestColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkestColorBrush{ nullptr };
};


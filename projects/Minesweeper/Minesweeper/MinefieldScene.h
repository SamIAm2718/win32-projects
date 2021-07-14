#pragma once
#include "BaseScene.h"

#include <vector>

class MineTile;
class MinefieldWindow;

class MinefieldScene : public BaseScene
{
public:
    MinefieldScene();
    HRESULT CreateDeviceIndependentResources();
    void    DiscardDeviceIndependentResources();
    HRESULT CreateDeviceDependentResources();
    void    DiscardDeviceDependentResources();
    void    CalculateLayout();
    void    RenderScene();

    void    DrawTile(const MineTile& tile);
    void    DrawTileContents(const MineTile& tile);

private:
    MinefieldWindow* m_pMinefield{ nullptr };

    CComPtr<ID2D1PathGeometry> m_pTileEdgeGeometry{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightestColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkestColorBrush{ nullptr };

    CComPtr<ID2D1Bitmap> m_pXMarkBitmap{ nullptr };

    CComPtr<ID2D1SolidColorBrush> m_pOneColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTwoColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pThreeColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pFourColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pFiveColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pSixColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pSevenColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pEightColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pQuestionMarkColorBrush{ nullptr };

    CComPtr<IDWriteTextFormat> m_pTextFormat{ nullptr };
    CComPtr<IDWriteTextFormat> m_pEmojiFormat{ nullptr };
};
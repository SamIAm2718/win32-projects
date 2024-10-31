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

private:
    MinefieldWindow* m_pMinefield{ nullptr };

    CComPtr<ID2D1PathGeometry> m_pTileEdgeGeometry{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightestColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkestColorBrush{ nullptr };

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

    CComPtr<ID2D1Bitmap> m_pOneBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pTwoBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pThreeBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pFourBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pFiveBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pSixBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pSevenBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pEightBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pQuestionMarkBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pBombBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pFlagBitmap{ nullptr };
    CComPtr<ID2D1Bitmap> m_pXMarkBitmap{ nullptr };

    HRESULT  CreateCharacterBitmap(const WCHAR* pChar, const UINT width, const UINT height, IDWriteTextFormat* pTextFormat, 
        ID2D1Brush* pFillBrush, D2D1_DRAW_TEXT_OPTIONS drawTextOptions, ID2D1Bitmap** destBitmap);
    void    DrawTile(const MineTile& tile);
    void    DrawTileContents(const MineTile& tile);
};
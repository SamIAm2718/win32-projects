#pragma once
#include "BaseScene.h"

#include "enums.h"

class SmileScene : public BaseScene
{
public:
	SmileScene();

	// Methods required by BaseScene
	HRESULT CreateDeviceIndependentResources();
	void    DiscardDeviceIndependentResources();
	HRESULT CreateDeviceDependentResources();
	void    DiscardDeviceDependentResources();
	void    CalculateLayout();
	void    RenderScene();

	BOOL    IsClicked();
	BOOL	IsDebugEnabled();
	void    SetClicked();
	void    UnsetClicked();
	void	ToggleDebug();
	void	ResetGame();
	void	SetSmileState(SmileState state);
	void	SetCurrentTileContent(TileContent content);

private:
	BOOL m_bClicked{ FALSE };
	BOOL m_bDebugEnabled{ FALSE };
	BOOL m_bCheatsUsed{ FALSE };

	SmileState m_smileState{ SmileState::SMILE };
	TileContent m_curTileContent{ TileContent::EMPTY };

	CComPtr<ID2D1PathGeometry> m_pTileEdgeGeometry{ nullptr };
	CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightestColorBrush{ nullptr };
	CComPtr<ID2D1SolidColorBrush> m_pTileEdgeLightColorBrush{ nullptr };
	CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkColorBrush{ nullptr };
	CComPtr<ID2D1SolidColorBrush> m_pTileEdgeDarkestColorBrush{ nullptr };

	CComPtr<ID2D1SolidColorBrush> m_pTextColorBrush{ nullptr };
	CComPtr<IDWriteTextFormat> m_pEmojiFormat{ nullptr };
};
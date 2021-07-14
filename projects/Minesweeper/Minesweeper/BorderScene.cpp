#include "BorderScene.h"

#include "BorderHelper.h"
#include "colors.h"
#include "GameWindow.h"

/*
*	==========================
*	===== Public Methods =====
*	==========================
*/

BorderScene::BorderScene() {}

HRESULT BorderScene::CreateDeviceIndependentResources()
{
	m_pGame = reinterpret_cast<GameWindow*>(GetWindowLongPtr(m_hOwnerWnd, GWLP_USERDATA));
	return (m_pGame ? S_OK : E_FAIL);
}

void BorderScene::DiscardDeviceIndependentResources()
{

}

HRESULT BorderScene::CreateDeviceDependentResources()
{
	HRESULT	hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeLightest)), &m_pTileEdgeLightestColorBrush);

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeLight)), &m_pTileEdgeLightColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeDark)), &m_pTileEdgeDarkColorBrush);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(RGBA(colors::tileEdgeDarkest)), &m_pTileEdgeDarkestColorBrush);
	}

	return hr;
}

void BorderScene::DiscardDeviceDependentResources()
{
	m_pTileEdgeLightestColorBrush.Release();
	m_pTileEdgeLightColorBrush.Release();
	m_pTileEdgeDarkColorBrush.Release();
	m_pTileEdgeDarkestColorBrush.Release();
}

void BorderScene::CalculateLayout()
{

}

void BorderScene::RenderScene()
{
	m_pRenderTarget->Clear(D2D1::ColorF(RGBA(colors::tileBackground)));

	BorderHelper::DrawBorder(m_pGame->MinefieldBoundingBox(), static_cast<FLOAT>(m_pGame->GetTileSize() / 16),
		m_pFactory, m_pRenderTarget, m_pTileEdgeDarkColorBrush, m_pTileEdgeDarkestColorBrush,
		m_pTileEdgeLightColorBrush, m_pTileEdgeLightestColorBrush);

	BorderHelper::DrawBorder(m_pGame->InfoBarBoundingBox(), static_cast<FLOAT>(m_pGame->GetTileSize() / 16),
		m_pFactory, m_pRenderTarget, m_pTileEdgeDarkColorBrush, m_pTileEdgeDarkestColorBrush,
		m_pTileEdgeLightColorBrush, m_pTileEdgeLightestColorBrush);
}
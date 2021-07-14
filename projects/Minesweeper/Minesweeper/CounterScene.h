#pragma once
#include "BaseScene.h"

#include <array>

#include "constants.h"

class CounterScene : public BaseScene
{

public:
    CounterScene();

    // Methods required by BaseScene
    HRESULT CreateDeviceIndependentResources();
    void    DiscardDeviceIndependentResources();
    HRESULT CreateDeviceDependentResources();
    void    DiscardDeviceDependentResources();
    void    CalculateLayout();
    void    RenderScene();

    // Set the counter to count.
    void SetCounter(INT32 count);

private:
    std::array<UINT, constants::COUNTER_SIZE> m_aDigitStates{};
    std::array<CComPtr<ID2D1PathGeometry>, 7> m_aDigitLEDs{};

    CComPtr<ID2D1SolidColorBrush> m_pBackgroundColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pDigitLEDOffColorBrush{ nullptr };
    CComPtr<ID2D1SolidColorBrush> m_pDigitLEDOnColorBrush{ nullptr };
};


#pragma once
#include <JuceHeader.h>

class MenuBarComponent : public SystemTrayIconComponent,
                         private Timer
{
public:
    MenuBarComponent();
    ~MenuBarComponent() override;

private:
    void mouseDown(const MouseEvent& event) override;
    void timerCallback() override;
    void updateMenu();
    void launchAtLogin(bool shouldLaunch);
    bool isLaunchAtLoginEnabled();

    std::unique_ptr<PopupMenu> menu;
    bool isDarkMode = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBarComponent)
};

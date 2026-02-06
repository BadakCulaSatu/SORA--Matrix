#include "MenuBarComponent.h"
#include "../Audio/AudioEngine.h"
#include "../Settings/AppConfig.h"

MenuBarComponent::MenuBarComponent()
{
    updateMenu();
    startTimer(1000); // Cek dark mode setiap detik
}

MenuBarComponent::~MenuBarComponent() {}

void MenuBarComponent::mouseDown(const MouseEvent& event)
{
    if (event.mods.isRightButtonDown() || event.mods.isCommandDown())
    {
        updateMenu();
        menu->showMenu(PopupMenu::Options());
    }
}

void MenuBarComponent::timerCallback()
{
    bool dark = Desktop::getInstance().getDefaultLookAndFeel()
                .isColourSchemeDark();
    if (dark != isDarkMode)
    {
        isDarkMode = dark;
        repaint();
    }
}

void MenuBarComponent::updateMenu()
{
    menu = std::make_unique<PopupMenu>();
    menu->addItem(1, "Audio Devices");
    menu->addItem(2, "Add Mapping");
    menu->addSeparator();
    
    PopupMenu launchMenu;
    launchMenu.addItem(11, "Launch at Startup", true, 
                       isLaunchAtLoginEnabled());
    menu->addSubMenu("Settings", launchMenu);
    
    menu->addSeparator();
    menu->addItem(99, "QUIT");
    
    menu->addItemCallback = [this](int result)
    {
        if (result == 99) JUCEApplication::quit();
        else if (result == 11) launchAtLogin(!isLaunchAtLoginEnabled());
    };
}

void MenuBarComponent::launchAtLogin(bool shouldLaunch)
{
    AppConfig::getInstance().setLaunchAtLogin(shouldLaunch);
}

bool MenuBarComponent::isLaunchAtLoginEnabled()
{
    return AppConfig::getInstance().getLaunchAtLogin();
}

#include "OledMenuFunctions.h"

OledMenuFunctions::OledMenuFunctions() : currentMenu(&mainMenu)
{
}

void OledMenuFunctions::setupMenus()
{
    // Main Menu
    mainMenu.name = "Marauder Extended";
    mainMenu.items = new LinkedList<MenuItem>();
    mainMenu.items->add(MenuItem{"WiFi Menu", false, [this]()
                                 { this->changeMenu(&wifiMenu); }});
    mainMenu.items->add(MenuItem{"RF Menu", false, []() { /* this->changeMenu(&rfMenu); */ }});
    mainMenu.items->add(MenuItem{"Shutdown", false, []() { /* Shutdown code */ }});
    mainMenu.parentMenu = NULL;

    // WiFi Menu
    wifiMenu.name = "WiFi Menu";
    wifiMenu.items = new LinkedList<MenuItem>();
    wifiMenu.items->add(MenuItem{"WiFi Sniffer", false, []() { /* WiFi Sniffer code */ }});
    wifiMenu.items->add(MenuItem{"WiFi Scanner", false, [this]()
                                 { this->changeMenu(&scanAPsMenu); }}); // FIX: changed [] to [this] to allow access to changemenu
    wifiMenu.items->add(MenuItem{"WiFi Attack", false, []() { /* WiFi Attack code */ }});
    wifiMenu.items->add(MenuItem{"Change MAC", false, []() { /* Change MAC code */ }});
    wifiMenu.items->add(MenuItem{"Change Mode", false, []() { /* Change mode code */ }});
    wifiMenu.items->add(MenuItem{"Channel Analyzer", false, []() { /* Channel Analyzer code */ }});
    wifiMenu.items->add(MenuItem{"back", false, [this]()
                                 {this->changeMenu(this->wifiMenu.parentMenu); /* Back to main menu code */ }}); // fix: added actual back navigation
    wifiMenu.parentMenu = &mainMenu;

    // Scan APs Menu
    scanAPsMenu.name = "Scan APs";
    scanAPsMenu.items = new LinkedList<MenuItem>();
    scanAPsMenu.items->add(MenuItem{"Scan APs on same channel", false, []() { /* same channel scan code */ }});
    scanAPsMenu.items->add(MenuItem{"Scan APs on all channels", false, []() { /* all channel scan code */ }});
    scanAPsMenu.items->add(MenuItem{"back", false, []() { /* Back to WiFi menu code */ }});
    scanAPsMenu.parentMenu = &wifiMenu;
}

void OledMenuFunctions::changeMenu(Menu *menu)
{
    currentMenu = menu;
    this->displayMenu(currentMenu);
}

void OledMenuFunctions::displayMenu(Menu *menu, int starting_index)
{
    oled_display_obj.clear();
    oled_display_obj.drawBanner(menu->name);
    for (int i = 0; i < menu->items->size(); i++)
    {
        oled_display_obj.drawMenuItem(menu->items->get(i).name, i, i == starting_index);
    }
    oled_display_obj.show();
}

void OledMenuFunctions::serialNavigateMenu(String direction)
{
    if (direction == "up")
    {
        oled_display_obj.navigateMenu("up", currentMenu->items->size());
    }
    else if (direction == "down")
    {
        oled_display_obj.navigateMenu("down", currentMenu->items->size());
    }
    else if (direction == "select")
    {
        // Call the function associated with the selected menu item
        MenuItem selectedItem = currentMenu->items->get(oled_display_obj.currentMenuIndex);
        if (selectedItem.callable)
        {
            selectedItem.callable();
        }
        else
        {
            Serial.println(F("No action assigned to this menu item"));
        }
    }

    // this->displayMenu(currentMenu, oled_display_obj.currentMenuIndex); // Refresh menu display after navigation
}

Menu *OledMenuFunctions::getCurrentMenu()
{
    return currentMenu;
}

OledMenuFunctions::~OledMenuFunctions()
{
    delete mainMenu.items;
    delete mainMenu.parentMenu; // Assuming parentMenu is dynamically allocated, if not this line should be removed

    delete wifiMenu.items;
    delete wifiMenu.parentMenu; // Assuming parentMenu is dynamically allocated, if not this line should be removed
}

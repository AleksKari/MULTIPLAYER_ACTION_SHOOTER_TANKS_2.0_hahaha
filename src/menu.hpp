#pragma once
#include <string>

enum class ControlMode {
    MouseAndKeyboard,
    KeyboardOnly      
};

enum class MapType {
    Template,
    RandomGenerated
};

struct GameMenu {
    std::string skinPath = "textures/player.png"; 
    MapType mapType = MapType::Template;
    ControlMode controlMode = ControlMode::KeyboardOnly;
    
};
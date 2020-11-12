// Created by Modar Nasser on 11/11/2020.

#include <fstream>
#include <iostream>

#include "LDtkLoader/World.hpp"

using namespace ldtk;
using json = nlohmann::json;

World::World() = default;

void World::loadFromFile(const std::string& filepath) {
    m_name = filepath;
    std::ifstream in(filepath);
    json j;
    in >> j;

    m_default_pivot_x = j["defaultPivotX"].get<float>();
    m_default_pivot_y = j["defaultPivotY"].get<float>();
    m_default_cell_size = j["defaultGridSize"].get<unsigned int>();

    m_background_color = Color(j["bgColor"].get<std::string>());

    const auto& defs = j["defs"];

    // parsing layers defs
    for (const auto& layer_def : defs["layers"]) {
        LayerDef new_layer_def{layer_def};
        m_layers_defs.push_back(new_layer_def);
    }

    // parsing tilesets
    for (const auto& tileset : defs["tilesets"]) {
        Tileset new_tileset{tileset};
        m_tilesets.push_back(new_tileset);
    }

    // parsing levels
    for (const auto& level : j["levels"]) {
        Level new_level{level, this};
        m_levels.push_back(std::move(new_level));
    }
}

auto World::getDefaultPivot() const -> FloatPoint {
    return {m_default_pivot_x, m_default_pivot_y};
}

auto World::getDefaultGridSize() const -> unsigned int {
    return m_default_cell_size;
}

auto World::getBgColor() const -> const Color& {
    return m_background_color;
}

auto World::getLayerDef(unsigned int id) const -> const LayerDef& {
    for (const auto& layer_def : m_layers_defs)
        if (layer_def.uid == id)
            return layer_def;
    throw std::invalid_argument("LayerDef id "+std::to_string(id)+" not found in World "+m_name+".");
}

auto World::getLayerDef(const std::string& name) const -> const LayerDef& {
    for (const auto& layer_def : m_layers_defs)
        if (layer_def.name == name)
            return layer_def;
    throw std::invalid_argument("LayerDef name "+name+" not found in World "+m_name+".");
}

auto World::getTileset(unsigned int id) const -> const Tileset& {
    for (const auto& tileset : m_tilesets)
        if (tileset.uid == id)
            return tileset;
    throw std::invalid_argument("Tileset id "+std::to_string(id)+" not found in World "+m_name+".");
}

auto World::getTileset(const std::string& name) const -> const Tileset& {
    for (const auto& tileset : m_tilesets)
        if (tileset.name == name)
            return tileset;
    throw std::invalid_argument("Tileset name "+name+" not found in World "+m_name+".");
}

auto World::allLevels() const -> const std::vector<Level>& {
    return m_levels;
}
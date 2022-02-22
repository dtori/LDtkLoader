// Created by Modar Nasser on 12/11/2020.

#include <iostream>

#include "LDtkLoader/Layer.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Layer::Layer(const nlohmann::json& j, const World* w, const Level* l) :
level(l),
iid(j["iid"].get<std::string>()),
m_definition(&w->getLayerDef(j["layerDefUid"].get<int>())),
m_visible(j["visible"].get<bool>()),
m_total_offset(j["__pxTotalOffsetX"].get<int>(), j["__pxTotalOffsetY"].get<int>()),
m_opacity(j["__opacity"].get<float>()),
m_grid_size({j["__cWid"].get<int>(), j["__cHei"].get<int>()})
{
    if ( !j["overrideTilesetUid"].is_null() ) {
        m_override_tileset = &w->getTileset(j["overrideTilesetUid"].get<int>());
    }

    std::string key = "gridTiles";
    int coordId_index = 0;
    if (getType() == LayerType::IntGrid || getType() == LayerType::AutoLayer) {
        key = "autoLayerTiles";
        coordId_index = 1;
    }
    m_tiles.reserve(j[key].size());
    for (const auto& tile : j[key]) {
        m_tiles.emplace_back(
                this,
                tile["d"].get<std::vector<int>>().at(coordId_index),
                tile["t"].get<int>(),
                tile["f"].get<int>()
        );
    }

    for (auto& tile : m_tiles)
        m_tiles_map[tile.coordId] = &tile;

    // LDtk v0.8+
    if (j.contains("intGridCsv")) {
        int coord_id = 0;
        for (const auto& val : j["intGridCsv"]) {
            if (val.get<int>() != 0)
                m_intgrid[coord_id] = &m_definition->m_intgrid_values.at(val.get<int>());
            coord_id++;
        }
    }
    // LDtk pre v0.8
    else {
        for (const auto& val : j["intGrid"]) {
            m_intgrid[val["coordId"].get<int>()] = &m_definition->m_intgrid_values.at(val["v"].get<int>());
        }
    }

    m_entities.reserve(j["entityInstances"].size());
    for (const auto& ent : j["entityInstances"]) {
        m_entities.emplace_back(ent, w);
        auto& new_ent = m_entities.back();
        m_entities_by_name[new_ent.getName()].emplace_back(new_ent);
        for (const auto& tag : new_ent.getTags()) {
            m_entities_by_tag[tag].emplace_back(new_ent);
        }
    }
}

auto Layer::getType() const -> const LayerType& {
    return m_definition->type;
}

auto Layer::getName() const -> const std::string& {
    return m_definition->name;
}

auto Layer::isVisible() const -> bool {
    return m_visible;
}

auto Layer::getCellSize() const -> int {
    return m_definition->cell_size;
}

auto Layer::getGridSize() const -> const IntPoint& {
    return m_grid_size;
}

auto Layer::getOffset() const -> const IntPoint& {
    return m_total_offset;
}

auto Layer::getOpacity() const -> float {
    return m_opacity;
}

auto Layer::hasTileset() const -> bool {
    return m_override_tileset != nullptr || m_definition->m_tileset != nullptr;
}

auto Layer::getTileset() const -> const Tileset& {
    return m_override_tileset == nullptr ? *m_definition->m_tileset : *m_override_tileset;
}

auto Layer::allTiles() const -> const std::vector<Tile>& {
    return m_tiles;
}

auto Layer::getTile(int grid_x, int grid_y) const -> const Tile& {
    auto id = grid_x + m_grid_size.x*grid_y;
    if (m_tiles_map.count(id) > 0)
        return *(m_tiles_map.at(id));
    return Tile::None;
}

auto Layer::getIntGridVal(int grid_x, int grid_y) const -> const IntGridValue& {
    auto id = grid_x + m_grid_size.x*grid_y;
    if (m_intgrid.count(id) > 0)
        return *(m_intgrid.at(id));
    return IntGridValue::None;
}

auto Layer::allEntities() const -> const std::vector<Entity>& {
    return m_entities;
}

auto Layer::hasEntity(const std::string& entity_name) const -> bool {
    return m_entities_by_name.count(entity_name) > 0 && !m_entities_by_name.at(entity_name).empty();
}

auto Layer::getEntitiesByName(const std::string& entity_name) const -> const std::vector<std::reference_wrapper<Entity>>& {
    if (m_entities_by_name.count(entity_name) > 0)
        return m_entities_by_name.at(entity_name);
    else
        return m_entities_by_name[entity_name];
}

auto Layer::getEntitiesByTag(const std::string& tag) const -> const std::vector<std::reference_wrapper<Entity>>& {
    if (m_entities_by_tag.count(tag) > 0)
        return m_entities_by_tag.at(tag);
    else
        return m_entities_by_tag[tag];
}


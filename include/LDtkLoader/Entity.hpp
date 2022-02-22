// Created by Modar Nasser on 22/11/2020.

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/FieldsContainer.hpp"
#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class World;

    class Entity : public FieldsContainer {
    public:
        Entity(const Entity&) = delete;
        Entity(Entity&&) = default;
        auto operator=(const Entity&) -> Entity& = delete;

        const std::string iid;

        auto getName() const -> const std::string&;
        auto getSize() const -> const IntPoint&;
        auto getPosition() const -> const IntPoint&;
        auto getGridPosition() const -> const IntPoint&;
        auto getColor() const -> const Color&;
        auto getPivot() const -> const FloatPoint&;

        auto hasSprite() const -> bool;
        auto getTexturePath() const -> const std::string&;
        auto getTextureRect() const -> const IntRect&;

        auto hasTag(const std::string& tag) -> bool;
        auto getTags() -> const std::vector<std::string>&;

        explicit Entity(const nlohmann::json& j, const World* w);

    private:
        const EntityDef* m_definition = nullptr;

        const IntPoint m_size;
        const IntPoint m_position;
        const IntPoint m_grid_pos;
        const Color m_color;

        const Tileset* m_tileset = nullptr;
        const IntRect m_texture_rect;
    };

}

// Created by Modar Nasser on 16/11/2020.

#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

EntityDef::EntityDef(const nlohmann::json& j, World* w) :
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<unsigned int>()),
size({j["width"].get<unsigned int>(), j["height"].get<unsigned int>()}),
color(j["color"].get<std::string>()),
pivot({j["pivotX"].get<float>(), j["pivotY"].get<float>()}),
m_tileset(j["tilesetId"].is_null() ? nullptr : &w->getTileset(j["tilesetId"].get<unsigned int>())),
m_tile_id(j["tileId"].is_null() ? -1 : j["tileId"].get<int>())
{}
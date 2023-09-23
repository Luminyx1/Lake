#pragma once

#include "Lake/Common.h"

#include <simdjson.h>
#include <glm/glm.hpp>

namespace lake::json {

    inline glm::vec2 getVec2(simdjson::simdjson_result<simdjson::haswell::ondemand::object>& object, const std::string_view key) {
        auto array = object[key].get_array();

        auto it = array.begin();

        const f32 x = static_cast<f32>(f64(*it.value()));
        const f32 y = static_cast<f32>(f64(*(++it).value()));

        return glm::vec2(x, y);
    }

    inline glm::vec3 getVec3(simdjson::simdjson_result<simdjson::haswell::ondemand::object>& object, const std::string_view key) {
        auto array = object[key].get_array();

        auto it = array.begin();

        const f32 x = static_cast<f32>(f64(*it.value()));
        const f32 y = static_cast<f32>(f64(*(++it).value()));
        const f32 z = static_cast<f32>(f64(*(++it).value()));

        return glm::vec3(x, y, z);
    }

    inline glm::vec4 getVec4(simdjson::simdjson_result<simdjson::haswell::ondemand::object>& object, const std::string_view key) {
        auto array = object[key].get_array();

        auto it = array.begin();

        const f32 x = static_cast<f32>(f64(*it.value()));
        const f32 y = static_cast<f32>(f64(*(++it).value()));
        const f32 z = static_cast<f32>(f64(*(++it).value()));
        const f32 w = static_cast<f32>(f64(*(++it).value()));

        return glm::vec4(x, y, z, w);
    }

} // namespace lake::json

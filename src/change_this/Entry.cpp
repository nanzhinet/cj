#include "Entry.h"

#include <fmt/format.h>
#include <functional>
#include <ll/api/Config.h>
#include <ll/api/io/FileUtils.h>
#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/plugin/PluginManagerRegistry.h>
#include <memory>
#include <stdexcept>

namespace change_this {

namespace {

std::unique_ptr<std::reference_wrapper<ll::plugin::NativePlugin>>
    selfPluginInstance; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

bool disable(ll::plugin::NativePlugin& /*self*/) {
    auto& logger = getSelfPluginInstance().getLogger();

    logger.info("disabling...");

    // Your code here.

    logger.info("disabled");

    return true;
}

bool enable(ll::plugin::NativePlugin& /*self*/) {
    auto& logger = getSelfPluginInstance().getLogger();

    logger.info("enabling...");

    logger.info("enabled");

    return true;
}

bool load(ll::plugin::NativePlugin& self) {
    auto& logger = self.getLogger();

    logger.info("loading...");

    selfPluginInstance = std::make_unique<std::reference_wrapper<ll::plugin::NativePlugin>>(self);

    // Your code here.

    logger.info("loaded");

    return true;
}

bool unload(ll::plugin::NativePlugin& self) {
    auto& logger = self.getLogger();

    logger.info("unloading...");

    selfPluginInstance.reset();

    // Your code here.

    logger.info("unloaded");

    return true;
}

} // namespace

ll::plugin::NativePlugin& getSelfPluginInstance() {
    if (!selfPluginInstance) {
        throw std::runtime_error("selfPluginInstance is null");
    }

    return *selfPluginInstance;
}

} // namespace change_this

extern "C" {
_declspec(dllexport) bool ll_plugin_disable(ll::plugin::NativePlugin& self) { return change_this::disable(self); }
_declspec(dllexport) bool ll_plugin_enable(ll::plugin::NativePlugin& self) { return change_this::enable(self); }
_declspec(dllexport) bool ll_plugin_load(ll::plugin::NativePlugin& self) { return change_this::load(self); }
_declspec(dllexport) bool ll_plugin_unload(ll::plugin::NativePlugin& self) { return change_this::unload(self); }
}

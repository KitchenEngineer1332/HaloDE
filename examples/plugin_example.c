#include <stdio.h>
#include <stdlib.h>
#include "../../libs/libplugin/halo_plugin.h"

static struct halo_plugin_api *g_api = NULL;

static void on_kill_shortcut(void *data) {
    (void)data;
    g_api->log(1, "Plugin: Kill shortcut triggered!");
    
    halo_view_t *view = g_api->get_focused_view();
    if (view) {
        g_api->close_view(view);
    }
}

// Plugin entry point
bool halo_plugin_init(struct halo_plugin_context *ctx) {
    if (!ctx || !ctx->api) return false;
    
    g_api = ctx->api;
    g_api->log(0, "Example Plugin Loaded Successfully.");
    
    // Bind SUPER + SHIFT + Q to close focused window
    g_api->add_keybind("SUPER+SHIFT", "q", on_kill_shortcut, NULL);
    
    return true;
}

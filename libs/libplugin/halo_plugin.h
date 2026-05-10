#ifndef HALO_PLUGIN_H
#define HALO_PLUGIN_H

#include <stdbool.h>

/**
 * Stable ABI for Halo DE Plugins.
 * Version: 1
 */

typedef struct halo_server halo_server_t;
typedef struct halo_view halo_view_t;

// The vtable exposed to the plugin
struct halo_plugin_api {
    void (*log)(int level, const char *fmt, ...);
    void (*add_keybind)(const char *mods, const char *key, void (*callback)(void*), void *data);
    halo_view_t* (*get_focused_view)(void);
    void (*close_view)(halo_view_t *view);
};

// The initialization struct passed to the plugin
struct halo_plugin_context {
    halo_server_t *server;
    struct halo_plugin_api *api;
};

// Plugin entry point signature
typedef bool (*halo_plugin_init_func_t)(struct halo_plugin_context *ctx);

#endif // HALO_PLUGIN_H

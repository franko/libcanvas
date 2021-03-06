extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

#include "elem/elem.h"
#include "elem/elem_lua.h"

// We define a raw string with a Lua script to test
// the elementary plot functions.
static const char *my_script = R"raw(
local function fxline(x0, x1, f, N)
    N = N or 512
    local line = elem.Path.new()
    local dx = (x1 - x0) / (N - 1)
    for i = 0, N - 1 do
        local x = x0 + dx * i
        line:LineTo(x, f(x))
    end
    return line
end

local plot = elem.Plot.new()
local x0, x1 = 0.0001, 8 * math.pi * 2
plot:AddStroke(fxline(x0, x1, function(x) return math.sin(x) / x end), 0x0000B4FF, 1.5, elem.property.Stroke)
plot:SetTitle("Function plot example")
plot:SetXAxisTitle("x variable")
plot:SetYAxisTitle("y variable")

local window = elem.Window.new()
window:Attach(plot, "")
window:Start(640, 480, elem.WindowResize)

plot:AddStroke(fxline(0.8, x1, function(x) return math.cos(x) / x end), 0xB40000FF, 1.5, elem.property.Stroke)
window:Wait()
)raw";

int main() {
    // We create a standard Lua state.
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // We load the elementary plot's Lua bindings into the Lua state.
    elem::LuaOpenLibrary(L);
    elem::InitializeFonts();

    // Standard Lua code to load the script stored in the string
    // declared above, my_script.
    int load_status = luaL_loadstring(L, my_script);
    if (load_status == 0) {
        int run_status = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (run_status != 0) {
            const char *error_msg = lua_tostring(L, -1);
            fprintf(stderr, "error running script: %s\n", error_msg);
            return 1;
        }
    } else {
        const char *error_msg = lua_tostring(L, -1);
        fprintf(stderr, "error loading script: %s\n", error_msg);
        return 1;
    }
    return 0;
}

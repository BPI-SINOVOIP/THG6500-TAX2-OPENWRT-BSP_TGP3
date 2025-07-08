--[[
LuCI model for easycwmp configuration management
Copyright PIVA Software <pivasoftware.com>
]]--

local datatypes = require("luci.cbi.datatypes")

m = Map("tr069_stun", "tr069-stun",
    [[TR069-STUN is from <a href='https://www.pivasoftware.com'>
 PIVA Software</a>.
 Note, OpenWrt has mostly complete UCI support for tr069_stun, but this LuCI applet
 only covers a few of those options. In particular, there is very little/no validation
 or help.
See /etc/config/tr069_stun for manual configuration.
 ]])
  
s = m:section(TypedSection, "stun", "TR069-STUN settings")
s.anonymous = true
p = s:option(Flag, "enable", "enable")
p = s:option(Value, "server_address", "server_address")
p = s:option(Value, "username", "username")
p = s:option(Value, "password", "password")
p = s:option(Value, "server_port", "server_port")
p = s:option(Value, "min_keepalive", "min_keepalive")
p = s:option(Value, "max_keepalive", "max_keepalive")
p = s:option(Value, "client_port", "client_port")
p = s:option(ListValue, "log_level", "log_level")
p.default = "3"
p:value("0", "Critic")
p:value("1", "Warning")
p:value("2", "Notice")
p:value("3", "Info")
p:value("4", "Debug")

return m

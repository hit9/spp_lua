Simple Protocol Parser
======================

*Actually, this is [ssdb](http://ssdb.io)'s network protocol, and I think it can
be used on other projects.*

Support Lua5.1/Lua5.2/Lua5.3/LuaJIT

- Nodejs Port: https://github.com/hit9/spp_node
- Python Port: https://github.com/hit9/spp_py

Protocol
--------

```
Packet := Block+ '\n'
Block  := Size '\n' Data '\n'
Size   := literal_integer
Data   := string_bytes
```

For example:

```
3
set
3
key
3
val

```

Build
-----

```bash
$ make 
```

Usage
-----

This package only provides parser, because packing is
easy to do.


Parsing example:

```lua
local spp = require('spp_lua')
local parser = spp:new()

parser:feed("2\nok\n4\nbody\n\n")

while true do
    res = parser:get()
    if res == nil then 
        break
    else
        for i=1, #res do print(res[i]) end
    end
end
-- Output:
-- ok
-- body
```

API Ref
-------

- spp:new()
- parser:feed(string)
- parser:get()
- parser:clear()

Bench
-----

```
spp parser: 60000 in 2.000000s => 30000.000000ops
lua parser: 60000 in 18.000000s => 3333.333333ops
```

PS: Nodejs is much faster on this job, close to C speed.

License
-------

MIT (c) 2014, hit9 (Chao Wang).

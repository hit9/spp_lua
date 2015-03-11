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

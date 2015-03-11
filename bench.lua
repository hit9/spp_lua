local spp = require('spp_lua')
local parser = spp:new()

n = 100000

start_at = os.clock()

for i = 0, n do
    local s = tostring(i)
    parser:feed(string.format('2\nok\n%d\n%s\n\n', #s, s))
end

for i = 0, n do
    local t = parser:get()
    assert(t[1] == 'ok')
    assert(t[2] == tostring(i))
end

end_at = os.clock()

print(string.format('%d in %fs => %fops', n, 
end_at - start_at, n / (end_at - start_at)))

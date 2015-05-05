local spp = require('spp_lua')
local parser = spp:new()

n = 500000

---------------------- SPP Implemetation ---------------
start_at = os.time()

for i = 0, n do
    local s = tostring(i)
    parser:feed(string.format('2\nok\n%d\n%s\n\n', #s, s))
    local t = parser:get()
    assert(t[1] == 'ok')
    assert(t[2] == tostring(i))
end

end_at = os.time()

print(string.format('spp parser: %d in %fs => %fops', n,
end_at - start_at, n / (end_at - start_at)))

---------------------- SPP Implemetation ---------------

local Parser = {}
Parser.__index = Parser

function Parser.new()
    local self = setmetatable({}, Parser)
    self.buf = ''
    return self
end

function Parser.feed(self, buf)
    self.buf = self.buf .. buf
end

function Parser.get(self)
    local len = string.len(self.buf)
    local ptr = 1
    local ch = 1
    local chunk = {}

    while len > 0 do
        ch = string.find(self.buf, '\n', ptr)

        if (ch == nil) then
            break
        end

        ch = ch + 1

        local dis = ch - ptr

        if dis == 1 or (dis == 2 and
            string.sub(self.buf, ptr, ptr) == '\r') then
            self.buf = string.sub(self.buf, ch, string.len(self.buf))
            return chunk
        end

        local sz = tonumber(string.sub(self.buf, ptr, ch - 1))

        len = len - (dis + sz)
        ptr = ptr + (dis + sz)

        if len < 0 then
            break
        end

        if len >= 1 and string.sub(self.buf, ptr, ptr) == '\n' then
            len = len - 1
            ptr = ptr + 1
        elseif len >= 2 and string.sub(self.buf, ptr, ptr) == '\r'
            and string.sub(self.buf, ptr + 1, ptr + 1) == '\n' then
            len = len - 2
            ptr = ptr + 2
        else
            break
        end

        table.insert(chunk, string.sub(self.buf, ch, ch + sz - 1))
    end
end

local parser = Parser:new()
start_at = os.time()

for i = 0, n do
    local s = tostring(i)
    parser:feed(string.format('2\nok\n%d\n%s\n\n', #s, s))
    local t = parser:get()
    assert(t[1] == 'ok')
    assert(t[2] == tostring(i))
end

end_at = os.time()

print(string.format('lua parser: %d in %fs => %fops', n,
end_at - start_at, n / (end_at - start_at)))

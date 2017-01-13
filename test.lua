runt = require("lrunt")

vm = runt.init(1024, 4000000)
tab = {1, 101, 1234, 0.5}
runt.parse(vm, "'hello world' say")

--for k,v in pairs(tab) do print(v) end

runt.parse(vm, "2332")

print(runt.pop(vm))

runt.push(vm, 3.14159)
runt.parse(vm, "p")

runt.parse(vm, "[ : foo 'here is a new word!' say ; ]")
runt.parse(vm, "[ : pow dup * ; ]")
runt.parse(vm, "_foo")
id = runt.pop(vm)
runt.parse(vm, "_pow")
pow = runt.pop(vm)

runt.exec(vm, id)

tab = {1, 3, 7, 10}
-- Mapping values 
for k, v in pairs(tab) do
    runt.push(vm, v)
    runt.exec(vm, pow) 
    print("input:", v, "output:", runt.pop(vm))
end

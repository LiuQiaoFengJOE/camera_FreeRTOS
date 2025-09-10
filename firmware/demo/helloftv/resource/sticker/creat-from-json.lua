--local t = io.popen('a.exe')
--local a = t:read("*all")
--print(a)

local commonStr = "ffmpeg -i %s -y -s %s -pix_fmt nv12 %s\n"
local yuvStr = "yuv.exe %d %d %s bin\\%s\n"
local exist = 
[[if not exist bin (
   md bin
)
]]

local tcc = [[
tcc yuv.c -o yuv.exe
]]

print(exist)

local cjson = require "cjson"

local testJson = [[{"count":"3","testArray":{"array":["H","E","L"]},"Hello":"World"}]]

local saveStr = {}
local w,h

f = io.open("make.json","rb")
testJson = f:read("*a")
f:close()
--解析json字符串
local data1 = cjson.decode(testJson)
local data
print(#data1)
for i=1,#data1 do
	data = data1[i]
	if data["output"] and data["input"] and data["outSize"] then
		w,h = string.match(data["outSize"],"(%d+)[Xx](%d+)")
		print(w,h)
		table.insert(saveStr,string.format(commonStr,data["input"],data["outSize"],data["output"]))
		table.insert(saveStr,string.format(yuvStr,w,h,data["output"],data["outputList"]))
	end
end



f = io.open("make.bat","wb")

f:write(tcc)
f:write(exist)

for key,value in ipairs(saveStr) do
	f:write(value)
end
f:write("pause")
f:close()

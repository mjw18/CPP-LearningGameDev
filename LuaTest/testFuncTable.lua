testFuncTable = {}

testFuncTable["multiply"] = function(dooer)
	dooer:setP(12*4)
end

testFuncTable["adder"] = function(dooer)
	dooer:setP(5)
end

testFuncTable["printThis"] = function(dooer)
	dooer:setP(2 + 10)
end

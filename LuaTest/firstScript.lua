a = 10

b = 81

mName = "Mike"

mAge = 21

function add(a, b)

	return (a + b)

end

--simple_table = {4, 5, 6}
simple_table = {name="John", age=20, adder = add}

box = {spawnX = 0, spawnY = 9, id = 91820}
assaultGuy = {spawnX = 0, spawnY = 8, id = 299323}

entities = {box, assaultGuy}

level= {pictur = "dfd", levelObjects = entities}
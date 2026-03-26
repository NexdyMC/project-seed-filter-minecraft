import math 

def SpawnRadiusStructure(Spawn_x, Spawn_y, radius, Structure_x, Structure_y):
    
    # validasi
    if not type(Spawn_x) == int:
        return "nilai Spawn_x harus tipe data int"
        pass
    if not type(Spawn_y) == int:
        return "nilai Spawn_y harus tipe data int"
        pass
    if not type(radius) == str:
        return "nilai radius harus tipe data str"
        pass
    if not type(Structure_x) == int:
        return "nilai Structure_x harus tipe data int"
        pass
    if not type(Structure_y) == int:
        return "nilai Structure_y harus tipe data int"
        pass

    # type_radius
    radius = radius.lower()
    if "b" in radius:
        Block = int(radius.replace("b", "").replace("B", ""))
    elif "c" in radius:
        Block = int(radius.replace("c", "").replace("C", "")) * 16
    elif not "b" in radius or not "c" in radius:
        Block = int(radius)

    # atur_posisi
    tx = (Structure_x - Spawn_x)
    ty = (Structure_y - Spawn_y)
    
    # ubah nilai negatif/- => positif/+
    if tx < 0:
        structure_x = tx * -1
    else:
        structure_x = tx
    if ty < 0:
        structure_y = ty * -1
    else:
        structure_y = ty
    
    # spawn_radius_structure
    if structure_x < Block:
        if structure_y < Block:
            return "Structure berada di dalam radius : True"
        else:
            return "Structure berada di luar radius : False"
    else:
        return "Structure berada di luar radius : False"
    
    return Spawn_x , Spawn_y , Block, structure_x, structure_y


from pathlib import Path
from PIL import Image, ImageChops


# TODO: this function is repeated in 'extract_tileset.py' --merge file resources
def tile_in_tileset(tile, tileset):
    for ref in tileset:
        diff = ImageChops.difference(ref, tile)
        if not diff.getbbox():
            # Tileset already had tile
            return True
    return False


def load_tileset(tileset_path):
    tileset = []
    with Image.open(tileset_path) as im:
        tile_size = im.size[0]
        for y in range(0, im.size[1], tile_size):
            tile = im.crop((0, y, tile_size, y + tile_size))
            tileset.append(tile)
    return tileset


def validate_terrain(terrain_path, tileset):
    tile_size = tileset[0].size[0]
    with Image.open(terrain_path) as terrain:
        for y in range(0, terrain.size[1], tile_size):
            for x in range(0, terrain.size[0], tile_size):
                tile = terrain.crop((x, y, x + tile_size, y + tile_size))
                if not tile_in_tileset(tile, tileset):
                    return False
    return True


tileset_path = "temp/tileset_terrain.png"
tileset = load_tileset(tileset_path)

for path in [
    "../graphics/terrain_01.png",
    "../graphics/terrain_02.png",
    "../graphics/terrain_03.png",
]:
    is_valid = validate_terrain(path, tileset)

    if is_valid:
        print(f"terrain {path} is valid.")
    else:
        print(f"INVALID TERRAIN!: {path}")

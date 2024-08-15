# Convert a png to map data

# - Provide an image or reference tiles
# - Provide an image of terrain made of the reference tiles
# - Nominate tile size
#
# Reference image is split into tiles and catalogued into 'ref_tiles' as PIL Image instances.
# The terrain image is then iterated over in tile sized portions matching each tile to ref_tiles.
# Each terrain tile is converted to its index position in ref_tiles.
#
# Limitations: Where tiles can be stacked (ie have transparency) they need to be presented
# in all stacked variations. Some decoding later is needed to split these tiles into their
# component tiles.

# TODO: Cater for ref_tiles to be supplied in multiple rows
# TODO: Make a command line interface


from pathlib import Path
from PIL import Image, ImageChops

import validate_terrain


def index_of_tile(tile, tile_list):
    for i, ref in enumerate(tile_list):
        if not ImageChops.difference(ref, tile).getbbox():
            return i
    return None


def transparent_tile(tile):
    tile_size = tile.size[0]
    transparent_tile = Image.new("RGB", (tile_size, tile_size), (0, 255, 0))
    if not ImageChops.difference(tile, transparent_tile).getbbox():
        return True
    return False


def parse_terrain(filepath, tileset):
    terrain = []
    tile_size = tileset[0].size[0]
    with Image.open(filepath) as im:
        width = im.size[0]
        height = im.size[1]
        for y in range(0, height, tile_size):
            row = []
            for x in range(0, width, tile_size):
                tile = im.crop((x, y, x + tile_size, y + tile_size))
                row.append(index_of_tile(tile, tileset))
            terrain.append(row)
    return terrain


def save_terrain_as_c(terrain, filename):
    filename = Path(filename)
    filename.parent.mkdir(parents=True, exist_ok=True)
    with open(filename, "w") as f:
        f.write(f"#include <inttypes.h>\n\nconst uint8_t {filename.stem}[] = {{\n")
        for row in terrain:
            f.write("\t")
            f.write(", ".join([str(d) for d in row]))
            f.write(",\n")
        f.write("};")


if __name__ == "__main__":

    tile_size = 8
    tile_source = "../graphics/tileset_master.png"
    meta_source = "../graphics/tileset_master_meta.png"

    tileset = validate_terrain.load_tileset("output/tileset_terrain.png")

    terrain_src = "../graphics/terrain_02.png"
    terrain_dst = "temp/terrain_02.c"
    terrain = parse_terrain(terrain_src, tileset)

    # Save output
    save_terrain_as_c(terrain, terrain_dst)

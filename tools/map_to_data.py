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


def rgb_as_meta_val(rgbtuple):
    # convert rgb value to meta value
    meta_vals = {
        # (0, 255, 0): 0,  # transparent tile
        (255, 0, 0): 0,  # wall (blocks movement)
        (0, 0, 255): 1,  # floor (allows movement)
        (255, 0, 255): 2,  # floor / overlay (allows movement, renders in front)
    }
    return meta_vals[rgbtuple]


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


def create_tileset(tiles_filepath, meta_filepath, tile_size):
    tileset = []
    tilemeta = []
    with Image.open(tiles_filepath) as tile_im:
        with Image.open(meta_filepath) as meta_im:
            width = tile_im.size[0]
            height = tile_im.size[1]
            for y in range(0, height, tile_size):
                for x in range(0, width, tile_size):
                    tile = tile_im.crop((x, y, x + tile_size, y + tile_size))
                    if not transparent_tile(tile) and not index_of_tile(tile, tileset):
                        tileset.append(tile)
                        tilemeta.append(rgb_as_meta_val(meta_im.getpixel((x, y))))
    return (tileset, tilemeta)


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


def tileset_as_image(tileset):
    tile_size = tileset[0].size[0]
    im = Image.new("RGB", (tile_size, len(tileset) * tile_size))
    for i, tile in enumerate(tileset):
        im.paste(tile, (0, i * tile_size))
    return im


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


def save_tilemeta_as_c(tilemeta, filename):
    filename = Path(filename)
    filename.parent.mkdir(parents=True, exist_ok=True)
    with open(filename, "w") as f:
        f.write("const TileMeta tile_meta[] =  {\n")
        for m in tilemeta:
            overlay = "true" if m == 2 else "false"
            blocks_motion = "true" if m == 0 else "false"
            f.write(f"\t {{ {overlay}, {blocks_motion} }},\n")
        f.write("};\n")


if __name__ == "__main__":

    tile_size = 8
    tileset_src = "sprites/tileset_master_output.png"
    tileset_meta_src = "sprites/tileset_master_meta_output.png"

    tileset_dst = "sprites/output/terrain_tiles.png"
    tilemeta_dst = "sprites/output/tile_meta.c"

    (tileset, tilemeta) = create_tileset(tileset_src, tileset_meta_src, tile_size)

    terrain_src = "sprites/terrain_01.png"
    terrain_dst = "sprites/output/terrain_01.c"
    terrain = parse_terrain(terrain_src, tileset)

    # Save output
    # tileset_as_image(tileset).save(tileset_dst, "png")
    # save_tilemeta_as_c(tilemeta, tilemeta_dst)
    save_terrain_as_c(terrain, terrain_dst)

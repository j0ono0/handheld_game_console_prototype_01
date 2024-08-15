#!/usr/bin/python

from pathlib import Path
from PIL import Image, ImageChops


def tile_in_tileset(tile, tileset):
    for ref in tileset:
        diff = ImageChops.difference(ref, tile)
        if not diff.getbbox():
            # Tileset already had tile
            return True
    return False


def transparent_tile(tile):
    pixel = tile.getpixel((0, 0))
    if pixel == (0, 255, 0):
        return True
    return False


def create_tileset(src_img, tile_size):
    tileset = []
    with Image.open(src_img) as im:
        im_height = im.size[1]
        im_width = im.size[0]

        for y in range(0, im_height, tile_size):
            for x in range(0, im_width, tile_size):
                tile = im.crop((x, y, x + tile_size, y + tile_size))
                if not tile_in_tileset(tile, tileset) and not transparent_tile(tile):
                    tileset.append(tile)
    return tileset


def tile_location(tile, ref_im):
    tile_size = tile.size[0]
    with Image.open(ref_im) as im:
        for y in range(0, im.size[1], tile_size):
            for x in range(0, im.size[0], tile_size):
                ref_tile = im.crop((x, y, x + tile_size, y + tile_size))
                if not ImageChops.difference(ref_tile, tile).getbbox():
                    return (x, y)
    raise ValueError("tile not found in reference image.")


def create_tileset_meta(src_img, meta_img, tileset):
    legend = {
        # [ < layer > , <blocks motion> ]
        # layer: 0 = base layer
        # layer: 3 = ovelay
        (255, 0, 0): [0, True],
        (255, 0, 255): [3, False],
        (0, 0, 255): [0, False],
    }
    tilemeta = []
    for tile in tileset:
        coord = tile_location(tile, src_img)
        with Image.open(meta_img) as im:
            pixel = im.getpixel((coord))
            tilemeta.append(legend[pixel])
    return tilemeta


def save_tileset(tileset, filename):
    filename = Path(filename)
    filename.parent.mkdir(parents=True, exist_ok=True)
    tile_size = tileset[0].size[0]
    img = Image.new("RGB", (tile_size, tile_size * len(tileset)))
    for i, tile in enumerate(tileset):
        img.paste(tile, (0, i * tile_size))

    img.save(filename)
    print(f"tileset image saved to {filename}")


def save_tileset_meta(tilemeta, filename):
    filename = Path(filename)
    filename.parent.mkdir(parents=True, exist_ok=True)
    with open(filename, "w") as f:
        f.write("const TileMeta tilemeta[] = {\n")
        for item in tilemeta:
            layer = str(item[0])
            blocks_motion = "true" if item[1] is True else "false"
            f.write(f"\t{{{layer}, {blocks_motion}}},\n")

        f.write("};")
    print(f"tileset meta saved to {filename}")


def main():

    tile_source = "../graphics/tileset_master.png"
    meta_source = "../graphics/tileset_master_meta.png"
    tile_size = 8

    ## Tileset

    tileset_destination = "./temp/tileset_terrain.png"
    tileset = create_tileset(tile_source, tile_size)
    save_tileset(tileset, tileset_destination)

    ## Tilemeta

    meta_destination = "./temp/tileset_terrain_meta.c"
    tilemeta = create_tileset_meta(tile_source, meta_source, tileset)
    save_tileset_meta(tilemeta, meta_destination)


if __name__ == "__main__":
    main()

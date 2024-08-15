from pathlib import Path
from PIL import Image


###################################################
#
#   IMPORTANT !
#
#   color order matches "terrain_color_lut"
#
###################################################

color_palette = [
    # Interior terrain
    (218, 218, 218),  # surface edge (interior)
    (179, 179, 179),  # surface (interior)
    (152, 152, 152),  # wall edge
    (102, 102, 102),  # surface shadow (interior)
    (77, 77, 77),  # wall
    (0, 0, 0),  # surface boundary
    # Exterior terrain
    (255, 255, 255),  # lip edge
    (90, 90, 90),  # lip wall
    (115, 115, 115),  # lip shadow
    (128, 128, 128),  # surface (exterior)
]


def load_image(filepath):
    with Image.open(filepath) as im:
        return im.load(), im.size[0], im.size[1]


def save_as_var(filename, lst):
    filename = Path(filename)
    filename.parent.mkdir(parents=True, exist_ok=True)
    with open(filename, "w") as f:
        f.write("#include <inttypes.h>\n\n")
        f.write(f"int {filename.stem}_indexed[] = {{")
        for i, n in enumerate(lst):

            # Arbitary line breaks for easier review
            if i % 8 == 0:
                f.write("\n\t")

            f.write(f"{n}, ")

        f.write("\n};")


def image_to_indexed_list(img, width, height, palette):
    indexed_img = []
    for y in range(height):
        for x in range(width):
            pixel = img[x, y]
            indexed_img.append(palette.index(pixel))
    return indexed_img


src_file = "temp/tileset_terrain.png"
dst_file = "temp/terrain_tiles_indexed.c"

img, width, height = load_image(src_file)
indexed_img = image_to_indexed_list(img, width, height, color_palette)
save_as_var(dst_file, indexed_img)

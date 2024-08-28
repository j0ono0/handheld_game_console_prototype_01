# convert a sprite sheet to a set of arrays
from collections import namedtuple
from pathlib import Path
from PIL import Image

Sprite = namedtuple("Sprite", ["name", "x", "y", "w", "h"])


def load_image(filepath):
    with Image.open(filepath) as im:
        im.load()
        return im


def rgb_to_565(PILimg):
    img565 = []
    row = []
    for y in range(PILimg.size[1]):
        for x in range(PILimg.size[0]):
            pixel = PILimg.getpixel((x, y))
            # Convert RGB to 565
            R = round(pixel[0] / 255 * 31)
            G = round(pixel[1] / 255 * 63)
            B = round(pixel[2] / 255 * 31)
            rgb565 = (R << 11) | (G << 5) | B
            row.append(hex(rgb565))
        img565.append(row)
        row = []
    return img565


def split_spritesheet(spritesheet, data):
    spriteset = []
    for name, x, y, w, h in data:
        spriteset.append(
            {
                "name": name,
                "width": w,
                "height": h,
                "data": rgb_to_565(spritesheet.crop((x, y, x + w, y + h))),
            }
        )
    return spriteset


def save_sprite_arrays(spriteset, dst_path):
    filename = Path(dst_path)
    filename.parent.mkdir(parents=True, exist_ok=True)

    sprite_declarations = ""
    extern_statements = ""

    with open(filename, "w") as f:
        f.write("#include <avr/pgmspace.h>\n")
        f.write("#include <inttypes.h>\n")
        f.write('#include "customtypes.h"\n\n')

        for sprite in spriteset:
            # write sprite data array
            f.write(f"const uint16_t sprite_data_{sprite['name']}[] PROGMEM = {{\n")
            for row in sprite["data"]:
                f.write("\n\t")
                for number in row:
                    f.write(number)
                    f.write(", ")
            f.write("\n};\n\n")

            # write sprite struct
            sprite_declarations += f"const Sprite sprite_{sprite['name']} PROGMEM = {{ {sprite['width']}, {sprite['height']}, sprite_data_{sprite['name']} }};\n"
            extern_statements += f"// extern const Sprite sprite_{sprite['name']};\n"

        f.write(f"{sprite_declarations}\n")
        f.write(extern_statements)

    print(f"file created: {filename}.")


#########################################################


# data are rects containing each sprite: x, y, width, height
prof_data = [
    Sprite("prof_stationary_west", 0, 0, 16, 32),
    Sprite("prof_stationary_east", 16, 0, 16, 32),
    Sprite("prof_walk_east_1", 0, 32, 16, 32),
    Sprite("prof_walk_east_2", 32, 32, 16, 32),
    Sprite("prof_walk_west_1", 48, 64, 16, 32),
    Sprite("prof_walk_west_2", 16, 64, 16, 32),
    Sprite("prof_walk_south_1", 0, 96, 16, 32),
    Sprite("prof_walk_south_2", 16, 96, 16, 32),
    Sprite("prof_walk_north_1", 32, 96, 16, 32),
    Sprite("prof_walk_north_2", 48, 96, 16, 32),
]
prof_sprites = split_spritesheet(load_image("../graphics/sprites_prof.png"), prof_data)
# save_sprite_arrays(prof_sprites, "./temp/sprites_prof.c")

entity_data = [
    Sprite("target", 48, 43, 16, 13),
    Sprite("crate", 64, 32, 16, 23),
    Sprite("crate_active", 80, 32, 16, 23),
    Sprite("powerconverter", 80, 55, 16, 25),
    Sprite("powerconverter_active", 64, 55, 16, 25),
]
entity_sprites = split_spritesheet(
    load_image("../graphics/entity_sprites_2.png"), entity_data
)


save_sprite_arrays(entity_sprites + prof_sprites, "./temp/sprites_entities.c")

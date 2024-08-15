from pathlib import Path
from PIL import Image

src_path = "sprites/terrain_color_palettes.png"
dst_path = "temp/color_lut.c"
swatch_size = 8


def palette_to_lut(filepath):
    lut = []

    with Image.open(filepath) as im:
        width = im.size[0]
        height = im.size[1]
        row = []
        for y in range(0, height, swatch_size):
            for x in range(0, width, swatch_size):
                pixel = im.getpixel((x, y))

                # Convert RGB to 565
                R = round(pixel[0] / 255 * 31)
                G = round(pixel[1] / 255 * 63)
                B = round(pixel[2] / 255 * 31)
                rgb565 = (R << 11) | (G << 5) | B

                row.append(hex(rgb565))
            lut.append(row)
            row = []

    return lut


def save_to_file(array, filename):
    filename = Path(dst_path)
    filename.parent.mkdir(parents=True, exist_ok=True)
    with open(filename, "w") as f:
        f.write("#include <inttypes.h>\n")
        f.write("uint16_t terrain_color_table[] = {")

        for row in array:
            f.write("\n\t")
            for number in row:
                f.write(number)
                f.write(", ")

        f.write("\n};")


lut = palette_to_lut(src_path)
save_to_file(lut, dst_path)

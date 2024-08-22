from pathlib import Path
from PIL import Image


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


def save_to_file(array, dst_path):
    filename = Path(dst_path)
    filename.parent.mkdir(parents=True, exist_ok=True)
    with open(filename, "w") as f:
        f.write("	#include <avr/pgmspace.h>\n")
        f.write("#include <inttypes.h>\n")
        f.write(f"const uint16_t {filename.stem}[] PROGMEM = {{")

        for row in array:
            f.write("\n\t")
            for number in row:
                f.write(number)
                f.write(", ")

        f.write("\n};")
    print(f"file created: {filename}.")


################################################################

img = load_image("../graphics/prof_walk_02.png")
img565 = rgb_to_565(img)
save_to_file(img565, "./temp/prof_walk_02.c")
print("img_to_565 finished.")

from PIL import Image
import sys

# Function to convert RGB888 to RGB565
def rgb888_to_rgb565(r, g, b):
    # Convert RGB888 to RGB565 (5 bits red, 6 bits green, 5 bits blue)
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)

# Function to convert BMP to a C array
def bmp_to_c_array(bmp_path, array_name="image"):
    # Open BMP image
    img = Image.open(bmp_path)

    # Convert to RGB (24-bit color)
    img = img.convert("RGB")
    
    # Get image dimensions
    width, height = img.size

    # Start the C array declaration
    c_array = f"const uint16_t {array_name}[{width * height}] = {{\n"

    # Loop over all pixels and convert them to RGB565
    pixel_data = []
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            rgb565 = rgb888_to_rgb565(r, g, b)
            pixel_data.append(f"0x{rgb565:04X}")

    # Join the pixel data and format the array in a C style
    c_array += ", ".join(pixel_data)
    c_array += "\n};"

    # Return the final C array as a string
    return c_array

# Main function to save the C array to a file
def save_c_array(bmp_path, output_file="image.h"):
    # Get the C array from the BMP
    c_array = bmp_to_c_array(bmp_path)

    # Save to a file
    with open(output_file, "w") as f:
        f.write(c_array)
    print(f"C array saved to {output_file}")

if __name__ == "__main__":
    # Provide the path to your BMP file and the output C header file name
    bmp_path = sys.argv[1]  # BMP file path
    output_file = sys.argv[2] if len(sys.argv) > 2 else "image.h"  # Output file (default: image.h)

    # Convert BMP to C array and save to the file
    save_c_array(bmp_path, output_file)
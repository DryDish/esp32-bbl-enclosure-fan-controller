Import("env")
import os


html_file = "data/index.html"
header_file = "include/HTML/index.h"
variable_name = "indexPage"

def generate_header():
    os.makedirs(os.path.dirname(header_file), exist_ok=True)
    
    with open(html_file, 'r') as f:
        html_content = f.read()

    with open(header_file, 'w') as f:
        f.write(f"#ifndef {variable_name.upper()}_H\n")
        f.write(f"#define {variable_name.upper()}_H\n\n")
        f.write(f'const char {variable_name}[] PROGMEM = R"(\n')
        f.write(html_content)
        f.write(')";\n\n')
        f.write("#endif\n")

print("Generating C++ header from HTML file...")
generate_header()
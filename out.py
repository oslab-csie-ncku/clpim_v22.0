import configparser
import subprocess
import os

# 固定路径设置
CONFIG_INI_PATH = './pim_m5out/config.ini'  # 修改为你的 config.ini 路径
CONFIG_DOT_PATH = './pim_m5out/config.dot'  # 修改为你的 config.dot 路径
OUTPUT_PDF_PATH = './pim_m5out/config.pdf'  # 修改为你的输出 PDF 路径

def generate_dot(config_ini, config_dot):
    config = configparser.ConfigParser()
    config.read(config_ini)

    with open(config_dot, 'w') as dotfile:
        dotfile.write('digraph G {\n')
        
        for section in config.sections():
            dotfile.write(f'  "{section}" [label="{section}"];\n')
            for key, value in config.items(section):
                dotfile.write(f'  "{section}" -> "{value}" [label="{key}"];\n')
        
        dotfile.write('}\n')

def generate_pdf(config_dot, output_pdf):
    try:
        subprocess.run(['dot', '-Tpdf', config_dot, '-o', output_pdf], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error generating PDF: {e}")
        sys.exit(1)

if __name__ == '__main__':
    generate_dot(CONFIG_INI_PATH, CONFIG_DOT_PATH)
    generate_pdf(CONFIG_DOT_PATH, OUTPUT_PDF_PATH)

    print(f"Generated {OUTPUT_PDF_PATH} from {CONFIG_INI_PATH}")
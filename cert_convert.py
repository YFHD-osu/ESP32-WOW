import re, sys
import binascii
from argparse import ArgumentParser

""" This script can convert an pem ssl certificate to 
Original author: https://github.com/fhessel/ 
Original file: https://github.com/fhessel/esp32_https_server/blob/master/extras/create_cert.sh
Rewrite by: https://github.com/YFHD-osu
"""

def convert(path: str) -> str:
  file = open(path, "rb")
  hex_str = binascii.hexlify(file.read())
  hex_list = [f"0x{i.upper()}" for i in re.findall('..',hex_str.decode())]
  return f'''unsigned int CERTIFICATE_der_len = {len(hex_list)};
  \b\bunsigned char CERTIFICATE_der[] = {{ {", ".join(hex_list)} }};'''

def main() -> None:
  parser = ArgumentParser()
  parser.add_argument("-i", "--input", help="Input file path")
  args = parser.parse_args()
  sys.stdout.write(convert(args.input))

if __name__ == "__main__": main()
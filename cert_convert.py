from OpenSSL import crypto
import binascii
import re

LINE_LENGTH = 42
def pprint(ctx: str, anchor: str = "center") -> str:
  non_ascii = re.findall(r"[^ A-z0-9\!\@\#\$\%\^\&\*\(\)\{\}\[\]\:\-\/\*\-\+]", ctx)
  if (len(ctx)+len(non_ascii)) % 2 != LINE_LENGTH % 2:
    if anchor == "right": ctx = " "+ctx
    else: ctx += " "
    blank = LINE_LENGTH - len(ctx) - len(non_ascii) + 1 - 4
  else:  
    blank = LINE_LENGTH - len(ctx) - len(non_ascii) - 4

  if anchor == "center":
    print( "│ " + " "*(blank//2) + ctx + " "*(blank//2) + " │" )
  elif anchor == "left":
    print( "│ " + ctx + " "*(blank) + " │" )
  elif anchor == "right":
    print( "│ " + " "*(blank) + ctx + " │" )
  return

print("┌" + "─"*40 + "┐")
pprint("Certificate Converter")
pprint("作者: YFHD 參考 fhessel 製作")
print("├" + "─"*40 + "┤")
pprint("請選擇功能: (按下數字0-9) ", "left")
exit()
keyfile = open("test.txt", "r")
cert_file = keyfile.read()
cert_pem = crypto.load_certificate(crypto.FILETYPE_PEM, cert_file)

# 將二進制數據轉換為十六進制字符串
data = b'\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64'
hex_str = binascii.hexlify(data).decode('utf-8')
print(hex_str)  # '48656c6c6f20576f726c64'

# 將十六進制字符串轉換為二進制數據
bin_data = binascii.unhexlify(hex_str)
print(bin_data)  # b'Hello World'
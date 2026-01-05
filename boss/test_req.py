import lz4.frame
import lz4.block
import hexdump
from Crypto.Cipher import ARC4 as rc4cipher
import urllib.parse
from typing import Dict, Optional
import time
import hashlib

chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_'


def base64_encode(binary_data: bytes):
    padding_count = 3 - len(binary_data) % 3
    if padding_count == 3:
        padding_count = 0
    encode_data = binary_data + b'\x00' * padding_count

    result = ''
    for index in range(0, len(encode_data), 3):
        piece = encode_data[index:index+3]
        binary_string = bin(int.from_bytes(piece, 'big'))[2:].zfill(24)
        for i in range(0, 24, 6):
            result += chars[int(binary_string[i:i+6], base=2)]

    if padding_count:
        result = result[:-padding_count] + '~' * padding_count

    return result.encode('ascii')


def lz4_compress_fast_extstate_python(data, acceleration=1):
    compressed = lz4.frame.compress(data,
                                    compression_level=acceleration,  # 1=最快，9=最高压缩率
                                    # block_linked=True,              # 使用链式块
                                    # content_checksum=False,         # 校验和
                                    # block_checksum=False,
                                    store_size=False
                                    )
    return compressed


def create_data_package(magic_hex, num1, num2, num3, raw_data_hex):
    magic_int = int(magic_hex, 16)

    # 检查魔数是否是 8 字节
    if magic_int.bit_length() > 64:
        raise ValueError("魔数超过8字节")

    magic_bytes = magic_int.to_bytes(8, byteorder='little')

    result = bytearray()

    result.extend(magic_bytes)

    result.extend(b'\x00\x00\x00\x00')

    result.extend(num1.to_bytes(4, byteorder='little'))
    result.extend(num2.to_bytes(4, byteorder='little'))
    result.extend(num3.to_bytes(4, byteorder='little'))

    result.extend(raw_data_hex)

    return result


def d(map_data: Optional[Dict[str, str]]) -> Optional[str]:
    if map_data is None:
        return None

    sorted_items = sorted(map_data.items(), key=lambda x: x[0])

    result_parts = []
    first_item = True

    for key, value in sorted_items:
        if not first_item:
            result_parts.append("&")
        else:
            first_item = False

        result_parts.append(key)
        result_parts.append("=")

        if value:
            encoded_value = urllib.parse.quote(value, encoding='utf-8')
            result_parts.append(encoded_value)

        else:
            pass

    # 4. 拼接所有部分
    return ''.join(result_parts)


test_map_1 = {
    "batch_method_feed": '["method=zpCommon.adActivity.getV2&dataType=0&expectId=341699881&dataSwitch=1", "method=zpgeek.app.geek.trait.tip&encryptExpectId=f2464e72baa132571HB73dS0GFpR&expectId=341699881", "method=zpgeek.cvapp.applystatus.change.tip", "method=zpapptips.app.banner.query&encryptExpectId=f2464e72baa132571HB73dS0GFpR&sortType=1&expectPosition=100202&expectId=341699881&page=1&filterParams=%7B%22cityCode%22%3A%22101221000%22%2C%22switchCity%22%3A%220%22%7D", "method=zpinterview.geek.interview.f1.complainTip", "method=zpgeek.app.f1.banner.query&encryptExpectId=f2464e72baa132571HB73dS0GFpR&expectId=341699881&filterParams=%7B%22cityCode%22%3A%22101221000%22%2C%22switchCity%22%3A%220%22%7D&gpsCityCode=0&jobType=0&mixExpectType=0&sortType=1", "method=zpinterview.geek.interview.f1", "method=zpgeek.app.f1.recommend.filter&commute=&distance=0&encryptExpectId=f2464e72baa132571HB73dS0GFpR&expectPosition=&filterFlag=0&filterParams=%7B%22cityCode%22%3A%22101221000%22%2C%22switchCity%22%3A%220%22%7D&filterValue=&jobType=0&mixExpectType=0&partTimeDirection=&positionCode=&sortType=1", "method=zpgeek.cvapp.geek.homeexpectaddress.query&cityCode=101221000&encryptExpectId=", "method=zpgeek.app.geek.recommend.joblist&encryptExpectId=f2464e72baa132571HB73dS0GFpR&sortType=1&expectPosition=100202&extParams=&pageSize=15&expectId=341699881&page=1&filterParams=%7B%22cityCode%22%3A%22101221000%22%2C%22switchCity%22%3A%220%22%7D", "method=zpgeek.app.recommend.listad.query&encryptExpectId=f2464e72baa132571HB73dS0GFpR&sortType=1&expectPosition=100202&expectId=341699881&page=1&filterParams=%7B%22cityCode%22%3A%22101221000%22%2C%22switchCity%22%3A%220%22%7D", "method=zpgeek.cvapp.geek.resume.queryquality"]',
    "curidentity": "0",
    "v": "13.141",
    "req_time": str((time.time()) * 1000),
    "uniqid": "e7dfbaa6-0fdd-4752-a26b-9403e5fe5e5f",
    "client_info": '{"version":"12","os":"Android","start_time":"1767405350171","resume_time":"1767405350171","channel":"28","model":"google||Pixel 4","dzt":0,"loc_per":0,"uniqid":"e7dfbaa6-0fdd-4752-a26b-9403e5fe5e5f","oaid":"922c9565-4e59-4346-affb-0ca8fd0b81d0","oaid_honor":"922c9565-4e59-4346-affb-0ca8fd0b81d0","did":"DUbz59eQvhyRAZokzO_BGmcp18mRwFTf6i4dRFViejU5ZVF2aHlSQVpva3pPX0JHbWNwMThtUndGVGY2aTRkc2h1","tinker_id":"Prod-arm64-v8a-release-13.141.1314110_0812-10-06-09","is_bg_req":0,"network":"wifi","operator":"UNKNOWN","abi":1}'
}


data = d(test_map_1)

lz4_ret = lz4_compress_fast_extstate_python(data.encode())[11:][:-4]

magic_hex = "0x6B636F6C42505A42"
num3 = len(lz4_ret) ^ len(data)
append_bytes = create_data_package(magic_hex, len(lz4_ret), len(data), num3, lz4_ret)
rc4_key = b"a308f3628b3f39f7d35cdebeb6920e21"
enc = rc4cipher.new(rc4_key)
res = enc.encrypt(append_bytes)
sp = base64_encode(res)
sign_data_round = data + "a308f3628b3f39f7d35cdebeb6920e21"
md5hash = hashlib.md5(sign_data_round.encode())
sign = md5hash.hexdigest()
print(f"sp ==> {sp}")
print(f"sig ==> {sign}")

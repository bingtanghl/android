

/**
 *
 * sub_D4DEC >> md5
 * base64:
 * 00000000  73 48 33 66 cd 23 3a 32 53 6d 62 d7 6f f6 9b 90  |sH3fÍ#:2Smb×oö..|
 * 00000010  0f 4a 46 31 a0 50 4c 50 b4 64 8a 19 3a 25 9e 02  |.JF1 PLP´d..:%..|
 * 00000020  c2 b1 e5 62                                      |Â±åb|
 *
 * 0x66334873 ==> 随机数
 *
 *
 * "X-Ladon", "c0gzZs0jOjJTbWLXb/abkA9KRjGgUExQtGSKGTolngLCseVi"
 *
 * Line 4379112	0x76cb3b28fc: "eor x8, x8, x10" x8=0x3863663066373766 x10=0x69656e686d6a6365 => x8=0x510608580b5d5403
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include "md5.h"
#include "base64.h"

using namespace std;

uint64_t circularRightShift(uint64_t value, uint64_t shift)
{
    // 确保shift在0到63之间，因为uint64_t最多有64位
    shift &= 0x3F;                                     // 使用AND操作限制shift的范围
    return (value >> shift) | (value << (64 - shift)); // 右移后与左移结果按位或，实现循环
}

std::vector<uint8_t> uint64ToBytes(uint64_t value)
{
    std::vector<uint8_t> bytes(8);
    for (size_t i = 0; i < 8; ++i)
    {
        bytes[i] = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
    }
    return bytes;
}

#if 0
void x_Ladon(){
    uint64_t no_1, no_2, no_3, no_4;
    uint64_t x8, x10;
    uint64_t encry[4] = {};
    uint64_t w11[7] = {};
    string ent = "1698065544-1588093228-1128";
    if ((ent.length() % 8 != 0))
    {
        ent.append(8 - ent.length() % 8, 0x6);
    }

    // 0x31312d38
    uint64_t no_1 = 0;
    for (int i = 7; i >= 0; i--)
    {
        no_1 = no_1 << 8;
        no_1 += (char)ent[i];
    }
    uint64_t no_2 = 0;

    for (int i = 15; i >= 8; i--)
    {
        no_2 = no_2 << 8;
        no_2 += (char)ent[i];
    }
    uint64_t no_3 = 0;

    for (int i = 23; i >= 16; i--)
    {
        no_3 = no_3 << 8;
        no_3 += (char)ent[i];
    }
    cout << "no_3 : 0x" << hex << no_3 << endl;
    uint64_t no_4 = 0;

    for (int i = 31; i >= 24; i--)
    {
        no_4 = no_4 << 8;
        no_4 += (char)ent[i];
    }
    cout << "no_4 : 0x" << hex << no_4 << endl;

    // 0x606060606063832
    // for (int j = 0; j < 2; j++)
    // {
        // if (j == 0)
        // {
            w11[4] = no_1;
            w11[6] = no_2;
        // }
        // else
        // {
        //     w11[6] = no_4;
        //     w11[4] = no_3;
        // }
        for (int i = 0; i < 0x22; i++)
        {
            x8 = w11[6];
            x8 = circularRightShift(x8, 8);
            w11[1] = x8;
            x10 = w11[1];
            x8 = w11[4];
            x8 = x8 + x10;
            w11[1] = x8;
            if (i == 0)
            {
                w11[6] = data_1[i];
            }
            else
            {
                w11[6] = encrypt[i - 1];
            }
            x10 = w11[1];
            x8 = w11[6];
            x8 = x8 ^ x10;
            w11[6] = x8;
            x8 = w11[4];
            x8 = circularRightShift(x8, 0x3d);
            w11[1] = x8;
            x10 = w11[1];
            x8 = w11[6];
            x8 = x8 ^ x10;
            w11[4] = x8;
        }
        // encry[j * 2] = x8;
        // encry[(j * 2) + 1] = w11[6];
        cout << "第一组 : 0x" << hex << x8 << endl;
        cout << "第二组 : 0x" << hex << w11[6] << endl;
    // }
}
#endif

int main(int argc, char const *argv[])
{
    uint64_t x10, x9, x100, x99, x8;
    int x22 = 0;
    vector<unsigned char> data = {0x73, 0x48, 0x33, 0x66};
    uint64_t w11[7] = {};

    const char *aid = "1128";
    for (char i = 0; i < 4; i++)
    {
        data.push_back(aid[i]);
    }
    for (int byte : data)
        cout << hex << byte << endl;
    string p = MD5(data).toStr();

    cout << p << endl;
    const byte *d = (const unsigned char *)(p.c_str());
    uint64_t data_[4] = {0x0, 0x0, 0x0, 0x0};
    uint64_t data_1[4] = {0x0, 0x0, 0x0, 0x0};
    uint64_t encrypt[0x22] = {};
    for (int i = 0; i < 8; i++)
    {
        x10 = ((uint64_t)d[i] << (i * 8)) | x10;
    }
    data_[0] = x10;
    data_1[0] = x10;
    for (int i = 8; i < 16; i++)
    {
        x9 = ((uint64_t)d[i] << (i * 8)) | x9;
    }
    data_[1] = x9;
    data_1[1] = x9;
    for (int i = 16; i < 24; i++)
    {
        x100 = ((uint64_t)d[i] << (i * 8)) | x100;
    }
    data_[2] = x100;
    data_1[2] = x100;
    for (int i = 24; i < 0x20; i++)
    {
        x99 = ((uint64_t)d[i] << (i * 8)) | x99;
    }
    data_[3] = x99;
    data_1[3] = x99;
    for (uint64_t b : data_)
    {
        cout << hex << "0x" << b << endl;
    }

    while (x22 < 0x22)
    {
        x10 = data_[0];
        x9 = data_[1];
        cout << "x9:  " << hex << x9 << endl;
        x9 = circularRightShift(x9, 0x8);
        x9 = x9 + x10;
        x9 = x9 ^ x22;
        x10 = x9 ^ circularRightShift(x10, 61);
        cout << x22 << "x10: " << hex << x10 << endl;
        encrypt[x22] = x10;
        data_[0] = x10;
        if (x8 != 1)
        {
            data_[1] = data_[2];
            data_[2] = data_[3];
        }
        data_[3] = x9;
        for (size_t i = 0; i < 4; i++)
        {
            cout << "data_[" << i << "]: " << hex << data_[i] << endl;
        }
        x22++;
    }

    uint64_t encry[4] = {};
    string ent = "1698065544-1588093228-1128";
    if ((ent.length() % 8 != 0))
    {
        ent.append(8 - ent.length() % 8, 0x6);
    }

    // 0x31312d38
    uint64_t no_1 = 0;
    for (int i = 7; i >= 0; i--)
    {
        no_1 = no_1 << 8;
        no_1 += (char)ent[i];
    }
    uint64_t no_2 = 0;

    for (int i = 15; i >= 8; i--)
    {
        no_2 = no_2 << 8;
        no_2 += (char)ent[i];
    }
    uint64_t no_3 = 0;

    for (int i = 23; i >= 16; i--)
    {
        no_3 = no_3 << 8;
        no_3 += (char)ent[i];
    }
    cout << "no_3 : 0x" << hex << no_3 << endl;
    uint64_t no_4 = 0;

    for (int i = 31; i >= 24; i--)
    {
        no_4 = no_4 << 8;
        no_4 += (char)ent[i];
    }
    cout << "no_4 : 0x" << hex << no_4 << endl;

    // 0x606060606063832
    for (int j = 0; j < 2; j++)
    {
        if (j == 0)
        {
            w11[4] = no_1;
            w11[6] = no_2;
        }
        else
        {
            w11[6] = no_4;
            w11[4] = no_3;
        }
        for (int i = 0; i < 0x22; i++)
        {
            x8 = w11[6];
            x8 = circularRightShift(x8, 8);
            w11[1] = x8;
            x10 = w11[1];
            x8 = w11[4];
            x8 = x8 + x10;
            w11[1] = x8;
            if (i == 0)
            {
                w11[6] = data_1[i];
            }
            else
            {
                w11[6] = encrypt[i - 1];
            }
            x10 = w11[1];
            x8 = w11[6];
            x8 = x8 ^ x10;
            w11[6] = x8;
            x8 = w11[4];
            x8 = circularRightShift(x8, 0x3d);
            w11[1] = x8;
            x10 = w11[1];
            x8 = w11[6];
            x8 = x8 ^ x10;
            w11[4] = x8;
        }
        encry[j * 2] = x8;
        encry[(j * 2) + 1] = w11[6];
        cout << "第一组 : 0x" << hex << x8 << endl;
        cout << "第二组 : 0x" << hex << w11[6] << endl;
    }

    vector<uint8_t> base64_data;
    long time_ = 0x66334873;
    for (size_t i = 0; i < 4; i++)
    {
        int byte_rand = (time_ >> (i * 8)) & 0xff;
        base64_data.push_back(byte_rand);
    }

    for (uint64_t j : encry)
    {
        vector<uint8_t> bytes = uint64ToBytes(j);
        base64_data.insert(base64_data.end(), bytes.begin(), bytes.end());
    }

    for (uint8_t j : base64_data)
        cout << "0x" << hex << (int)j << endl;
    // cout << "0x" << hex << j << endl;

    std::string encoded = base64_encode(base64_data.data(), base64_data.size(), 0);
    cout << "X-L: " << encoded << endl;
    return 0;
}

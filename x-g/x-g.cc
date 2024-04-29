#include <string>
#include <iostream>
#include <vector>
#include <sstream>


using namespace std;
int main(int argc, char const *argv[])
{
    int data[] = {0x96, 0xe7, 0xc6, 0x54};


    long time = (0x18B5AE18834 + 0x1b66c98) / 1000;

    vector<int> en = {0x84, 0x4, 0x0, 0x29, 0x0, 0x4};
    int tatbl_1[8] = {0x4A,
                      0x00,
                      0x16,
                      0x29,
                      0x47,
                      0x6C,
                      0x04,
                      0x00};
    int tatbl[257] = {};
    for (int i = 0; i < 256; i++)
    {
        tatbl[i] = i;
    }

    int x21 = 0;
    int x22 = 0;
    int w8 = 0;
    int x9;
    int w9;
    int x23;
    int x20;
    for (; x21 < 0x100; x21++)
    {
        w8 = (x21 + 0x38) & 0xff;
        if (w8 == 3)
        {
        }
        w8 = tatbl[x21];
        x9 = x21 & 7;
        w9 = tatbl_1[x9];
        w8 = x22 + w8;
        w8 = w8 + w9;
        w9 = w8 / 0x100;
        x22 = w8 - (w9 << 8);
        x23 = tatbl[x22];
        w8 = (x21 + 0x22) & 0xff;
        x20 = x22;
        tatbl[x21] = x23;
        if (w8 == 2)
        {
            // todo:
        }
        tatbl[x20] = x23;
    }

    int x25 = 0;
    int x26 = 0;
    x21 = 0x14;
    x23 = 0x100;
    int x27 = 0;
    int w10;
    for (; x25 < x21; x25++)
    {
        if (!x25)
        { 
            for (int i = 0; i < 4; i++)
            {
                en.push_back(data[i]);
            }
            for (int i = 0; i < 8; i++)
            {
                en.push_back((int)(0x0));
            }
            en.push_back(0x0);
            en.push_back(0x7);
            en.push_back(0x4);
            en.push_back(0x4);
            for (int i = 3; i >= 0; i--)
            {
                int hex_value = (time >> (i * 8)) & 0xff;
                en.push_back(hex_value);
            }

        }
        w8 = x26 + 1;
        x9 = (w8 / x23);
        x26 = w8 - (x9 << 8);
        w8 = tatbl[x26];
        w8 = x27 + w8;
        x9 = w8 / x23;
        x27 = w8 - (x9 << 8);
        w8 = tatbl[x27];
        tatbl[x26] = w8;
        tatbl[x27] = w8;
        x9 = tatbl[x26];
        w10 = en.at(6 + x25);
        w8 = (w8 + x9) & 0xff;
        w8 = tatbl[w8] ^ w10;
        en.erase(en.begin() + 6 + x25);
        en.insert(en.begin() + 6 + x25, w8);
    }


    // where 0x5e
    int a[] = {0x5e, 0x14};
    int x3 = 0;
    int x4 = 0;
    int x5;
    int x6;
    int x10 = 0x16;
    int x11 = 0x122;
    int x12 = 0 | 1;
    int x15 = 0x55;
    int x16 = 0x33;
    int index;
    int x14 = 0xffffffaa;
    int x8 = 0xffffffeb;
    while (x4 < 0x14)
    {
        x3 = x4 + 1;
        index = 6 + x4;
        int w7 = en.at(index);

        int w5 = ((w7 >> 4) + (w7 << 4)) & 0xff;
        en.erase(en.begin() + index);
        en.insert(en.begin() + index, w5);
        if (x21 > x3)
        {
            x5 = en.at(index + 1);
            x6 = en.at(index);
            x5 = x5 ^ x6;
            en.erase(en.begin() + index);
            en.insert(en.begin() + index, x5);
        }else{
            x6 = en.at(6);
            x5 =  x6 ^ w5;

            en.erase(en.begin() + index);
            en.insert(en.begin() + index, x5);
        }
        if (!x4)
        {
            x4 = x10;
            x5 = x11;
            x4 = x4 * x4 + x12;
            x5 = x5 * x5;
            x6 = (x5 << 3) & 0xffffffff;
            x5 = x6 - x5;
            // if ( x5 != x6){

            // }
        }
        x4 = en.at(index);
        x5 = x14 & (x4 << 1);
        x4 = x15 & (x4 >> 1);
        x4 = x4 | x5;
        x5 = x4 << 2;
        x5 = x5 & 0xffffffcf;
        x4 = x16 & (x4 >> 2);
        x4 = x4 | x5;
        x5 = (x4 >> 4) & 0xf;
        x5 = (x4 << 4) + x5;
        x4 = (x5 ^ x8) & 0xff;
        en.erase(en.begin() + index);
        en.insert(en.begin() + index, x4);
        x4 = x3;
    }
    stringstream ss;

    for (int byte : en)
    {
        if(byte < 16){
            ss << "0";
        }
        ss << hex  << byte;
        cout << "en:"
             << "0x" << hex << byte << endl;
    }

    cout << "X-G: " << ss.str() << endl;
    return 0;
}



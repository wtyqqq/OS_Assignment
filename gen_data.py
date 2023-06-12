#!/usr/bin/env python3
# -*- encoding: utf-8 -*-
'''
@File    :   gen_data.py
@Time    :   2023/04/25 09:40:20
@Author  :   Tianyi Wang
@Version :   1.0
@Contact :   tianyiwang58@gmail.com
@Desc    :   None
'''

# here put the import lib

def gen_data():
    # 生成1到1000的数据
    with open('testdata.txt', 'w') as f:
        for i in range(1, 1000001):
            f.write(str(i) + '\n')


if __name__ == "__main__":
    gen_data()
    print("data.txt generated.")

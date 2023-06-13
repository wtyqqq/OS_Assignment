# OS_Assignment

Tianyi Wang 2023/06/13

共有三个文件夹，分别为`pipe`、`shared_memory`和`less`。每个文件夹都有其对应的Makefile。

作业的报告在`report`目录下，若要从源码构建，请使用  `xelatex -> biber/bibtex -> xelatex -> xelatex`（编译两次生成reference）的编译链，若没有编译环境，请直接查看目录下的`report.pdf`。

文件结构
```bash
.
├── data 包含生成测试数据的 Python 脚本和生成好的测试数据。
│   ├── data.txt 生成测试数据的 Python 脚本
│   └── gen_data.py 生成好的测试数据
├── less 包含自制 less 文件的源代码和 Makefile 文件，以及在 X86 平台上编译好的二进制可执行文件。
│   ├── main.c 自制 less 文件的源代码
│   ├── Makefile
│   └── myless 二进制可执行文件
├── pipe 包含使用管道实现实验第一部分的所有源代码和 Makefile 文件，以及在 X86 平台上编译好的二进制可执行文件。
│   ├── main.c 源代码
│   ├── Makefile
│   ├── pipe 二进制可执行文件
│   ├── process_data.c 源代码
│   └── process_data.h 源代码
├── README.md
├── report 包含报告的所有 tex 源代码文件和编译好的 PDF 版本
│   ├── elegantpaper.cls
│   ├── image
│   │   ├── less_diff_height.png
│   │   ├── less_enter.png
│   │   ├── less_j.png
│   │   ├── less_k.png
│   │   ├── less_open.png
│   │   └── neofetch.png
│   ├── License
│   ├── README.md
│   ├── reference.bib
│   └── report.tex
├── result 存储结果的文件夹，这里存放了我进行实验时候生成的数据。
│   ├── pipe.txt
│   └── shared_mem.txt
└── shared_memory 包含使用共享内存实现实验第一部分的所有源代码和 Makefile 文件，以及在 X86平台上编译好的二进制可执行文件。
    ├── main.c 源代码
    ├── Makefile
    ├── share_mem 编译好的二进制可执行文件
    ├── process_data.c 源代码
    └── process_data.h 源代码
```

## 运行本项目

```bash
cd data
python3 gen_data.py # 生成数据集并创建result文件夹
cd ../pipe
make # 编译pipe
./pipe ../data/data.txt ../result/pipe.txt
# 如果锁核为以下命令
taskset 01 ./pipe ../data/data.txt ../result/pipe.txt
cd ../shared_memory
make # 编译shared_memory
./share_mem ../data/data.txt ../result/shared_mem.txt
# 如果锁核为以下命令
taskset 01 ./share_mem ../data/data.txt ../result/shared_mem.txt
cd ../less
make # 编译less
./myless ../result/pipe.txt
```


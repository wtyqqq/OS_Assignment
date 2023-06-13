# OS_Assignment

共有三个文件夹，分别为`pipe`、`shared_memory`和`less`。每个文件夹都有其对应的Makefile。

作业的报告在`report`目录下，若要从源码构建，请使用  `xelatex -> biber/bibtex -> xelatex -> xelatex`（编译两次生成reference）的编译链，若没有编译环境，请直接查看目录下的`report.pdf`。

## run

```bash
make
./out 
```

## generate data

```bash
python3 generate_data.py
```

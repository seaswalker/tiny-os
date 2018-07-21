# tiny-os
## 环境准备

### Bochs

在Mac上使用以下命令安装:

```shell
brew install bochs
```

### 配置

每个章节下的bios配置部分应视bochs的具体版本而定，可自行修改：

```html
# 对应真实机器的bios
romimage: file=/usr/local/Cellar/bochs/2.6.9_2/share/bochs/BIOS-bochs-latest
# 对应真实机器的VGA bios
vgaromimage: file=/usr/local/Cellar/bochs/2.6.9_2/share/bochs/VGABIOS-lgpl-latest
```

## 说明

每个章节下的代码均可以独立运行，在对应的目录下执行以下命令即可启动：

```shell
./build.sh
```

即可启动执行。完毕之后可执行以下命令清理环境：

```shell
./clean.sh
```

## 运行

### 第三章

![结果](images/chapter_3_result.png)


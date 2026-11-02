# framebuffer-gomoku

基于 Linux framebuffer 的五子棋游戏，直接操作 `/dev/fb0` 完成全部图形绘制，无 GUI 框架依赖，可运行于 i.MX6ULL 开发板的 800×480 LCD，也可在任意 Linux 主机的帧缓冲设备上运行。

## 功能特性

- **帧缓冲绘图**：封装 framebuffer 基础绘图库——像素点、水平/竖直线、矩形、圆、全屏清屏，支持 32 位 ARGB 颜色
- **棋盘渲染**：20×20 网格棋盘，网格线动态计算绘制，红色方块光标指示当前落子位置，黑白棋子实时刷新
- **BMP 解析**：自行按文件头结构（`BITMAPFILEHEADER` / `BITMAPINFOHEADER`，`#pragma pack(1)`）解析 24 位 BMP 位图，实现棋子贴图绘制
- **游戏逻辑**：wasd 移动光标、空格落子，黑白轮流；四方向（横、竖、两条对角线）连珠判胜
- **Makefile 一键编译**，多文件工程管理（头文件声明 / 源文件实现分离）

## 目录结构

```
├── main.c          # 程序入口：初始化 framebuffer → 绘制棋盘 → 开始游戏
├── framebuffer.c/h # 帧缓冲绘图库：init_fb / draw_point / draw_h_line / draw_circle ...
├── game.c/h        # 游戏逻辑：棋盘绘制、光标移动、落子、四方向判胜
├── black.bmp       # 黑棋子位图素材
├── white.bmp       # 白棋子位图素材
└── Makefile
```

## 编译与运行

```bash
make          # 生成 a.out
./a.out       # 需要 /dev/fb0 的读写权限
```

操作方式：`w/s/a/d` 移动光标，空格落子。

在开发板上运行时，通过 NFS 或 scp 将可执行文件拷贝到板端，串口/SSH 终端执行即可。

## 技术要点

- framebuffer 像素级绘图：`mmap` 映射显存，按屏幕分辨率与 bpp 计算偏移量直接写内存
- BMP 文件按 `#pragma pack(1)` 紧凑结构解析，避免编译器对齐导致的偏移错位
- 判胜算法：以落子点为中心，向四个方向的两侧延伸计数，任意方向连成五子即获胜
- 全局棋盘状态用二维数组 `chess_g[20][20]` 表示（1 = 白子，-1 = 黑子，0 = 空）

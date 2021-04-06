# Twister技术文档

## 项目结构表

### 主程序

|         文件名          |                   说明                   |
| :---------------------: | :--------------------------------------: |
|      `twister.cpp`      |  包含程序主函数入口`main()`和程序主流程  |

### 局面结构

|         文件名          |                   说明                   |
| :---------------------: | :--------------------------------------: |
|   `representation.h`    |     数据的表示，包括棋盘 棋子 着法等     |
|  `representation.cpp`   |     数据的表示，包括棋盘 棋子 着法等     |
|       `preset.h`        |           预置的数组与相关判断           |
|      `preset.cpp`       |           预置的数组与相关判断           |
|   `moves_getter.cpp`    |       着法生成器，生成当前所有着法       |
|     `moves_sort.h`      |         着法排序，将所有着法排序         |
|    `moves_sort.cpp`     |         着法排序，将所有着法排序         |

### 搜索技术

|         文件名          |                   说明                   |
| :---------------------: | :--------------------------------------: |
|       `search.h`        |          实现博弈树搜索相关算法          |
|      `search.cpp`       |          实现博弈树搜索相关算法          |
|  `hash_table.h`  |              置换表相关操作              |
| `hash_table.cpp` |              置换表相关操作              |

### 局面评价

|         文件名          |                   说明                   |
| :---------------------: | :--------------------------------------: |
|  `value.cpp`  |               局面评估方法               |
|   `value.h`   |               局面评估方法               |

### 其他

|         文件名          |                   说明                   |
| :---------------------: | :--------------------------------------: |
|    `ucci_protocol.h`    | 处理`UCCI`协议，完成界面与引擎之间的通讯 |
|   `ucci_protocol.cpp`   | 处理`UCCI`协议，完成界面与引擎之间的通讯 |
|  `game_start_repos.h`   |            游戏开局库相关操作            |
| `game_start_repos.cpp`  |            游戏开局库相关操作            |

## 规范

### 命名

- 命名空间`C++ std`

```c++
using namespace std;
```

- 命名规范

①不允许使用缩写

```c++
// α-β剪枝搜索函数
// 不允许使用
void ABSearch();
// 应使用命名
void AlphaBetaSearch();

// 着法计数器
// 不允许使用
int cnt;
// 应使用
int movements_counter;
```

③大括号使用`{}` 及代码书写

```c++
// { }
int main() {
}

// 空格
i = 9;
for(int i = 0; i < cnt; i ++){
    a = b;
    c += d;
}
```

④类型定义 和 宏定义 命名应使用全大写

```c++
// 类型定义
typedef unsigned char PLAYER;
// 宏定义
#define ISBLACK(x) (x >> 8)
```

⑤命名规则

```c++
// 函数命名用大驼峰
int GetMovementNumber();

// 全局变量用大驼峰
int School;
    
// 常量用大写
const int MAX_DEPTH;

// 局部变量命名用下划线
Movement best_move;

// 结构体用大驼峰
struct Movement{
    
}
```

⑥注释

```c++
// 变量注释(尽量在同一行，不同变量的注释尽量tab对齐)

Movement best_move;     // 最佳的着法
const int MAX_DEPTH;    // 最大搜索深度

// 函数注释

/* 
 * 函数名：xxx
 * 描述：xxxxxxxxx
 * 入参：
 * - int a: xxxx
 * - bool b: xxxx
 * - double c: xxxx
 * 返回值：
 * - int h: xxxx
 * 最后更新时间: xxxx
 */
int GetMovementNumber(){
    // 1. xxxxx(独立成行)
    a = 1;
    // 2. xxxxx
    b = 1;
    // 3. xxxxx
    cout << a + b << endl;
    // 4. xxxx
    return c;
}

// 文件注释

/* 
 * 文件名：xxx.xx
 * 描述：xxxxxxxx
 * 作者: xxx
 * 最后更新时间: xxxx 
 */
#include <iostream>
xxxxx
xxxxx
xxxxx
...

```

### Git协作

- `push`规范

```bash
$ git add .
$ git commit -m '-member -time -version -description'
$ git push ...

# 或者多行commit
$ git commit
# 在vscode中写多行注释
# title : -member -time -version -short_description
# content: -long_description
```

```bash
# 例子
$ git add.
$ git commit -m '-yao -03.21.17 -v0.5 -完善search.h' 
```

## API

> 由组员在开发过程中分文件进行不断完善，包括结构体定义，变量的含义，函数的接口定义以及备注是否存在bug

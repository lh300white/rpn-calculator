import nimib

nbInit
nbText: """
# RPN 计算器使用说明书

##1. 安装和编译指南
- sudo apt install g++ -y
- GCC 4.8+ 或 Clang 3.3+
- C++11 支持

##2. 基本使用示例
"""

nbCode:
  echo "示例: 计算 '3 4 +'"
  echo "输入: 3 4 +"
  echo "输出: 7"

nbText: """
##3. 所有支持的操作说明

- `+` : 加法
- `-` : 减法  
- `*` : 乘法
- `/` : 除法
- `^` : 幂运算
操作数和运算符之间用空格分隔

##4. 错误代码和异常说明
"""

nbCode:
  echo "0: SUCCESS - 成功"
  echo "1: INVALID_TOKEN - 无效令牌"
  echo "2: DIVISION_BY_ZERO - 除零错误"
  echo "3: INSUFFICIENT_OPERANDS - 操作数不足"
  echo "4: UNSUPPORTED_OPERATION - 不支持的操作"

nbText: """
##5. 示例输入输出
"""

nbCode:
  let examples = @[
    ("2 3 +", "5"),
    ("5 1 -", "4"),
    ("3 4 *", "12"),
    ("10 2 /", "5"),
    ("2 3 ^", "8"),
    ("1 2 + 4 *", "12")
  ]
  
  for (input, output) in examples:
    echo "输入: ", input
    echo "输出: ", output
    echo ""

nbSave

### 文件目录
 ```shell
├─defense
│      debug.txt        //输出二元式、产生式、标识符表、临时变量表、四元式  
│      defense.md       //答辩文件  
│      GCompiler.exe    //程序  
│      in.txt           //缺省输入程序  
│      out.txt          //输出四元式、格式化源程序  
│      optiable.txt     //可优化输入程序  
```
### 参数设定
```shell
-i  指定输入的参数
-v  程序版本
-O  进行优化
-h  使用帮助
```
### 额外功能
1. 源代码格式化输出
2. 无用代码删除
3. 公共子表达式消除

### 使用示例
```shell
Your/File/Path> .\GCompiler.exe -v
GCompiler Vision : 1.0.0-BETA
Your/File/Path> .\GCompiler.exe -h
[-i]      Choose the input file,the default read file is in.txt.
[-v]      Check the version.

[-O]      Open the optimization.
```
1. 源代码格式化输出
```cpp
Your/File/Path> .\GCompiler.exe
//>>out.txt
string y;
y = "yqw";
y = y * 9;
do
  y = y + "y"
while(y * 5 >= y * 2);
if(y + "e" == y + "v")
  start
    y = y + "l";
    y = y * 2;
  end
else
  y = y + "j"
;
```
2. 无用代码删除
```shell
Your/File/Path> .\GCompiler.exe -i optiable.txt [-O]
```  

删除前
```
//>>debug.txt
(0)(=,a,null,a)
(1)(=,b,null,b)
(2)(=,c,null,c)
(3)(=,d,null,d)
(4)(=,a,null,a)
```
删除后
```
//>>debug.txt
(0)(=,a,null,a)
(1)(=,b,null,b)
(2)(=,c,null,c)
(3)(=,d,null,d)
```
3. 公共子表达式消除  
```shell
Your/File/Path> .\GCompiler.exe -i optiable.txt [-O]
```  

消除前
```
//>>debug.txt
(7)(+,a,b,T2)
(8)(=,T2,null,b)
(9)(+,a,b,T3)
(10)(=,T3,null,c)
(11)(+,a,b,T4)
(12)(=,T4,null,d)
```  
消除后
```
//>>debug.txt
(6)(+,a,b,T2)
(7)(=,T2,null,b)
(8)(+,a,b,T3)
(9)(=,T3,null,c)
(10)(=,T3,null,d)
```
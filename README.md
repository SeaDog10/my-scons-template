# my-scons-template
## 项目简介

一套通用 SCons+Kconfig 工程管理模板

## python工具及其依赖

1. `python 3.6.8`

2. 执行以下命令安装依赖：

   ```cmd
   pip install -r requirements.txt
   ```

   `requirements.txt`文件位于` .\my-scons-template\template\py_tools`路径下。

3. `config.py `文件主要用于解析 `Kconfig `文件并生成` config.h `文件，如不需要该功能，需在 `SConstruct `目录下放置一个空的` config.h` 文件即可。

4. `building.py `文件主要用于提供一些编译所需的工具函数，无需任何修改。

## 编译功能

1. 执行 `scons` 命令后会根据目录下的 `build.json` 中定义的信息开始编译工程，其模板位于 `.\my-scons-template\template\json_template` 目录下，其内容如下：

   ```json
   {
       "biuld_flags":{
           "exec_path" : "",
           "target"    : "",
           "biuld_dir" : ""
       },
       "environment":{
           "tools"     : "",
           "AS"        : "",
           "CC"        : "",
           "CXX"       : "",
           "AR"        : "",
           "LINK"      : "",
           "ARFLAGS"   : [
               "",
               "",
               "",
               ""
           ],
           "LINKFLAGS" : [
               "",
               "",
               "",
               ""
           ],
           "ASFLAGS" : [
               "",
               "",
               "",
               ""
           ],
           "CFLAGS" : [
               "",
               "",
               "",
               ""
           ],
           "CXXFLAGS" : [
               "",
               "",
               "",
               ""
           ]
       },
       "pre_defines" : [
           "",
           "",
           "",
           ""
       ]
   }
   ```

   * `biuld_flags`：执行编译所需的基本属性。
   * `biuld_flags->exec_path`：工具链路径。
   * `biuld_flags->target`：编译产物的名称，多为 `xxx.elf` 。
   * `biuld_flags->biuld_dir`：编译过程产物和最终产物的放置路径。
   * `environment`：编译和链接参数定义。
   * `environment->tools`：用于指定所使用的编译器。
   * `environment->AS`：汇编器名称。
   * `environment->CC`：C 编译器名称。
   * `environment->CXX`：C++ 编译器名称。
   * `environment->AR`：静态库管理器名称。
   * `environment->LINK`：链接器的名称。
   * `environment->ARFLAGS`：传递给静态库管理器的参数。
   * `environment->LINKFLAGS`：传递给链接器的参数。
   * `environment->ASFLAGS`：传递给汇编器的参数。
   * `environment->CFLAGS`：传递给 C 编译器的参数。
   * `environment->CXXFLAGS`：传递给 C++ 编译器的参数。
   * `pre_defines`：预定义宏，主要用于条件编译。

## 配置功能

1. 本套模板支持使用 `Kconfig `定义工程的配置或依赖等，`Kconfig `具体使用方法在此不做多余介绍，请自行搜索了解，如需使用本模板的配置功能，请在 `config.py `文件同级目录下创建 `Kconfig `文件，在执行 `python config.py `命令后将自行生成 `config.h `文件。

2. 工程配置保存过后会自动生成 `config.h` 文件，该文件会由 `scons` 进行解析并进行条件编译，在 `Sconscript` 文件中可判断是否定义某个宏定义从而进行条件编译，`SConscript` 文件模板内容如下：

   ```python
   import os
   from building import *
   
   Import('env', 'pre_defines')
   
   def GetCurrentDir():
       conscript = File('SConscript')
       fn = conscript.rfile()
       name = fn.name
       path = os.path.dirname(fn.abspath)
       return path
   
   cwd           = GetCurrentDir()
   list          = os.listdir(cwd)
   objs          = []
   source        = []
   include_path  = []
   user_defines  = []
   
   # User Define
   
   # User Define
   
   pre_defines += user_defines
   objs = [env.Object(src) for src in source]
   
   for d in list:
       path = os.path.join(cwd, d)
       if os.path.isfile(os.path.join(path, 'SConscript')):
           sub_objs, sub_path = SConscript(os.path.join(d, 'SConscript'))
           objs.extend(sub_objs)
           include_path.extend(sub_path)
   
   Return('objs', 'include_path')
   ```

   该文件为通用文件，仅需修改 `# User Define` 部分内容即可适应绝大多数工程目录管理，可配置内容有：

   * `source`：该列表为源文件列表，可往其中添加 `.c、.S、.cpp` 等源文件，语法如下：

     ``` python
     # User Define
     source += Glob('hello_world.c') #添加当前目录下的 hello_world.c 文件
     source += Glob('*.c') #添加当前目录下所有.c文件，‘*’为通配符
     source.remove(['hello_world.c']) #从源文件列表删除 hello_world.c 文件
     
     source += Glob('hello_world.cpp') #添加当前目录下的 hello_world.cpp 文件
     source += Glob('*.cpp') #添加当前目录下所有.cpp文件，‘*’为通配符
     source.remove(['hello_world.cpp']) #从源文件列表删除 hello_world.cpp 文件
     
     source += Glob('hello_world.S') #添加当前目录下的 hello_world.S 文件
     source += Glob('*.S') #添加当前目录下所有.S文件，‘*’为通配符
     source.remove(['hello_world.S']) #从源文件列表删除 hello_world.S 文件
     # User Define
     ```

   * `include_path`：该列表为头文件路径列表，可往其中添加头文件路径以供调用，语法如下：

     ```python
     # User Define
     include_path.append(cwd) #添加当前路径到头文件列表
     # User Define
     ```

   * `user_defines`：该列表为用户定义宏列表，可往其中添加预定义的宏定义，主要供编译使用，语法如下：

     ```python
     # User Define
     user_defines += ['NUM1=1', 'NUM2=2'] #添加两个用户宏定义
     # User Define
     ```

   * `GetDepend`：该方法可以判断配置文件中是否定义了某个宏，语法如下：

     ```python
     # User Define
     #如果定义了 USER_USING_HELLO_WORLD ，将 hello_world.c 添加到源文件列表
     if GetDepend('USER_USING_HELLO_WORLD'):
         source += Glob('hello_world.c')
     
     #如果未定义 USER_USING_HELLO_WORLD ，将 hello_world.c 从源文件列表移除
     if GetDepend('USER_USING_HELLO_WORLD') == False:
         source.remove(['hello_world.c'])
     # User Define
     ```


## 示例工程

关于本模板的具体使用示例，请参照 `.\my-scons-template\demo_project` 目录下的示例工程。

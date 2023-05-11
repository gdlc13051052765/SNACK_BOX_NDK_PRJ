
APP_OPTIM := debug

APP_CFLAGS += -std=c99
#做或者c++编程的都应该知道编译器编译参数设置
APP_CPPFLAGS +=
#做或者c++编程的都应该知道编译器编译参数设置
APP_LDFLAGS += -llog
#做或者c++编程的都应该知道编译器链接参数设置，这里是填写依赖了哪些动态库
APP_STL +=
#生成 对应 某一运行时库的动态库文件

APP_PIE = true
# 生成位置独立的代码，

APP_ABI = armeabi-v7a
#这个变量我们会经常遇到，主要是指定app或者说so运行的cpu指令集。

APP_PLATFORM = android-7
#这个对应的android版本号

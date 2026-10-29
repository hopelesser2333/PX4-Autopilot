#include <px4_log.h>

/**
 * @brief 测试应用程序的主函数
 * @param argc 命令行参数的数量，包括应用程序自身路径和程序名
 * @param argv 命令行参数的数组，字符串指针数组类型的参数向量，数组中每一个元素指向命令行中的一个命令字符串
 * @return 0 表示成功，非0表示失败
 * @note PX4 系统所基于的编译器语法规则比较严格，因此在编写代码之前，必须生命该函数不然会报错
 * @note 该函数使用了 extern "C" 声明，以确保在 C++ 编译器中使用 C 链接方式，避免名称修饰问题
 * @note __EXPORT 声明的函数一般作为应用程序的入口函数，且必须采用 <module_name>_main 的命名方式
 */
extern "C" __EXPORT int test_app_main(int argc, char *argv[]);

int test_app_main(int argc,char *argv[])
{
	PX4_INFO("Hello World!");//日志函数会自动还行
	return 0;
}

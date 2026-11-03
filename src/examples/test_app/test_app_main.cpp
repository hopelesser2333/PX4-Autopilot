#include <px4_log.h>
#include <string.h>
#include <px4_platform_common/tasks.h>
#include <drivers/drv_hrt.h>


using namespace time_literals; // 使用时间字面量命名空间，以便在代码中使用时间单位的字面量表示法，如 1_s 表示 1 秒

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
int start_command_base(int argc,char *argv[]);
int status_command();
int stop_commnad();
int custom_command(int argc, char *argv[]);
int print_usage();

//static int is_running = 0; // 用于标记应用程序是否正在运行，0表示未运行，1表示正在运行
static px4_task_t task_id = -1; // 用于存储应用程序的任务ID，初始值为-1，表示未启动任务
static bool _task_should_exit = false; // 用于标记应用程序是否应该退出，false表示不退出，true表示应该退出


int test_app_main(int argc,char *argv[])
{
	if(argc <= 1 || strcmp(argv[1] ,"-h") == 0 || strcmp(argv[1], "help") == 0 ||
	  strcmp(argv[1],"info")== 0 || strcmp(argv[1],"usage") == 0)
	{
		return print_usage();
	}
	else if(strcmp(argv[1], "start") == 0)
	{
		return start_command_base(argc, argv);
	}
	else if(strcmp(argv[1], "status") == 0)
	{
		return status_command();
	}
	else if(strcmp(argv[1], "stop") == 0)
	{
		return stop_commnad();
	}
	int ret = custom_command(argc - 1, argv + 1);

	return ret;
}

/**
 * @brief 代替原来的static int is_running变量，使用函数来判断应用程序是否正在运行
 *
 * @return true 应用程序正在运行
 * @return false 应用程序未运行
 */
bool is_running()
{
	return task_id != -1;
}

/**
 * @brief 休眠指定的时间，单位为微秒
 *
 */
void run()
{

	PX4_INFO("Hello World!");
	while (!_task_should_exit){
		hrt_abstime now = hrt_absolute_time();// 获取当前时间
		PX4_INFO("PX4 running time is %u (s)",(unsigned int)(now / 1_s));// 将时间转换为秒并打印
		hrt_abstime time_count = hrt_elapsed_time(&now);// 计算从上次获取时间到现在的时间间隔
		px4_usleep(1_s - time_count);// 休眠一秒减去已经消耗的时间，确保每次循环间隔为一秒
	}
}

/**
 * @brief 启动应用程序的运行函数
 *
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return int 返回PX4_OK表示成功
 */
int run_trampoline(int argc,char * argv[]){
	_task_should_exit = false;
	run();
	return PX4_OK;
}

/**
 * @brief 启动应用程序的任务
 *
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return int 返回0表示成功，非0表示失败
 */
int task_spawn(int argc,char * argv[])
{
	task_id = px4_task_spawn_cmd("test_app",SCHED_DEFAULT,SCHED_PRIORITY_MIN + 5,1024 ,(px4_main_t)&run_trampoline,(char *const *)argv);
	if(task_id < 0){
		task_id = -1;
		return -1;
	}
	return 0;
}


/**
 * @brief 启动应用程序的基础命令函数，检查应用程序是否已经在运行，如果未运行则启动任务
 *
 * @param argc
 * @param argv
 * @return int
 */
int start_command_base(int argc,char *argv[])
{
	//is_running = 1; // 标记应用程序正在运行
	int ret = 0;

	if (is_running())
	{// 如果应用程序已经在运行，返回错误码-1，并打印提示信息
		ret = -1;
		PX4_INFO("app is already running!");
	}else{// 如果应用程序未运行，启动任务并返回结果

		ret = task_spawn(argc,argv);
		if (ret < 0)
		{
			PX4_INFO("Task start failed (%i)",ret);
		}
	}
	return ret;
}

int status_command()
{
	if (is_running())
	{PX4_INFO("app is running!");}
	else
	{PX4_INFO("app is not running!");}
	return 0;
}

int stop_commnad()
{
	_task_should_exit = true; // 标记应用程序应该退出
	px4_sleep(2); // 等待2秒，确保任务有足够的时间退出
	task_id = -1; // 重置任务ID，表示应用程序未运行
	PX4_INFO("app is stopped!");
	return 0;
}

int custom_command(int argc, char *argv[])
{
	PX4_ERR("Unknown command!");
	return 0;
}

int print_usage()
{
	PX4_INFO("This is a test app!");
	return 0;
}




px4_add_module(
	MODULE examples__test_app	#模块名。命名规则：通过“__”（这里是两个下划线）分割文件路径。起始路径是./src 无需给出.
	MAIN test_app             	#主函数名。用于确定程序的主函数。命名规则：主函数名去掉"_main"后缀。这里是test_app_main.cpp，所以主函数名是test_app。
	COMPILE_FLAGS
	SRCS				#源文件列表。这里是test_app_main.cpp
		test_app_main.cpp
	DEPENDS				#依赖项列表，这里没有用到，此处为空
)


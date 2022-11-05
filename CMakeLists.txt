# CMakeList.txt: GCompiler 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("GCompiler")

# 将源代码添加到此项目的可执行文件。
add_executable (GCompiler "GCompiler.cpp" "GCompiler.h" "LexicalAnalysis/lexical_analyzer.cpp" "LexicalAnalysis/lexical_analyzer.h" "LexicalAnalysis/two_tuple.cpp" "LexicalAnalysis/two_tuple.h" "GrammaticalAnalysis/gramma_analyzer.h" "GrammaticalAnalysis/gramma_analyzer.cpp" "SemanticAnalysis/identifer_table.cpp" "SemanticAnalysis/identifer_table.h" "SemanticAnalysis/four_tuple.cpp" "SemanticAnalysis/four_tuple.h" "SemanticAnalysis/middle_code_table.cpp" "SemanticAnalysis/middle_code_table.h" "SemanticAnalysis/temp_var_table.h" "SemanticAnalysis/temp_var_table.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GCompiler PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

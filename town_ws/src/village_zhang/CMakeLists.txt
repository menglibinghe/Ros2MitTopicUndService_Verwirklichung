cmake_minimum_required(VERSION 3.8)
project(village_zhang)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

#新添加的依赖
find_package(std_msgs REQUIRED)
find_package(village_interfaces REQUIRED)



if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

#让编译器编译wang2.cpp这个文件
add_executable(zhang3_node src/zhang3.cpp)
add_executable(zhang4_node src/zhang4.cpp)
ament_target_dependencies(zhang3_node
 rclcpp std_msgs        #Added Std_msgs
  village_interfaces)   

ament_target_dependencies(zhang4_node
 rclcpp std_msgs        #Added Std_msgs
  village_interfaces)   #添加自定义接口包 


#将手动将编译好的文件安装到install/village_zhang/lib/village_zhang下
install(TARGETS
  zhang3_node
  zhang4_node
  DESTINATION lib/${PROJECT_NAME}
)

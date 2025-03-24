cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_COMPILER "g++")

add_compile_options(-Wall -lstdc++)
add_compile_options(-Wextra -Wpedantic)

# cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find QT components
find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()

file(GLOB SRCS
	"*.h"
	"*.cc"
	"*.ui"
)

# gather gui source files
qt_add_resources(GUI_RESOURCES "resources.qrc")

add_executable(risc_vector ${SRCS} ${GUI_RESOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib Qt6::Widgets)

set_target_properties(risc_vector PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
	

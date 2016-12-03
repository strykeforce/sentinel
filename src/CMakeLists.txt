add_executable(sentinel
  board_tests.cc
  sentinel.cc
  test.cc
)

target_link_libraries(sentinel
  PUBLIC
  sidewinder
  wpi
)

add_custom_target(deploy
COMMAND
scp ${CMAKE_BINARY_DIR}/src/sentinel roborio:
VERBATIM
DEPENDS sentinel
)

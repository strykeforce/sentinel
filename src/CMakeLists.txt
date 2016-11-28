add_executable(sentinel
  sentinel.cc
  test.cc
  board_tests.cc
)

target_link_libraries(sentinel
  PUBLIC
  wpi
)

add_custom_target(deploy
COMMAND
scp ${CMAKE_BINARY_DIR}/src/sentinel roborio:
VERBATIM
DEPENDS sentinel
)

qt_add_executable(${PROJECT_NAME}
  main.cpp
  qml/qml.qrc
  image_provider.cpp
  pattern_generator.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Quick)

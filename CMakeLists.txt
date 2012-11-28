add_library(procmounter SHARED service.cpp service_ipc_client.cpp sockets.cpp)

add_executable(procmounter_daemon service_daemon.cpp service_ipc_server.cpp diy.cpp sockets.cpp)

add_executable(procmounter_test test.cpp)
target_link_libraries(procmounter_test
	procmounter
)

install(TARGETS procmounter DESTINATION lib)
install(FILES procmounter.h DESTINATION include)

install(TARGETS procmounter_daemon DESTINATION sbin)

##FIXME: Clean this up. Should it exist at all? As a different package?
install(TARGETS procmounter_test DESTINATION bin)
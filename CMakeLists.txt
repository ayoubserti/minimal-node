cmake_minimum_required(VERSION 2.8)


project(minimal-node)


#function target builder
function(register_target tname)
		file(GLOB SOURCE_FILES_${tname} "${CMAKE_CURRENT_SOURCE_DIR}/src/${tname}/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/src/${tname}/*.h")
		
		add_executable(${tname} ${SOURCE_FILES_${tname}})
		target_include_directories(${tname} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/v8"
													 "${CMAKE_CURRENT_SOURCE_DIR}/include/v8/include")
 		SET_TARGET_PROPERTIES("${tname}" PROPERTIES COMPILE_FLAGS "-MTd")
		target_link_libraries(${tname} "${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/v8_libplatform.lib"

								"${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/v8_base_0.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/v8_base_1.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/v8_base_2.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/v8_base_3.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/v8_libbase.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/v8/v8_snapshot.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/icu/icuucx.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/icu/icui18n.lib"	
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/icu/icudata.lib"
								"${CMAKE_CURRENT_SOURCE_DIR}/libs/icu/icustubdata.lib"
								"winmm.lib")

endfunction()


register_target(tuto1)
register_target(tuto2)
register_target(tuto3)
register_target(tuto4)
register_target(tuto5)
register_target(tuto6)
register_target(tuto7)
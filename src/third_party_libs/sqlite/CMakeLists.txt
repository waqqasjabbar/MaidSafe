#==================================================================================================#
#                                                                                                  #
#  Copyright 2014 MaidSafe.net limited                                                             #
#                                                                                                  #
#  This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,        #
#  version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which    #
#  licence you accepted on initial access to the Software (the "Licences").                        #
#                                                                                                  #
#  By contributing code to the MaidSafe Software, or to this project generally, you agree to be    #
#  bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root        #
#  directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also available   #
#  at: http://www.maidsafe.net/licenses                                                            #
#                                                                                                  #
#  Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed    #
#  under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF   #
#  ANY KIND, either express or implied.                                                            #
#                                                                                                  #
#  See the Licences for the specific language governing permissions and limitations relating to    #
#  use of the MaidSafe Software.                                                                   #
#                                                                                                  #
#==================================================================================================#


project(sqlite)

include(${CMAKE_SOURCE_DIR}/cmake_modules/standard_setup.cmake)

add_library(sqlite STATIC ${PROJECT_SOURCE_DIR}/include/sqlite3.h ${PROJECT_SOURCE_DIR}/src/sqlite3.c)
#target_compile_options(sqlite PRIVATE $<$<BOOL:${MSVC}>:/wd4244>)
ms_target_include_system_dirs(sqlite PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(sqlite PRIVATE SQLITE_OMIT_LOAD_EXTENSION)

if(MSVC)
  target_link_libraries(sqlite ${JustThread_LIBRARIES})
else()
  target_link_libraries(sqlite ${JustThread_LIBRARIES} -lpthread)
endif()

set(AllStaticLibsForCurrentProject sqlite)

ms_add_executable(sqlite_test "." ${PROJECT_SOURCE_DIR}/src/sqlite_test.cc)
ms_add_executable(speedtest1 "." ${PROJECT_SOURCE_DIR}/src/speedtest1.cc)
ms_add_executable(wordcount "." ${PROJECT_SOURCE_DIR}/src/wordcount.c)

target_link_libraries(sqlite_test sqlite maidsafe_test ${BoostFilesystemLibs})
target_link_libraries(speedtest1 sqlite maidsafe_test ${BoostFilesystemLibs})
target_link_libraries(wordcount sqlite maidsafe_test ${BoostFilesystemLibs})

if(MSVC)
  set_target_properties(sqlite sqlite_test speedtest1 wordcount PROPERTIES FOLDER "Third Party/SQLite")
  set(AllExesForCurrentProject sqlite_test speedtest1 wordcount)
else()
  ms_add_executable(crashtest1 "." ${PROJECT_SOURCE_DIR}/src/crashtest1.cc)
  ms_add_executable(threadtest1 "." ${PROJECT_SOURCE_DIR}/src/threadtest1.c)
  ms_add_executable(threadtest2 "." ${PROJECT_SOURCE_DIR}/src/threadtest2.c)
  ms_add_executable(threadtest3 "." ${PROJECT_SOURCE_DIR}/src/threadtest3.c)

  target_link_libraries(crashtest1 sqlite maidsafe_test ${BoostFilesystemLibs})
  target_link_libraries(threadtest1 sqlite maidsafe_test ${BoostFilesystemLibs})
  target_link_libraries(threadtest2 sqlite maidsafe_test ${BoostFilesystemLibs})
  target_link_libraries(threadtest3 sqlite maidsafe_test ${BoostFilesystemLibs})

#  target_include_directories(threadtest3 ${PROJECT_SOURCE_DIR}/src/)  // FIXME(Brian)

  set_target_properties(sqlite sqlite_test crashtest1 speedtest1 threadtest1 threadtest2 threadtest3 wordcount PROPERTIES FOLDER "Third Party/SQLite")
  set(AllExesForCurrentProject sqlite_test speedtest1 crashtest1 speedtest1 threadtest1 threadtest2 threadtest3 wordcount)
endif()

add_definitions(-DSQLITE_ENABLE_RTREE)

include(../../../cmake_modules/standard_flags.cmake)

install(TARGETS sqlite COMPONENT Development CONFIGURATIONS Debug Release ARCHIVE DESTINATION lib)
install(FILES ${PROJECT_SOURCE_DIR}/include/sqlite.h COMPONENT Development DESTINATION include/maidsafe/third_party_libs/sqlite)
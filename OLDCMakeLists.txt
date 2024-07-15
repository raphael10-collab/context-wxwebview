cmake_minimum_required(VERSION 3.5)
project(ContextMenuWebView)


find_package(wxWidgets REQUIRED COMPONENTS base core webview aui stc net xml html richtext)
include(${wxWidgets_USE_FILE})


include(FetchContent)

find_package(CURL)
find_package(OpenSSL)
find_package(Threads)
find_package(nng CONFIG REQUIRED)
find_package(json-c)
find_package(PkgConfig REQUIRED)

FetchContent_Declare(
  PDFHummus
  URL https://github.com/galkahana/PDF-Writer/archive/refs/tags/v4.6.2.tar.gz
  URL_HASH SHA256=0a36815ccc9d207028567f90039785c824b211169ba5da68de84d0c15455ab62
  DOWNLOAD_EXTRACT_TIMESTAMP FALSE
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(PDFHummus)


pkg_check_modules(tesseract REQUIRED IMPORTED_TARGET tesseract)
set_property(TARGET PkgConfig::tesseract APPEND PROPERTY INTERFACE_LINK_LIBRARIES curl archive)
pkg_check_modules(leptonica REQUIRED IMPORTED_TARGET lept)
pkg_check_modules(libcurl REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(json-c REQUIRED IMPORTED_TARGET json-c)
pkg_check_modules(sqlite3 REQUIRED IMPORTED_TARGET sqlite3)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "-fexceptions")

#add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE context-webview.cpp)
#set_target_properties(${PROJECT_NAME})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  add_executable(${PROJECT_NAME}
    context-webview.cpp
    EmptyFileTest.cpp
  )

elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_executable(${PROJECT_NAME
    context-webview.cpp
    EmptyFileTest.cpp
  )
endif()  


target_link_libraries (${PROJECT_NAME} PUBLIC
    wxWidgets::wxWidgets
    PDFHummus::PDFWriter
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    curl
    nng::nng
    xml2
    json-c::json-c
    icui18n
    icuuc
    stfl
    ncursesw
    sqlite3
    pdfio
)


target_compile_definitions(${PROJECT_NAME} PRIVATE NNG_ELIDE_DEPRECATED)

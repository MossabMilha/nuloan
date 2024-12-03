cmake_minimum_required(VERSION 3.10)
project(nuloan_project C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Include the Structures directory
include_directories(${CMAKE_SOURCE_DIR})

# Find GTK4 using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Check if GTK4 was found
if (NOT GTK4_FOUND)
    message(FATAL_ERROR "GTK4 not found. Please install it.")
endif()

# Include GTK include directories
include_directories(${GTK4_INCLUDE_DIRS})

# Find libgcrypt
pkg_check_modules(GCRYPT REQUIRED libgcrypt)
include_directories(${GCRYPT_INCLUDE_DIRS})
link_directories(${GCRYPT_LIBRARY_DIRS})

# Find the CURL library
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# Add your source files
set(SOURCE_FILES
        main.c
        Check_User_Information.c
        Check_Signup_Information.c
        Check_email.c
        Send_Email.c
        hash_password.c
        Bank_information.c
        Check_Bank_Information.c
        Save_file.c
        Eligibility.c
        Transactions.c
        LOAN_REPAYMENT_SCHEDULES.c
        LOAN_APPLICATIONS.c
        LOAN_TERMS.c
        SignUp.c
        SignUp.h
)

# Add your executable and link it with GTK4, libgcrypt, CURL, and regex libraries
add_executable(nuloan_project ${SOURCE_FILES})
target_link_libraries(nuloan_project ${GTK4_LIBRARIES} ${GCRYPT_LIBRARIES} ${CURL_LIBRARIES} regex)

# Ensure the executable uses the GTK framework correctly
target_compile_options(nuloan_project PRIVATE ${GTK4_CFLAGS})

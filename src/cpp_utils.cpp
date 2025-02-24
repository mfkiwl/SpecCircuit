#include "cpp_utils.h"

#include <chrono>

using namespace std::chrono;
using namespace std;

#include <iostream>

namespace FlatCircuit {

  void compileCppLib(const std::string& cppName,
                     const std::string& targetBinary) {

    std::string optFlags = "-O3";

#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
    assert(false);
#ifdef _WIN64
    //define something for Windows (64-bit only)
    assert(false);
#else
    //define something for Windows (32-bit only)
    assert(false);
#endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    assert(false);
#elif TARGET_OS_IPHONE
    // iOS device
    assert(false);
#elif TARGET_OS_MAC
    // Other kinds of Mac OS
    string compileCommand =
      "g++ " + optFlags + " -std=c++11 -fPIC -dynamiclib -Ijitted_utils/ " +
      cppName + " -o " + targetBinary;

#else
#   error "Unknown Apple platform"
    assert(false);
#endif
#elif __linux__
    // linux
    string compileCommand =
      "g++ " + optFlags + " -std=c++11 -fPIC -rdynamic -shared -Ijitted_utils/ " +
      cppName + " -o " + targetBinary;

#elif __unix__ // all unices not caught above
    // Unix
    string compileCommand =
      "g++ " + optFlags + " -std=c++11 -fPIC -rdynamic -shared -Ijitted_utils/ " +
      cppName + " -o " + targetBinary;

#elif defined(_POSIX_VERSION)
    // POSIX
    string compileCommand =
      "g++ " + optFlags + " -std=c++11 -fPIC -rdynamic -shared -Ijitted_utils/ " +
      cppName + " -o " + targetBinary;

#else
#   error "Unknown compiler"
    assert(false);
#endif

    cout << "Compile command = " << compileCommand << endl;

    auto start = high_resolution_clock::now();

    int ret =
      system(compileCommand.c_str());

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time taken to compile " << cppName
         << " " << "to binary " << targetBinary << " : "
         << duration.count() << " milliseconds" << endl;
    
    assert(ret == 0);
  }

  std::string cppLibName(const std::string& baseName) {

#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
    assert(false);
#ifdef _WIN64
    //define something for Windows (64-bit only)
    assert(false);
#else
    //define something for Windows (32-bit only)
    assert(false);
#endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    assert(false);
#elif TARGET_OS_IPHONE
    // iOS device
    assert(false);
#elif TARGET_OS_MAC
    // Other kinds of Mac OS
    
    return "./lib" + baseName + ".dylib";    

#else
#   error "Unknown Apple platform"
    assert(false);
#endif
#elif __linux__
    // linux
    return "./lib" + baseName + ".so";
#elif __unix__ // all unices not caught above
    // Unix
    return "./lib" + baseName + ".so";
#elif defined(_POSIX_VERSION)
    // POSIX
    return "./lib" + baseName + ".so";
#else
#   error "Unknown compiler"
    assert(false);
#endif

  }
}

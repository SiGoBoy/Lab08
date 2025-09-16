## Laboratory work V

Данная лабораторная работа посвещена изучению фреймворков для тестирования на примере **GTest**

```sh
$ open https://github.com/google/googletest
```

## Tasks

- [ ] 1. Создать публичный репозиторий с названием **lab05** на сервисе **GitHub**
- [ ] 2. Выполнить инструкцию учебного материала
- [ ] 3. Ознакомиться со ссылками учебного материала
- [ ] 4. Составить отчет и отправить ссылку личным сообщением в **Slack**

## Tutorial

```sh
$ export GITHUB_USERNAME=<имя_пользователя>
$ alias gsed=sed # for *-nix system
```

```sh
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```

```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab04 projects/lab05
$ cd projects/lab05
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab05
```

```sh
$ mkdir third-party
$ git submodule add https://github.com/google/googletest third-party/gtest
$ cd third-party/gtest && git checkout release-1.8.1 && cd ../..
$ git add third-party/gtest
$ git commit -m"added gtest framework"
```

```sh
$ gsed -i '/option(BUILD_EXAMPLES "Build examples" OFF)/a\
option(BUILD_TESTS "Build tests" OFF)
' CMakeLists.txt
$ cat >> CMakeLists.txt <<EOF

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB \${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check \${\${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(check \${PROJECT_NAME} gtest_main)
  add_test(NAME check COMMAND check)
endif()
EOF
```

```sh
$ mkdir tests
$ cat > tests/test1.cpp <<EOF
#include <print.hpp>

#include <gtest/gtest.h>

TEST(Print, InFileStream)
{
  std::string filepath = "file.txt";
  std::string text = "hello";
  std::ofstream out{filepath};

  print(text, out);
  out.close();

  std::string result;
  std::ifstream in{filepath};
  in >> result;

  EXPECT_EQ(result, text);
}
EOF
```

```sh
$ cmake -H. -B_build -DBUILD_TESTS=ON
$ cmake --build _build
$ cmake --build _build --target test
```

```sh
$ _build/check
$ cmake --build _build --target test -- ARGS=--verbose
```

```sh
$ gsed -i 's/lab04/lab05/g' README.md
$ gsed -i 's/\(DCMAKE_INSTALL_PREFIX=_install\)/\1 -DBUILD_TESTS=ON/' .travis.yml
$ gsed -i '/cmake --build _build --target install/a\
- cmake --build _build --target test -- ARGS=--verbose
' .travis.yml
```

```sh
$ travis lint
```

```sh
$ git add .travis.yml
$ git add tests
$ git add -p
$ git commit -m"added tests"
$ git push origin master
```

```sh
$ travis login --auto
$ travis enable
```

```sh
$ mkdir artifacts
$ sleep 20s && gnome-screenshot --file artifacts/screenshot.png
# for macOS: $ screencapture -T 20 artifacts/screenshot.png
# open https://github.com/${GITHUB_USERNAME}/lab05
```

## Report

```sh
$ popd
$ export LAB_NUMBER=05
$ git clone https://github.com/tp-labs/lab${LAB_NUMBER} tasks/lab${LAB_NUMBER}
$ mkdir reports/lab${LAB_NUMBER}
$ cp tasks/lab${LAB_NUMBER}/README.md reports/lab${LAB_NUMBER}/REPORT.md
$ cd reports/lab${LAB_NUMBER}
$ edit REPORT.md
$ gist REPORT.md
```

## Homework

### Задание
1. Создайте `CMakeList.txt` для библиотеки *banking*.
```cmake
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(banking)

add_library(banking STATIC Transaction.cpp Account.cpp)
target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
```

2. Создайте модульные тесты на классы `Transaction` и `Account`.
    * Используйте mock-объекты.
    * Покрытие кода должно составлять 100%.
```cpp
#include <Account.h>
#include <Transaction.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Throw;

class MockAccount : public Account {
public:
	MockAccount(int id, int balance) : Account(id, balance) {}

	MOCK_METHOD(int, GetBalance, (), (const, override));
	MOCK_METHOD(void, ChangeBalance, (int), (override));
	MOCK_METHOD(void, Lock, (), (override));
	MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction : public Transaction {
public:
	MOCK_METHOD(void, SaveToDataBase, (Account&, Account&, int), (override));
};

TEST(Account, ChangeBalanceWOLock) {
	Account account(1, 100);
	EXPECT_THROW(account.ChangeBalance(10), std::runtime_error);
}

TEST(Account, LockUnlock) {
	Account account(1, 100);

	account.Lock();
	EXPECT_THROW(account.Lock(), std::runtime_error);
	account.Unlock();
	EXPECT_NO_THROW(account.Lock());
}

TEST(Account, GetBalanceValue) {
	Account account(1, 150);
	account.Lock();
	account.ChangeBalance(-50);
	account.Unlock();
	EXPECT_EQ(account.GetBalance(), 100);
}

TEST(Transaction, SameAccount) {
	MockAccount account(1, 1000);
	MockTransaction tx;
	EXPECT_THROW(tx.Make(account, account, 100), std::logic_error);
}

TEST(Transaction, NegativeSum) {
	MockAccount from(1, 1000);
	MockAccount to(2, 1000);
	MockTransaction tx;
	EXPECT_THROW(tx.Make(from, to, -100), std::invalid_argument);
}

TEST(Transaction, SmallSum) {
	MockAccount from(1, 1000);
	MockAccount to(2, 1000);
	MockTransaction tx;
	EXPECT_THROW(tx.Make(from, to, 50), std::logic_error);
}

TEST(Transaction, HighFee) {
	MockAccount from(1, 1000);
	MockAccount to(2, 1000);
	MockTransaction tx;
	tx.set_fee(100);

	EXPECT_FALSE(tx.Make(from, to, 100));
}

TEST(Transaction, Successful) {
	MockAccount from(1, 1000);
	MockAccount to(2, 1000);
	MockTransaction tx;
	tx.set_fee(10);

	{
		EXPECT_CALL(from, Lock());
		EXPECT_CALL(to, Lock());

		EXPECT_CALL(to, ChangeBalance(200));
		EXPECT_CALL(to, GetBalance()).WillOnce(Return(1200));
		EXPECT_CALL(to, ChangeBalance(-210));
		EXPECT_CALL(from, GetBalance()).WillRepeatedly(Return(1000));
    
		EXPECT_CALL(tx, SaveToDataBase(_, _, 200));
	}

	EXPECT_CALL(from, Unlock());
	EXPECT_CALL(to, Unlock());

	EXPECT_TRUE(tx.Make(from, to, 200));
}

TEST(Transaction, NotEnoughFunds) {
	MockAccount from(1, 1000);
	MockAccount to(2, 1000);
	MockTransaction tx;
	tx.set_fee(10);

	{
		EXPECT_CALL(from, Lock());
		EXPECT_CALL(to, Lock());

		EXPECT_CALL(to, ChangeBalance(100));
		EXPECT_CALL(to, GetBalance()).WillOnce(Return(100));
		EXPECT_CALL(to, ChangeBalance(-100));

		EXPECT_CALL(tx, SaveToDataBase(_, _, 100));
	}

	EXPECT_CALL(from, Unlock());
	EXPECT_CALL(to, Unlock());

	EXPECT_FALSE(tx.Make(from, to, 100));
}

```

3. Настройте сборочную процедуру на **TravisCI**.
```yml
name: gtest

on:
 push:
  branches: [main]
 pull_request:
  branches: [main]

jobs:
 build:

  runs-on: ubuntu-latest

  steps:
  - uses: actions/checkout@v3

  - name: Adding gtest
    run: git clone https://github.com/google/googletest.git third-party/gtest

  - name: Install lcov
    run: sudo apt-get install -y lcov

  - name: Config banking with tests
    run: cmake -H. -B ${{github.workspace}}/build -DBUILD_TESTS=ON

  - name: Build banking
    run: cmake --build ${{github.workspace}}/build

  - name: Run tests
    run: build/check
```

CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

project(lab05)

add_subdirectory(banking)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB BANKING_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp)
  add_executable(check ${BANKING_TEST_SOURCES})
  target_link_libraries(check banking gtest_main gmock_main)
  add_test(NAME check COMMAND check)
endif()
```
4. Настройте [Coveralls.io](https://coveralls.io/): [![Coverage Status](https://coveralls.io/repos/github/SiGoBoy/Lab05/badge.svg?branch=main)](https://coveralls.io/github/SiGoBoy/Lab05?branch=main)
```yml
name: CI with gtest and lcov

on:
 push:
  branches: [main]
 pull_request:
  branches: [main]

jobs:
 build:

  runs-on: ubuntu-latest

  steps:
  - uses: actions/checkout@v4

  - name: Adding gtest
    run: git clone https://github.com/google/googletest.git third-party/gtest

  - name: Install cpp-coveralls
    run: pip install cpp-coveralls

  - name: Config banking with tests
    run: cmake -H. -B${{github.workspace}}/build -DBUILD_TESTS=ON -DCODE_COVERAGE=ON

  - name: Build banking
    run: cmake --build ${{github.workspace}}/build

  - name: Run tests
    run: build/check --enable-gcov

  - name: Measure coverage
    env:
      COVERALLS_REPO_TOKEN: ${{ secrets.COVERALLS_REPO_TOKEN }}
    run: coveralls -r build
```
CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

project(lab05)

add_subdirectory(banking)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB BANKING_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp)
  add_executable(check ${BANKING_TEST_SOURCES})
  target_link_libraries(check banking gtest_main gmock_main)
  add_test(NAME check COMMAND check)
endif()

if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(check PRIVATE --coverage)
  target_link_options(check PRIVATE --coverage)
endif()
```
## Links

- [C++ CI: Travis, CMake, GTest, Coveralls & Appveyor](http://david-grs.github.io/cpp-clang-travis-cmake-gtest-coveralls-appveyor/)
- [Boost.Tests](http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/)
- [Catch](https://github.com/catchorg/Catch2)

```
Copyright (c) 2015-2021 The ISC Authors
```

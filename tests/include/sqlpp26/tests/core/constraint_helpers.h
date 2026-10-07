#pragma once

/*
 * Copyright (c) 2024, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef BUILD_WITH_MODULES
import sqlpp26.core;
#else
#include <sqlpp26/sqlpp26.h>
#endif

template <typename S, sqlpp::fixed_string Expected>
consteval bool expect_basic_consistency_fails() {
  constexpr std::string_view expected = Expected;
  constexpr auto check = sqlpp::check_basic_consistency(sqlpp::type_v<std::decay_t<S>>{});
  static_assert(not check, std::format("missing error: '{}'", expected));
  static_assert(check.error() == expected, std::format("wrong error: '{}' != '{}'", expected, check.error()));
  return true;
}

template <typename S>
consteval bool expect_basic_consistency_succeeds() {
  constexpr auto check = sqlpp::check_basic_consistency(sqlpp::type_v<std::decay_t<S>>{});
  static_assert(check, std::format("unexpected error: '{}'", check.error()));
  return true;
}

template <typename S, sqlpp::fixed_string Expected>
consteval bool expect_prepare_consistency_fails() {
  constexpr std::string_view expected = Expected;
  constexpr auto check = sqlpp::check_prepare_consistency(sqlpp::type_v<std::decay_t<S>>{});
  static_assert(not check, std::format("missing error: '{}'", expected));
  static_assert(check.error() == expected, std::format("wrong error: '{}' != '{}'", expected, check.error()));
  return true;
}

template <typename S>
consteval bool expect_prepare_consistency_succeeds() {
  constexpr auto check = sqlpp::check_prepare_consistency(sqlpp::type_v<std::decay_t<S>>{});
  static_assert(check, std::format("unexpected error: '{}'", check.error()));
  return true;
}

template <typename S, sqlpp::fixed_string Expected>
consteval bool expect_run_consistency_fails() {
  constexpr std::string_view expected = Expected;
  constexpr auto check = sqlpp::check_run_consistency(sqlpp::type_v<std::decay_t<S>>{});
  static_assert(not check, std::format("missing error: '{}'", expected));
  static_assert(check.error() == expected, std::format("wrong error: '{}' != '{}'", expected, check.error()));
  return true;
}

template <typename S>
consteval bool expect_run_consistency_succeeds() {
  constexpr auto check = sqlpp::check_run_consistency(sqlpp::type_v<std::decay_t<S>>{});
  static_assert(check, std::format("unexpected error: '{}'", check.error()));
  return true;
}

template <typename Context, typename S, sqlpp::fixed_string Expected>
consteval bool expect_compatibility_fails() {
  constexpr std::string_view expected = Expected;
  constexpr auto check = check_compatibility(sqlpp::type_v<Context>{}, sqlpp::type_v<std::decay_t<S>>{});
  static_assert(not check, std::format("missing error: '{}'", expected));
  static_assert(check.error() == expected, std::format("wrong error: '{}' != '{}'", expected, check.error()));
  return true;
}

template <typename Context, typename S>
consteval bool expect_compatibility_succeeds() {
  constexpr auto check = check_compatibility(sqlpp::type_v<Context>{}, sqlpp::type_v<std::decay_t<S>>{});
  static_assert(check, std::format("unexpected error: '{}'", check.error()));
  return true;
}



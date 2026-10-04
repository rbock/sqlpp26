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

#include <sqlpp26/tests/core/all.h>

namespace {
template <typename A, typename B>
constexpr bool is_same_type() {
  return std::is_same<A, B>::value;
}
}  // namespace

template <typename Left, typename Right, typename DataType>
void test_plus(Left raw_l, Right raw_r, DataType) {
  using OptDataType = std::optional<DataType>;

  auto l = sqlpp::value(raw_l);
  auto r = sqlpp::value(raw_r);

  auto opt_l = sqlpp::value(std::optional{raw_l});
  auto opt_r = sqlpp::value(std::optional{raw_r});

  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l + r)>, DataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l + opt_r)>, OptDataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l + r)>, OptDataType>);
  static_assert(is_same_type<sqlpp::data_type_of_t<decltype(opt_l + opt_r)>,
                             OptDataType>());

  // Arithmetic expressions enable the `as` member function.
  static_assert(sqlpp::has_enabled_as<decltype(l + opt_r)>::value);

  // Arithmetic expressions enable comparison member functions.
  static_assert(sqlpp::has_enabled_comparison<decltype(l + opt_r)>::value);

  // Arithmetic expressions have their arguments as nodes
  using L = typename std::decay<decltype(l)>::type;
  using R = typename std::decay<decltype(opt_r)>::type;
  static_assert(std::is_same<sqlpp::nodes_of_t<decltype(l + opt_r)>,
                             sqlpp::detail::type_vector<L, R>>::value);
}

template <typename Left, typename Right, typename DataType>
void test_minus(Left raw_l, Right raw_r, DataType) {
  using OptDataType = std::optional<DataType>;

  auto l = sqlpp::value(raw_l);
  auto r = sqlpp::value(raw_r);

  auto opt_l = sqlpp::value(std::optional{raw_l});
  auto opt_r = sqlpp::value(std::optional{raw_r});

  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l - r)>, DataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l - opt_r)>, OptDataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l - r)>, OptDataType>);
  static_assert(std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l - opt_r)>, OptDataType>);

  // Arithmetic expressions enable the `as` member function.
  static_assert(sqlpp::has_enabled_as<decltype(l - opt_r)>::value);

  // Arithmetic expressions enable comparison member functions.
  static_assert(sqlpp::has_enabled_comparison<decltype(l - opt_r)>::value);

  // Arithmetic expressions have their arguments as nodes
  using L = typename std::decay<decltype(l)>::type;
  using R = typename std::decay<decltype(opt_r)>::type;
  static_assert(std::is_same<sqlpp::nodes_of_t<decltype(l - opt_r)>, sqlpp::detail::type_vector<L, R>>::value);
}

template <typename Left, typename Right, typename DataType>
void test_multiplies(Left raw_l, Right raw_r, DataType) {
  using OptDataType = std::optional<DataType>;

  auto l = sqlpp::value(raw_l);
  auto r = sqlpp::value(raw_r);

  auto opt_l = sqlpp::value(std::optional{raw_l});
  auto opt_r = sqlpp::value(std::optional{raw_r});

  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l * r)>, DataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l * opt_r)>, OptDataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l * r)>, OptDataType>);
  static_assert(std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l * opt_r)>, OptDataType>);

  // Arithmetic expressions enable the `as` member function.
  static_assert(sqlpp::has_enabled_as<decltype(l * opt_r)>::value);

  // Arithmetic expressions enable comparison member functions.
  static_assert(sqlpp::has_enabled_comparison<decltype(l * opt_r)>::value);

  // Arithmetic expressions have their arguments as nodes
  using L = typename std::decay<decltype(l)>::type;
  using R = typename std::decay<decltype(opt_r)>::type;
  static_assert(std::is_same<sqlpp::nodes_of_t<decltype(l * opt_r)>,
                             sqlpp::detail::type_vector<L, R>>::value);
}

template <typename Right, typename DataType>
void test_negate(Right raw_r, DataType) {
  using OptDataType = std::optional<DataType>;

  auto r = sqlpp::value(raw_r);

  auto opt_r = sqlpp::value(std::optional{raw_r});

  static_assert(std::is_same_v<sqlpp::data_type_of_t<decltype(-r)>, DataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(-opt_r)>, OptDataType>);

  // Arithmetic expressions enable the `as` member function.
  static_assert(sqlpp::has_enabled_as<decltype(-opt_r)>::value);

  // Arithmetic expressions enable comparison member functions.
  static_assert(sqlpp::has_enabled_comparison<decltype(-opt_r)>::value);

  // Arithmetic expressions have their arguments as nodes
  using R = typename std::decay<decltype(opt_r)>::type;
  static_assert(
      std::is_same<sqlpp::nodes_of_t<decltype(-opt_r)>,
                   sqlpp::detail::type_vector<sqlpp::noop, R>>::value);
}

template <typename Left, typename Right, typename DataType>
void test_divides(Left raw_l, Right raw_r, DataType) {
  using OptDataType = std::optional<DataType>;

  auto l = sqlpp::value(raw_l);
  auto r = sqlpp::value(raw_r);

  auto opt_l = sqlpp::value(std::optional{raw_l});
  auto opt_r = sqlpp::value(std::optional{raw_r});

  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l / r)>, DataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l / opt_r)>, OptDataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l / r)>, OptDataType>);
  static_assert(std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l / opt_r)>, OptDataType>);

  // Arithmetic expressions enable the `as` member function.
  static_assert(sqlpp::has_enabled_as<decltype(l / opt_r)>::value);

  // Arithmetic expressions enable comparison member functions.
  static_assert(sqlpp::has_enabled_comparison<decltype(l / opt_r)>::value);

  // Arithmetic expressions have their arguments as nodes
  using L = typename std::decay<decltype(l)>::type;
  using R = typename std::decay<decltype(opt_r)>::type;
  static_assert(std::is_same<sqlpp::nodes_of_t<decltype(l / opt_r)>, sqlpp::detail::type_vector<L, R>>::value);
}

template <typename Left, typename Right, typename DataType>
void test_modulus(Left raw_l, Right raw_r, DataType) {
  using OptDataType = std::optional<DataType>;

  auto l = sqlpp::value(raw_l);
  auto r = sqlpp::value(raw_r);

  auto opt_l = sqlpp::value(std::optional{raw_l});
  auto opt_r = sqlpp::value(std::optional{raw_r});

  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l % r)>, DataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(l % opt_r)>, OptDataType>);
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l % r)>, OptDataType>);
  static_assert(std::is_same_v<sqlpp::data_type_of_t<decltype(opt_l % opt_r)>, OptDataType>);

  // Arithmetic expressions enable the `as` member function.
  static_assert(sqlpp::has_enabled_as<decltype(l % opt_r)>::value);

  // Arithmetic expressions enable comparison member functions.
  static_assert(sqlpp::has_enabled_comparison<decltype(l % opt_r)>::value);

  // Arithmetic expressions have their arguments as nodes
  using L = typename std::decay<decltype(l)>::type;
  using R = typename std::decay<decltype(opt_r)>::type;
  static_assert(std::is_same<sqlpp::nodes_of_t<decltype(l % opt_r)>, sqlpp::detail::type_vector<L, R>>::value);
}

template <typename Value>
void test_concatenation_expressions(Value v) {
  using DataType = sqlpp::text;
  using OptDataType = std::optional<sqlpp::text>;

  auto value = sqlpp::value(v);
  auto opt_value = sqlpp::value(std::optional{v});

  // Concatenating non-optional values
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(value + value)>, DataType>);

  // Concatenating non-optional with optional values
  static_assert(std::is_same_v<sqlpp::data_type_of_t<decltype(value + opt_value)>, OptDataType>);

  // Concatenating optional with non-optional values
  static_assert(std::is_same_v<sqlpp::data_type_of_t<decltype(opt_value + value)>, OptDataType>);

  // Concatenating optional with optional values
  static_assert(
      std::is_same_v<sqlpp::data_type_of_t<decltype(opt_value + opt_value)>, OptDataType>);

  // Modulus expressions enable the `as` member function.
  static_assert(sqlpp::has_enabled_as<decltype(value + opt_value)>::value);

  // Modulus expressions enable comparison member functions.
  static_assert(
      sqlpp::has_enabled_comparison<decltype(value + opt_value)>::value);

  // Modulus expressions have their arguments as nodes
  using L = typename std::decay<decltype(value)>::type;
  using R = typename std::decay<decltype(opt_value)>::type;
  static_assert(std::is_same<sqlpp::nodes_of_t<decltype(value + opt_value)>, sqlpp::detail::type_vector<L, R>>::value);
}

int main() {
  auto fp = 7.f;
  auto in = int{7};
  auto ui = unsigned{7};
  auto bo = bool{1};

  // plus
  test_plus(fp, fp, float{});
  test_plus(fp, in, float{});
  test_plus(fp, ui, float{});
  test_plus(fp, bo, float{});

  test_plus(in, fp, float{});
  test_plus(in, in, int{});
  test_plus(in, ui, unsigned{});
  test_plus(in, bo, int{});

  test_plus(ui, fp, float{});
  test_plus(ui, in, unsigned{});
  test_plus(ui, ui, unsigned{});
  test_plus(ui, bo, unsigned{});

  test_plus(bo, fp, float{});
  test_plus(bo, in, int{});
  test_plus(bo, ui, unsigned{});
  test_plus(bo, bo, int{});

  // minus
  test_minus(fp, fp, float{});
  test_minus(fp, in, float{});
  test_minus(fp, ui, float{});
  test_minus(fp, bo, float{});

  test_minus(in, fp, float{});
  test_minus(in, in, int{});
  test_minus(in, ui, unsigned{});
  test_minus(in, bo, int{});

  test_minus(ui, fp, float{});
  test_minus(ui, in, unsigned{});
  test_minus(ui, ui, unsigned{});
  test_minus(ui, bo, unsigned{});

  test_minus(bo, fp, float{});
  test_minus(bo, in, int{});
  test_minus(bo, ui, unsigned{});
  test_minus(bo, bo, int{});

  // multiplies
  test_multiplies(fp, fp, float{});
  test_multiplies(fp, in, float{});
  test_multiplies(fp, ui, float{});
  test_multiplies(fp, bo, float{});

  test_multiplies(in, fp, float{});
  test_multiplies(in, in, int{});
  test_multiplies(in, ui, unsigned{});
  test_multiplies(in, bo, int{});

  test_multiplies(ui, fp, float{});
  test_multiplies(ui, in, unsigned{});
  test_multiplies(ui, ui, unsigned{});
  test_multiplies(ui, bo, unsigned{});

  test_multiplies(bo, fp, float{});
  test_multiplies(bo, in, int{});
  test_multiplies(bo, ui, unsigned{});
  test_multiplies(bo, bo, int{});

  // divides
  test_divides(fp, double{}, double{});
  test_divides(fp, fp, float{});
  test_divides(fp, in, float{});
  test_divides(fp, ui, float{});
  test_divides(fp, bo, float{});

  test_divides(in, fp, float{});
  test_divides(in, in, int{});
  test_divides(in, ui, unsigned{});
  test_divides(int64_t{}, ui, int64_t{});
  test_divides(in, bo, int{});

  test_divides(ui, fp, float{});
  test_divides(ui, in, unsigned{});
  test_divides(ui, ui, unsigned{});
  test_divides(ui, bo, unsigned{});

  test_divides(bo, fp, float{});
  test_divides(bo, in, int{});
  test_divides(bo, ui, unsigned{});
  test_divides(bo, bo, int{});

  // TODO: Need to test with NULL

  // negate
  test_negate(fp, float{});
  test_negate(in, int{});
  test_negate(ui, unsigned{});
  test_negate(bo, int{});

  // modulus
  test_modulus(in, in, int{});
  test_modulus(in, ui, unsigned{});

  test_modulus(ui, in, unsigned{});
  test_modulus(ui, int64_t{}, int64_t{});
  test_modulus(ui, ui, unsigned{});

  // concatenation
  test_concatenation_expressions("seven");
  test_concatenation_expressions(std::string("seven"));
  test_concatenation_expressions(std::string_view("seven"));
}

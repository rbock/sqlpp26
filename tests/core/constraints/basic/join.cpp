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
// ---- lhs.join(rhs) -------------------
template <typename Lhs, typename Rhs>
concept can_call_join_with = requires(Lhs lhs, Rhs rhs) {
  lhs.join(rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_inner_join_with = requires(Lhs lhs, Rhs rhs) {
  lhs.join(rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_full_outer_join_with = requires(Lhs lhs, Rhs rhs) {
  lhs.join(rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_left_outer_join_with = requires(Lhs lhs, Rhs rhs) {
  lhs.join(rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_right_outer_join_with = requires(Lhs lhs, Rhs rhs) {
  lhs.join(rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_cross_join_with = requires(Lhs lhs, Rhs rhs) {
  lhs.join(rhs);
};

// ---- lhs.join(rhs).on(expr) -------------------

template <typename Lhs, typename Rhs, typename Expr>
concept can_call_join_on_with = requires(Lhs lhs, Rhs rhs, Expr expr) {
  lhs.join(rhs).on(expr);
};
template <typename Lhs, typename Rhs, typename Expr>
concept can_call_inner_join_on_with = requires(Lhs lhs, Rhs rhs, Expr expr) {
  lhs.join(rhs).on(expr);
};
template <typename Lhs, typename Rhs, typename Expr>
concept can_call_full_outer_join_on_with = requires(Lhs lhs, Rhs rhs, Expr expr) {
  lhs.join(rhs).on(expr);
};
template <typename Lhs, typename Rhs, typename Expr>
concept can_call_left_outer_join_on_with = requires(Lhs lhs, Rhs rhs, Expr expr) {
  lhs.join(rhs).on(expr);
};
template <typename Lhs, typename Rhs, typename Expr>
concept can_call_right_outer_join_on_with = requires(Lhs lhs, Rhs rhs, Expr expr) {
  lhs.join(rhs).on(expr);
};

// can_call_all joins_with
template <typename Lhs, typename Rhs>
void can_call_all_joins_with(const Lhs&, const Rhs&) {
  static_assert(can_call_join_with<Lhs, Rhs>);
  static_assert(can_call_inner_join_with<Lhs, Rhs>);
  static_assert(can_call_left_outer_join_with<Lhs, Rhs>);
  static_assert(can_call_right_outer_join_with<Lhs, Rhs>);
  static_assert(can_call_full_outer_join_with<Lhs, Rhs>);
  static_assert(can_call_cross_join_with<Lhs, Rhs>);
}

template <typename Lhs, typename Rhs>
void cannot_call_any_join_with(const Lhs&, const Rhs&) {
  static_assert(not can_call_join_with<Lhs, Rhs>);
  static_assert(not can_call_inner_join_with<Lhs, Rhs>);
  static_assert(not can_call_left_outer_join_with<Lhs, Rhs>);
  static_assert(not can_call_right_outer_join_with<Lhs, Rhs>);
  static_assert(not can_call_full_outer_join_with<Lhs, Rhs>);
  static_assert(not can_call_cross_join_with<Lhs, Rhs>);
}

template <typename Lhs, typename Rhs, typename Expr>
void can_call_all_joins_on_with(const Lhs&, const Rhs&, const Expr&) {
  static_assert(can_call_join_on_with<Lhs, Rhs, Expr>);
  static_assert(can_call_inner_join_on_with<Lhs, Rhs, Expr>);
  static_assert(can_call_left_outer_join_on_with<Lhs, Rhs, Expr>);
  static_assert(can_call_right_outer_join_on_with<Lhs, Rhs, Expr>);
  static_assert(can_call_full_outer_join_on_with<Lhs, Rhs, Expr>);
}

template <typename Lhs, typename Rhs, typename Expr>
void cannot_call_any_join_on_with(const Lhs& lhs, const Rhs& rhs, const Expr&) {
  can_call_all_joins_with(lhs, rhs);
  static_assert(not can_call_join_on_with<Lhs, Rhs, Expr>);
  static_assert(not can_call_inner_join_on_with<Lhs, Rhs, Expr>);
  static_assert(not can_call_left_outer_join_on_with<Lhs, Rhs, Expr>);
  static_assert(not can_call_right_outer_join_on_with<Lhs, Rhs, Expr>);
  static_assert(not can_call_full_outer_join_on_with<Lhs, Rhs, Expr>);
}

struct weird_table : public sqlpp::enable_join {};

}  // namespace

namespace sqlpp {
template <>
struct is_table<weird_table> : public std::true_type {};

inline consteval sqlpp::detail::type_info_set get_required_tables_of(type_v<weird_table>) {
  return sqlpp::detail::make_type_info_set<::test::tab_bar>();
}
}  // namespace sqlpp

int main() {
  const auto maybe = true;
  const auto foo = test::tab_foo{};
  const auto bar = test::tab_bar{};
  const auto aFoo = foo.as<"a">();
  const auto bFoo = foo.as<"b">();

  // OK
  can_call_all_joins_with(bar, foo);
  can_call_all_joins_with(bar, foo.as<"something">());

  // Cannot join with a non-table
  cannot_call_any_join_with(bar, foo.id);

  // Cannot join two identical tables.
  cannot_call_any_join_with(foo, foo);

  // Cannot join two tables with identical names.
  cannot_call_any_join_with(foo, bar.as<"tab_foo">());

  // JOIN must not be called with tables that depend on other tables.
  // Not sure this can happen in the wild, which is why we are using the
  // `weird_table` to simulate the situation.
  cannot_call_any_join_with(weird_table{}, foo);
  cannot_call_any_join_with(foo, weird_table{});

  // JOIN ... ON can be called with any boolean expression, but will fail with
  // static_assert if it uses the wrong tables. Here, bFoo is not provided by
  // the join.
  cannot_call_any_join_on_with(foo, bar, bFoo.id == bar.id);
  cannot_call_any_join_on_with(foo, bar, bFoo.id == aFoo.id);
  //
  // Here, bar is provided /dynamically/ by the first join, but required
  // /statically/ by the second join.
  cannot_call_any_join_on_with(foo.join(dynamic(maybe, bar))
                                  .on(foo.id == bar.id), aFoo, bar.id == aFoo.id);
}

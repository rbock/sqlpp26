/*
 * Copyright (c) 2026, Roland Bock
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
// Returns true if `declval<Lhs> == declval<Rhs>()` is a valid function
// call.
template <typename Lhs, typename Rhs>
concept can_call_equal_with = requires(Lhs lhs, Rhs rhs) {
  lhs == rhs;
};

template <typename Lhs, typename Rhs>
concept can_call_member_in_with = requires(Lhs lhs, Rhs rhs) {
  lhs.in(rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_free_in_with = requires(Lhs lhs, Rhs rhs) {
  in(lhs, rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_member_not_in_with = requires(Lhs lhs, Rhs rhs) {
  lhs.not_in(rhs);
};
template <typename Lhs, typename Rhs>
concept can_call_free_not_in_with = requires(Lhs lhs, Rhs rhs) {
  not_in(lhs, rhs);
};

// Returns true if `lhs.in(rhs)` is a valid function
// call.
template <typename Lhs, typename Rhs>
concept can_call_in_with =
    can_call_member_in_with<Lhs, Rhs> and
    can_call_free_in_with<Lhs, Rhs> and
    can_call_member_not_in_with<Lhs, Rhs> and
    can_call_free_not_in_with<Lhs, Rhs>;

template <typename Lhs, typename Rhs>
concept cannot_call_in_with =
    not can_call_member_in_with<Lhs, Rhs> and
    not can_call_free_in_with<Lhs, Rhs> and
    not can_call_member_not_in_with<Lhs, Rhs> and
    not can_call_free_not_in_with<Lhs, Rhs>;
}  // namespace

int main() {
  const auto bar = test::tab_bar{};
  const auto foo = test::tab_foo{};

  // ---------------------------------------------------
  // -----  Equality                   -----------------
  // ---------------------------------------------------

  // OK
  bar.int_n == 7;
  bar.int_n.is_distinct_from(7);
  bar.int_n.is_distinct_from(std::nullopt);
  bar.bool_nn.is_distinct_from(std::nullopt);
  static_assert(can_call_equal_with<decltype(bar.int_n), decltype(7)>);
  static_assert(can_call_equal_with<decltype(bar.int_n), decltype(std::optional<int>{7})>);

  // Cannot compare directly with std::nullopt.
  static_assert(not can_call_equal_with<decltype(bar.int_n), std::nullopt_t>);

  // If you really want to do this (you should use .is_null() instead), you can:
  static_assert(can_call_equal_with<decltype(bar.int_n), decltype(std::optional<int>{})>);

  // Cannot compare with default value.
  static_assert(not can_call_equal_with<decltype(bar.int_n), decltype(sqlpp::default_value)>);


  // ---------------------------------------------------
  // -----  in / not_in                -----------------
  // ---------------------------------------------------

  static_assert(can_call_in_with<decltype(bar.int_n), decltype(std::make_tuple(7))>);
  static_assert(can_call_in_with<decltype(bar.int_n), decltype(std::make_tuple(7, 8.f, true))>);
  static_assert(can_call_in_with<decltype(bar.int_n), decltype(std::vector{7, 8, 9})>);
  static_assert(can_call_in_with<decltype(bar.int_n), decltype(select(foo.id).from(foo))>);

  // Cannot call in with incompatible value types
  static_assert(cannot_call_in_with<decltype(bar.int_n), decltype(std::make_tuple("seven"))>);
  static_assert(cannot_call_in_with<decltype(bar.int_n), decltype(std::vector({"seven"}))>);
  static_assert(cannot_call_in_with<decltype(bar.int_n), decltype(select(foo.text_nn_d).from(foo))>);
}

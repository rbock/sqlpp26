/*
 * Copyright (c) 2026, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
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

#include <print>
#include <stdexcept>

#include <sqlpp26/ranges/ranges.h>

namespace test {
struct Foo {
  int id;
  std::string_view something;
};
}

template <typename Statement>
constexpr void run(const Statement&) {
  constexpr auto check = check_basic_consistency(sqlpp::type_v<Statement>{});
  static_assert(check, check.error());
}

int main() {
  constexpr auto tab_foo = sqlpp::ranges::make_table<test::Foo>();

  auto foo_data = std::vector<test::Foo>{};
  auto db = sqlpp::ranges::connection(foo_data);
  db(insert_into(tab_foo).set(tab_foo.id = 123, tab_foo.something = "cheese"));
  db(insert_into(tab_foo).set(tab_foo.id = 456, tab_foo.something = "cake"));

  if (foo_data.size() != 2) {
    throw std::runtime_error("unexpected data size");
  }
  if (foo_data.front().id != 123 or foo_data.front().something != "cheese") {
    throw std::runtime_error("unexpected first value");
  }
  if (foo_data.back().id != 456 or foo_data.back().something != "cake") {
    throw std::runtime_error("unexpected last value");
  }
}

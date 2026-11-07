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

#include <sqlpp26/ranges/ranges.h>

namespace test {
struct Foo {
  int8_t number;
  std::string_view text;
};
}

int main() {
  constexpr auto tab_foo = sqlpp::ranges::make_table<test::Foo>();

  auto foo_data = std::vector<test::Foo>{{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}, {6, "six"}};
  auto db = sqlpp::ranges::connection(foo_data);

  for (const auto& row :
       db(select(all_of(tab_foo)).from(tab_foo).where(tab_foo.number > 1))) {
    // TODO: Turn this into a test
    std::println("number: {}, text: {}", row.number, row.text);
  }

  for (const auto& row :
       db(select(tab_foo.number, sqlpp::value(7).as<"seven">()).from(tab_foo).where(tab_foo.number != 2))) {
    // TODO: Turn this into a test
    std::println("number: {}, seven: {}", row.number, row.seven);
  }

  for (const auto& row : db(select(count(tab_foo.number).as<"total_count">(),
                                   avg(tab_foo.number).as<"avg_id">(),
                                   max(tab_foo.number).as<"max_id">(),
                                   min(tab_foo.text).as<"min_something">(),
                                   sum(tab_foo.number).as<"sum_id">())
                                .from(tab_foo)
                                .where(tab_foo.number > 1))) {
    // TODO: Turn this into a test
    std::println("total_count: {}, avg_id: {}, max_id: {}, min_something: {}, sum_id: {}",
                 row.total_count, row.avg_id.value_or(-10000.), row.max_id.value_or(-1),
                 row.min_something.value_or("NULL"), row.sum_id);
  }

  for (const auto& row :
       db(select(all_of(tab_foo)).from(tab_foo).where(tab_foo.number > 1).order_by(tab_foo.text.desc()))) {
    // TODO: Turn this into a test
    std::println("number: {}, text: {}", row.number, row.text);
  }

  std::println("-----------------------------");
  for (const auto& row : db(select(count(tab_foo.number).as<"count">(),
                                   max(tab_foo.number).as<"max">())
                                .from(tab_foo)
                                .where(tab_foo.number > 1)
                                .group_by(tab_foo.number % 2))) {
    // TODO: Turn this into a test
    std::println("count: {}, max: {}", row.count, row.max.value_or(-1));
  }
}

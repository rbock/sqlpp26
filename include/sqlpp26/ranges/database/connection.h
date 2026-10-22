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

#include <sqlpp26/core/query/statement_handler.h>
#include <sqlpp26/core/type_traits.h>
#include <sqlpp26/ranges/to_filter_expression.h>

namespace sqlpp::ranges {
template <typename Table>
struct connection {
  struct context_t {};

  public:
  connection(Table& table) : _table{table} {}

  // Directly executed statements start here
  template <typename T>
    requires(sqlpp::is_statement_v<T>)
  constexpr auto operator()(const T& t) {
    constexpr auto run_check = check_run_consistency(type_v<T>{});
    static_assert(run_check, run_check.error());
    constexpr auto compatibility_check = check_compatibility(type_v<context_t>{}, type_v<T>{});
    static_assert(compatibility_check, compatibility_check.error());

    return sqlpp::statement_handler_t{}.run(t, *this);
  }

  private:
   friend sqlpp::statement_handler_t;

   template <typename T>
     requires(sqlpp::is_statement_v<T>)
   constexpr auto _insert(const T& t) {
     auto filter = to_filter_expression(t);
     return filter.insert(_table);
   }

    Table& _table;
};
}  // namespace sqlpp::ranges



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

int main(int, char*[]) {
  auto compare = comparer{};

  const auto foo = test::tab_foo{};
  const auto bar = test::tab_bar{};

  // No expression (not super useful).
  compare(sqlpp::cte<"x">(), "x");

  // Simple CTE: X AS SELECT
  {
    using S = decltype(select(foo.id).from(foo));
    static_assert(sqlpp::has_result_row<S>::value, "");
    const auto x = sqlpp::cte<"x">().as(select(foo.id).from(foo));
    const auto a = x.as<"a">();
    compare(x, "x AS (SELECT tab_foo.id FROM tab_foo)");
    compare(make_table_ref(x), "x");
    compare(x.id, "x.id");
    compare(a, "x AS a");
    compare(a.id, "a.id");
    compare(select(all_of(x)), "SELECT x.id");
    compare(select(all_of(a)), "SELECT a.id");
  }

  // Non-recursive union CTE: X AS SELECT ... UNION ALL SELECT ...
  {
    const auto x =
        sqlpp::cte<"x">()
            .as(select(foo.id).from(foo).union_all(select(bar.id).from(bar)));
    const auto a = x.as<"a">();
    compare(x,
                  "x AS (SELECT tab_foo.id FROM tab_foo UNION ALL "
                  "SELECT tab_bar.id FROM tab_bar)");
    compare(make_table_ref(x), "x");
    compare(x.id, "x.id");
    compare(a, "x AS a");
    compare(a.id, "a.id");
    compare(select(all_of(x)), "SELECT x.id");
    compare(select(all_of(a)), "SELECT a.id");
  }

  // Recursive CTE: X AS SELECT ... UNION ALL SELECT ... FROM X ...
  {
    const auto x_base =
        sqlpp::cte<"x">().as(select(sqlpp::value(0).as<"a">()));
    const auto x = x_base.union_all(select((x_base.a + 1).as<"a">())
                                        .from(x_base)
                                        .where(x_base.a < 10));
    const auto y = x.as<"y">();
    compare(x,
                  "x AS (SELECT 0 AS a UNION ALL SELECT (x.a + 1) AS a FROM "
                  "x WHERE x.a < 10)");
    compare(make_table_ref(x), "x");
    compare(x.a, "x.a");
    compare(y, "x AS y");
    compare(y.a, "y.a");
    compare(select(all_of(x)), "SELECT x.a");
    compare(select(all_of(y)), "SELECT y.a");
  }

  // A CTE depending on another CTE
  {
    const auto x = sqlpp::cte<"x">().as(select(foo.id).from(foo));
    const auto y =
        sqlpp::cte<"y">()
            .as(select(x.id, sqlpp::value(7).as<"a">()).from(x));
    const auto z = y.as<"z">();
    compare(y, "y AS (SELECT x.id, 7 AS a FROM x)");
    compare(make_table_ref(y), "y");
    compare(y.id, "y.id");
    compare(z, "y AS z");
    compare(z.id, "z.id");
    compare(select(all_of(y)), "SELECT y.id, y.a");
    compare(select(all_of(z)), "SELECT z.id, z.a");
  }

  // Dynamically recursive CTE: X AS SELECT ... UNION ALL SELECT ... FROM X ...
  {
    const auto x_base =
        sqlpp::cte<"x">().as(select(sqlpp::value(0).as<"a">()));
    auto x = x_base.union_all(
        dynamic(true, select((x_base.a + 1).as<"a">())
                          .from(x_base)
                          .where(x_base.a < 10)));

    compare(x,
                  "x AS (SELECT 0 AS a UNION ALL SELECT (x.a + 1) AS a FROM "
                  "x WHERE x.a < 10)");

    x = x_base.union_all(
        dynamic(false, select((x_base.a + 1).as<"a">())
                           .from(x_base)
                           .where(x_base.a < 10)));
    compare(x, "x AS (SELECT 0 AS a)");
  }

  // Dynamically recursive CTE: X AS SELECT ... UNION DISTINCT SELECT ... FROM X
  // ...
  {
    const auto x_base =
        sqlpp::cte<"x">().as(select(sqlpp::value(0).as<"a">()));
    auto x = x_base.union_distinct(
        dynamic(true, select((x_base.a + 1).as<"a">())
                          .from(x_base)
                          .where(x_base.a < 10)));

    compare(x,
                  "x AS (SELECT 0 AS a UNION DISTINCT SELECT (x.a + 1) AS a "
                  "FROM x WHERE x.a < 10)");

    x = x_base.union_distinct(
        dynamic(false, select((x_base.a + 1).as<"a">())
                           .from(x_base)
                           .where(x_base.a < 10)));
    compare(x, "x AS (SELECT 0 AS a)");
  }

  return 0;
}

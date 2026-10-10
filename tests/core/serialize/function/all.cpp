/*
 * Copyright (c) 2025, Roland Bock
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

  {
    compare(sqlpp::coalesce("a"), "COALESCE('a')");
    compare(sqlpp::coalesce("a", "b"), "COALESCE('a', 'b')");
    compare(sqlpp::coalesce("a", sqlpp::dynamic(true, "b"), "c"),
                  "COALESCE('a', 'b', 'c')");
    compare(sqlpp::coalesce("a", sqlpp::dynamic(false, "b"), "c"),
                  "COALESCE('a', NULL, 'c')");
  }
  {
    compare(sqlpp::concat("a"), "CONCAT('a')");
    compare(sqlpp::concat("a", "b"), "CONCAT('a', 'b')");
    compare(sqlpp::concat("a", sqlpp::dynamic(true, "b"), "c"),
                  "CONCAT('a', 'b', 'c')");
    compare(sqlpp::concat("a", sqlpp::dynamic(false, "b"), "c"),
                  "CONCAT('a', NULL, 'c')");
  }
  {
    compare(sqlpp::current_date, "CURRENT_DATE");
  }
  {
    compare(sqlpp::current_time, "CURRENT_TIME");
  }
  {
    compare(sqlpp::current_timestamp, "CURRENT_TIMESTAMP");
  }
  {
    auto ctx = sqlpp::mock_db::context_t{};

    compare(flatten(ctx, test::tab_foo{}.id), "tab_foo.id");
    compare(flatten(ctx, from(test::tab_foo{})), "FROM tab_foo");
    compare(flatten(ctx, test::tab_foo{}.id).asc(), "tab_foo.id ASC");
  }
  {
    const auto bar = test::tab_bar{};

    // Single column.
    compare(lower(bar.text_n), "LOWER(tab_bar.text_n)");

    // Expression.
    compare(lower(bar.text_n + "suffix"),
                  "LOWER(CONCAT(tab_bar.text_n, 'suffix'))");

    // With sub select.
    compare(lower(value(select(sqlpp::value("something").as<"a">()))),
                  "LOWER(SELECT 'something' AS a)");
  }
  {
    const auto bar = test::tab_bar{};

    // Single column.
    compare(trim(bar.text_n), "TRIM(tab_bar.text_n)");

    // Expression.
    compare(trim(bar.text_n + "suffix"),
                  "TRIM(CONCAT(tab_bar.text_n, 'suffix'))");

    // With sub select.
    compare(trim(value(select(sqlpp::value("something").as<"a">()))),
                  "TRIM(SELECT 'something' AS a)");
  }
  {
    const auto bar = test::tab_bar{};

    // Single column.
    compare(upper(bar.text_n), "UPPER(tab_bar.text_n)");

    // Expression.
    compare(upper(bar.text_n + "suffix"),
                  "UPPER(CONCAT(tab_bar.text_n, 'suffix'))");

    // With sub select.
    compare(upper(value(select(sqlpp::value("something").as<"a">()))),
                  "UPPER(SELECT 'something' AS a)");
  }
  return 0;
}

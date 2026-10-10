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
#include "sqlpp26/core/basic/column_spec.h"

int main(int, char*[]) {
  auto compare = comparer{};

  {
    const auto val = sqlpp::value(17);

    compare(any(select(val.as<"v">())), "ANY (SELECT 17 AS v)");
    compare(val == any(select(val.as<"v">())), "17 = ANY (SELECT 17 AS v)");
  }
  {
    const auto val = sqlpp::value(1);
    const auto expr = sqlpp::value(17) + 4;

    // Operands are enclosed in parenheses where required.
    compare(val + val, "1 + 1");
    compare(val - val, "1 - 1");
    compare(val * val, "1 * 1");
    compare(val / val, "1 / 1");
    compare(val % val, "1 % 1");

    compare(val + expr, "1 + (17 + 4)");
    compare(val - expr, "1 - (17 + 4)");
    compare(val * expr, "1 * (17 + 4)");
    compare(val / expr, "1 / (17 + 4)");
    compare(val % expr, "1 % (17 + 4)");

    compare(expr + val, "(17 + 4) + 1");
    compare(expr - val, "(17 + 4) - 1");
    compare(expr * val, "(17 + 4) * 1");
    compare(expr / val, "(17 + 4) / 1");
    compare(expr % val, "(17 + 4) % 1");

    compare(expr + expr, "(17 + 4) + (17 + 4)");
    compare(expr - expr, "(17 + 4) - (17 + 4)");
    compare(expr * expr, "(17 + 4) * (17 + 4)");
    compare(expr / expr, "(17 + 4) / (17 + 4)");
    compare(expr % expr, "(17 + 4) % (17 + 4)");

    // Same for unary expressions.
    compare(-val, "-1");
    compare(-val + val, "(-1) + 1");
    compare(-expr, "-(17 + 4)");

    const auto text = sqlpp::value("a");
    const auto text_expr = sqlpp::value("b") + "c";

    // Same for concatenation.
    compare(text + text, "CONCAT('a', 'a')");
    compare(text + text_expr, "CONCAT('a', CONCAT('b', 'c'))");
    compare(text_expr + text, "CONCAT(CONCAT('b', 'c'), 'a')");
    compare(text_expr + text_expr,
                  "CONCAT(CONCAT('b', 'c'), CONCAT('b', 'c'))");

    // Arithmetic expressions can be named with AS
    compare((val + val).as<"a">(), "(1 + 1) AS a");
    compare((val - val).as<"a">(), "(1 - 1) AS a");
    compare((val * val).as<"a">(), "(1 * 1) AS a");
    compare((val / val).as<"a">(), "(1 / 1) AS a");
    compare((val % val).as<"a">(), "(1 % 1) AS a");

    // Arithmetic expressions can be compared
    compare((val + val) < 17, "(1 + 1) < 17");
    compare((val - val) < 17, "(1 - 1) < 17");
    compare((val * val) < 17, "(1 * 1) < 17");
    compare((val / val) < 17, "(1 / 1) < 17");
    compare((val % val) < 17, "(1 % 1) < 17");
    compare(-val < 17, "(-1) < 17");
    compare((text + text) < "z", "CONCAT('a', 'a') < 'z'");
  }
  {
    const auto val = sqlpp::value(17);
    const auto expr = sqlpp::value(17) + 4;

    const auto col_id = test::tab_foo{}.id;

    compare(val.as<"v">(), "17 AS v");
    compare(expr.as<"v">(), "(17 + 4) AS v");
    compare(count(val).as<"v">(), "COUNT(17) AS v");

    compare(select_columns(dynamic(false, val.as<"v">())), "NULL AS v");
    compare(select_columns(dynamic(false, expr.as<"v">())), "NULL AS v");
    compare(select_columns(dynamic(false, count(val).as<"v">())),
                  "NULL AS v");
    compare(select_columns(dynamic(false, col_id.as<"v">())), "NULL AS v");
  }
  {
    constexpr auto t = test::tab_foo{};
    const auto val = sqlpp::value(17);

    // Operands in assignments are enclosed in parentheses as required.
    compare(t.int_n = val, "int_n = 17");
    compare(t.int_n = val + 4, "int_n = (17 + 4)");
    compare(t.int_n = std::nullopt, "int_n = NULL");
  }
  {
    const auto val = sqlpp::value(1);
    const auto expr = sqlpp::value(17) + 4;

    // Operands are enclosed in parenheses where required
    compare(val.between(val, val), "1 BETWEEN 1 AND 1");
    compare(val.between(val, expr), "1 BETWEEN 1 AND (17 + 4)");
    compare(val.between(expr, val), "1 BETWEEN (17 + 4) AND 1");
    compare(val.between(expr, expr), "1 BETWEEN (17 + 4) AND (17 + 4)");
    compare(expr.between(val, val), "(17 + 4) BETWEEN 1 AND 1");
    compare(expr.between(val, expr), "(17 + 4) BETWEEN 1 AND (17 + 4)");
    compare(expr.between(expr, val), "(17 + 4) BETWEEN (17 + 4) AND 1");
    compare(expr.between(expr, expr),
                  "(17 + 4) BETWEEN (17 + 4) AND (17 + 4)");

    compare(val.between(val, val) and true, "(1 BETWEEN 1 AND 1) AND 1");
  }
  {
    const auto val = sqlpp::value(1);
    const auto expr = sqlpp::value(17) + 4;

    // Operands are enclosed in parentheses where required.
    compare(val & val, "1 & 1");
    compare(val | val, "1 | 1");
    compare(val ^ val, "1 ^ 1");
    compare(val << val, "1 << 1");
    compare(val >> val, "1 >> 1");

    compare(val & expr, "1 & (17 + 4)");
    compare(val | expr, "1 | (17 + 4)");
    compare(val ^ expr, "1 ^ (17 + 4)");
    compare(val << expr, "1 << (17 + 4)");
    compare(val >> expr, "1 >> (17 + 4)");

    compare(expr & val, "(17 + 4) & 1");
    compare(expr | val, "(17 + 4) | 1");
    compare(expr ^ val, "(17 + 4) ^ 1");
    compare(expr << val, "(17 + 4) << 1");
    compare(expr >> val, "(17 + 4) >> 1");

    compare(expr & expr, "(17 + 4) & (17 + 4)");
    compare(expr | expr, "(17 + 4) | (17 + 4)");
    compare(expr ^ expr, "(17 + 4) ^ (17 + 4)");
    compare(expr << expr, "(17 + 4) << (17 + 4)");
    compare(expr >> expr, "(17 + 4) >> (17 + 4)");

    // Same for unary operators
    compare(~val, "~1");
    compare(~expr, "~(17 + 4)");
  }
  {
    // Keep existing test variables if they don't conflict
    const auto cond = sqlpp::value(true);
    const auto cond2 = sqlpp::value(false);
    const auto val = 11;
    const auto val2 = 13;
    const auto expr = sqlpp::value(17) + 4;

    // Case operands use parentheses where required.
    compare(case_when(cond).then(val).else_(val),
                  "CASE WHEN 1 THEN 11 ELSE 11 END");
    compare(case_when(cond).then(val).else_(expr),
                  "CASE WHEN 1 THEN 11 ELSE (17 + 4) END");
    compare(case_when(cond).then(expr).else_(val),
                  "CASE WHEN 1 THEN (17 + 4) ELSE 11 END");
    compare(case_when(cond).then(expr).else_(expr),
                  "CASE WHEN 1 THEN (17 + 4) ELSE (17 + 4) END");
    compare(case_when(false or cond).then(val).else_(val),
                  "CASE WHEN (0 OR 1) THEN 11 ELSE 11 END");
    compare(case_when(false or cond).then(val).else_(expr),
                  "CASE WHEN (0 OR 1) THEN 11 ELSE (17 + 4) END");
    compare(case_when(false or cond).then(expr).else_(val),
                  "CASE WHEN (0 OR 1) THEN (17 + 4) ELSE 11 END");
    compare(case_when(false or cond).then(expr).else_(expr),
                  "CASE WHEN (0 OR 1) THEN (17 + 4) ELSE (17 + 4) END");

    // Mulitple when/then pairs serialize as expected.
    compare(case_when(cond).then(val).when(cond2).then(val2).else_(expr),
                  "CASE WHEN 1 THEN 11 WHEN 0 THEN 13 ELSE (17 + 4) END");
  }
  {
    compare(cast("7", sqlpp::as<bool>()), "CAST('7' AS BOOLEAN)");
    compare(cast("7", sqlpp::as<int>()), "CAST('7' AS BIGINT)");
    compare(cast("7", sqlpp::as<unsigned>()),
                  "CAST('7' AS BIGINT UNSIGNED)");
    compare(cast("7", sqlpp::as<float>()),
                  "CAST('7' AS DOUBLE PRECISION)");
    compare(cast("7", sqlpp::as<sqlpp::text>()), "CAST('7' AS VARCHAR)");
    compare(cast("7", sqlpp::as<sqlpp::blob>()), "CAST('7' AS BLOB)");
    compare(cast("7", sqlpp::as<std::chrono::sys_days>()), "CAST('7' AS DATE)");
    compare(cast("7", sqlpp::as<sqlpp::timestamp>()), "CAST('7' AS TIMESTAMP)");
    compare(cast("7", sqlpp::as<sqlpp::time_of_day>()), "CAST('7' AS TIME)");
  }
  {
    const auto val = sqlpp::value(1);
    const auto expr = sqlpp::value(17) + 4;

    // Operands are enclosed in parentheses where required.
    compare(val < val, "1 < 1");
    compare(val <= val, "1 <= 1");
    compare(val == val, "1 = 1");
    compare(val != val, "1 <> 1");
    compare(val >= val, "1 >= 1");
    compare(val > val, "1 > 1");
    compare(val.is_distinct_from(val), "1 IS DISTINCT FROM 1");
    compare(val.is_not_distinct_from(val), "1 IS NOT DISTINCT FROM 1");

    compare(val < expr, "1 < (17 + 4)");
    compare(val <= expr, "1 <= (17 + 4)");
    compare(val == expr, "1 = (17 + 4)");
    compare(val != expr, "1 <> (17 + 4)");
    compare(val >= expr, "1 >= (17 + 4)");
    compare(val > expr, "1 > (17 + 4)");
    compare(val.is_distinct_from(expr), "1 IS DISTINCT FROM (17 + 4)");
    compare(val.is_not_distinct_from(expr),
                  "1 IS NOT DISTINCT FROM (17 + 4)");

    compare(expr < val, "(17 + 4) < 1");
    compare(expr <= val, "(17 + 4) <= 1");
    compare(expr == val, "(17 + 4) = 1");
    compare(expr != val, "(17 + 4) <> 1");
    compare(expr >= val, "(17 + 4) >= 1");
    compare(expr > val, "(17 + 4) > 1");
    compare(expr.is_distinct_from(val), "(17 + 4) IS DISTINCT FROM 1");
    compare(expr.is_not_distinct_from(val),
                  "(17 + 4) IS NOT DISTINCT FROM 1");

    compare(expr < expr, "(17 + 4) < (17 + 4)");
    compare(expr <= expr, "(17 + 4) <= (17 + 4)");
    compare(expr == expr, "(17 + 4) = (17 + 4)");
    compare(expr != expr, "(17 + 4) <> (17 + 4)");
    compare(expr >= expr, "(17 + 4) >= (17 + 4)");
    compare(expr > expr, "(17 + 4) > (17 + 4)");
    compare(expr.is_distinct_from(expr),
                  "(17 + 4) IS DISTINCT FROM (17 + 4)");
    compare(expr.is_not_distinct_from(expr),
                  "(17 + 4) IS NOT DISTINCT FROM (17 + 4)");

    // Same for unary operators
    compare(val.is_null(), "1 IS NULL");
    compare(val.is_not_null(), "1 IS NOT NULL");

    compare(expr.is_null(), "(17 + 4) IS NULL");
    compare(expr.is_not_null(), "(17 + 4) IS NOT NULL");
  }
  {
    const auto val = sqlpp::value(17);

    compare(exists(select(val.as<"v">())), "EXISTS (SELECT 17 AS v)");
    compare(true and exists(select(val.as<"v">())),
                  "1 AND EXISTS (SELECT 17 AS v)");
    compare(exists(select(val.as<"v">())) and true,
                  "EXISTS (SELECT 17 AS v) AND 1");

    compare(exists(select(val.as<"v">())).as<"exists_">(),
                  "EXISTS (SELECT 17 AS v) AS exists_");
  }
  {
    const auto val = sqlpp::value(17);
    const auto expr = sqlpp::value(17) + 4;
    using expr_t = typename std::decay<decltype(expr)>::type;

    // IN expression with single select or other singe expression: No extra
    // parentheses.
    compare(val.in(std::make_tuple(val)), "17 IN (17)");
    compare(val.in(std::make_tuple(expr)), "17 IN (17 + 4)");
    compare(val.in(std::make_tuple(value(select(val.as<"v">())))), "17 IN (SELECT 17 AS v)");
    compare(val.in(select(val.as<"v">())), "17 IN (SELECT 17 AS v)");

    compare(val.not_in(std::make_tuple(val)), "17 NOT IN (17)");
    compare(val.not_in(std::make_tuple(expr)), "17 NOT IN (17 + 4)");
    compare(val.not_in(std::make_tuple(value(select(val.as<"v">())))), "17 NOT IN (SELECT 17 AS v)");
    compare(val.not_in(select(val.as<"v">())), "17 NOT IN (SELECT 17 AS v)");

    // IN expressions with multiple arguments require inner parentheses.
    compare(val.in(std::make_tuple(1, value(select(val.as<"v">())), 23)),
                  "17 IN (1, (SELECT 17 AS v), 23)");
    compare(val.in(std::vector<int>{17, 18, 19}), "17 IN (17, 18, 19)");
    compare(val.in(std::vector<expr_t>{expr, expr, expr}),
                  "17 IN ((17 + 4), (17 + 4), (17 + 4))");

    compare(val.not_in(std::make_tuple(1, value(select(val.as<"v">())))),
                  "17 NOT IN (1, (SELECT 17 AS v))");
    compare(val.not_in(std::vector<int>{17, 18, 19}),
                  "17 NOT IN (17, 18, 19)");
    compare(val.not_in(std::vector<expr_t>{expr, expr, expr}),
                  "17 NOT IN ((17 + 4), (17 + 4), (17 + 4))");

    // IN expressions with no arguments are an error in SQL. No magic
    // protection.
    compare(val.in(std::vector<expr_t>{}), "17 IN ()");
    compare(val.not_in(std::vector<expr_t>{}), "17 NOT IN ()");

  }
  {
    const auto val = sqlpp::value(true);
    const auto expr = sqlpp::value(17) > 15;

    // Operands are enclosed in parenheses where required
    compare(val and val, "1 AND 1");
    compare(val and expr, "1 AND (17 > 15)");
    compare(expr and val, "(17 > 15) AND 1");
    compare(expr and expr, "(17 > 15) AND (17 > 15)");

    compare(val or val, "1 OR 1");
    compare(val or expr, "1 OR (17 > 15)");
    compare(expr or val, "(17 > 15) OR 1");
    compare(expr or expr, "(17 > 15) OR (17 > 15)");

    compare(not val, "NOT 1");
    compare(not expr, "NOT (17 > 15)");

    // Combined logical expression.
    compare(not val and not expr, "(NOT 1) AND (NOT (17 > 15))");
    compare(not val or not expr, "(NOT 1) OR (NOT (17 > 15))");
    compare(not(val and expr), "NOT (1 AND (17 > 15))");
    compare(not(val or expr), "NOT (1 OR (17 > 15))");

    // Chains are not nested in parentheses.
    compare(val and val and val and val and val,
                  "1 AND 1 AND 1 AND 1 AND 1");
    compare(val or val or val or val or val, "1 OR 1 OR 1 OR 1 OR 1");

    // Broken chains use parentheses for the respective blocks.
    compare((val and val and val) or (val and val),
                  "(1 AND 1 AND 1) OR (1 AND 1)");
    compare((val or val or val) and (val or val),
                  "(1 OR 1 OR 1) AND (1 OR 1)");

    // NOT is not chained gracefully, but hey, don't do that anyways.
    compare(not not not val, "NOT (NOT (NOT 1))");

    // Operands are enclosed in parentheses where required or completely dropped
    // if inactive
    compare(val and dynamic(true, val), "1 AND 1");
    compare(val and dynamic(true, expr), "1 AND (17 > 15)");
    compare(expr and dynamic(true, val), "(17 > 15) AND 1");
    compare(expr and dynamic(true, expr), "(17 > 15) AND (17 > 15)");

    compare(val or dynamic(true, val), "1 OR 1");
    compare(val or dynamic(true, expr), "1 OR (17 > 15)");
    compare(expr or dynamic(true, val), "(17 > 15) OR 1");
    compare(expr or dynamic(true, expr), "(17 > 15) OR (17 > 15)");

    compare(val and dynamic(false, val), "1");
    compare(val and dynamic(false, expr), "1");
    compare(expr and dynamic(false, val), "(17 > 15)");
    compare(expr and dynamic(false, expr), "(17 > 15)");

    compare(val or dynamic(false, val), "1");
    compare(val or dynamic(false, expr), "1");
    compare(expr or dynamic(false, val), "(17 > 15)");
    compare(expr or dynamic(false, expr), "(17 > 15)");

    // Chained partially dynamic expressions
    compare(val and dynamic(true, val) and expr, "1 AND 1 AND (17 > 15)");
    compare(val and dynamic(false, val) and expr, "1 AND (17 > 15)");

    compare(val or dynamic(true, val) or expr, "1 OR 1 OR (17 > 15)");
    compare(val or dynamic(false, val) or expr, "1 OR (17 > 15)");

    compare(val and dynamic(true, val) and dynamic(true, expr), "1 AND 1 AND (17 > 15)");
    compare(val and dynamic(false, val) and dynamic(true, expr), "1 AND (17 > 15)");

    compare(val or dynamic(true, val) or dynamic(true, expr), "1 OR 1 OR (17 > 15)");
    compare(val or dynamic(false, val) or dynamic(true, expr), "1 OR (17 > 15)");

    compare(val and dynamic(true, val) and dynamic(false, expr), "1 AND 1");
    compare(val and dynamic(false, val) and dynamic(false, expr), "1");

    compare(val or dynamic(true, val) or dynamic(false, expr), "1 OR 1");
    compare(val or dynamic(false, val) or dynamic(false, expr), "1");

    // More complex expressions
    compare((val and dynamic(true, expr)) or dynamic(true, val),
                  "(1 AND (17 > 15)) OR 1");
    // The extra parentheses are not great, but also difficult to avoid and not
    // a problem I believe.
    compare((val and dynamic(false, expr)) or dynamic(true, val),
                  "(1) OR 1");
    compare((val and dynamic(true, expr)) or dynamic(false, val),
                  "(1 AND (17 > 15))");
    compare((val and dynamic(false, expr)) or dynamic(false, val), "(1)");
  }
  {
    const auto val = sqlpp::value(1);
    const auto expr = sqlpp::value(17) + 4;

    // Operands are enclosed in parentheses where required.
    compare(val.asc(), "1 ASC");
    compare(val.desc(), "1 DESC");
    compare(val.order(sqlpp::sort_type::asc), "1 ASC");
    compare(val.order(sqlpp::sort_type::desc), "1 DESC");

    compare(expr.asc(), "(17 + 4) ASC");
    compare(expr.desc(), "(17 + 4) DESC");
    compare(expr.order(sqlpp::sort_type::asc), "(17 + 4) ASC");
    compare(expr.order(sqlpp::sort_type::desc), "(17 + 4) DESC");

    compare(val.asc().nulls_first(), "1 ASC NULLS FIRST");
    compare(val.desc().nulls_first(), "1 DESC NULLS FIRST");
    compare(val.order(sqlpp::sort_type::asc).nulls_first(), "1 ASC NULLS FIRST");
    compare(val.order(sqlpp::sort_type::desc).nulls_first(), "1 DESC NULLS FIRST");

    compare(expr.asc().nulls_last(), "(17 + 4) ASC NULLS LAST");
    compare(expr.desc().nulls_last(), "(17 + 4) DESC NULLS LAST");
    compare(expr.order(sqlpp::sort_type::asc).nulls_last(), "(17 + 4) ASC NULLS LAST");
    compare(expr.order(sqlpp::sort_type::desc).nulls_last(), "(17 + 4) DESC NULLS LAST");
    compare(expr.order(sqlpp::sort_type::asc, sqlpp::null_position::last), "(17 + 4) ASC NULLS LAST");
    compare(expr.order(sqlpp::sort_type::desc, sqlpp::null_position::first), "(17 + 4) DESC NULLS FIRST");
  }

  return 0;
}

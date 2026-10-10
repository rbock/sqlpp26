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

  const auto val = sqlpp::value(17);

  const auto foo = test::tab_foo{};
  const auto bar = test::tab_bar{};

  // -----------------------------------------
  // --  returning(<columns>)
  // -----------------------------------------
  // Single column
  compare(returning(foo.float_n), " RETURNING tab_foo.double_n AS float_n");

  // Two columns
  compare(returning(foo.float_n, bar.id),
                " RETURNING tab_foo.double_n AS float_n, tab_bar.id");

  // All columns of a table
  compare(returning(all_of(foo)),
                " RETURNING tab_foo.id, tab_foo.text_nn_d, tab_foo.int_n, "
                "tab_foo.int_c_n, tab_foo.double_n AS float_n, tab_foo.u_int_n, "
                "tab_foo.blob_n, tab_foo.bool_n");

  // Optional column
  compare(returning(dynamic(true, bar.id)), " RETURNING tab_bar.id");
  compare(returning(dynamic(false, bar.id)), " RETURNING NULL AS id");

  // Multiple plain columns.
  compare(returning(foo.id, foo.text_nn_d, foo.bool_n),
                " RETURNING tab_foo.id, tab_foo.text_nn_d, tab_foo.bool_n");

  // Single expression
  compare(returning((foo.id + 17).as<"cake">()),
                " RETURNING (tab_foo.id + 17) AS cake");

  // Single dynamic column.
  compare(returning(dynamic(true, foo.id)), " RETURNING tab_foo.id");
  compare(returning(dynamic(false, foo.id)), " RETURNING NULL AS id");
  compare(returning(dynamic(false, foo.id.as<"cake">())),
                " RETURNING NULL AS cake");

  // Multiple dynamic columns (this is odd if all are dynamic)
  compare(returning(dynamic(true, foo.id), foo.text_nn_d, foo.bool_n),
                " RETURNING tab_foo.id, tab_foo.text_nn_d, tab_foo.bool_n");
  compare(returning(foo.id, dynamic(true, foo.text_nn_d), foo.bool_n),
                " RETURNING tab_foo.id, tab_foo.text_nn_d, tab_foo.bool_n");
  compare(returning(foo.id, foo.text_nn_d, dynamic(true, foo.bool_n)),
                " RETURNING tab_foo.id, tab_foo.text_nn_d, tab_foo.bool_n");

  compare(returning(dynamic(false, foo.id), foo.text_nn_d, foo.bool_n),
                " RETURNING NULL AS id, tab_foo.text_nn_d, tab_foo.bool_n");
  compare(returning(foo.id, dynamic(false, foo.text_nn_d), foo.bool_n),
                " RETURNING tab_foo.id, NULL AS text_nn_d, tab_foo.bool_n");
  compare(returning(foo.id, foo.text_nn_d, dynamic(false, foo.bool_n)),
                " RETURNING tab_foo.id, tab_foo.text_nn_d, NULL AS bool_n");

  compare(returning(foo.id, dynamic(false, foo.text_nn_d),
                               dynamic(false, foo.bool_n)),
                " RETURNING tab_foo.id, NULL AS text_nn_d, NULL AS bool_n");
  compare(returning(dynamic(false, foo.id), foo.text_nn_d,
                               dynamic(false, foo.bool_n)),
                " RETURNING NULL AS id, tab_foo.text_nn_d, NULL AS bool_n");
  compare(returning(dynamic(false, foo.id),
                               dynamic(false, foo.text_nn_d), foo.bool_n),
                " RETURNING NULL AS id, NULL AS text_nn_d, tab_foo.bool_n");

  // Single value
  compare(returning(val.as<"cheese">()),
                " RETURNING 17 AS cheese");
  compare(returning((foo.id + 17).as<"cake">()),
                " RETURNING (tab_foo.id + 17) AS cake");

  // Mixed column and value
  compare(returning(foo.id, val.as<"cheese">()),
                " RETURNING tab_foo.id, 17 AS cheese");
  compare(returning(val.as<"cake">(), foo.id),
                " RETURNING 17 AS cake, tab_foo.id");

  // Mixed column and dynamic value
  compare(
      returning(foo.id,
                     dynamic(true, val.as<"cheese">())),
      " RETURNING tab_foo.id, 17 AS cheese");
  compare(
      returning(dynamic(true, val.as<"cake">()),
                     foo.id),
      " RETURNING 17 AS cake, tab_foo.id");

  compare(
      returning(foo.id,
                     dynamic(false, val.as<"cheese">())),
      " RETURNING tab_foo.id, NULL AS cheese");
  compare(
      returning(dynamic(false, val.as<"cake">()),
                     foo.id),
      " RETURNING NULL AS cake, tab_foo.id");

  return 0;
}

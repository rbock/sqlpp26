#pragma once

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
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
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

#include <sqlpp26/ranges/aggregate_function/count.h>
#include <sqlpp26/ranges/aggregate_function/sum.h>
#include <sqlpp26/ranges/basic/column.h>
#include <sqlpp26/ranges/basic/table.h>
#include <sqlpp26/ranges/clause/for_update.h>
#include <sqlpp26/ranges/clause/from.h>
#include <sqlpp26/ranges/clause/group_by.h>
#include <sqlpp26/ranges/clause/having.h>
#include <sqlpp26/ranges/clause/insert.h>
#include <sqlpp26/ranges/clause/insert_value_list.h>
#include <sqlpp26/ranges/clause/into.h>
// TODO limit and offset should be disabled
#include <sqlpp26/ranges/clause/limit.h>
#include <sqlpp26/ranges/clause/offset.h>
#include <sqlpp26/ranges/clause/order_by.h>
#include <sqlpp26/ranges/clause/select.h>
#include <sqlpp26/ranges/clause/select_column_list.h>
#include <sqlpp26/ranges/clause/single_table.h>
#include <sqlpp26/ranges/clause/union.h>
#include <sqlpp26/ranges/clause/update.h>
#include <sqlpp26/ranges/clause/update_set_list.h>
#include <sqlpp26/ranges/clause/where.h>
#include <sqlpp26/ranges/database/connection.h>
#include <sqlpp26/ranges/operator/as_expression.h>
#include <sqlpp26/ranges/operator/assign_expression.h>
#include <sqlpp26/ranges/operator/comparison_expression.h>
#include <sqlpp26/ranges/operator/logical_expression.h>
#include <sqlpp26/ranges/operator/sort_order_expression.h>
#include <sqlpp26/ranges/query/statement.h>
#include <sqlpp26/ranges/ranges.h>
#include <sqlpp26/ranges/to_filter_expression.h>


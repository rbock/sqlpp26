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

#include <sqlpp26/core/operator/arithmetic_expression.h>
#include <functional>

namespace sqlpp::filter {

template <typename Lhs, typename Operator, typename Rhs>
struct arithmetic_expression{
template <typename Struct>
  constexpr auto operator()(const Struct& t) const {
    return _operator(_lhs(t), _rhs(t));
  }

template <typename Range>
  constexpr auto aggregate(const Range& t) const {
    return _operator(_lhs.aggregate(t), _rhs.aggregate(t));
  }

  Lhs _lhs;
  Operator _operator;
  Rhs _rhs;
};

template <typename Operator, typename Rhs>
struct unary_arithmetic_expression{
template <typename Struct>
  constexpr auto operator()(const Struct& t) const {
    return _operator(_rhs(t));
  }

template <typename Range>
  constexpr auto aggregate(const Range& t) const {
    return _operator(_rhs.aggregate(t));
  }

  Operator _operator;
  Rhs _rhs;
};

template<typename Operator>
struct filter_operator_of;

template<typename Operator>
using filter_operator_of_t = typename filter_operator_of<Operator>::type;

template<> struct filter_operator_of<::sqlpp::plus> { using type = std::plus<void>; };
template<> struct filter_operator_of<::sqlpp::minus> { using type = std::minus<void>; };
template<> struct filter_operator_of<::sqlpp::multiplies> { using type = std::multiplies<void>; };
template<> struct filter_operator_of<::sqlpp::divides> { using type = std::divides<void>; };
template<> struct filter_operator_of<::sqlpp::modulus> { using type = std::modulus<void>; };
template<> struct filter_operator_of<::sqlpp::negate> { using type = std::negate<void>; };

}

namespace sqlpp {

template <typename Lhs, typename Operator, typename Rhs>
constexpr auto to_filter_expression(const arithmetic_expression<Lhs, Operator, Rhs>& expr) {
  return filter::arithmetic_expression{to_filter_expression(read.lhs(expr)), filter::filter_operator_of_t<Operator>{},
                               to_filter_expression(read.rhs(expr))};
}

template <typename Operator, typename Rhs>
constexpr auto to_filter_expression(const arithmetic_expression<noop, Operator, Rhs>& expr) {
  return filter::unary_arithmetic_expression{filter::filter_operator_of_t<Operator>{},
                               to_filter_expression(read.rhs(expr))};
}

}  // namespace sqlpp


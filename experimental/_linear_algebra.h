//* experimental linear algebra utilities (extension of fresa_math.h)
#pragma once

#include "fresa_math.h"

namespace fresa
{
    namespace detail
    {
        //: pivot rows
        template <std::size_t From, std::size_t To, concepts::SquareMatrix M>
        requires (From < To and To < M::size().first)
        constexpr void pivot_rows(M& m) {
            if constexpr (From == To) return;
            constexpr auto N = M::size().first;
            for_<0, N>([&](auto j) {
                std::swap(m.template get<From, j>(), m.template get<To, j>());
            });
        }

        //: max index of an initializer list
        template <typename T>
        constexpr auto max_i(std::initializer_list<T> ilist) {
            return std::distance( ilist.begin(), std::max_element( ilist.begin(), ilist.end() ) );
        }
    }

    //: lu decomposition
    template <concepts::SquareMatrix M, concepts::SquareMatrix MR = M>
    requires (std::is_floating_point_v<typename MR::value_type>)
    constexpr std::tuple<bool, M, std::size_t, MR> lu(const M& m) {
        constexpr auto N = M::size().first;
        using T = typename M::value_type;
        bool singular = false;
        MR result = to<MR>(m);

        //: pivot rows
        M p = identity<M>();
        std::size_t np = 0;
        for_<0, N>([&](auto j){
            //: find max element in column j
            auto max = [&] <std::size_t ... I> (std::index_sequence<I...>) {
                return detail::max_i({ result.template get<I + j, j>()... });
            } (std::make_index_sequence<M::size().first - j>{}) + j;
            for_<j.value, N>([&](auto i){
                if (max == i.value) {
                    //: if max element is 0, matrix is singular
                    if (m.template get<i, j>() < 1e-10) {
                        singular = true;
                        return false;
                    }
                    //: swap rows
                    if constexpr (j != i) {
                        detail::pivot_rows<j.value, i.value>(result);
                        detail::pivot_rows<j.value, i.value>(p);
                        np++;
                        return false;
                    }
                }
                return true;
            });
        });

        //: lu decomposition
        if (not singular) {
            for_<0, N>([&](auto j){
                for_<j.value + 1, N>([&](auto i){
                    result.template get<i, j>() /= result.template get<j, j>();
                    for_<j.value + 1, N>([&](auto k){
                        result.template get<i, k>() -= result.template get<i, j>() * result.template get<j, k>();
                    });
                });
            });
        }

        return std::make_tuple(singular, p, np, result);
    }

    //: get lu components
    template <concepts::SquareMatrix M>
    constexpr std::tuple<M, M> lu_components(const M& m) {
        constexpr auto N = M::size().first;
        M l, u;
        for_<0, N>([&](auto j){
            for_<0, N>([&](auto i){
                if constexpr (i == j) {
                    l.template get<i, j>() = 1;
                    u.template get<i, j>() = m.template get<i, j>();
                } else if constexpr (i > j) {
                    l.template get<i, j>() = m.template get<i, j>();
                } else {
                    u.template get<i, j>() = m.template get<i, j>();
                }
            });
        });
        return std::make_tuple(l, u);
    }

    //: determinant
    template <concepts::SquareMatrix M>
    constexpr auto determinant(const M& m) {
        constexpr auto N = M::size().first;
        if constexpr (N == 1) {
            return m.template get<0, 0>();
        } else if constexpr (N == 2) {
            return m.template get<0, 0>() * m.template get<1, 1>() - m.template get<0, 1>() * m.template get<1, 0>();
        } else if constexpr (N == 3) {
            return m.template get<0, 0>() * (m.template get<1, 1>() * m.template get<2, 2>() - m.template get<1, 2>() * m.template get<2, 1>())
                 - m.template get<0, 1>() * (m.template get<1, 0>() * m.template get<2, 2>() - m.template get<1, 2>() * m.template get<2, 0>())
                 + m.template get<0, 2>() * (m.template get<1, 0>() * m.template get<2, 1>() - m.template get<1, 1>() * m.template get<2, 0>());
        } else {
            auto decomposition = lu(m);
            auto singular = std::get<0>(decomposition);
            auto np = std::get<2>(decomposition);
            auto lu_ = std::get<3>(decomposition);
            using T = typename decltype(lu_)::value_type;

            if (singular) return T{0};
            
            T result = lu_.template get<0, 0>();
            for_<0, N>([&](auto i){
                result *= lu_.template get<i, i>();
            });
            return (np - N) % 2 == 0 ? result : -result;
        }
    }

    //: inverse
    template <concepts::SquareMatrix M>
    constexpr auto inverse(const M& m) {
        constexpr auto N = M::size().first;
        auto decomposition = lu(m);
        auto singular = std::get<0>(decomposition);
        auto p = std::get<1>(decomposition);
        auto np = std::get<2>(decomposition);
        auto lu_ = std::get<3>(decomposition);
        using T = typename decltype(lu_)::value_type;

        if (singular) return identity<M>();
        
        M result = identity<M>();
        for_<0, N>([&](auto j){
            for_<0, N>([&](auto i){
                result.template get<i, j>() = p.template get<i, j>();
                for_<0, i>([&](auto k){
                    result.template get<i, j>() -= lu_.template get<i, k>() * result.template get<k, j>();
                });
            });
            for_<0, N>([&](auto i_){
                constexpr auto i = N - 1 - i_.value;
                for_<i+1, N>([&](auto k){
                    result.template get<i, j>() -= lu_.template get<i, k>() * result.template get<k, j>();
                });
                result.template get<i, j>() /= lu_.template get<i, i>();
            });
        });
        return result;
    }
}
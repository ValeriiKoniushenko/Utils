// MIT License
//
// Copyright (c) 2024 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "Core/CommonInterfaces.h"

#include <type_traits>

namespace Core
{

    template<class T, class DerivedIterator, class CopyAndMovePolicy, bool IsIgnoreDataRef = false>
    class IInputIterator : public CopyAndMovePolicy, public ISwappable<DerivedIterator>
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;
        using CopyAndMovePolicyT = CopyAndMovePolicy;
        using DataT = T;
        using DataRefT = std::conditional_t<IsIgnoreDataRef, DataT, DataT&>;

        [[nodiscard]] virtual const DataRefT operator*() const noexcept = 0;

        [[nodiscard]] virtual const DataRefT operator->() const = 0;

    protected:
        IInputIterator() = default;
    };

    template<class T, class DerivedIterator, class CopyAndMovePolicy, bool IsIgnoreDataRef = false>
    class IOutputIterator : public CopyAndMovePolicy, public ISwappable<DerivedIterator>
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;
        using CopyAndMovePolicyT = CopyAndMovePolicy;
        using DataT = T;
        using DataRefT = std::conditional_t<IsIgnoreDataRef, DataT, DataT&>;

        [[nodiscard]] virtual DataRefT operator*() noexcept = 0;

        [[nodiscard]] virtual DataRefT operator->() noexcept = 0;

    protected:
        IOutputIterator() = default;
    };

    template<class T, class DerivedIterator, class CopyAndMovePolicy, bool IsIgnoreDataRef = false>
    class IForwardIterator : public CopyAndMovePolicy, public ISwappable<DerivedIterator>
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;
        using CopyAndMovePolicyT = CopyAndMovePolicy;
        using DataT = T;
        using DataRefT = std::conditional_t<IsIgnoreDataRef, DataT, DataT&>;

        [[nodiscard]] virtual const DataRefT operator*() const noexcept = 0;

        [[nodiscard]] virtual const DataRefT operator->() const = 0;

        [[nodiscard]] virtual DataRefT operator*() noexcept = 0;

        [[nodiscard]] virtual DataRefT operator->() noexcept = 0;

        virtual DerivedIterator& operator++() noexcept = 0;

        virtual DerivedIterator operator++(int) noexcept = 0;

    protected:
        IForwardIterator() = default;
    };

    template<class T, class DerivedIterator, class CopyAndMovePolicy, bool IsIgnoreDataRef = false>
    class IBidirectionalIterator : public CopyAndMovePolicy, public ISwappable<DerivedIterator>
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;
        using CopyAndMovePolicyT = CopyAndMovePolicy;
        using DataT = T;
        using DataRefT = std::conditional_t<IsIgnoreDataRef, DataT, DataT&>;

        [[nodiscard]] virtual const DataRefT operator*() const noexcept = 0;

        [[nodiscard]] virtual const DataRefT operator->() const = 0;

        [[nodiscard]] virtual DataRefT operator*() noexcept = 0;

        [[nodiscard]] virtual DataRefT operator->() noexcept = 0;

        virtual DerivedIterator& operator++() noexcept = 0;

        virtual DerivedIterator operator++(int) noexcept = 0;

        virtual DerivedIterator& operator--() noexcept = 0;

        virtual DerivedIterator operator--(int) noexcept = 0;

    protected:
        IBidirectionalIterator() = default;
    };

    template<class T, class DerivedIterator, class CopyAndMovePolicy, bool IsIgnoreDataRef = false>
    class IRandomAccessIterator : public CopyAndMovePolicy, public ISwappable<DerivedIterator>
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;
        using CopyAndMovePolicyT = CopyAndMovePolicy;
        using DataT = T;
        using DataRefT = std::conditional_t<IsIgnoreDataRef, DataT, DataT&>;

        [[nodiscard]] virtual bool operator==(const DerivedIterator&) const noexcept = 0;

        [[nodiscard]] virtual bool operator!=(const DerivedIterator&) const noexcept = 0;

        [[nodiscard]] virtual const DataRefT operator*() const noexcept = 0;

        [[nodiscard]] virtual const DataRefT operator->() const = 0;

        [[nodiscard]] virtual DataRefT operator*() noexcept = 0;

        [[nodiscard]] virtual DataRefT operator->() noexcept = 0;

        virtual DerivedIterator& operator++() noexcept = 0;

        virtual DerivedIterator operator++(int) noexcept = 0;

        virtual DerivedIterator& operator--() noexcept = 0;

        virtual DerivedIterator operator--(int) noexcept = 0;

        virtual DerivedIterator& operator+=(int step) noexcept = 0;

        virtual DerivedIterator& operator-=(int step) noexcept = 0;

        virtual DerivedIterator operator+(int step) const noexcept = 0;

        virtual DerivedIterator operator-(int step) const noexcept = 0;

        [[nodiscard]] virtual bool operator>(const DerivedIterator& other) const noexcept = 0;

        [[nodiscard]] virtual bool operator>=(const DerivedIterator& other) const noexcept = 0;

        [[nodiscard]] virtual bool operator<(const DerivedIterator& other) const noexcept = 0;

        [[nodiscard]] virtual bool operator<=(const DerivedIterator& other) const noexcept = 0;

    protected:
        IRandomAccessIterator() = default;
    };

} // namespace Core
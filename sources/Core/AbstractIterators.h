// Utils
// Copyright 2018-2026 Valerii Koniushenko
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include <type_traits>

namespace Core
{

    template<class T, class DerivedIterator, bool IsIgnoreDataRef = false>
    class IInputIterator
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;

        using DataT = T;
        using DataRefT = std::conditional_t<IsIgnoreDataRef, DataT, DataT&>;

        [[nodiscard]] virtual bool operator==(const DerivedIterator&) const noexcept = 0;

        [[nodiscard]] virtual bool operator!=(const DerivedIterator&) const noexcept = 0;

        [[nodiscard]] virtual const DataRefT operator*() const noexcept = 0;

        [[nodiscard]] virtual const DataRefT operator->() const = 0;

    protected:
        IInputIterator() = default;
    };

    template<class T, class DerivedIterator, bool IsIgnoreDataRef = false>
    class IOutputIterator
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;

        using DataT = T;
        using DataRefT = std::conditional_t<IsIgnoreDataRef, DataT, DataT&>;

        [[nodiscard]] virtual bool operator==(const DerivedIterator&) const noexcept = 0;

        [[nodiscard]] virtual bool operator!=(const DerivedIterator&) const noexcept = 0;

        [[nodiscard]] virtual DataRefT operator*() noexcept = 0;

        [[nodiscard]] virtual DataRefT operator->() noexcept = 0;

    protected:
        IOutputIterator() = default;
    };

    template<class T, class DerivedIterator, bool IsIgnoreDataRef = false>
    class IForwardIterator
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;

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

    protected:
        IForwardIterator() = default;
    };

    template<class T, class DerivedIterator, bool IsIgnoreDataRef = false>
    class IBidirectionalIterator
    {
    public:
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;

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

    protected:
        IBidirectionalIterator() = default;
    };

    template<class T, class DerivedIterator, bool IsIgnoreDataRef = false>
    class IRandomAccessIterator
    {
    public:
        virtual ~IRandomAccessIterator() = default;
        constexpr static bool isIgnoreDataRef = IsIgnoreDataRef;

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
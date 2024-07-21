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

#include "Core/Assert.h"
#include "Utils/CopyableAndMoveableBehaviour.h"
#include "Core/GlobalEnums.h"

namespace Core
{
    template<class DataT>
    class Iterator : public Utils::CopyableAndMoveable
    {
    public:
        [[nodiscard]] virtual bool operator==(const Iterator& other) const noexcept
        {
            if (_data)
            {
                return _data == other._data;
            }
            return false;
        }

        [[nodiscard]] virtual bool operator!=(const Iterator& other) const noexcept
        {
            if (_data)
            {
                return _data != other._data;
            }
            return false;
        }

    protected:
        explicit Iterator(DataT* data)
            : _data(data)
        {
        }

    protected:
        DataT* _data = nullptr;
    };

    template<class DataT>
    class InputIterator : public Iterator<DataT>
    {
    public:
        [[nodiscard]] virtual const DataT& operator*() const noexcept
        {
            Assert(this->_data, "Iterator wasn't initialized");
            return *this->_data;
        }

        [[nodiscard]] virtual const DataT& operator->() const
        {
            Assert(this->_data, "Iterator wasn't initialized");
            return *this->_data;
        }

    protected:
        InputIterator() = default;
    };

    template<class DataT>
    class OutputIterator : public Iterator<DataT>
    {
    public:
        [[nodiscard]] virtual DataT& operator*() noexcept
        {
            Assert(this->_data, "Iterator wasn't initialized");
            return *this->_data;
        }

        [[nodiscard]] virtual DataT& operator->() noexcept
        {
            Assert(this->_data, "Iterator wasn't initialized");
            return *this->_data;
        }

    protected:
        OutputIterator() = default;
    };

    template<class DataT>
    class ForwardIterator : public InputIterator<DataT>
    {
    public:
        using Self = ForwardIterator<DataT>;

    public:
        [[nodiscard]] virtual DataT& operator*() noexcept
        {
            Assert(this->_data, "Iterator wasn't initialized");
            return *this->_data;
        }

        [[nodiscard]] virtual DataT& operator->() noexcept
        {
            Assert(this->_data, "Iterator wasn't initialized");
            return *this->_data;
        }

        virtual Self& operator++() noexcept
        {
            MakeStep(1);
            return *this;
        }

        virtual Self operator++(int) noexcept
        {
            const auto temp = *this;
            MakeStep(1);
            return temp;
        }

    protected:
        ForwardIterator() = default;

        virtual void MakeStep(int step) noexcept = 0;
    };

    template<class DataT>
    class BidirectionalIterator : public ForwardIterator<DataT>
    {
    public:
        using Self = BidirectionalIterator<DataT>;

    public:
        virtual Self& operator--() noexcept
        {
            MakeStep(-1);
            return *this;
        }

        virtual Self operator--(int) noexcept
        {
            const auto temp = *this;
            MakeStep(-1);
            return temp;
        }

    protected:
        BidirectionalIterator() = default;
    };

    template<class DataT>
    class RandomAccessIterator : public ForwardIterator<DataT>
    {
    public:
        using Self = RandomAccessIterator<DataT>;

    public:
        virtual Self& operator+=(int step) noexcept
        {
            MakeStep(step);
            return *this;
        }

        virtual Self& operator-=(int step) noexcept
        {
            MakeStep(-step);
            return *this;
        }

        virtual Self operator+(int step) const noexcept
        {
            auto temp = *this;
            temp.MakeStep(step);
            return temp;
        }

        virtual Self operator-(int step) const noexcept
        {
            auto temp = *this;
            temp.MakeStep(-step);
            return temp;
        }

        [[nodiscard]] virtual bool operator>(const Self& other) const noexcept
        {
            const auto result = *this <=> other;
            return result == Comparison::Greater;
        }

        [[nodiscard]] virtual bool operator>=(const Self& other) const noexcept
        {
            const auto result = *this <=> other;
            return result == Comparison::Greater || result == Comparison::Equal;
        }

        [[nodiscard]] virtual bool operator<(const Self& other) const noexcept
        {
            const auto result = *this <=> other;
            return result == Comparison::Less;
        }

        [[nodiscard]] virtual bool operator<=(const Self& other) const noexcept
        {
            const auto result = *this <=> other;
            return result == Comparison::Less || result == Comparison::Equal;
        }

    protected:
        RandomAccessIterator() = default;

        virtual void MakeStep(int step) noexcept = 0;
        [[nodiscard]] virtual Comparison operator<=>(const Self& other) const noexcept = 0;
    };

} // namespace Core
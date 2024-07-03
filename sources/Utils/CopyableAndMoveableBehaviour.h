// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
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

namespace Utils
{
    class CopyableAndMoveableBehaviour
    {
      public:
        virtual ~CopyableAndMoveableBehaviour() = default;
    };

    class CopyableAndMoveable : public CopyableAndMoveableBehaviour
    {
      public:
        CopyableAndMoveable() = default;
        ~CopyableAndMoveable() override = default;
        CopyableAndMoveable(CopyableAndMoveable&&) = default;
        CopyableAndMoveable& operator=(CopyableAndMoveable&&) = default;
        CopyableAndMoveable(const CopyableAndMoveable&) = default;
        CopyableAndMoveable& operator=(const CopyableAndMoveable&) = default;
    };

    class CopyableButNotMoveable : public CopyableAndMoveableBehaviour
    {
      public:
        CopyableButNotMoveable() = default;
        ~CopyableButNotMoveable() override = default;
        CopyableButNotMoveable(CopyableButNotMoveable&&) = delete;
        CopyableButNotMoveable& operator=(CopyableButNotMoveable&&) = delete;
        CopyableButNotMoveable(const CopyableButNotMoveable&) = default;
        CopyableButNotMoveable& operator=(const CopyableButNotMoveable&) = default;
    };

    class NotCopyableAndNotMoveable : public CopyableAndMoveableBehaviour
    {
      public:
        NotCopyableAndNotMoveable() = default;
        ~NotCopyableAndNotMoveable() override = default;
        NotCopyableAndNotMoveable(NotCopyableAndNotMoveable&&) = delete;
        NotCopyableAndNotMoveable& operator=(NotCopyableAndNotMoveable&&) = delete;
        NotCopyableAndNotMoveable(const NotCopyableAndNotMoveable&) = delete;
        NotCopyableAndNotMoveable& operator=(const NotCopyableAndNotMoveable&) = delete;
    };

    class NotCopyableButMoveable : public CopyableAndMoveableBehaviour
    {
      public:
        NotCopyableButMoveable() = default;
        ~NotCopyableButMoveable() override = default;
        NotCopyableButMoveable(NotCopyableButMoveable&&) = default;
        NotCopyableButMoveable& operator=(NotCopyableButMoveable&&) = default;
        NotCopyableButMoveable(const NotCopyableButMoveable&) = delete;
        NotCopyableButMoveable& operator=(const NotCopyableButMoveable&) = delete;
    };

} // namespace Utils
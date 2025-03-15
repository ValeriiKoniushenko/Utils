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

#include "Core/String.h"

#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

namespace Core::SPcre2
{

    class BaseRegex : public Utils::CopyableAndMoveable
    {
    public:
        using String = StringAtom;

    public:
        BaseRegex() = default;
        ~BaseRegex() override;

        explicit BaseRegex(String::StdStringViewT pattern) { SetPattern(pattern); }

        BaseRegex& SetPattern(String::StdStringViewT pattern)
        {
            _pattern = pattern;
            return *this;
        }
        [[nodiscard]] String GetPattern() const { return _pattern; }

        BaseRegex& SetOptions(uint32_t options) noexcept
        {
            _options = options;
            return *this;
        }
        [[nodiscard]] uint32_t GetOptions(uint32_t options) const noexcept { return _options; }

        BaseRegex& SetSubject(const String::CharT* subject) noexcept
        {
            _subject = subject;
            return *this;
        }
        [[nodiscard]] const String::CharT* GetSubject() const noexcept { return _subject; }

        [[nodiscard]] static String GetErrorString(const BaseRegex& regex);
        [[nodiscard]] String GetErrorString() const { return GetErrorString(*this); }
        [[nodiscard]] bool HasError() const noexcept { return _errorCode != 0; }
        [[nodiscard]] PCRE2_SIZE GetErrorOffset() const noexcept { return _errorOffset; }

        void Clear()
        {
            _errorOffset = 0;
            _errorCode = 0;
            _pattern.Clear();
            _subject = nullptr;

            if (_regex)
            {
                pcre2_code_free(_regex);
            }
        }

        bool Compile();
        [[nodiscard]] bool IsCompiled() const noexcept { return _regex; }

        [[nodiscard]] pcre2_code* GetRawPcre2Code() noexcept { return _regex; }

    protected:
        String _pattern;
        uint32_t _options = 0;

        int _errorCode = 0;
        PCRE2_SIZE _errorOffset = 0;

        const String::CharT* _subject = nullptr;
        pcre2_code* _regex = nullptr;
    };

    using Regex = BaseRegex;

} // namespace Core::SPcre2
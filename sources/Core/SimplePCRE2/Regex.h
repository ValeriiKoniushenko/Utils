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

#include "Utils/CopyableAndMoveableBehaviour.h"

#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

#include <string>
#include <string_view>
#include <vector>

namespace Core::SPcre2
{

    class BaseRegex : public Utils::CopyableAndMoveable
    {
    public:
        BaseRegex() = default;
        ~BaseRegex() override;

        explicit BaseRegex(std::string_view pattern) { SetPattern(pattern); }

        void SetPattern(std::string_view pattern) { _pattern = pattern; }
        [[nodiscard]] std::string GetPattern() const { return _pattern; }

        void SetCompileOptions(uint32_t options) noexcept { _compileOptions = options; }
        [[nodiscard]] uint32_t GetCompileOptions(uint32_t options) const noexcept { return _compileOptions; }

        void SetSubject(const char* subject) noexcept { _subject = subject; }
        [[nodiscard]] const char* GetSubject() const noexcept { return _subject; }

        [[nodiscard]] static std::string GetErrorString(const BaseRegex& regex);
        [[nodiscard]] std::string GetErrorString() const { return GetErrorString(*this); }
        [[nodiscard]] bool HasError() const noexcept { return _errorCode != 0; }
        [[nodiscard]] PCRE2_SIZE GetErrorOffset() const noexcept { return _errorOffset; }

        virtual void Clear() { _Clear(); }

        bool Compile();
        [[nodiscard]] bool IsCompiled() const noexcept { return _regex; }

        [[nodiscard]] PCRE2_SIZE GetLimit() const noexcept { return _limit; }
        void SetLimit(PCRE2_SIZE limit) noexcept { _limit = limit; }

        [[nodiscard]] pcre2_code* GetRawPcre2Code() noexcept { return _regex; }

    protected:
        virtual void OnRegexCompiled() {}

    protected:
        std::string _pattern;
        uint32_t _compileOptions = 0;
        PCRE2_SIZE _limit = PCRE2_ZERO_TERMINATED;

        int _errorCode = 0;
        PCRE2_SIZE _errorOffset = 0;

        const char* _subject = nullptr;
        pcre2_code* _regex = nullptr;

    private:
        void _Clear();
        void _FreeRegex();
    };

    class BaseRegexMatch : public BaseRegex
    {
    public:
        struct MatchedData
        {
            constexpr static auto invalid = ~static_cast<uint64_t>(0);
            uint64_t offset = invalid;
            uint64_t size = invalid;

            [[nodiscard]] bool IsMatched() const noexcept { return size != invalid && offset != invalid; }
            [[nodiscard]] explicit operator bool() const noexcept { return IsMatched(); }
        };

    public:
        BaseRegexMatch() = default;
        ~BaseRegexMatch() override;

        explicit BaseRegexMatch(const char* pattern, const char* subject = nullptr);

        void Clear() override;

        [[nodiscard]] MatchedData Match(PCRE2_SIZE offset = 0);
        [[nodiscard]] std::vector<MatchedData> MatchAll(PCRE2_SIZE offset = 0);

        void SetMatchOptions(uint32_t options) noexcept { _matchOptions = options; }
        [[nodiscard]] uint32_t GetMatchOptions(uint32_t options) const noexcept { return _matchOptions; }

    protected:
        void OnRegexCompiled() override;

    protected:
        pcre2_match_data* _matchData = nullptr;
        uint32_t _matchOptions = 0;

    private:
        void _Clear();
    };

    using Regex = BaseRegex;
    using RegexMatch = BaseRegexMatch;

} // namespace Core::SPcre2
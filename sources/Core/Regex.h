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

#include "Assert.h"
#include "Utils/CopyableAndMoveableBehaviour.h"
#include "Utils/CrossString.h"

#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

#include <functional>
#include <string>
#include <vector>

namespace Core
{

    class BaseRegex : public Utils::CopyableAndMoveable
    {
    public:
        BaseRegex() = default;
        ~BaseRegex() override;

        explicit BaseRegex(const char* pattern, const char* subject = nullptr);

        void SetPattern(const char* pattern) { _pattern = pattern; }
        [[nodiscard]] std::string GetPattern() const { return _pattern; }

        void SetCompileOptions(uint32_t options) noexcept { _compileOptions = options; }
        [[nodiscard]] uint32_t GetCompileOptions(uint32_t options) const noexcept { return _compileOptions; }

        void SetSubject(const char* subject) noexcept { _subject = subject; }
        [[nodiscard]] const char* GetSubject() const noexcept { return _subject; }

        [[nodiscard]] static std::string GetErrorString(const BaseRegex& regex);
        [[nodiscard]] static std::string GetErrorString(int errorCode);
        [[nodiscard]] std::string GetErrorString() const { return GetErrorString(*this); }
        [[nodiscard]] bool HasError() const noexcept { return _errorCode != 0; }
        [[nodiscard]] PCRE2_SIZE GetErrorOffset() const noexcept { return _errorOffset; }

        virtual void Clear() { _Clear(); }

        bool Compile();
        [[nodiscard]] bool IsCompiled() const noexcept { return _regex; }

        [[nodiscard]] PCRE2_SIZE GetLimit() const noexcept { return _limit; }
        void SetLimit(PCRE2_SIZE limit) noexcept { _limit = limit; }

        [[nodiscard]] PCRE2_SIZE GetOffset() const noexcept { return _offset; }
        void SetOffset(PCRE2_SIZE offset) noexcept { _offset = offset; }

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
        PCRE2_SIZE _offset = 0;

    private:
        void _Clear();
        void _FreeRegex();
    };

    template<class T>
    concept BaseRegexMatch_MatchedData_Convert_Reqs = requires(T t) {
        { t.resize(0) };
        { t.size() };
        { t.data() };
        { T() };
    };

    class BaseRegexMatch : public BaseRegex
    {
    public:
        struct MatchedData
        {
            constexpr static auto invalid = 0;
            uint64_t offset = invalid;
            uint64_t size = invalid;

            [[nodiscard]] bool IsMatched() const noexcept { return !(size == invalid && offset == invalid); }
            [[nodiscard]] explicit operator bool() const noexcept { return IsMatched(); }

            template<BaseRegexMatch_MatchedData_Convert_Reqs T>
            [[nodiscard]] T ConvertTo(const T& original) const
            {
                return ConvertTo<T>(original.data(), original.size());
            }

            template<BaseRegexMatch_MatchedData_Convert_Reqs T>
            [[nodiscard]] T ConvertTo(const char* origStr, uint64_t origSize) const
            {
                if (offset + size >= origSize) [[unlikely]]
                {
                    Assert();
                    return {};
                }

                T out;
                out.resize(size);
                memcpy_s(out.data(), size * sizeof(*out.data()), origStr + offset, size * sizeof(*origStr));
                return out;
            }
        };

        using BaseRegex::BaseRegex;
        using MatchedDataVector = std::vector<MatchedData>;

    public:
        BaseRegexMatch() = default;
        ~BaseRegexMatch() override;

        void Clear() override;

        [[nodiscard]] MatchedData Match() const;
        [[nodiscard]] MatchedDataVector MatchAll() const;

        /**
         * @brief Will iterate over every match until the end.
         * @param callback can take a few function's type:
         * - void(MatchedData) - will iterate until the end.
         * - bool(MatchedData) - will iterate until 'true' is returned from the callback.
         */
        template<class FuncT>
        void IterateOverMatches(FuncT&& callback)
        {
            Impl_IterateOverMatches(std::forward<decltype(callback)>(callback), _offset);
        }

        void SetMatchOptions(uint32_t options) noexcept { _matchOptions = options; }
        [[nodiscard]] uint32_t GetMatchOptions(uint32_t options) const noexcept { return _matchOptions; }

    protected:
        void OnRegexCompiled() override;

    protected:
        pcre2_match_data* _matchData = nullptr;
        uint32_t _matchOptions = 0;

    private:
        void _FreeMatchData();
        void _Clear();

    private:
        // ================== PIPMPLs =======================
        template<class FuncT>
        void Impl_IterateOverMatches(FuncT&& callback, PCRE2_SIZE offset = 0)
        {
            if (!IsCompiled() || _matchData == nullptr) [[unlikely]]
            {
                Assert("Regex wasn't compiled or match data was failed!");
                return;
            }

            int result = 0;
            do
            {
                result = pcre2_match(_regex,                                  // Compiled regex
                                     reinterpret_cast<PCRE2_SPTR8>(_subject), // Subject string
                                     _limit,                                  // Subject is null-terminated
                                     offset,                                  // Start at offset 0
                                     _matchOptions,                           // Default options
                                     _matchData,                              // Match data
                                     nullptr                                  // Default match context
                );

                if (result > 0)
                {
                    MatchedData md;
                    md.offset = pcre2_get_startchar(_matchData);
                    const auto* ovector = pcre2_get_ovector_pointer(_matchData);
                    if (!ovector) [[unlikely]]
                    {
                        Assert(false);
                        return;
                    }

                    md.size = ovector[1] - ovector[0];

                    if constexpr (std::is_void_v<decltype(callback(md))>)
                    {
                        std::invoke(callback, md);
                    }
                    else
                    {
                        if (!std::invoke(callback, md))
                        {
                            return;
                        }
                    }

                    offset = ovector[1];
                }
            } while (result > 0);
        }
    };

    class BaseRegexReplace : public BaseRegex
    {
    public:
        using BaseRegex::BaseRegex;

    public:
        bool Replace();

        void SetReplaceOptions(uint32_t options) noexcept { _replaceOptions = options; }
        [[nodiscard]] uint32_t GetReplaceOptions(uint32_t options) const noexcept { return _replaceOptions; }

        void SetReplacementString(const char* string) noexcept { _replacement = string; }
        [[nodiscard]] const char* GetReplacementString() const noexcept { return _replacement; }

        void SetOutputString(char* allocatedString, PCRE2_SIZE size) noexcept;

        void SetReplaceAll(bool value);

        void Clear() override;

    protected:
        uint32_t _replaceOptions = 0;
        const char* _replacement = nullptr;

        char* _allocatedString = nullptr;
        PCRE2_SIZE _allocatedSize = 0;

    private:
        void _Clear();
    };

    using Regex = BaseRegex;
    using RegexMatch = BaseRegexMatch;
    using RegexReplace = BaseRegexReplace;

} // namespace Core
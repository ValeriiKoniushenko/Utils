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

#include "Regex.h"

namespace Core
{
    BaseRegex::~BaseRegex()
    {
        _Clear();
    }

    BaseRegex::BaseRegex(const char* pattern, const char* subject)
    {
        if (pattern)
        {
            SetPattern(pattern);
        }

        if (subject)
        {
            SetSubject(subject);
        }
    }

    std::string BaseRegex::GetErrorString(const BaseRegex& regex)
    {
        auto str = GetErrorString(regex._errorCode);

        if (!str.empty())
        {
            std::string msg;
            msg += "Regex failed at the offset: ";
            msg += std::to_string(regex._errorOffset);
            msg += ". Error message: ";
            msg += str;
            return msg;
        }

        return {};
    }

    std::string BaseRegex::GetErrorString(int errorCode)
    {
        char buffer[256]{};
        pcre2_get_error_message(errorCode, reinterpret_cast<PCRE2_UCHAR8*>(buffer), sizeof(buffer));
        if (buffer[0] != '\0')
        {
            return { buffer };
        }

        return {};
    }

    bool BaseRegex::Compile()
    {
        if (_pattern.empty()) [[unlikely]]
        {
            Assert("The pattern is empty.");
            return false;
        }

        _FreeRegex();
        _regex = pcre2_compile(reinterpret_cast<PCRE2_SPTR8>(_pattern.c_str()), // The regex pattern
                               _limit,                                          // Pattern is null-terminated
                               _compileOptions,                                 // Default options
                               &_errorCode,                                     // Error code
                               &_errorOffset,                                   // Error offset
                               nullptr                                          // Default compile context
        );

        if (_regex == nullptr || _errorCode != 100) [[unlikely]] // 100 == no errors
        {
            Assert(GetErrorString().c_str());
            return false;
        }

        OnRegexCompiled();

        return true;
    }

    void BaseRegex::_Clear()
    {
        _pattern.clear();
        _compileOptions = 0;
        _limit = PCRE2_ZERO_TERMINATED;

        _errorCode = 0;
        _errorOffset = 0;

        _subject = nullptr;
        _offset = 0;

        _FreeRegex();
    }

    void BaseRegex::_FreeRegex()
    {
        if (_regex)
        {
            pcre2_code_free(_regex);
            _regex = nullptr;
        }
    }

    BaseRegexMatch::~BaseRegexMatch()
    {
        BaseRegexMatch::_Clear();
    }

    void BaseRegexMatch::Clear()
    {
        BaseRegex::Clear();
        BaseRegexMatch::_Clear();
    }

    BaseRegexMatch::MatchedData BaseRegexMatch::Match()
    {
        if (!IsCompiled() || _matchData == nullptr) [[unlikely]]
        {
            Assert("Regex wasn't compiled or match data was failed!");
            return {};
        }

        if (!_subject) [[unlikely]]
        {
            Assert("The subject is nullptr.");
            return {};
        }

        const auto result = pcre2_match(_regex,                                  // Compiled regex
                                        reinterpret_cast<PCRE2_SPTR8>(_subject), // Subject string
                                        _limit,                                  // Subject is null-terminated
                                        _offset,                                 // Start at offset 0
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
                return {};
            }

            md.size = ovector[1] - ovector[0];

            return md;
        }
        else if (result == PCRE2_ERROR_NOMATCH)
        {
            return {};
        }

        Assert(false);
        return {};
    }

    std::vector<BaseRegexMatch::MatchedData> BaseRegexMatch::MatchAll()
    {
        if (!IsCompiled() || _matchData == nullptr) [[unlikely]]
        {
            Assert("Regex wasn't compiled or match data was failed!");
            return {};
        }

        if (!_subject) [[unlikely]]
        {
            Assert("The subject is nullptr.");
            return {};
        }

        std::vector<MatchedData> matches;
        matches.reserve(10);

        decltype(_offset) offset = _offset;
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
                    return matches;
                }

                md.size = ovector[1] - ovector[0];
                offset = ovector[1];

                matches.emplace_back(md);
            }
        } while (result > 0);

        return matches;
    }

    void BaseRegexMatch::OnRegexCompiled()
    {
        if (IsCompiled())
        {
            _FreeMatchData();
            _matchData = pcre2_match_data_create_from_pattern(_regex, nullptr);
        }
    }

    void BaseRegexMatch::_FreeMatchData()
    {
        if (_matchData)
        {
            pcre2_match_data_free(_matchData);
            _matchData = nullptr;
        }
    }

    void BaseRegexMatch::_Clear()
    {
        _matchOptions = 0;
        _FreeMatchData();
    }

    bool BaseRegexReplace::Replace()
    {
        if (!IsCompiled()) [[unlikely]]
        {
            Assert("Regex wasn't compiled!");
            return false;
        }

        if (!_subject) [[unlikely]]
        {
            Assert("The subject is nullptr.");
            return false;
        }

        if (!_replacement) [[unlikely]]
        {
            Assert("The replacement string is nullptr.");
            return false;
        }

        if (!_allocatedString) [[unlikely]]
        {
            Assert("The output string is nullptr.");
            return false;
        }

        int rc = pcre2_substitute(_regex,                                            // Compiled regex
                                  reinterpret_cast<PCRE2_SPTR8>(_subject),           // Subject string
                                  _limit,                                            // Subject is null-terminated
                                  0,                                                 // Start at offset 0
                                  _replaceOptions,                                   // Options
                                  nullptr,                                           // Default match context
                                  nullptr,                                           // Default substitute context
                                  reinterpret_cast<PCRE2_SPTR8>(_replacement),       // Replacement string
                                  PCRE2_ZERO_TERMINATED,                             // Replacement is null-terminated
                                  reinterpret_cast<PCRE2_UCHAR8*>(_allocatedString), // Output buffer
                                  &_allocatedSize                                    // Length of the output buffer
        );

        if (rc >= 0)
        {
            return true;
        }

        Assert(GetErrorString(rc).c_str());

        return false;
    }

    void BaseRegexReplace::SetOutputString(char* allocatedString, size_t size) noexcept
    {
        _allocatedString = allocatedString;
        _allocatedSize = size;
    }

    void BaseRegexReplace::SetReplaceAll(bool value)
    {
        if (value)
        {
            _replaceOptions |= PCRE2_SUBSTITUTE_GLOBAL;
        }
        else
        {
            _replaceOptions &= ~PCRE2_SUBSTITUTE_GLOBAL;
        }
    }

    void BaseRegexReplace::Clear()
    {
        BaseRegex::Clear();
        BaseRegexReplace::_Clear();
    }

    void BaseRegexReplace::_Clear()
    {
        _replaceOptions = 0;
        _replacement = nullptr;
        _allocatedString = nullptr;
        _allocatedSize = 0;
    }
} // namespace Core
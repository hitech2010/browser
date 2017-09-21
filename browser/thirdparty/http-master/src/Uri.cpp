/*
 * Copyright (c) 2014 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "http/Common.hpp"
#include "http/Uri.hpp"
#include "http/Parse.hpp"

namespace http {

static bool isReserved(char ch) {
    switch (ch) {
    case '/': return true;
    case '#': return true;
    case '?': return true;
    default: return false;
    }
}

static ParseResult<std::string> parseScheme(char const* str) {
    auto result = parseWhile(str, [](char ch) {
        return ch != ':' && !isReserved(ch);
    });
    result.ch = (result.ch[0] == ':') ? (result.ch+1) : (result.ch);
    return result;
}

static ParseResult<std::string> parseUser(char const* str) {
    auto result = parseWhile(str, [](char ch) {
        return ch != '@' && !isReserved(ch);
    });
    if (result.ch[0] == '@') {
        result.ch = result.ch+1;
    } else {
        result.ch = str;
        result.value = "";
    }
    return result;
}

static ParseResult<std::string> parseHost(char const* str) {
    return parseWhile(str, [](char ch) {
        return ch != ':' && !isReserved(ch);
    });
}

static ParseResult<uint16_t> parsePort(char const* str) {
    ParseResult<uint16_t> result;
    if (str[0] != ':') {
        result.value = 0;
        result.ch = str;
        return result;
    }
    auto tmp = parseWhile(str+1, [](char ch) {
        return !isReserved(ch);
    });
    result.value = uint16_t(strtol(tmp.value.c_str(), 0, 10));
    result.ch = tmp.ch; 
    return result; 
}

static ParseResult<Authority> parseAuthority(char const* str) {
    ParseResult<Authority> result{};
    if (str[0] == '\0' || str[0] != '/' || str[1] != '/') {
        result.ch = str;
        return result;
    } 
    
    auto user = parseUser(str+2); // For "//"
    auto host = parseHost(user.ch);
    auto port = parsePort(host.ch);

    result.value.userIs(user.value);
    result.value.hostIs(host.value);
    result.value.portIs(port.value);
    result.ch = port.ch;

    return result; 
}

static ParseResult<std::string> parsePath(char const* str) {
    // Return query/frag as part of path for now
    ParseResult<std::string> result = parseWhile(str, [](char ch) {
        return true; 
    }); 
/*
    ParseResult<std::string> result = parseWhile(str, [](char ch) {
        return ch != '/' && !isReserved(ch);
    }); 
    result.ch = (result.ch[0] == '?') ? (result.ch+1) : (result.ch);
*/
    return result;

}

static Uri parseUri(char const* str) {
    Uri uri;

    auto scheme = parseScheme(str);
    auto authority = parseAuthority(scheme.ch);
    auto path = parsePath(authority.ch);

    uri.schemeIs(scheme.value);
    uri.authorityIs(authority.value);
    uri.pathIs(path.value);
    return uri; 
}


Authority::Authority(std::string const& user, std::string const& host, uint16_t port) {
    user_ = user;
    host_ = host;
    port_ = port;
}

Authority::Authority() {
    port_ = 0;
}

void Authority::userIs(std::string const& user) {
    user_ = user;
}

void Authority::hostIs(std::string const& host) {
    host_ = host;
}

void Authority::portIs(uint16_t port) {
    port_ = port;
}

Uri::Uri(char* const value) {
    *this = parseUri(value);
}

Uri::Uri(std::string const& value) {
    *this = parseUri(value.c_str());
}

Uri::Uri() {
}

void Uri::schemeIs(std::string const& scheme) {
    scheme_ = scheme;
}

void Uri::authorityIs(Authority const& authority) {
    authority_ = authority;
}

void Uri::pathIs(std::string const& path) {
    path_ = path;
}

}

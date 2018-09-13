/*
 * Copyright (C) 2018 Intel Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice(s),
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice(s),
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <memory> //runtime error
#include <atomic>
#include <cstddef>
#include "memkind.h"
/*
 * Header file for the C++ allocator compatible with the C++ standard library allocator concepts.
 * More details in hbwallocator(3) man page.
 * Note: memory heap management is based on hbwmalloc, refer to the hbwmalloc man page for more information.
 *
 * Functionality defined in this header is considered as EXPERIMENTAL API.
 * API standards are described in memkind(3) man page.
 */
namespace pmem
{
    template<typename T>
    class allocator
    {
    public:
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using const_reference = const value_type&;
        using reference = value_type&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        
        template<class U>
        struct rebind {
            using other = allocator<U>;
        };

        template<typename U>
        friend class allocator;
#ifndef _GLIBCXX_USE_CXX11_ABI
        /* This is a workaround for compilers that uses C++11 standard,
         * but use old - non C++11 ABI */
        template<typename V = void>
        explicit allocator()
        {
            static_assert(std::is_same<V, void>::value,
                          "pmem::allocator cannot be compiled without CXX11 ABI");
        }
#endif

        explicit allocator(const char *dir, size_t max_size)
        {
            int err_c  = memkind_create_pmem(dir, max_size, &kind_ptr);
            if (err_c) {
                throw std::runtime_error(
                    std::string("An error occured while creating pmem kind; error code: ") +
                    std::to_string(err_c));
            }
            kind_cnt = new std::atomic<size_t>(1);
        }

        explicit allocator(const std::string& dir,
                           size_t max_size) : allocator(dir.c_str(), max_size)
        {
        }

        allocator(const allocator& other) noexcept
        {
            assign(other);
        }

        template <typename U>
        allocator(const allocator<U>& other) noexcept
        {
            assign(other);
        }

        allocator(const allocator&& other) noexcept
        {
            assign(other);
        }

        template <typename U>
        allocator(const allocator<U>&& other) noexcept
        {
            assign(other);
        }

        void operator = (const allocator& other) noexcept
        {
            clean_up();
            assign(other);
        }

        template <typename U>
        void operator = (const allocator<U>& other) noexcept
        {
            clean_up();
            assign(other);
        }

        void operator = (const allocator&& other) noexcept
        {
            clean_up();
            assign(other);
        }

        template <typename U>
        void operator = (const allocator<U>&& other) noexcept
        {
            clean_up();
            assign(other);
        }

        template <typename U>
        bool operator ==(const allocator<U>& other) const
        {
            return kind_ptr == other.kind_ptr;
        }

        template <typename U>
        bool operator !=(const allocator<U>& other) const
        {
            return !(*this ==other);
        }

        pointer allocate(size_type n) const
        {
            pointer result = static_cast<pointer>(memkind_malloc(kind_ptr, n*sizeof(T)));
            if (!result) {
                throw std::bad_alloc();
            }
            return result;
        }

        void deallocate(T* p, size_type n) const
        {
            memkind_free(kind_ptr, static_cast<void*>(p));
        }

        template <class U, class... Args>
        void construct(U* p, Args... args) const
        {
            new(p) U(std::forward<Args>(args)...);
        }

        template <class U>
        void destroy(U* p) const
        {
            p->~U();
        }

        ~allocator()
        {
            clean_up();
        }
    private:
        memkind_t kind_ptr;
        std::atomic<size_t> * kind_cnt;

        void clean_up()
        {
            if (kind_cnt->fetch_sub(1) == 1) {

                memkind_destroy_kind(kind_ptr);
                delete kind_cnt;
            }
        }

        template <typename U>
        inline void assign(U&& other)
        {
            kind_ptr = other.kind_ptr;
            kind_cnt = other.kind_cnt;
            ++(*kind_cnt);
        }

    };
}

/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** The number of a resource or component type, given by its name
*/
#ifndef _KRONKWORLD_TYPE_ID_H
    #define _KRONKWORLD_TYPE_ID_H
    #include <cstddef>
    #include <string_view>

namespace kw::detail
{
    // The name of T as the compiler spells it (the whole signature: it is only a key).
    template<typename T>
    constexpr std::string_view typeName(void) noexcept
    {
#if defined(_MSC_VER) && !defined(__clang__)
        return __FUNCSIG__;
#else
        return __PRETTY_FUNCTION__;
#endif
    }

    // The id of a type is a number taken from a table that has ONE copy per process, defined in
    // src/world/TypeIds.cpp: the same name always gets the same number, whichever binary asks (a
    // program, a DLL, several DLLs). A counter in a header would give each binary its own numbers
    // (a DLL and a program have their own copy of a `static` of an inline function). So the table
    // must not be linked twice: see docs of the project that builds a DLL.
    // Two different types with the same name (two structs in anonymous namespaces of two files)
    // share a number: harmless unless both are used in the same World.
    size_t resourceTypeId(std::string_view name) noexcept;
    size_t componentTypeId(std::string_view name) noexcept;
}

#endif /* _KRONKWORLD_TYPE_ID_H */

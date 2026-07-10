/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Entity errors
*/
#ifndef _KRONKWORLD_RESOURCE_ERROR_H
    #define _KRONKWORLD_RESOURCE_ERROR_H
    #include <exception>
    #include <string>
    #include <string_view>
    #include <format>
    #include <utility>

namespace kw
{

    class ResourceError : public std::exception
    {
        public:
            const char *what() const noexcept
            {
                return m_message.c_str();
            }

        protected:
            template<typename... Args>
            ResourceError(std::string_view f, Args&&... args) 
            { 
                m_message = std::vformat(f, std::make_format_args(std::forward<Args>(args)...)); 
            };

        private:
            std::string m_message;
    };

    class MaxResourceReached : public ResourceError
    {
        public:
            MaxResourceReached(void) : ResourceError("Max resource reached") {};
    };

    class BadResource : public ResourceError
    {
        public:
            template<typename ...Args>
            BadResource(std::string_view format, Args&&... args) : ResourceError(format, std::forward<Args>(args)...) {};
    };

}

#endif /* _KRONKWORLD_RESOURCE_H */

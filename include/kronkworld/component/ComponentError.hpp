/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Entity errors
*/
#ifndef _KRONKWORLD_COMPONENT_ERROR_H
    #define _KRONKWORLD_COMPONENT_ERROR_H
    #include <exception>
    #include <string>
    #include <string_view>
    #include <format>
    #include <utility>

namespace kw
{

    class ComponentError : public std::exception
    {
        public:
            const char *what() const noexcept
            {
                return m_message.c_str();
            }

        protected:
            template<typename... Args>
            ComponentError(std::string_view f, Args&&... args) 
            { 
                m_message = std::vformat(f, std::make_format_args(std::forward<Args>(args)...)); 
            };

        private:
            std::string m_message;
    };

    class MaxComponentReached : public ComponentError
    {
        public:
            MaxComponentReached() : ComponentError("Max entity reached") {};
    };

}

#endif /* _KRONKWORLD_COMPONENT_ERROR_H */

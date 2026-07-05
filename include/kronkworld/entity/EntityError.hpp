/*
** FREE PROJECT, 2026
** KRONKWORLD
** File description:
** Entity errors
*/
#ifndef _KRONKWORLD_ENTITY_ERROR_H
    #define _KRONKWORLD_ENTITY_ERROR_H
    #include <exception>
    #include <string>
    #include <string_view>
    #include <format>
    #include <utility>

namespace kw
{

    class EntityError : public std::exception
    {
        public:
            const char *what() const noexcept
            {
                return m_message.c_str();
            }

        protected:
            template<typename... Args>
            EntityError(std::string_view f, Args&&... args) 
            { 
                m_message = std::vformat(f, std::make_format_args(std::forward<Args>(args)...)); 
            };

        private:
            std::string m_message;
    };

    class MaxEntitiesReached : public EntityError
    {
        public:
            MaxEntitiesReached() : EntityError("Max entity reached") {};
    };

}

#endif /* _KRONKWORLD_ENTITY_ERROR_H */

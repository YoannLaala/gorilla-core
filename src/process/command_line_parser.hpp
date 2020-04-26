#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../container/vector.hpp"
#include "../container/hash_map.hpp"
#include "../string/string.hpp"
#include "../string/string_helper.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class CommandLineParser
    {
    public:
        CommandLineParser(const char *title, const char *description, const char *version);
        ~CommandLineParser();

        void add_option(const char *long_pattern, const char *description, bool trigger = false);

        void parse(const char **arguments, uint32_t argument_count);

        bool has_bool(const char *long_pattern);
        bool has_integer(const char *long_pattern);
        bool has_float(const char *long_pattern);
        bool has_string(const char *long_pattern);
        bool has_trigger(const char *long_pattern);

        bool get_bool(const char *long_pattern);
        int64_t get_integer(const char *long_pattern);
        double get_float(const char *long_pattern);
        const char* get_string(const char *long_pattern);

        void generate_usage(String *value_out) const;

    private:
        StringValue* get_value(const char *long_pattern);

    private:
        String m_title;
        String m_description;
        String m_version;

    private:
        struct CommandLineOption
        {
            String LongPattern;
            String Description;
            StringValue Value;
            bool Trigger;
        };
        Vector<CommandLineOption> m_options;
        HashMap<uint32_t, uint32_t> m_option_indices;
    };
}

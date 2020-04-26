/******************************************************************************
**  Includes
******************************************************************************/
#include "command_line_parser.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      constructor
    CommandLineParser::CommandLineParser(const char *title, const char *description, const char *version)
        : m_title(title)
        , m_description(description)
        , m_version(version)
    {
        // nothing to do
    }

    //! @brief      destructor
    CommandLineParser::~CommandLineParser()
    {
        // nothing to do
    }

    //! @brief      add_option
    void CommandLineParser::add_option(const char *long_pattern, const char *description, bool trigger /*= false*/)
    {
        CommandLineOption& option = m_options.add();
        option.LongPattern.set("--").append(long_pattern);
        option.Description = description;
        option.Trigger = trigger;
    }

    //! @brief      parse
    void CommandLineParser::parse(const char **arguments, uint32_t argument_count)
    {
        // iterates on each option
        m_option_indices.clear();
        const uint32_t option_count = m_options.get_size();
        for(uint32_t i = 0; i < option_count; ++i)
        {
            CommandLineOption& option = m_options[i];
            for(uint32_t j = 0; j < argument_count; ++j)
            {
                if (option.LongPattern == arguments[j])
                {
                    uint32_t hash = Hash::generate(&option.LongPattern[2]);
                    m_option_indices.add(hash, i);

                    if (!option.Trigger)
                    {
                        ++j;
                        StringHelper::to_value(arguments[j], (uint32_t)STRLEN(arguments[j]), &option.Value);
                    }
                }
            }
        }
    }

    //! @brief      get_value
    StringValue* CommandLineParser::get_value(const char *long_pattern)
    {
        uint32_t hash = Hash::generate(long_pattern);
        uint32_t index = m_option_indices.get(hash, (uint32_t)-1);
        if (index == (uint32_t)-1)
            return nullptr;

        return &m_options[index].Value;
    }

    //! @brief      has_bool
    bool CommandLineParser::has_bool(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return false;

        return value->DetectedType == StringValue::Type::BOOLEAN;
    }

    //! @brief      has_integer
    bool CommandLineParser::has_integer(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return false;

        return value->DetectedType == StringValue::Type::INTEGER;
    }

    //! @brief      has_float
    bool CommandLineParser::has_float(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return false;

        return value->DetectedType == StringValue::Type::FLOAT;
    }

    //! @brief      has_string
    bool CommandLineParser::has_string(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return false;

        return value->DetectedType == StringValue::Type::STRING;
    }

    //! @brief      has_trigger
    bool CommandLineParser::has_trigger(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return false;

        return true;
    }

    //! @brief      get_bool
    bool CommandLineParser::get_bool(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return false;

        return value->Boolean;
    }

    //! @brief      get_integer
    int64_t CommandLineParser::get_integer(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return 0;

        return value->Integer;
    }

    //! @brief      get_float
    double CommandLineParser::get_float(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return 0.0f;

        return value->Float;
    }

    //! @brief      get_float
    const char* CommandLineParser::get_string(const char *long_pattern)
    {
        StringValue *value = get_value(long_pattern);
        if (!value)
            return nullptr;

        return value->String.Buffer;
    }

    //! @brief      generate_usage
    //! @details    generate a string to display to give more information on the usage of the process
    void CommandLineParser::generate_usage(String *value_out) const
    {
        // title
        value_out->append("/******************************************************************************\n");
        value_out->append("**   ").append(m_title);
        value_out->append("\n** ").append(m_description);
        value_out->append("\n** version:").append(m_version);
        value_out->append("\n******************************************************************************/\n");

        // options
        const uint32_t option_count = m_options.get_size();
        for(uint32_t i = 0; i < option_count; ++i)
        {
            const CommandLineOption& option = m_options[i];
            value_out->append(option.LongPattern).append("  ").append(option.Description).append('\n');
        }
    }
}

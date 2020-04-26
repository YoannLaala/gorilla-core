/******************************************************************************
**  Includes
******************************************************************************/
#include "commandline_parser.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      constructor
    ArgumentParser::ArgumentParser(const char *title, const char *description, const char *version)
        : m_title(title)
        , m_description(description)
        , m_version(version)
    {
        add('h', "help", "display all the options available", false);
    }

    //! @brief      destructor
    ArgumentParser::~ArgumentParser()
    {
        // Nothing to do
    }

    //! @brief      add
    void ArgumentParser::add(const char *long_pattern, const char *description = nullptr, bool trigger = false)
    {
        CommandLineOption& option = m_options.add();
        option.LongPattern.set("--").append(long_pattern);
        option.Description = description;
        option.Trigger = trigger;
    }

    //! @brief      parse
    bool ArgumentParser::parse(const char **arguments, uint32_t argument_count)
    {
        // iterates on each option
        const uint32_t option_count = m_options.get_size();
        for(uint32_t i = 1; i < option_count; ++i)
        {
            CommandLineOption& option = m_options[i];

            // Find the current argument in those given by the application
            bool bArgumentFound = false;
            ArgumentExtended& kArgument = m_vArgument[uiArgument];
            for(uint32_t j = 0; j < argument_count; ++j)
            {
                if (option.LongPattern == arguments[j])
                {
                    if (option.Action)
                    {

                    }
                    else
                    {
                        ++j
                        if (!StringHelper::to_value(arguments[j], STRLEN(arguments[j]), &option.Value))
                            return false;
                    }
                }
            }
                return false;
            }
        }

        return true;
    }

    //! @brief      generate_usage
    //! @details    generate a string to display to give more information on the usage of the application
    void ArgumentParser::generate_usage(String *value_out) const
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

    //! @brief      GetArgument
    bool ArgumentParser::get_bool(const char *long_pattern) const
    {
        const uint32_t uiArgumentCount = m_vArgument.GetSize();
        for(uint32_t uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
        {
            const ArgumentExtended& kArgument = m_vArgument[uiArgument];
            if(kArgument.Match(_szPattern))
            {
                return &kArgument;
            }
        }

        return nullptr;
    }
}

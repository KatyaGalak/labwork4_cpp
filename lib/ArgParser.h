#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace ArgumentParser {

    class ArgParser {
    private:
        class IntValue {
            friend ArgParser;
        private:
            char short_name_ = ' ';
            std::string full_name_;
            std::string description_;

            int* value_ptr_;
            std::vector<int>* vector_multi_values_ptr_;

            int value_;
            std::vector<int> vector_multi_values_;

            std::vector<int> vector_default_values_;
            int default_value_;

            size_t min_args_cnt_ = 1;
            size_t number_of_values_ = 0;

            bool positional_ = false;
            bool is_default_ = false;
            bool is_recorded_ = false;
            bool one_value_ = true;
            bool multi_value_ = false;
            bool store_value_ = false;
        public:
            explicit IntValue(std::string full_name, std::string description = "")
                    : full_name_(std::move(full_name)), description_(std::move(description)){};
            explicit IntValue(char short_name, std::string full_name = "", std::string description = "")
                    : short_name_(short_name), full_name_(std::move(full_name)), description_(std::move(description)){};

            IntValue& MultiValue(size_t min_args_cnt = 0);

            IntValue& StoreValue(int& value);
            IntValue& StoreValues(std::vector<int>& values);

            IntValue& Default(const int& default_value);
            IntValue& Default(const std::vector<int>& default_value);

            IntValue& Positional();
        };

        class StringValue {
            friend ArgParser;
        private:
            char short_name_ = ' ';
            std::string full_name_;
            std::string description_;

            std::string* value_ptr_;
            std::vector<std::string>* vector_multi_values_ptr_;

            std::string value_;
            std::vector<std::string> vector_multi_values_;

            std::vector<std::string> vector_default_values_;
            std::string default_value_;

            size_t min_args_cnt_ = 1;
            size_t number_of_values_ = 0;

            bool positional_ = false;
            bool is_default_ = false;
            bool is_recorded_ = false;
            bool one_value_ = true;
            bool multi_value_ = false;
            bool store_value_ = false;
        public:
            explicit StringValue(std::string full_name, std::string description = "")
                    : full_name_(std::move(full_name)), description_(std::move(description)){};
            explicit StringValue(char short_name, std::string full_name = "", std::string description = "")
                    : short_name_(short_name), full_name_(std::move(full_name)), description_(std::move(description)){};

            StringValue& MultiValue(size_t min_args_cnt = 0);

            StringValue& StoreValue(std::string& value);
            StringValue& StoreValues(std::vector<std::string>& values);

            StringValue& Default(const std::string& default_value);
            StringValue& Default(const std::vector<std::string>& default_value);

            StringValue& Positional();
        };

        class FlagValue {
            friend ArgParser;
        private:
            char short_name_ = ' ';
            std::string full_name_;
            std::string description_;

            bool* value_ptr_;
            bool value_ = false;

            bool default_value_;

            bool store_value_ = false;
            bool is_default_ = false;
            bool is_recorded_ = false;
        public:
            explicit FlagValue(std::string full_name, std::string description = "")
                    : full_name_(std::move(full_name)), description_(std::move(description)){};
            explicit FlagValue(char short_name, std::string full_name = "", std::string description = "")
                    : short_name_(short_name), full_name_(std::move(full_name)), description_(std::move(description)){};


            FlagValue& StoreValue(bool& value);

            FlagValue& Default(bool default_value);
        };
        class HelpArg {
            friend ArgParser;
        private:
            std::string short_name_help_;
            std::string full_name_help_;
            std::string description_help_;

            bool have_help_arg = false;
            bool found_help_ = false;
        };

        std::vector<IntValue> int_arg_;
        std::vector<StringValue> string_arg_;
        std::vector<FlagValue> flag_arg_;

        HelpArg help_;

    public:
        std::string pars_name_;
        std::string description_;

        explicit ArgParser(std::string pars_name, std::string description = "")
                : pars_name_(std::move(pars_name)), description_(std::move(description)){};
        ~ArgParser() = default;

        // INT
        IntValue& AddIntArgument(const std::string& name, const std::string& description = "");
        IntValue& AddIntArgument(char short_name,
                                 const std::string& name,
                                 const std::string& description = "");
        int GetIntValue(const std::string& s, size_t index = 0);
        int GetIntValue(char s, size_t index = 0);

        // STRING
        StringValue& AddStringArgument(const std::string& name, const std::string& description = "");
        StringValue& AddStringArgument(char short_name,
                                 const std::string& name,
                                 const std::string& description = "");
        std::string GetStringValue(const std::string& s, size_t index = 0);
        std::string GetStringValue(char s, size_t index = 0);

        //FLAG
        FlagValue& AddFlag(const std::string& name, const std::string& description = "");
        FlagValue& AddFlag(char short_name,
                                       const std::string& name,
                                       const std::string& description = "");
        bool GetFlag(const std::string& s);
        bool GetFlag(char s);

        bool Parse(int argc, char** argv);
        bool Parse(const std::vector<std::string>& argv_string);

        bool AddArgumentWithEqually(const std::string& argv_string);
        bool AddFlagArgument(const std::string& argv_string);
        bool AddFlagArgumentOneDash(const std::string& argv_string);
        bool AddHelpArgument(const std::string& argv_string);
        bool AddIntPositionalArgument(const std::string& argv_string);
        bool AddStringPositionalArgument(const std::string& argv_string);
        bool CheckPositionalArgument(const std::string& argv_string);
        bool CheckCorrectnessSettingArguments();

        void AddHelp(char short_name, const std::string& full_name, const std::string& description);
        std::string HelpDescription();
        bool Help();
    };

} // namespace ArgumentParser

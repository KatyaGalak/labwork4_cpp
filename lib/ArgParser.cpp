#include "ArgParser.h"

using namespace ArgumentParser;

// INT
ArgParser::IntValue& ArgParser::IntValue::MultiValue(size_t min_args_cnt) {
    min_args_cnt_ = min_args_cnt;
    multi_value_ = true;
    one_value_ = false;

    return *this;
}

ArgParser::IntValue& ArgParser::IntValue::StoreValue(int& value) {
    if (multi_value_) {
        throw std::logic_error("It is not possible to store a multi-valued value in a single");
    }
    value_ptr_ = &value;
    store_value_ = true;

    return *this;
}

ArgParser::IntValue& ArgParser::IntValue::StoreValues(std::vector<int>& values) {
    if (!multi_value_) {
        throw std::logic_error("It is not possible to store a single value in a multi-valued");
    }
    vector_multi_values_ptr_ = &values;
    store_value_ = true;

    return *this;
}

ArgParser::IntValue& ArgParser::IntValue::Default(const int& default_value) {
    if (multi_value_) {
        throw std::invalid_argument("It will not work to set a single value in a multi-values argument");
    }
    default_value_ = default_value;
    value_ = default_value;
    is_default_ = true;

    return *this;
}

ArgParser::IntValue& ArgParser::IntValue::Default(const std::vector<int>& default_value) {
    if (!multi_value_) {
        throw std::invalid_argument("The argument is not multivalued");
    }
    vector_default_values_ = default_value;
    is_default_ = true;
    vector_multi_values_ = default_value;

    return *this;
}

ArgParser::IntValue& ArgParser::IntValue::Positional() {
    positional_ = true;
    return *this;
}

ArgParser::IntValue& ArgParser::AddIntArgument(const std::string& full_name, const std::string& description) {
    IntValue val(full_name, description);
    int_arg_.emplace_back(val);
    return reinterpret_cast<IntValue&>(int_arg_.back());
}

ArgParser::IntValue& ArgParser::AddIntArgument(char short_name,
                                               const std::string& full_name, const std::string& description) {
    IntValue val(short_name, full_name, description);
    int_arg_.emplace_back(val);
    return reinterpret_cast<IntValue&>(int_arg_.back());
}

int ArgParser::GetIntValue(const std::string& desired_value, size_t index) {
    for (int i = 0; i < int_arg_.size(); ++i) {
        std::pair<char, std::string> args_name;
        args_name = {int_arg_[i].short_name_, int_arg_[i].full_name_};
        if (desired_value.size() == 1 && args_name.first == desired_value[0]
            || args_name.second == desired_value) {
            if (!int_arg_[i].multi_value_) {
                return int_arg_[i].store_value_ ? *int_arg_[i].value_ptr_ : int_arg_[i].value_;
            }

            if (int_arg_[i].multi_value_ && int_arg_[i].number_of_values_ >= int_arg_[i].min_args_cnt_){
                if (int_arg_[i].store_value_ && index >= int_arg_[i].vector_multi_values_ptr_->size()
                    || !int_arg_[i].store_value_ && index >= int_arg_[i].vector_multi_values_.size()) {
                    throw std::invalid_argument("The index is too large. There is no element with such an index.");
                }

                return int_arg_[i].store_value_ ?
                    (*int_arg_[i].vector_multi_values_ptr_)[index] : int_arg_[i].vector_multi_values_[index];
            }

            if (!int_arg_[i].is_default_) {
                throw std::invalid_argument("The default value is not det and the number of values is less then the min number of values");
            }

            return int_arg_[i].vector_default_values_[index];
        }
    }

    throw std::invalid_argument("There is no value with name " + desired_value);
}

int ArgParser::ArgParser::GetIntValue(char desired_value, size_t index) {
    return ArgumentParser::ArgParser::GetIntValue(std::string(1, desired_value), index);
}

// STRING
ArgParser::StringValue& ArgParser::StringValue::MultiValue(size_t min_args_cnt) {
    min_args_cnt_ = min_args_cnt;
    multi_value_ = true;
    one_value_ = false;

    return *this;
}

ArgParser::StringValue& ArgParser::StringValue::StoreValue(std::string& value) {
    if (multi_value_) {
        throw std::logic_error("It is not possible to store a multi-valued value in a single");
    }
    value_ptr_ = &value;
    store_value_ = true;

    return *this;
}

ArgParser::StringValue& ArgParser::StringValue::StoreValues(std::vector<std::string>& values) {
    if (!multi_value_) {
        throw std::logic_error("It is not possible to store a single value in a multi-valued");
    }
    vector_multi_values_ptr_ = &values;
    store_value_ = true;

    return *this;
}

ArgParser::StringValue& ArgParser::StringValue::Default(const std::string& default_value) {
    if (multi_value_) {
        throw std::invalid_argument("It will not work to set a single value in a multi-values argument");
    }
    default_value_ = default_value;
    is_default_ = true;
    value_ = default_value;

    return *this;
}

ArgParser::StringValue& ArgParser::StringValue::Default(const std::vector<std::string>& default_value) {
    if (!multi_value_) {
        throw std::invalid_argument("The argument is not multivalued");
    }
    vector_default_values_ = default_value;
    is_default_ = true;
    vector_multi_values_ = default_value;

    return *this;
}

ArgParser::StringValue& ArgParser::StringValue::Positional() {
    positional_ = true;
    return *this;
}

ArgParser::StringValue& ArgParser::AddStringArgument(const std::string& full_name, const std::string& description) {
    StringValue val(full_name, description);
    string_arg_.emplace_back(val);
    return reinterpret_cast<StringValue&>(string_arg_.back());
}

ArgParser::StringValue& ArgParser::AddStringArgument(char short_name,
                                               const std::string& full_name, const std::string& description) {
    StringValue val(short_name, full_name, description);
    string_arg_.emplace_back(val);
    return reinterpret_cast<StringValue&>(string_arg_.back());
}

std::string ArgParser::GetStringValue(const std::string& desired_value, size_t index) {
    for (int i = 0; i < string_arg_.size(); ++i) {
        std::pair<char, std::string> args_name;
        args_name = {string_arg_[i].short_name_, string_arg_[i].full_name_};
        if (desired_value.size() == 1 && args_name.first == desired_value[0]
            || args_name.second == desired_value) {
            if (!string_arg_[i].multi_value_) {
                return string_arg_[i].store_value_ ? *string_arg_[i].value_ptr_ : string_arg_[i].value_;
            }

            if (string_arg_[i].multi_value_ && string_arg_[i].number_of_values_ >= string_arg_[i].min_args_cnt_){
                if (string_arg_[i].store_value_ && index >= string_arg_[i].vector_multi_values_ptr_->size()
                    || !string_arg_[i].store_value_ && index >= string_arg_[i].vector_multi_values_.size()) {
                    throw std::invalid_argument("The index is too large. There is no element with such an index.");
                }

                return string_arg_[i].store_value_ ?
                    (*string_arg_[i].vector_multi_values_ptr_)[index] : string_arg_[i].vector_multi_values_[index];
            }

            if (!string_arg_[i].is_default_) {
                throw std::invalid_argument("The default value is not det and the number of values is less then the min number of values");
            }

            return string_arg_[i].vector_default_values_[index];
        }
    }

    throw std::invalid_argument("There is no value with name " + desired_value);
}

std::string ArgParser::ArgParser::GetStringValue(char desired_value, size_t index) {
    return ArgumentParser::ArgParser::GetStringValue(std::string(1, desired_value), index);
}

// FLAG
ArgParser::FlagValue& ArgParser::FlagValue::Default(bool default_value) {
    value_ = default_value;
    default_value_ = default_value;
    is_default_ = true;

    return *this;
}

ArgParser::FlagValue& ArgParser::FlagValue::StoreValue(bool& value) {
    value_ptr_ = &value;
    store_value_ = true;

    return *this;
}

ArgParser::FlagValue& ArgParser::AddFlag(const std::string& full_name, const std::string& description) {
    FlagValue val(full_name, description);
    flag_arg_.emplace_back(val);
    return reinterpret_cast<FlagValue&>(flag_arg_.back());
}

ArgParser::FlagValue& ArgParser::AddFlag(char short_name,
                                                     const std::string& full_name, const std::string& description) {
    FlagValue val(short_name, full_name, description);
    flag_arg_.emplace_back(val);
    return reinterpret_cast<FlagValue&>(flag_arg_.back());
}

bool ArgParser::GetFlag(const std::string& desired_value) {
    for (int i = 0; i < flag_arg_.size(); ++i) {
        std::pair<char, std::string> args_name;
        args_name = {flag_arg_[i].short_name_, flag_arg_[i].full_name_};
        if (desired_value.size() == 1 && args_name.first == desired_value[0]
            || args_name.second == desired_value) {
            return flag_arg_[i].store_value_ ? *flag_arg_[i].value_ptr_ : flag_arg_[i].value_;
        }
    }

    throw std::invalid_argument("There is no value with name " + desired_value);
}

bool ArgParser::ArgParser::GetFlag(char desired_value) {
    return ArgumentParser::ArgParser::GetFlag(std::string(1, desired_value));
}

// HELP
void ArgParser::AddHelp(char short_name, const std::string& full_name, const std::string& description) {
    help_.short_name_help_ = short_name;
    help_.full_name_help_ = full_name;
    help_.description_help_ = description;
    help_.have_help_arg = true;
}

std::string ArgParser::HelpDescription() {
    if (!help_.have_help_arg) {
        return "";
    }
    std::stringstream output_ans;
    output_ans << pars_name_ << '\n' << help_.description_help_ << '\n';

    for (int i = 0; i < string_arg_.size(); ++i) {
        output_ans << '-' << string_arg_[i].short_name_ << ", --" << string_arg_[i].full_name_ << "=<string> ";
        output_ans << string_arg_[i].description_;
        if (string_arg_[i].multi_value_) {
            output_ans << " [repeated, min args = " << std::to_string(string_arg_[i].min_args_cnt_) << "]";
        }

        if (string_arg_[i].is_default_) {
            output_ans << "[default = " << string_arg_[i].default_value_ << "]";
        }
        output_ans << "\n";
    }

    for (int i = 0; i < flag_arg_.size(); ++i) {
        output_ans << '-' << flag_arg_[i].short_name_ << ", --" << flag_arg_[i].full_name_;
        output_ans << "=<flag> " << flag_arg_[i].description_;

        if (flag_arg_[i].is_default_) {
            output_ans << "[default = " << std::to_string(flag_arg_[i].default_value_) << "]";
        }
        output_ans << "\n";
    }

    for (int i = 0; i < int_arg_.size(); ++i) {
        output_ans << '-' << int_arg_[i].short_name_ << ", --" << int_arg_[i].full_name_;
        output_ans << "=<int> " << int_arg_[i].description_;

        if (int_arg_[i].multi_value_) {
            output_ans << " [repeated, min args = " << std::to_string(int_arg_[i].min_args_cnt_) << "]";
        }

        if (int_arg_[i].is_default_) {
            output_ans << "[default = " << std::to_string(int_arg_[i].default_value_) << "]";
        }
        output_ans << "\n";
    }

    return output_ans.str();
}

bool ArgParser::Help() {
    return help_.found_help_;
}

// PARSING
bool ArgParser::AddArgumentWithEqually(const std::string &argv_string) {
    std::string lines = argv_string.substr(argv_string.find('=') + 1, std::string::npos);
    std::string name = argv_string.substr(2, argv_string.find('=') - 2);

    if (std::all_of(lines.begin(), lines.end(), ::isdigit) || lines[0] == '-'
        && std::all_of(lines.substr(1, std::string::npos).begin(), lines.substr(1, std::string::npos).end(), ::isdigit)) {
        for (int i = 0; i < int_arg_.size(); ++i) {
            std::pair<char, std::string> args_name;
            args_name = {int_arg_[i].short_name_, int_arg_[i].full_name_};

            if (name.size() == 1 && args_name.first == name[0]
                || args_name.second == name) {
                if (int_arg_[i].multi_value_) {
                    if (int_arg_[i].store_value_) {
                        int_arg_[i].vector_multi_values_ptr_->push_back(std::stoi(lines));
                    } else {
                        int_arg_[i].vector_multi_values_.push_back(std::stoi(lines));
                    }
                    ++int_arg_[i].number_of_values_;
                } else {
                    if (int_arg_[i].store_value_) {
                        *int_arg_[i].value_ptr_ = std::stoi(lines);
                    } else {
                        int_arg_[i].value_ = std::stoi(lines);
                    }
                }

                int_arg_[i].is_recorded_ = true;
                return true;
            }
        }
    }

    for (int i = 0; i < string_arg_.size(); ++i) {
        std::pair<char, std::string> args_name;
        args_name = {string_arg_[i].short_name_, string_arg_[i].full_name_};

        if (name.size() == 1 && args_name.first == name[0]
            || args_name.second == name) {
            if (string_arg_[i].multi_value_) {
                if (string_arg_[i].store_value_) {
                    string_arg_[i].vector_multi_values_ptr_->push_back(lines);
                } else {
                    string_arg_[i].vector_multi_values_.push_back(lines);
                }
                ++string_arg_[i].number_of_values_;
            } else {
                if (string_arg_[i].store_value_) {
                    *string_arg_[i].value_ptr_ = lines;
                } else {
                    string_arg_[i].value_ = lines;
                }
            }

            string_arg_[i].is_recorded_ = true;
            return true;
        }
    }

    for (int i = 0; i < flag_arg_.size(); ++i) {
        std::pair<char, std::string> args_name;
        args_name = {flag_arg_[i].short_name_, flag_arg_[i].full_name_};

        if (name.size() == 1 && args_name.first == name[0]
            || args_name.second == name) {
            throw std::invalid_argument("It is not possible to set the value of the flag in this way");
        }
    }

    return false;
}

bool ArgParser::AddHelpArgument(const std::string& argv_string) {
    if (help_.have_help_arg && argv_string.substr(2, std::string::npos) == help_.full_name_help_
        || argv_string.substr(2, std::string::npos) == help_.short_name_help_) {
        help_.found_help_ = true;
        return true;
    }
    return false;
}

bool ArgParser::AddFlagArgument(const std::string& argv_string) {
    std::string name = argv_string.substr(2, argv_string.find('=') - 2);
    for (int i = 0; i < flag_arg_.size(); ++i) {
        std::pair<char, std::string> args_name;
        args_name = {flag_arg_[i].short_name_, flag_arg_[i].full_name_};

        if (name.size() == 1 && args_name.first == name[0]
            || args_name.second == name) {
            if (flag_arg_[i].store_value_) {
                *flag_arg_[i].value_ptr_ = true;
            } else {
                flag_arg_[i].value_ = true;
            }

            flag_arg_[i].is_recorded_ = true;
            return true;
        }
    }

    return false;
}

bool ArgParser::AddFlagArgumentOneDash(const std::string& argv_string) {
    bool valid_arg = false;
    for (int i = 1; i < argv_string.size(); ++i) {
        for (int index = 0; index < flag_arg_.size(); ++index) {
            if (argv_string[i] == flag_arg_[index].short_name_) {
                if (flag_arg_[index].store_value_) {
                    *flag_arg_[index].value_ptr_ = true;
                } else {
                    flag_arg_[index].value_ = true;
                }

                valid_arg = true;
                flag_arg_[index].is_recorded_ = true;
                continue;
            }
        }
        if (!valid_arg) {
            return false;
        }
    }

    return true;
}

bool ArgParser::AddIntPositionalArgument(const std::string& argv_string) {
    for (int i = 0; i < int_arg_.size(); ++i) {
        if (!int_arg_[i].positional_) {
            continue;
        }
        if (int_arg_[i].multi_value_) {
            if (int_arg_[i].store_value_) {
                int_arg_[i].vector_multi_values_ptr_->push_back(std::stoi(argv_string));
            } else {
                int_arg_[i].vector_multi_values_.push_back(std::stoi(argv_string));
            }
            ++int_arg_[i].number_of_values_;
        } else {
            if (int_arg_[i].store_value_) {
                *int_arg_[i].value_ptr_ = std::stoi(argv_string);
            } else {
                int_arg_[i].value_ = std::stoi(argv_string);
            }
        }

        int_arg_[i].is_recorded_ = true;
        return true;
    }

    return false;
}

bool ArgParser::AddStringPositionalArgument(const std::string& argv_string) {
    for (int i = 0; i < string_arg_.size(); ++i) {
        if (!string_arg_[i].positional_) {
            continue;
        }
        if (string_arg_[i].multi_value_) {
            if (string_arg_[i].store_value_) {
                string_arg_[i].vector_multi_values_ptr_->push_back(argv_string);
            } else {
                string_arg_[i].vector_multi_values_.push_back(argv_string);
            }
            ++string_arg_[i].number_of_values_;
        } else {
            if (string_arg_[i].store_value_) {
                *string_arg_[i].value_ptr_ = argv_string;
            } else {
                string_arg_[i].value_ = argv_string;
            }
        }
        string_arg_[i].is_recorded_ = true;
        return true;
    }

    return false;
}

bool ArgParser::CheckPositionalArgument(const std::string& argv_string) {
    if (std::all_of(argv_string.substr(1, std::string::npos).begin(), argv_string.substr(1, std::string::npos).end(), ::isdigit)
        && !AddIntPositionalArgument(argv_string) && !AddStringPositionalArgument(argv_string)) {
        return false;
    }
    if (!std::all_of(argv_string.substr(1, std::string::npos).begin(), argv_string.substr(1, std::string::npos).end(), ::isdigit)
        && !AddStringPositionalArgument(argv_string)) {
        return false;
    }

    return true;
}

bool ArgParser::CheckCorrectnessSettingArguments() {
    for (int i = 0; i < int_arg_.size(); ++i) {
        if (int_arg_[i].multi_value_ && int_arg_[i].number_of_values_ < int_arg_[i].min_args_cnt_) {
            return false;
        }
        if (!int_arg_[i].is_default_ && !int_arg_[i].is_recorded_) {
            return false;
        }
    }

    for (int i = 0; i < string_arg_.size(); ++i) {
        if (string_arg_[i].multi_value_ && string_arg_[i].number_of_values_ < string_arg_[i].min_args_cnt_) {
            return false;
        }
        if (!string_arg_[i].is_default_ && !string_arg_[i].is_recorded_) {
            return false;
        }
    }

    for (int i = 0; i < flag_arg_.size(); ++i) {
        if (!flag_arg_[i].is_default_ && !flag_arg_[i].is_recorded_) {
            return false;
        }
    }

    return true;
}

bool ArgParser::Parse(const std::vector<std::string>& argv_string) {
    for (int i = 1; i < argv_string.size(); ++i) {
        if (argv_string[i][0] == '-') {
            if (argv_string[i] == "-" || argv_string[i] == "--") {
                return false;
            }
            if (argv_string[i].size() > 1 && argv_string[i][1] == '-') {
                if (argv_string[i].find('=') != std::string::npos) {
                    if (!AddArgumentWithEqually(argv_string[i])) {
                        if (!AddStringPositionalArgument(argv_string[i])) {
                            return false;
                        }
                    }
                } else {
                    if (AddHelpArgument(argv_string[i])) {
                        std::cout << HelpDescription() << std::endl;
                        return true;
                    }
                    if (!AddFlagArgument(argv_string[i]) && !AddStringPositionalArgument(argv_string[i])) {
                        return false;
                    }
                }
            } else {
                if (argv_string[i].find('=') != std::string::npos) {
                    if (!AddArgumentWithEqually("-" + argv_string[i])
                        && !AddStringPositionalArgument(argv_string[i])) {
                        return false;
                    }
                } else {
                    if (AddHelpArgument("-" + argv_string[i])) {
                        std::cout << HelpDescription() << std::endl;
                        return true;
                    }
                    if (!AddFlagArgumentOneDash(argv_string[i])) {
                        if(!CheckPositionalArgument(argv_string[i])) {
                            return false;
                        }
                    }
                }
            }
        } else {
            if(!CheckPositionalArgument(argv_string[i])) {
                return false;
            }
        }
    }

    return CheckCorrectnessSettingArguments();
}

bool ArgParser::Parse(int argc, char **argv) {
    std::vector<std::string> parse(argc);
    for (int i = 0; i < argc; ++i) {
        parse[i] = argv[i];
    }

    return Parse(parse);
}

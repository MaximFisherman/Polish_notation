#include "project.h"

namespace dev {


value_t Project::run(std::string input_str) {

    return processData(input_str);
}


bool Project::is_valid_count_value(stack_numbers_t& stack)
{
    if (stack.size() < 2) {
        error_code_val = error_code::NOT_CORRECT_COUNT_OF_OPERATION;

        return false;
    }
    else
    {
        return true;
    }
}

value_t Project::take_value_from_stack(stack_numbers_t& stack)
{
    value_t value = ERROR_VALUE;

    if (stack.size() != 0) {
        value = stack.top();
        stack.pop();
    }
    else{
        error_code_val = error_code::STACK_EMPTY;
    }

    return value;
}



bool Project::is_number(const std::string& s)
{
    char* end = 0;
    double val = std::strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}

void Project::clear_stack(stack_numbers_t & stack)
{
    while(!stack.empty())
    {
        stack.pop();
    }
}

int Project::get_error_code()
{
    return static_cast<uint32_t>(error_code_val);
}

value_t Project::calculateStackValue(std::string temp_str, stack_numbers_t& stack_numbers)
{
    double left_operand, right_operand;

    right_operand = take_value_from_stack(stack_numbers);
    left_operand  = take_value_from_stack(stack_numbers);

    left_operand = project_operation_.make_operation(temp_str[0], left_operand, right_operand);

    if(left_operand == ERROR_VALUE)
    {
        error_code_val = error_code::NOT_CORRECT_EXPRESSION;
        return ERROR_VALUE;
    }

    if(left_operand != polish_notation_constant::ERROR_VALUE && temp_str[0] == '/') {
        error_code_val = error_code::ZERO_DEVISION;
    }

    return left_operand;
}

value_t Project::processData(std::string& input_str)
{
    stack_numbers_t stack_numbers;
    error_code_val = error_code::NO_ERROR;

    value_t return_value = ERROR_VALUE;
    std::istringstream f(input_str);
    std::string temp_str;


    while (std::getline(f, temp_str, ' ')) {
        std::cout << "Stack value: " << temp_str << std::endl;

        if(is_number(temp_str)) {
            stack_numbers.push(std::stod(temp_str.c_str()));
        }
        else {
            if (true == is_valid_count_value(stack_numbers)) {
                value_t value = calculateStackValue(temp_str, stack_numbers);

                if (value != ERROR_VALUE){
                    stack_numbers.push(value);
                }
                else{
                    break;
                }

            } else {
                error_code_val = error_code::NOT_CORRECT_COUNT_OF_OPERATION;
            }
        }

        if(error_code_val != error_code::NO_ERROR) {
                    clear_stack(stack_numbers);
                    break;
        }
    }


    if (input_str.empty())
    {
        error_code_val = error_code::STACK_EMPTY;
    }

    if (stack_numbers.size() != 0) {
        return_value = stack_numbers.top();
    }

    return return_value;
}
}  // namespace dev

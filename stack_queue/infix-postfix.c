#include <ctype.h>
#include <stdio.h>

#define STACK_CAPACITY 100

/* 返回运算符优先级，数字越大优先级越高 */
int priority(char op)
{
    switch (op)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

/* 判断当前字符是否是支持的四则运算符 */
int is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int main(void)
{
    char stack[STACK_CAPACITY];
    int top = 0; /* top 指向下一个可插入位置，所以栈顶元素是 stack[top - 1] */
    int c;

    /* 逐字符读入中缀表达式，直到文件结束或换行 */
    while ((c = getchar()) != EOF && c != '\n')
    {
        if (isspace(c))
            continue;

        /* 连续读出一个数字 token，例如 12 或 3.14，直接输出到后缀表达式 */
        if (isdigit(c) || c == '.')
        {
            do
            {
                putchar(c);
                c = getchar();
            } while (isdigit(c) || c == '.');

            putchar(' ');

            if (c == EOF || c == '\n')
                break;
        }
        /* 连续读出一个变量名 token，例如 a、sum 或 value_1 */
        else if (isalpha(c) || c == '_')
        {
            do
            {
                putchar(c);
                c = getchar();
            } while (isalnum(c) || c == '_');

            putchar(' ');

            if (c == EOF || c == '\n')
                break;
        }

        if (c == '(')
        {
            /* 左括号直接入栈，等待遇到右括号时再处理 */
            if (top < STACK_CAPACITY)
                stack[top++] = (char)c;
        }
        else if (c == ')')
        {
            /* 右括号会弹出并输出栈中运算符，直到遇到匹配的左括号 */
            while (top > 0 && stack[top - 1] != '(')
                printf("%c ", stack[--top]);

            if (top > 0 && stack[top - 1] == '(')
                top--;
        }
        else if (is_operator((char)c))
        {
            /*
             * 当前运算符入栈前，先弹出栈顶优先级更高或相同的运算符。
             * 这样可以保证 * / 先于 + - 输出，同级运算符按从左到右计算。
             */
            while (top > 0 && stack[top - 1] != '(' &&
                   priority(stack[top - 1]) >= priority((char)c))
                printf("%c ", stack[--top]);

            if (top < STACK_CAPACITY)
                stack[top++] = (char)c;
        }
    }

    /* 输入结束后，栈中剩余的运算符依次输出 */
    while (top > 0)
    {
        if (stack[top - 1] != '(')
            printf("%c ", stack[top - 1]);
        top--;
    }

    putchar('\n');

    return 0;
}
